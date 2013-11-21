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

#include "x-def.h"
#include "x-gfx.h"
#include <string>

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
void strafe_left();
void strafe_right();
void move_forward();
void move_back();
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
