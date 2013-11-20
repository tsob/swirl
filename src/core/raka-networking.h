//----------------------------------------------------------------------------
// Name: raka-networking.h
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "raka-globals.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"

#define ADDRESS "127.0.0.1"
#define PORT 7000

// Function declaration
void * oscListener(void * args);
