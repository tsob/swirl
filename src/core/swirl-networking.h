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
    
    void startListenerThread();
};


class SWIRLMessageListener : public osc::OscPacketListener
{
public:
    SWIRLMessageListener(const char* anAddress, int aPort);
    void startListenerThread();
    
    NetworkLocation networkLocation;
    
private:
    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );
};

void* listen(void * args);

class SWIRLClientProxy : public SWIRLEntityProxy
{
public:
    SWIRLClientProxy(const char* anAddress, int aPort);
    
    void addAvatar( int avatarId, Vector3D loc, Vector3D ori);
    void removeAvatar(int avatarId);
    void receiveId(int avatarId);
    void perform(int avatarId, const char* messageName, float parameter);
};


class SWIRLServer : public SWIRLMessageListener {
public:
    SWIRLServer(const char* anAddress, int aPort);
    virtual ~SWIRLServer();
    
    int addClientProxy(const char* clientAddress, int clientPort); //TODO id will be se later
    void removeClientProxy(int id);
    
private:
    std::map <int, SWIRLClientProxy*> clientProxies;
    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );
    
};

class SWIRLServerProxy : public SWIRLEntityProxy
{
public:
    SWIRLServerProxy(const char* anAddress, int aPort);
    
public:
    void addAvatar(const char* clientAddress, int clientPort, Vector3D loc, Vector3D ori);
    void removeAvatar(int avatarId);
    void perform(int avatarId, const char* messageName, float parameter);
};

class SWIRLClient : public SWIRLMessageListener {
public:
    SWIRLClient(const char* anAddress, int aPort, const char* serverAddress, int serverPort,
                Vector3D avatarLoc, Vector3D avatarOri);
    ~SWIRLClient();
    
    SWIRLAvatar* addAvatar(int id, Vector3D loc, Vector3D ori);
    void removeAvatar(int id);
    
    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint );
    
    SWIRLServerProxy* serverProxy;
    
    std::map<int, SWIRLAvatar*> avatars;
    SWIRLAvatar* myAvatar;
    
};

#endif



