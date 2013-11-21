//-----------------------------------------------------------------------------
// name: swirl-entity.cpp
// desc:
//
// author:
//   date: 2013
//-----------------------------------------------------------------------------
#include "swirl-entity.h"
#include "swirl-globals.h"
using namespace std;

//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void SWIRLTeapot::render()
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
