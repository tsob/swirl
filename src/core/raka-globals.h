//-----------------------------------------------------------------------------
// name: raka-globals.h
// desc: global stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_GLOBALS_H__
#define __RAKA_GLOBALS_H__

// x-api
#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"

// y-api
#include "y-entity.h"
#include "y-waveform.h"

// C++
#include <string>
#include <map>
#include <vector>
#include <utility>

#include <iostream>

// Networking and OSC
#include "UdpSocket.h" //TODO

#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"


// Defines
#define RAKA_SRATE        44100
#define RAKA_FRAMESIZE    1024
#define RAKA_NUMCHANNELS  2


// forward reference
class RAKASim;

//TODO
YEntity* getAvatar();

//TODO
UdpTransmitSocket* getTransmitSocket();
UdpTransmitSocket* getTransmitSocket(const char* address, int port);

class ExamplePacketListener : public osc::OscPacketListener {
protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint )
    {
        (void) remoteEndpoint; // suppress unused parameter warning

        try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.

            /*if( std::strcmp( m.AddressPattern(), "/cameraReferenceZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;
                args >> f >> osc::EndMessage;

                std::cout << "received '/cameraReferenceZ' message with arguments: "
                << f << "\n";

                //TODO
                getAvatar()->loc.x = f;

            } else */ if( std::strcmp( m.AddressPattern(), "/cameraEyeZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->loc.z = f;

               // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraEyeX" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->loc.x = f;

                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraReferenceX" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->ori.y += (180.0f / ONE_PI * f);


                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }
            else if( std::strcmp( m.AddressPattern(), "/cameraReferenceZ" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                float f;

                std::cout << "RECEVING \n";

                args >> f >> osc::EndMessage;

                std::cout << f << std::endl;

                getAvatar()->ori.y += (180 / (3.14f) * f);

                // getAvatar()->loc.y = f;


                std::cout << "received '/cameraEyeZ' message with arguments: "
                << f << "\n";
            }

        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
            << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};


//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // Top level root simulation
    static RAKASim * sim;

    static int app ;

    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;

    // last audio buffer
    static SAMPLE * lastAudioBuffer;
    static SAMPLE * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;

    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;

    // waveform
    static YWaveform * waveform;

    // graphics fullscreen
    static GLboolean fullscreen;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;
    // view stuff
    static Vector3D viewRadius;
    static Vector3D viewEyeY;

    //TODO
    static Vector3D cameraEye;
    static Vector3D cameraReference;

    static Vector3D fov;

    // light 0 position
    static GLfloat light0_pos[4];
    // light 1 parameters
    static GLfloat light1_ambient[4];
    static GLfloat light1_diffuse[4];
    static GLfloat light1_specular[4];
    static GLfloat light1_pos[4];
    // line width
    static GLfloat linewidth;
    // do
    static GLboolean fog;
    static GLuint fog_mode[4];   // storage for three/four types of fog
    static GLuint fog_filter;    // which fog to use
    static GLfloat fog_density;  // fog density

    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;


};




#endif
