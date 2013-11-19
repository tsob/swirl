//-----------------------------------------------------------------------------
// name: raka-globals.h
// desc: global stuff for visquin visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-globals.h"
#include "y-entity.h"
#include "raka-entity.h"


// defaults
#define DEFAULT_FULLSCREEN    TRUE
#define DEFAULT_WINDOW_WIDTH  600
#define DEFAULT_WINDOW_HEIGHT 400
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_FOG           FALSE
#define DEFAULT_VERSION       "0.0.1"

RAKASim * Globals::sim     = NULL;

//TODO
/*static*/ UdpTransmitSocket* transmitSocket = NULL;
YEntity* avatar = NULL;

YWaveform * Globals::waveform = NULL;

YEntity* getAvatar()
{
    //TODO
    if (!avatar)
    {
        avatar = new RAKATeapot();
        // create test teapot
        // set attributes
        avatar->col = Globals::ourSoftYellow;
        avatar->loc.x = 0;
        avatar->loc.y = 0;

        if (Globals::app == 1)
            avatar->loc.z = -1;
        else
            avatar->loc.z = -2;
    }
    return avatar;
}


//TODO
UdpTransmitSocket* getTransmitSocket()
{
    return transmitSocket;
}

//TODO
UdpTransmitSocket* getTransmitSocket(const char* address, int port)
{
    //TODO
    if (!transmitSocket)
        transmitSocket = new UdpTransmitSocket( IpEndpointName(address, port) );
    return transmitSocket;

}

GLsizei Globals::windowWidth      = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight     = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth  = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;

SAMPLE *     Globals::lastAudioBuffer         = NULL;
SAMPLE *     Globals::lastAudioBufferMono     = NULL;
SAMPLE *     Globals::audioBufferWindow       = NULL;
unsigned int Globals::lastAudioBufferFrames   = 0;
unsigned int Globals::lastAudioBufferChannels = 0;

GLboolean Globals::fullscreen  = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat  Globals::blendRed = 0.0f;
GLenum   Globals::fillmode = GL_FILL;
iSlew3D  Globals::bgColor( .5 );
Vector3D Globals::viewRadius( 5, 2, 1 );
Vector3D Globals::viewEyeY( 2, 0, 1.5f );

int Globals::app = 0;

//TODO
Vector3D Globals::cameraEye( 0, 0, -1 );
Vector3D Globals::cameraReference( 0, 0, 0 );


//Vector3D Globals::cameraEye( 0, 0, -1 );
//Vector3D Globals::cameraReference( 0, 0, 0 );

Vector3D Globals::fov( 80, 100, .2f );

GLfloat   Globals::light0_pos[4]      = {  2.0f, 1.2f,  4.0f, 1.0f };
GLfloat   Globals::light1_ambient[4]  = {   .2f,  .2f,   .2f, 1.0f };
GLfloat   Globals::light1_diffuse[4]  = {  1.0f, 1.0f,  1.0f, 1.0f };
GLfloat   Globals::light1_specular[4] = {  1.0f, 1.0f,  1.0f, 1.0f };
GLfloat   Globals::light1_pos[4]      = { -2.0f, 0.0f, -4.0f, 1.0f };
GLfloat   Globals::linewidth          = 1.0f;
GLboolean Globals::fog                = DEFAULT_FOG;
GLuint    Globals::fog_mode[4];
GLuint    Globals::fog_filter;
GLfloat   Globals::fog_density;

// Colors
Vector3D Globals::ourWhite(      1.0, 1.0,  1.0  );
Vector3D Globals::ourRed(        1.0, 0.5,  0.5  );
Vector3D Globals::ourBlue(       0.4, 0.8,  1.0  );
Vector3D Globals::ourOrange(     1.0, 0.75, 0.25 );
Vector3D Globals::ourGreen(      0.7, 1.0,  0.45 );
Vector3D Globals::ourGray(       0.4, 0.4,  0.4  );
Vector3D Globals::ourYellow(     1.0, 1.0,  0.25 );
Vector3D Globals::ourSoftYellow( 0.7, 0.7,  0.1  );
Vector3D Globals::ourPurple(     0.6, 0.25, 0.6  );

// Paths
std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;
