//----------------------------------------------------------------------------
// Name: swirl-networking.cpp
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "swirl-networking.h"
#include <pthread.h> //TODO necessary?
#include "swirl-globals.h"
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

using namespace std;

const int MAX_PACKET_SIZE = 1000;
const int MAX_MESSAGE_NAME_SIZE  = 25;

const char* ADD_AVATAR = "/addAvatar";
const char* REMOVE_AVATAR = "/removeAvatar";
const char* MOVE = "/move";
const char* TURN = "/turn";
const char* STRAFE = "/strafe";
const char* RECEIVE_ID = "/receiveId";

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
}

void SWIRLMessageListener::startListenerThread()
{
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
    
    cerr << "Starting listening thread!" << endl;
    
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

void SWIRLClientProxy::addAvatar(int avatarId, Vector3D loc, Vector3D ori)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( ADD_AVATAR )
    << avatarId
    << loc.x << loc.y << loc.z
    << ori.x << ori.y << ori.z
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

void SWIRLClientProxy::removeAvatar(int avatarId)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_AVATAR )
    << avatarId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

void SWIRLClientProxy::receiveId(int avatarId)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_AVATAR )
    << avatarId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}


void SWIRLClientProxy::perform(int avatarId, const char* messageName, float parameter)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( messageName )
    << avatarId
    << parameter
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

//////////////////////////////////////////////////////////////////
SWIRLServer::SWIRLServer(const char* anAddress, int aPort) : SWIRLMessageListener::SWIRLMessageListener(anAddress, aPort)
{
    
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
    static int maxClientId = 0;
    
    NetworkLocation clientLocation(clientAddress, clientPort);
    
    SWIRLClientProxy clientProxy(clientAddress, clientPort);
    clientProxy.id = maxClientId;
    clientProxies[clientProxy.id] = &clientProxy;
    
    maxClientId++;
    
    return maxClientId - 1;
    //TODO send back clientId
}

void SWIRLServer::removeClientProxy(int id)
{
    std::map<int, SWIRLClientProxy*>::iterator iter;
    iter = clientProxies.find(id);
    clientProxies.erase(iter);
}

