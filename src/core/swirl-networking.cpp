//----------------------------------------------------------------------------
// Name: swirl-networking.cpp
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include <pthread.h>
#include "UdpSocket.h"
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"
#include "swirl-sim.h"
#include "swirl-networking.h"
#include "swirl-globals.h"
#include "x-thread.h"

using namespace std;

const int MAX_PACKET_SIZE = 1000;
const int MAX_MESSAGE_NAME_SIZE  = 25;

const char* ADD_CLIENT = "/addClient";
const char* REMOVE_CLIENT = "/removeClient";
const char* ADD_ENTITY = "/addEntity";
const char* REMOVE_ENTITY = "/removeEntity";
const char* MOVE = "/move";
const char* TURN = "/turn";
const char* STRAFE = "/strafe";
const char* RECEIVE_ID = "/receiveId";
const char* ACC_XYZ = "/accxyz";


XMutex g_SWIRLNetworkingMutex;

SWIRLEntityProxy::SWIRLEntityProxy(const char* anAddress, int aPort) : networkLocation(anAddress, aPort)
{
    transmitSocket = new UdpTransmitSocket( IpEndpointName(networkLocation.address, networkLocation.port) );
}

SWIRLEntityProxy::~SWIRLEntityProxy()
{
    if (transmitSocket)
        delete transmitSocket;
}


/////////////////////////////////////////////////////////////

SWIRLMessageListener::SWIRLMessageListener(const char* anAddress, int aPort) : networkLocation(anAddress, aPort)
{
    startListenerThread(); //TODO right?
}

void SWIRLMessageListener::startListenerThread()
{
    cerr << "Starting listening thread!" << endl;
    
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, &(listen), this);
}

void SWIRLMessageListener::ProcessMessage( const osc::ReceivedMessage& m,
                                          const IpEndpointName& remoteEndpoint )
{
}

void* listen(void * args)
{
    SWIRLMessageListener* listener = (SWIRLMessageListener*)args;
    
    cerr << "Listening to " << listener->networkLocation.address << " " << listener->networkLocation.port << "..." << endl;
    
    UdpListeningReceiveSocket s(
                                IpEndpointName(
                                               //IpEndpointName::ANY_ADDRESS,
                                               listener->networkLocation.address,
                                               listener->networkLocation.port
                                               ),
                                listener
                                );
    
    //s.RunUntilSigInt();
    s.Run();
    
    pthread_exit(NULL);
    cerr << "Exiting thread!" << endl;
}

//////////////////////////////////////////////////////////////////

SWIRLClientProxy::SWIRLClientProxy(const char* anAddress, int aPort) : SWIRLEntityProxy(anAddress, aPort)
{
    
}

void SWIRLClientProxy::addEntity(const char* entityClassName, int entityId, Vector3D loc, Vector3D ori)
{
    //cerr << "SWIRLClientProxy::addEntity " << entityId << endl;
    
    char buffer[MAX_PACKET_SIZE];
    //char* buffer = new char[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( ADD_ENTITY )
    << entityClassName
    << entityId
    << loc.x << loc.y << loc.z
    << ori.x << ori.y << ori.z
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    
    //cerr << "addEntity sent" << endl;
}

void SWIRLClientProxy::removeEntity(int entityId)
{
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_ENTITY )
    << entityId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
    
}

void SWIRLClientProxy::receiveId(int entityId)
{
    
    //cerr << endl << "SWIRLClientProxy::receiveId port=" << networkLocation.port << " " << "entityId = " << entityId << endl;;
    
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( RECEIVE_ID )
    << entityId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    
    cout << "receiveId sent" << endl;
    
}


void SWIRLClientProxy::perform(int entityId, const char* messageName, float parameter)
{
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];

    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( messageName )
    << entityId
    << parameter
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
}

//////////////////////////////////////////////////////////////////

SWIRLServer::SWIRLServer(const char* anAddress, int aPort) : SWIRLMessageListener::SWIRLMessageListener(anAddress, aPort)
{
    maxEntityId = 0;
}

SWIRLServer::~SWIRLServer()
{
    std::map<int, SWIRLClientProxy*>::iterator iter;
    
    for (iter = clientProxies.begin(); iter != clientProxies.end(); iter++) {
        if (iter->second)
            delete iter->second;
    }
}

