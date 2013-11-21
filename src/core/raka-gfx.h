//----------------------------------------------------------------------------
// Name: raka-gfx.h
// Desc: Graphics stuff for swirl visualization
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#ifndef __RAKA_GFX_H__
#define __RAKA_GFX_H__

#include "x-def.h"
#include "x-gfx.h"
#include <string>


// entry point for graphics
bool raka_gfx_init( int argc, const char ** argv );
void raka_gfx_loop();
void raka_about();
void raka_keys();
void raka_help();
void raka_usage();
void raka_endline();
void raka_line();
//bool raka_initTexture( const std::string & filename, XTexture * tex );
//XTexture * raka_loadTexture( const std::string & filename );

// User movement functions
void strafe_left();
void strafe_right();
void move_forward();
void move_back();
void turn_left();
void turn_right();


#endif
