//-----------------------------------------------------------------------------
// name: raka-entity.cpp
// desc: entities for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-entity.h"
#include "raka-globals.h"
#include "x-fun.h"
using namespace std;

//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void RAKATeapot::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    
    //TODO
    if (Globals::app == 1)
        glColor4f( col.x, 0, col.z, alpha );
    else
        glColor4f( 0, col.y, col.z, alpha );

    // render stuff
    glutSolidTeapot( 1.0 );
    // disable lighting
    glDisable( GL_LIGHTING );
}