int SWIRLServer::addClientProxy(const char* clientAddress, int clientPort)
{
    //cerr << "SWIRLServer::addClientProxy " << clientAddress << " " << clientPort << endl;
    
    SWIRLClientProxy* clientProxy = new SWIRLClientProxy(clientAddress, clientPort);
    
    clientProxy->id = maxEntityId;

    clientProxies[clientProxy->id] = clientProxy;
    
    return maxEntityId;
}

void SWIRLServer::removeClientProxy(int id)
{
    std::map<int, SWIRLClientProxy*>::iterator iter;
    iter = clientProxies.find(id);
    clientProxies.erase(iter);
}


int SWIRLServer::addClient(const char* avatarClassName, const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori)
{
    std::map<int, SWIRLEntity*>::iterator entitiesIter;
    SWIRLEntity* entity;

    //first add client proxy and then add entity
    int clientId = addClientProxy(clientAddress, clientPort);

    //first add all the entities we have so far to the client
    for (entitiesIter = entities.begin(); entitiesIter != entities.end(); entitiesIter++) {
        entity = entitiesIter->second;
        
        clientProxies[clientId]->addEntity(entity->getClassName().c_str(), entity->id, entity->loc, entity->ori);
    }
    
    //add avatar to all the clients (including the sender). clientId is passed as the avatar entityId
    addEntity(avatarClassName, loc, ori);
    
    return clientId;
    
}

void SWIRLServer::removeClent(int clientId)
{
    //first remove entity and then remove the client proxy
    removeEntity(clientId);
    removeClientProxy(clientId);
}

void SWIRLServer::addEntity(const char* entityClassName, int entityId, Vector3D loc, Vector3D ori) {
    std::map<int, SWIRLClientProxy*>::iterator clientProxiesIter;
    SWIRLClientProxy* clientProxy;
    
    //add the new avatar to the server map
    SWIRLEntityFactory factory; //TODO no need to initialize
    //TODO wrong cast, revview class hierarchy
    
    SWIRLEntity* entity = (SWIRLEntity*)factory.createEntity(entityClassName, entityId, loc, ori);
    
    Globals::sim->root().addChild( entity ); //TODO

    
    entities[entityId] = entity;
    
    for (clientProxiesIter = clientProxies.begin(); clientProxiesIter != clientProxies.end(); clientProxiesIter++) {
        clientProxy = clientProxiesIter->second;
        clientProxy->addEntity(entityClassName, entityId, loc, ori);
    }
}

int SWIRLServer::addEntity(const char* entityClassName, Vector3D loc, Vector3D ori) {
    //g_SWIRLNetworkingMutex.acquire();
    
    std::map<int, SWIRLClientProxy*>::iterator clientProxiesIter;
    
    addEntity(entityClassName, maxEntityId, loc, ori);
    maxEntityId++;

    return maxEntityId - 1;
    //g_SWIRLNetworkingMutex.release();
}

void SWIRLServer::removeEntity(int entityId) {
    
    std::map<int, SWIRLClientProxy*>::iterator clientProxiesIter;
    SWIRLClientProxy* clientProxy;
    
    std::map<int, SWIRLEntity*>::iterator entitiesIter;
    entitiesIter = entities.find(entityId);
    entities.erase(entitiesIter);
    
    for (clientProxiesIter = clientProxies.begin(); clientProxiesIter != clientProxies.end(); clientProxiesIter++) {
        clientProxy = clientProxiesIter->second;
        clientProxy->removeEntity(entityId);
    }
    
}


