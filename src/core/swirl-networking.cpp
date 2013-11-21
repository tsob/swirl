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

using namespace std;

void * oscListener(void * args)
{
    int receivePort = ((int*)args)[0];

    ExamplePacketListener listener;
    UdpListeningReceiveSocket s(
        IpEndpointName(
          IpEndpointName::ANY_ADDRESS,
          receivePort
          ),
        &listener
        );

    s.RunUntilSigInt();
    pthread_exit(NULL);
}

