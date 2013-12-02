//----------------------------------------------------------------------------
// Name: swirl-gfx.h
// Desc: Graphics stuff for swirl visualization
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#ifndef __SWIRL_GFX_H__
#define __SWIRL_GFX_H__

#include "swirl-globals.h"
#include "swirl-sim.h"
#include "swirl-audio.h"
#include "swirl-networking.h"

#include "x-def.h"
#include "x-fun.h"
#include "x-gfx.h"
#include "x-loadlum.h"
#include "x-vector3d.h"

#include <string>
#include <iostream>
#include <vector>

//TODO networking -- see also swirl-networking
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"

#define ADDRESS "127.0.0.1"
#define PORT 7000

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------

// Entry point for graphics
bool swirl_gfx_init( int argc, const char ** argv );
void swirl_gfx_loop();
void swirl_about();
void swirl_keys();
void swirl_help();
void swirl_usage();
void swirl_endline();
void swirl_line();
//bool swirl_initTexture( const std::string & filename, XTexture * tex );
//XTexture * swirl_loadTexture( const std::string & filename );

// User movement functions
void strafe(float amount);
void strafe_left();
void strafe_right();
void move(float amount);
void move_forward();
void move_back();
void turn(float radAmount);
void turn_left();
void turn_right();

// GL stuff
void idleFunc();
void displayFunc();
void reshapeFunc( int width, int height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );

void initialize_graphics();
void initialize_simulation();
bool initialize_data();
//void loadTextures();
//bool checkTexDim( int dim );
//void setupTexture( const char * filename, GLuint texobj,
                  //GLenum minFilter, GLenum maxFilter,
                  //int useMipMaps = 0 );

void renderBackground();
void blendPane();
void updateNodeEntities();
void renderNodeEntities();

#endif