void SWIRLServer::ProcessMessage( const osc::ReceivedMessage& m,
                                 const IpEndpointName& remoteEndpoint )
{
    //g_SWIRLNetworkingMutex.acquire();
    
    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
    char messageName[MAX_MESSAGE_NAME_SIZE];
    
    //TODO remove
    int entityId;
    std::map<int, SWIRLClientProxy*>::iterator iter;
    SWIRLClientProxy* clientProxy;
    
    strcpy(messageName, m.AddressPattern());
    
    try {
        if ( !strcmp( messageName, MOVE ) ||
            !strcmp( messageName, TURN ) ||
            !strcmp( messageName, STRAFE ) ) {
            
            float f;
            args >> entityId >> f >> osc::EndMessage;

            SWIRLEntity* entity = entities[entityId];
            
            //TODO if (entity != myAvatar) //TODO needed?
            if (!strcmp( messageName, MOVE ))
                entity->move(f);
            else if (!strcmp( messageName, TURN ))
                entity->turn(f);
            else if (!strcmp( messageName, STRAFE ))
                entity->strafe(f);
            
            for (iter = clientProxies.begin(); iter != clientProxies.end(); iter++) {
                clientProxy = iter->second;
                
                //TODO if (clientProxy->id != entityId) // dont send again to the 'sender' client
                {
                    clientProxy->perform(entityId, messageName, f);
                }
            }
        }
        else if (!strcmp( messageName, ADD_ENTITY ))
        {
            //cerr << "receiving " << ADD_ENTITY << endl;
            
            const char* entityClassName;
            const char* clientAddress;
            int clientPort;
            Vector3D loc;
            Vector3D ori;
            
            args >> entityClassName >> loc.x >> loc.y >> loc.z >> ori.x >> ori.y >> ori.z >> osc::EndMessage;
            
            int entityId = addEntity(entityClassName, loc, ori);
        }
        
        else if (!strcmp( messageName, ADD_CLIENT ))
        {
            //cerr << "receving " << ADD_CLIENT << endl;
            
            const char* entityClassName;
            const char* clientAddress;
            int clientPort;
            Vector3D loc;
            Vector3D ori;
            
            args >> entityClassName >> clientAddress >> clientPort >> loc.x >> loc.y >> loc.z >> ori.x >> ori.y >> ori.z >> osc::EndMessage;

            int clientId = addClient(entityClassName, clientAddress, clientPort, loc, ori);
            
            clientProxies[clientId]->receiveId(clientId); //it is important to call receiveId after addEntity for avatar

        }
        else if (!strcmp( messageName, REMOVE_CLIENT))
        {
            args >> entityId >> osc::EndMessage;
            removeClent(entityId);
        }
        else if (!strcmp( messageName, REMOVE_ENTITY))
        {
            args >> entityId >> osc::EndMessage;
            removeEntity(entityId);
        }
        
        //g_SWIRLNetworkingMutex.release();

    }
    catch( osc::Exception& e ){
        // any parsing errors such as unexpected argument types, or
        // missing arguments get thrown as exceptions.
        std::cerr << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}

////////////////////////////////////////////////////////////////////////

SWIRLServerProxy::SWIRLServerProxy(const char* anAddress, int aPort) : SWIRLEntityProxy(anAddress, aPort)
{
}


void addClient(const char* entityClassName, const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori);
void removeClent(int entityId);

void addEntity(const char* entityClassName, Vector3D loc, Vector3D ori);
void removeEntity(int entityId);



void SWIRLServerProxy::addClient(const char* entityClassName, const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori)
{
    //cerr << "sending " << ADD_CLIENT << endl;
    
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( ADD_CLIENT )
    << entityClassName << clientAddress << clientPort
    << loc.x << loc.y << loc.z
    << ori.x << ori.y << ori.z
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
    
    //TODO receive back clientId
}

void SWIRLServerProxy::removeClient(int clientId)
{
    char buffer[MAX_PACKET_SIZE];
    //char* buffer = new char[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_CLIENT )
    << clientId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
}

void SWIRLServerProxy::addEntity(const char* entityClassName, Vector3D loc, Vector3D ori)
{
    //cerr << "sending " << ADD_ENTITY << endl;
    
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];

    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( ADD_ENTITY )
    << entityClassName
    << loc.x << loc.y << loc.z
    << ori.x << ori.y << ori.z
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
    
    //TODO receive back clientId
}

void SWIRLServerProxy::removeEntity(int entityId)
{
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_ENTITY )
    << entityId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    //delete[] buffer;
}

