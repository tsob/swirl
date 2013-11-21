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

//#include "OscOutboundPacketStream.h"
//#include "UdpSocket.h"


#define ADDRESS "127.0.0.1" // Hack for localhost //TODO change for client-server


using namespace std;


//----------------------------------------------------------------------------
// name: main()
// desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    // Networking //TODO change for client-server
    if( argc < 3 ){
      cerr << "[swirl]: please add send and receive ports as arguments" << endl;
      return -1;
    }
    (void) argc; // suppress unused parameter warnings
    (void) argv; // suppress unused parameter warnings

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

    // Start audio
    swirl_audio_start();

    // Graphics loop
    swirl_gfx_loop();

    // Stop Audio
    swirl_audio_stop();

    return 0;
}
