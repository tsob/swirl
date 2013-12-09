//----------------------------------------------------------------------------
// Name: swirl-networking.h
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#ifndef __SWIRL_NETWORKING_H__
#define __SWIRL_NETWORKING_H__

#include <pthread.h>
#include <map>

#include "swirl-globals.h"
#include "swirl-entity.h"
#include "UdpSocket.h"
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

#define MAX_ADDRESS_LENGTH 15


class NetworkLocation
{
public:
    NetworkLocation(const char* anAddress, int aPort) : port(aPort) { strcpy(address, anAddress); }

    char address[MAX_ADDRESS_LENGTH];
    int port;
};

class SWIRLEntityProxy
{
public:
    SWIRLEntityProxy(const char* anAddress, int aPort);
    virtual ~SWIRLEntityProxy();

    int id;
    NetworkLocation networkLocation;
    UdpTransmitSocket* transmitSocket;
};


class SWIRLMessageListener : public osc::OscPacketListener
{
public:
    SWIRLMessageListener(const char* anAddress, int aPort);
    void startListenerThread();

    NetworkLocation networkLocation;

protected:
    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );
};
void* listen(void * args);


class SWIRLClientProxy : public SWIRLEntityProxy
{
public:
    SWIRLClientProxy(const char* anAddress, int aPort);

    void addEntity( const char* entityClassName, int entityId, Vector3D loc, Vector3D ori);
    void removeEntity(int entityId);
    void receiveId(int entityId);
    void perform(int entityId, const char* messageName, float parameter);
};


class SWIRLServer : public SWIRLMessageListener {
public:
    SWIRLServer(const char* anAddress, int aPort);
    virtual ~SWIRLServer();
    int maxEntityId;

    int addEntity(const char* entityClassName, Vector3D loc, Vector3D ori); //also create a new entityId
    void removeEntity(int entityId);

private:
    int addClientProxy(const char* clientAddress, int clientPort); //TODO id will be se later
    void removeClientProxy(int id);

    int addClient(const char* avatarClassName, const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori);
    void removeClent(int entityId);

    void addEntity(const char* entityClassName, int entityId, Vector3D loc, Vector3D ori);

    std::map <int, SWIRLClientProxy*> clientProxies;
    std::map<int, SWIRLEntity*> entities;
    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );

};

class SWIRLServerProxy : public SWIRLEntityProxy
{
public:
    SWIRLServerProxy(const char* anAddress, int aPort);

public:
    void addClient(const char* entityClassName, const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori);
    void removeClient(int entityId);

    void addEntity(const char* entityClassName, Vector3D loc, Vector3D ori);
    void removeEntity(int entityId);

    void perform(int entityId, const char* messageName, float parameter);
};

class SWIRLClient : public SWIRLMessageListener {
public:
    SWIRLClient(const char* entityClassName, const char* anAddress, int aPort, const char* serverAddress, int serverPort,
                Vector3D avatarLoc, Vector3D avatarOri);
    ~SWIRLClient();

    void removeEntity(int id);

    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );

    int id;
    SWIRLServerProxy* serverProxy;

    std::map<int, SWIRLEntity*> entities;
    SWIRLEntity* myAvatar;

};

#endif



