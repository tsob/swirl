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
#include "raka-audio.h"
#include "raka-gfx.h"
#include "raka-globals.h"

//#include "OscOutboundPacketStream.h"
//#include "UdpSocket.h"


#define ADDRESS "127.0.0.1" //TODO
#define PORT 7000

#define OUTPUT_BUFFER_SIZE 1024 //TODO

using namespace std;


//----------------------------------------------------------------------------
// name: main()
// desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{

    (void) argc; // suppress unused parameter warnings
    (void) argv; // suppress unused parameter warnings

    if( !raka_audio_init(RAKA_SRATE, RAKA_FRAMESIZE, 2) )
    {
      cerr << "[swirl]: cannot initialize audio system..." << endl;
      return -1;
    }

    // Initiate graphics setup and loop
    if( !raka_gfx_init( argc, argv ) )
    {
        cerr << "[swirl]: cannot initialize graphics/data system..." << endl;
        return -1;
    }

    // Start audio
    raka_audio_start();

    // Graphics loop
    raka_gfx_loop();

    // Stop Audio
    raka_audio_stop();

    return 0;
}
