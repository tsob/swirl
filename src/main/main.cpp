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

using namespace std;

//----------------------------------------------------------------------------
// Name: main()
// Desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    // Initialize networking

    //TODO change for client-server
    if( argc < 3 ){
      cerr << "[swirl]: please add send and receive ports as arguments" << endl;
      return -1;
    }

    //TODO - networking development
    int sendPort = atoi(argv[1]);
    int receivePort = atoi(argv[2]);

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

    UdpTransmitSocket* transmitSocket = getTransmitSocket( ADDRESS, sendPort);
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, oscListener, &receivePort);


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