void SWIRLServerProxy::perform(int eneityId, const char* messageName, float parameter)
{
    //cerr << "sending perform" << endl;
    
    //char* buffer = new char[MAX_PACKET_SIZE];
    char buffer[MAX_PACKET_SIZE];

    //cerr << "after new buffer" << endl;
    
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( messageName )
    << eneityId
    << parameter
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

////////////////////////////////////////////////////////////////////////


SWIRLClient::SWIRLClient(const char* entityClassName, const char* anAddress, int aPort, const char* serverAddress, int serverPort,
                         Vector3D avatarLoc, Vector3D avatarOri) : SWIRLMessageListener(anAddress, aPort)
{
    serverProxy = new SWIRLServerProxy(serverAddress, serverPort);
    
    serverProxy->addClient(entityClassName, anAddress, aPort, avatarLoc, avatarOri);
    
}

SWIRLClient::~SWIRLClient()
{
    
    serverProxy->removeClient(id);

    std::map<int, SWIRLEntity*>::iterator iter;
    
    for (iter = entities.begin(); iter != entities.end(); iter++) {
        if (iter->second)
            delete iter->second;
    }
    if (serverProxy)
        delete serverProxy;
}

void SWIRLClient::removeEntity(int id)
{
    std::map<int, SWIRLEntity*>::iterator iter;
    
    iter = entities.find(id);
    entities.erase(iter);
}

void SWIRLClient::ProcessMessage( const osc::ReceivedMessage& m,
                                 const IpEndpointName& remoteEndpoint )
{
    //cerr << "-------##### CLIENT RECEIVE MESSAGE" << endl;
    
    //g_SWIRLNetworkingMutex.acquire();
    
    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
    char messageName[MAX_MESSAGE_NAME_SIZE];
    SWIRLEntity* entity;
    std::map<int, SWIRLEntity*>::iterator iter;
    
    int entityId;
    int i;
    
    strcpy(messageName, m.AddressPattern());
    
    //cerr << messageName << endl;
    
    try {
        if ( !strcmp( messageName, MOVE ) ||
            !strcmp( messageName, TURN ) ||
            !strcmp( messageName, STRAFE ) ) {
            
            float f;
            args >> entityId >> f >> osc::EndMessage;;
            
            entity = entities[entityId];
            
            //TODO if (entity != myAvatar) //TODO needed?
            {
                if (!strcmp( messageName, MOVE ))
                    entity->move(f);
                else if (!strcmp( messageName, TURN ))
                    entity->turn(f);
                else if (!strcmp( messageName, STRAFE ))
                    entity->strafe(f);
            }
        }
        else if (!strcmp( messageName, ADD_ENTITY ))
        {
            const char* entityClassName;
            Vector3D loc;
            Vector3D ori;
            
            //cerr << "addEntity received" << endl;
            
            args >> entityClassName >> entityId >> loc.x >> loc.y >> loc.z >> ori.x >> ori.y >> ori.z >> osc::EndMessage;
            
            //cerr << entityClassName << endl;
            
            SWIRLEntityFactory factory; //TODO no need to initialize
            SWIRLEntity* entity = (SWIRLEntity*)factory.createEntity(entityClassName, entityId, loc, ori);
            
            //cerr << "entity " << entityClassName << "created" << endl;
            entities[entityId] = entity;
            //cerr << "entity put inside hashmap" << endl;

            
            Globals::sim->root().addChild( entity );
            //cerr << "entity added to scene graph " << entityClassName << endl;
            
        }
        else if (!strcmp( messageName, REMOVE_ENTITY))
        {
            args >> entityId >> osc::EndMessage;
            removeEntity(entityId);
        }
        else if (!strcmp( messageName, RECEIVE_ID)) //TODO can be a method
        {
            //cerr << "receiveId received" << endl;
            
            args >> entityId >> osc::EndMessage;
            id = entityId;
            //cerr << "myAvatar is is now " << entityId << endl;
            
            myAvatar = entities[entityId];
            //TODO myAvatar->addChild(Globals::camera);
            
            //Globals::camera->loc    = Globals::firstPerson;
            //Globals::camera->absLoc = myAvatar->loc - Globals::firstPerson;
            
            myAvatar->id = entityId;
            //cerr << "put avatar " << myAvatar->id << "into map" << endl;
            
        }
        else if (!strcmp( messageName, ACC_XYZ) && Globals::useAccelerometer)
        {
            float x, y, z;

            SWIRLServerProxy* serverProxy = ((SWIRLClient*)Globals::application)->serverProxy;
            SWIRLEntity* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;
            
            args >> x >> y >> z >> osc::EndMessage;
            //cerr << "accxyz received " << x << " " << y << " " << z << endl;

            if (myAvatar) {
                serverProxy->perform( myAvatar->id, "/turn", x );
                serverProxy->perform( myAvatar->id, "/move", -y );
            }
        }
        
        //g_SWIRLNetworkingMutex.release();
    }
    catch( osc::Exception& e ){
        // any parsing errors such as unexpected argument types, or
        // missing arguments get thrown as exceptions.
        std::cerr << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}


