//----------------------------------------------------------------------------
// Name: swirl-globals.h
// Desc: Global stuff for swirl
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#ifndef __SWIRL_GLOBALS_H__
#define __SWIRL_GLOBALS_H__

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

// Defines
#define SWIRL_SRATE        44100
#define SWIRL_FRAMESIZE    1024
#define SWIRL_NUMCHANNELS  2

#define ADDRESS "127.0.0.1"
#define PORT 7000

// forward reference
class SWIRLSim;

//TODO
YEntity* getAvatar();

//-----------------------------------------------------------------------------
// Name: class Globals
// Desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // Top level root simulation
    static SWIRLSim * sim;

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
    static Vector3D skyBlue;
    static Vector3D nightSky;
    static Vector3D sunset;
    static Vector3D darkMossGreen;
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
