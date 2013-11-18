//-----------------------------------------------------------------------------
// name: raka-gfx.h
// desc: graphics stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
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
bool raka_initTexture( const std::string & filename, XTexture * tex );
XTexture * raka_loadTexture( const std::string & filename );




#endif
