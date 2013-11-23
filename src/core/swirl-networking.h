//----------------------------------------------------------------------------
// Name: swirl-networking.h
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include <pthread.h> //TODO necessary?
#include "swirl-globals.h"
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
void * oscListener(void * args);
void * swirl_send_message(const char* label, float value);

UdpTransmitSocket* getTransmitSocket();
UdpTransmitSocket* getTransmitSocket(const char* address, int port);

struct NetworkLocation;
//{
    //const char address[15];
    //int port;
//};
