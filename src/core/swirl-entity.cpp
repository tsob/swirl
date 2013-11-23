//-----------------------------------------------------------------------------
// Name: swirl-entity.cpp
// desc:
//
// author:
//   date: 2013
//-----------------------------------------------------------------------------
#include "swirl-entity.h"
#include "swirl-globals.h"
using namespace std;

//-------------------------------------------------------------------------------
// Name: render()
// Desc: ...
//-------------------------------------------------------------------------------
void SWIRLCamera::render()
{
  // Do nothing?
}

void SWIRLTeapot::render()
{
    glPushMatrix( );
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
    glPopMatrix( );
}

void SWIRLMoon::render()
{
    // Draw the moon
    glPushMatrix( );
        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        glRotatef(5.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(20.0f, -1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 200.0f);
        glutSolidSphere( 20.0f, 24, 24 );
    glPopMatrix( );
}

//-----------------------------------------------------------------------------
// Name: desc()
// Desc: Returns a description (usually type and name) of this object. Useful
// for debugging stuff.
//-----------------------------------------------------------------------------
std::string SWIRLCamera::desc() const
{
    return "YEntity [SWIRLCamera]";
}
std::string SWIRLMoon::desc() const
{
    return "YEntity [SWIRLMoon]";
}
std::string SWIRLTeapot::desc() const
{
    return "YEntity [SWIRLTeapot]";
}

//-----------------------------------------------------------------------------
// Name: update( YTimeInterval dt )
// Desc: ...
//-----------------------------------------------------------------------------
void SWIRLCamera::update( YTimeInterval dt )
{
  // Do nothing?
}
