//----------------------------------------------------------------------------
// Name: main.cpp
// Desc: Swirl entry point
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#include <iostream>
#include "swirl-audio.h"
#include "swirl-gfx.h"
#include "swirl-globals.h"
#include "swirl-networking.h"
#include <pthread.h> //TODO necessary?
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

using namespace std;

struct NetworkLocation
{
    char address[15];
    int port;
};

//----------------------------------------------------------------------------
// Name: main()
// Desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    // Initialize networking

    //TODO change for client-server
    if( argc < 5 ){
      cerr << "[swirl]: please add send and receive ports as arguments" << endl;
      return -1;
    }

    //TODO - networking development
    const char* sendAddress = argv[1];
    int sendPort = atoi(argv[2]);
    const char* receiveAddress = argv[3];
    int receivePort = atoi(argv[4]);

    // Hack for peer to peer
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

    UdpTransmitSocket* transmitSocket = getTransmitSocket( sendAddress, sendPort);
    pthread_t listenerThread;
    NetworkLocation receiveLocation;
    strcpy( receiveLocation.address, receiveAddress );
    receiveLocation.port = receivePort;
    pthread_create(&listenerThread, NULL, oscListener, &receiveLocation);

    // Initiate graphics setup and loop
    if( !swirl_gfx_init( argc, argv ) )
    {
        cerr << "[swirl]: cannot initialize graphics/data system..." << endl;
        return -1;
    }

    // Initialize audio
    if( !swirl_audio_init(SWIRL_SRATE, SWIRL_FRAMESIZE, SWIRL_NUMCHANNELS) )
    {
      cerr << "[swirl]: cannot initialize audio system..." << endl;
      return -1;
    }

    // Print about
    swirl_about();
    swirl_endline();

    // Start audio
    swirl_audio_start();

    // Graphics loop
    swirl_gfx_loop();

    // Stop Audio
    swirl_audio_stop();

    return 0;
}
