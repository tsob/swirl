//----------------------------------------------------------------------------
// Name: swirl-globals.cpp
// Desc: Global stuff for swirl
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#include "swirl-globals.h"
#include "swirl-entity.h"
#include "y-entity.h"


// defaults
#define DEFAULT_FULLSCREEN    FALSE
#define DEFAULT_WINDOW_WIDTH  600
#define DEFAULT_WINDOW_HEIGHT 400
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_FOG           TRUE
#define DEFAULT_VERSION       "0.0.1"

SWIRLSim * Globals::sim     = NULL;

YEntity* avatar = NULL;

YEntity* getAvatar()
{
    //TODO
    if (!avatar)
    {
        avatar = new SWIRLTeapot();
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


GLsizei Globals::windowWidth      = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight     = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth  = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;


// AUDIO STUFF ----------------------------------------------------------------
// fluidsynth
GeXFluidSynth * Globals::synth = new GeXFluidSynth;

// STK
YEcho * Globals::echo; // initialized in swirl-audio.cpp

SAMPLE *     Globals::lastAudioBuffer         = NULL;
SAMPLE *     Globals::lastAudioBufferMono     = NULL;
SAMPLE *     Globals::audioBufferWindow       = NULL;
unsigned int Globals::lastAudioBufferFrames   = 0;
unsigned int Globals::lastAudioBufferChannels = 0;
YWaveform *  Globals::waveform                = NULL;
//-----------------------------------------------------------------------------

GLboolean Globals::fullscreen  = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

// Colors
Vector3D Globals::skyBlue(  0.529, 0.8078, 0.9215);
Vector3D Globals::nightSky(      0,    0,     0);
Vector3D Globals::sunset(        1.0,  0.4784, 0.133);
Vector3D Globals::darkMossGreen( 0.29, 0.365, 0.137);
Vector3D Globals::ourWhite(      1.0, 1.0,  1.0  );
Vector3D Globals::ourRed(        1.0, 0.5,  0.5  );
Vector3D Globals::ourBlue(       0.4, 0.8,  1.0  );
Vector3D Globals::ourOrange(     1.0, 0.75, 0.25 );
Vector3D Globals::ourGreen(      0.7, 1.0,  0.45 );
Vector3D Globals::ourGray(       0.4, 0.4,  0.4  );
Vector3D Globals::ourYellow(     1.0, 1.0,  0.25 );
Vector3D Globals::ourSoftYellow( 0.7, 0.7,  0.1  );
Vector3D Globals::ourPurple(     0.6, 0.25, 0.6  );


Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat  Globals::blendRed = 0.0f;
GLenum   Globals::fillmode = GL_FILL;
iSlew3D  Globals::bgColor( Globals::skyBlue, 10.0 );
Vector3D Globals::viewRadius( 5, 2, 1 );
Vector3D Globals::viewEyeY( 2, 0, 1.5f );

int Globals::app = 0;


SWIRLCamera Globals::swirlCamera;

//TODO
//SWIRLCamera * Globals::swirlCamera =  new SWIRLCamera;
//Vector3D Globals::cameraEye( 0, 0, -1 );
//Vector3D Globals::cameraReference( 0, 0, 0 );

//TODO
//SWIRLCamera Globals::swirlCamera.eye(0, 0, -1);
//SWIRLCamera Globals::swirlCamera.reference(0, 0, 0);


//Vector3D Globals::cameraEye( 0, 0, -1 );
//Vector3D Globals::cameraReference( 0, 0, 0 );

Vector3D Globals::fov( 80, 60, .2f );

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


// Paths
std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;
