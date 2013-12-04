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
#include <pthread.h> //TODO necessary?
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

using namespace std;


//----------------------------------------------------------------------------
// Name: main()
// Desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    bool isClientApplication = true;
    
    if (argv[1][0] == 's') //server
    {
        if( argc < 3 ){
            cerr << "[swirl]: please add receive address and port as arguments" << endl;
            return -1;
        }
        
        const char* receiveAddress = argv[2];
        int receivePort = atoi(argv[3]);
        
        Globals::application = new SWIRLServer(receiveAddress, receivePort);
        Globals::application->startListenerThread();
        
        isClientApplication = false;
    }
    else if (argv[1][0] == 'c')
    {
        if( argc < 5 ){
            cerr << "[swirl]: please add send and receive addresses and ports as arguments" << endl;
            return -1;
        }
        
        const char* sendAddress = argv[2];
        int sendPort = atoi(argv[3]);
        const char* receiveAddress = argv[4];
        int receivePort = atoi(argv[5]);
        
        Vector3D avatarLoc;
        Vector3D avatarOri;
        
        Globals::application = new SWIRLClient (receiveAddress, receivePort,
                                                sendAddress, sendPort,
                                                avatarLoc, avatarOri);
        
        Globals::application->startListenerThread();
    }
    else {
        cout << "please specify the first argument: 's'=sever, 'c'=client'";
        return -1;
    }
    
    if (isClientApplication)
    {
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
        
        ((SWIRLClient*)Globals::application)->serverProxy->removeAvatar(
                                                                        ((SWIRLClient*)Globals::application)->myAvatar->id
                                                                        );
    }
    
    if (Globals::application)
        delete Globals::application;
    
    return 0;
}
