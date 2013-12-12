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
#include "swirl-fountain.h"

using namespace std;

XMutex mutex;

class MyServer : public SWIRLServer
{
public:
    MyServer(const char* anAddress, int aPort) : SWIRLServer(anAddress, aPort)
    {
       // addEntity("SWIRLBirdCube", Vector3D(1, 0, 2), Vector3D(0, 0, 0));
        
        //1 5 7 8
        int i;
        
        for (i = 0; i < 3; i++)
        {
            addEntity( "SWIRLFountain1", Vector3D(i - 5, 0, i - 5), Vector3D(0, 0, 0) );
           
        }
        
        for (i = 0; i < 2; i++)
        {
            addEntity( "SWIRLFountain2", Vector3D(1 - i, 0, 2), Vector3D(0, 0, 0) );
        }
        
        for (i = 0; i < 2; i++)
        {
            addEntity( "SWIRLFountain3", Vector3D(2 - i * 3, 0, 2), Vector3D(0, 0, 0) );
        }
        
        addEntity( "SWIRLFountain4", Vector3D(0, 0, 100), Vector3D(0, 0, 0) );
        
        for (i = 0; i < 3; i++)
        {

            addEntity( "SWIRLFountain5", Vector3D(i - 3, 0, -2), Vector3D(0, 0, 0) );
        }

        for (i = 0; i < 3; i++)
        {
            addEntity( "SWIRLFountain6", Vector3D(2 * i - 5, 0, 4), Vector3D(0, 0, 0) );
            
        }
        
        for (i = 0; i < 2; i++) {
            addEntity( "SWIRLFountain7", Vector3D(2 - i * 3, 0, -1), Vector3D(0, 0, 0) );
        }
        
        addEntity( "SWIRLFountain8", Vector3D(- 5, 0, 0), Vector3D(0, 0, 0) );
        
        for (i = 0; i < 2; i++)
        {
            addEntity( "SWIRLFountain1", Vector3D(i -3, 0, i - 3), Vector3D(0, 0, 0) );
            
        }

    }
    
};

//----------------------------------------------------------------------------
// Name: main()
// Desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    Globals::sim = new SWIRLSim(); //TODO
    
    bool isClientApplication = true;
    
    if (argv[1][0] == 'c')
    {
        cerr << "Creating client..." << endl;
        
        if( argc < 6 ){
            cerr << "[swirl]: please add send and receive addresses, ports and avatar as arguments" << endl;
            return -1;
        }
        
        const char* receiveAddress = argv[2];
        int receivePort = atoi(argv[3]);
        
        const char* sendAddress = argv[4];
        int sendPort = atoi(argv[5]);
        
        Vector3D avatarLoc;
        Vector3D avatarOri;
        
        char* avatarClassNames[3] = {"SWIRLSphereAvatar", "SWIRLConeAvatar", "SWIRLCubeAvatar"};
        Globals::application = new SWIRLClient (avatarClassNames[atoi(argv[6])], receiveAddress, receivePort,
                                                sendAddress, sendPort,
                                                avatarLoc, avatarOri);
        
        
        //TODO Globals::application->startListenerThread();
    }
    else if (argv[1][0] == 's') //server
    {
        cerr << "Creating server..." << endl;
        
        if( argc < 3 ){
            cerr << "[swirl]: please add receive address and port as arguments" << endl;
            return -1;
        }
        
        const char* receiveAddress = argv[2];
        int receivePort = atoi(argv[3]);
        
        Globals::application = new MyServer(receiveAddress, receivePort);
        //TODO Globals::application->startListenerThread();
        
        isClientApplication = false;
    }
    else {
        cerr << "please specify the first argument: 's'=sever, 'c'=client'";
        return -1;
    }
    
    //    if (isClientApplication)
    //  {
    // Initiate graphics setup and loop
    
    //TODO right place?
    //Globals::application->startListenerThread();

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
    
    cout << "audio started " << endl;
    
    // Graphics loop
    swirl_gfx_loop();
    
    // Stop Audio
    swirl_audio_stop();
    
    //  }
    if (!isClientApplication) //server
    {
        while (true);
    }
    
    if (Globals::application)
        delete Globals::application;
    
    return 0;
}
