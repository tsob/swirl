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

//-----------------------------------------------------------------------------
// Name: swirl_networking_init()
// Desc: initialize networking interface
//-----------------------------------------------------------------------------
bool swirl_networking_init( int argc, const char ** argv )
{
    //TODO change for client-server
    if( argc < 3 ){
      cerr << "[swirl]: please add send and receive ports as arguments" << endl;
      return false;
    }

    //TODO - networking development
    int sendPort = atoi(argv[1]);
    int receivePort = atoi(argv[2]);

    if (sendPort == 6000)
    {
        Globals::app = 1;
        cout << "ONE\n";
    }
    else
    {
        Globals::app = 2;
        cout << "TWO\n";
    }

    UdpTransmitSocket* transmitSocket = getTransmitSocket( ADDRESS, sendPort);

    //TODO
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, oscListener, &receivePort);

    return true;
}

//-----------------------------------------------------------------------------
// Name: oscListener()
// Desc: ...
//-----------------------------------------------------------------------------
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

//TODO
//-----------------------------------------------------------------------------
// Name: transmitSocket
// Desc: ...
//-----------------------------------------------------------------------------
/*static*/ UdpTransmitSocket* transmitSocket = NULL;

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket()
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket()
{
    return transmitSocket;
}

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket(const char* address, int port)
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket(const char* address, int port)
{
    //TODO
    if (!transmitSocket)
        transmitSocket = new UdpTransmitSocket( IpEndpointName(address, port) );
    return transmitSocket;

}
