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


// entry point for graphics
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


#endif