void SWIRLServer::ProcessMessage( const osc::ReceivedMessage& m,
                                 const IpEndpointName& remoteEndpoint )
{
    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
    char messageName[MAX_MESSAGE_NAME_SIZE];
    
    int avatarId;
    int i;
    std::map<int, SWIRLClientProxy*>::iterator iter;
    SWIRLClientProxy* clientProxy;

    
    strcpy(messageName, m.AddressPattern());
    
    //args >> f >> osc::EndMessage;
    //std::cout << "Receiving " << m.AddressPattern() << " " << f << std::endl;
    
    try {
        if ( !strcmp( messageName, MOVE ) ||
            !strcmp( messageName, TURN ) ||
            !strcmp( messageName, STRAFE ) ) {
            
            float f;
            args >> avatarId >> f >> osc::EndMessage;
            
            for (iter = clientProxies.begin(); iter != clientProxies.end(); iter++) {
                clientProxy = iter->second;
                
                if (clientProxy->id != avatarId) // dont send again to the 'sender' client
                    clientProxy->perform(avatarId, messageName, f);
            }
        }
        else if (strcmp( messageName, ADD_AVATAR ))
        {
            const char* clientAddress;
            int clientPort;
            Vector3D loc;
            Vector3D ori;
            
            args >> clientAddress >> clientPort >> loc.x >> loc.y >> loc.z >> ori.x >> ori.y >> ori.z >> osc::EndMessage;
            
            int avatarId = addClientProxy(clientAddress, clientPort);
            
            for (iter = clientProxies.begin(); iter != clientProxies.end(); iter++) {
                clientProxy = iter->second;
                
                if (clientProxy->id != avatarId) // dont send again to the 'sender' client
                    clientProxy->addAvatar(avatarId, loc, ori);
                else
                    clientProxy->receiveId(avatarId);
            }
        }
        else if (strcmp( messageName, REMOVE_AVATAR))
        {
            args >> avatarId >> osc::EndMessage;
            for (iter = clientProxies.begin(); iter != clientProxies.end(); iter++) {
                clientProxy = iter->second;
                
                if (clientProxy->id != avatarId) // dont send again to the 'sender' client
                    clientProxy->removeAvatar(avatarId);
            }
        }
    }
    catch( osc::Exception& e ){
        // any parsing errors such as unexpected argument types, or
        // missing arguments get thrown as exceptions.
        std::cout << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}

////////////////////////////////////////////////////////////////////////

SWIRLServerProxy::SWIRLServerProxy(const char* anAddress, int aPort) : SWIRLEntityProxy(anAddress, aPort)
{
}

void SWIRLServerProxy::addAvatar(const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( ADD_AVATAR )
    << clientAddress << clientPort
    << loc.x << loc.y << loc.z
    << ori.x << ori.y << ori.z
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
    
    //TODO receive back avatarId
}

void SWIRLServerProxy::removeAvatar(int avatarId)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( REMOVE_AVATAR )
    << avatarId
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

void SWIRLServerProxy::perform(int avatarId, const char* messageName, float parameter)
{
    char buffer[MAX_PACKET_SIZE];
    osc::OutboundPacketStream oscOutstream( buffer, MAX_PACKET_SIZE);
    
    oscOutstream
    << osc::BeginBundleImmediate
    << osc::BeginMessage( messageName )
    << avatarId
    << parameter
    << osc::EndMessage
    << osc::EndBundle;
    
    transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

////////////////////////////////////////////////////////////////////////


SWIRLClient::SWIRLClient(const char* anAddress, int aPort, const char* serverAddress, int serverPort,
                         Vector3D avatarLoc, Vector3D avatarOri) : SWIRLMessageListener(anAddress, aPort)
{
    serverProxy = new SWIRLServerProxy(serverAddress, serverPort);
    
    myAvatar = addAvatar( 0, avatarLoc, avatarOri); //id unknown yet, will be known at receiveId
    
    serverProxy->addAvatar(anAddress, aPort, avatarLoc, avatarOri);
}

SWIRLClient::~SWIRLClient()
{
    std::map<int, SWIRLAvatar*>::iterator iter;

    for (iter = avatars.begin(); iter != avatars.end(); iter++) {
        if (iter->second)
            delete iter->second;
    }
    if (serverProxy)
        delete serverProxy;
}

SWIRLAvatar* SWIRLClient::addAvatar(int id, Vector3D loc, Vector3D ori)
{
    SWIRLAvatar * avatar = new SWIRLAvatar( id, loc); //TODO we can have ori too
    avatars[id] = avatar;
    return avatar;
}

void SWIRLClient::removeAvatar(int id)
{
    std::map<int, SWIRLAvatar*>::iterator iter;

    iter = avatars.find(id);
    avatars.erase(iter);
}

void SWIRLClient::ProcessMessage( const osc::ReceivedMessage& m,
                                 const IpEndpointName& remoteEndpoint )
{
    osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
    char messageName[MAX_MESSAGE_NAME_SIZE];
    SWIRLAvatar* avatar;
    std::map<int, SWIRLAvatar*>::iterator iter;

    int avatarId;
    int i;
    
    strcpy(messageName, m.AddressPattern());
    
    try {
        if ( !strcmp( messageName, MOVE ) ||
            !strcmp( messageName, TURN ) ||
            !strcmp( messageName, STRAFE ) ) {
            
            float f;
            args >> avatarId >> f >> osc::EndMessage;;
            
            iter = avatars.find(avatarId);
            avatar = iter->second;
            if (!strcmp( messageName, MOVE ))
                avatars[i]->move(f);
            else if (!strcmp( messageName, TURN ))
                avatars[i]->turn(f);
            else if (!strcmp( messageName, STRAFE ))
                avatars[i]->strafe(f);
        }
        else if (!strcmp( messageName, ADD_AVATAR ))
        {
            Vector3D loc;
            Vector3D ori;
            
            args >> avatarId >> loc.x >> loc.y >> loc.z >> ori.x >> ori.y >> ori.z >> osc::EndMessage;
            
            addAvatar(avatarId, loc, ori);
            
        }
        else if (!strcmp( messageName, REMOVE_AVATAR))
        {
            args >> avatarId >> osc::EndMessage;
            removeAvatar(avatarId);
        }
        else if (!strcmp( messageName, RECEIVE_ID))
        {
            args >> avatarId >> osc::EndMessage;
            myAvatar->id = avatarId;
        }
    }
    catch( osc::Exception& e ){
        // any parsing errors such as unexpected argument types, or
        // missing arguments get thrown as exceptions.
        std::cout << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}


