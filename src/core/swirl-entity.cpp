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
using namespace stk;


//-------------------------------------------------------------------------------
// Name: class: SWIRLEntity method: tick()
// Desc: ...
//-------------------------------------------------------------------------------
SAMPLE SWIRLEntity::tick( SAMPLE input )
{
  //return unitGenerator->tick( input );
  return 0.0;
}

//-------------------------------------------------------------------------------
// Name: class: SWIRLEntity method: tickAll()
// Desc: Get one audio frame from this and every child
//-------------------------------------------------------------------------------
void SWIRLEntity::tickAll( SAMPLE * oneFrame, Vector3D listenerPosition )
{
    // oneFrame is the buffer we'll fill with the combined audio frame
    // For now, just do mono without spatialization

    // Stop if this entity is not active.
    if( !active )
        return;

    // render self if not hidden
    if( !hidden )
    {
        // TODO sanity check argument
        oneFrame[0] += this->tick( (SAMPLE)1 );

        // Mono expansion
        for (int i = 1; i < SWIRL_NUMCHANNELS; ++i)
        {
          oneFrame[i] = oneFrame[0];
        }
    }

    // tick children
    for( vector<YEntity *>::iterator itr = children.begin();
         itr != children.end(); itr++ )
    {
        if (dynamic_cast<SWIRLEntity *>(*itr))
          ((SWIRLEntity*)*itr)->tickAll(oneFrame, listenerPosition);
    }

}

//-----------------------------------------------------------------------------
// Name: class: SWIRLCamera method: render()
// Desc: ...
//-----------------------------------------------------------------------------
void SWIRLCamera::render()
{
  // Do nothing?
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLTeapot method: render()
// Desc: ...
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Name: class: SWIRLMoon method: render()
// Desc: renders the SWIRLMoon
//-----------------------------------------------------------------------------
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
// Name: class: SWIRL* method: desc()
// Desc: Returns a description (usually type and name) of this object. Useful
//       for debugging stuff.
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
// Name: class: SWIRLCamera method: update( YTimeInterval dt )
// Desc: updates (interpolates) camera position
//-----------------------------------------------------------------------------
void SWIRLCamera::update( YTimeInterval dt )
{

    iLoc.update(Globals::cameraEye);
    iRefLoc.update(Globals::cameraReference);

    // interp
    iLoc.interp( dt );
    iRefLoc.interp( dt );

    // update
    loc = iLoc.actual();
    refLoc = iRefLoc.actual();

}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLBirdy constructor
// Desc: ...
//-----------------------------------------------------------------------------
SWIRLBirdy::SWIRLBirdy()
{
    // Instantiate fluidsynth
    GeXFluidSynth * synth = new GeXFluidSynth();
    // Init fluidsynth
    synth->init( SWIRL_SRATE, 32 );
    // Load the soundfont
    synth->load( "data/soundfonts/birds.sf2", "" );
    // Map program changes
    synth->programChange( 0, 0 );
}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLBirdy method: tick()
// Desc: return a sample of audio from SWIRLBirdy
//-----------------------------------------------------------------------------
SAMPLE SWIRLBirdy::tick( SAMPLE input )
{
  SAMPLE oneFrame[2];
  synth->synthesize2( oneFrame, 1);
  return oneFrame[0];
}
