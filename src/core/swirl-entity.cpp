//-----------------------------------------------------------------------------
// Name: swirl-entity.cpp
// desc:
//
// author:
//   date: 2013
//-----------------------------------------------------------------------------
#include "swirl-entity.h"
#include "swirl-globals.h"
#include "swirl-networking.h"
#include <math.h>
#include "x-fun.h"

using namespace std;
using namespace stk;

//-----------------------------------------------------------------------------
// name: g_squareVertices
// desc: vertices for a cube
//-----------------------------------------------------------------------------
static const GLfloat g_squareVertices[] =
{
    // FRONT
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    // BACK
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    // LEFT
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    // RIGHT
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    // TOP
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    // BOTTOM
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
};

//-----------------------------------------------------------------------------
// name: g_squareNormals
// desc: normals for a cube
//-----------------------------------------------------------------------------
static const GLfloat g_squareNormals[] =
{
    // FRONT
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    // BACK
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    // LEFT
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    // RIGHT
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    // TOP
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    // BOTTOM
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0
};

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
        {
          ((SWIRLEntity*)*itr)->tickAll(oneFrame, listenerPosition);
        }
    }

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
        // enable lighting
        glEnable( GL_LIGHTING );

        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        glRotatef(5.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(20.0f, -1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 200.0f);
        glutSolidSphere( 20.0f, 24, 24 );

        // disable lighting
        glDisable( GL_LIGHTING );
    glPopMatrix( );
}

//-----------------------------------------------------------------------------
// Name: class: SWIRL* method: desc()
// Desc: Returns a description (usually type and name) of this object. Useful
//       for debugging stuff.
//-----------------------------------------------------------------------------
std::string SWIRLEntity::desc() const
{
    return "SWIRLEntity [unnamed]";
}
std::string SWIRLBirdCube::desc() const
{
    return "SWIRLEntity [SWIRLBirdCube]";
}
std::string SWIRLAvatar::desc() const
{
    return "SWIRLEntity [SWIRLAvatar]";
}
std::string SWIRLCamera::desc() const
{
    return "SWIRLEntity [SWIRLCamera]";
}
std::string SWIRLMoon::desc() const
{
    return "SWIRLEntity [SWIRLMoon]";
}
std::string SWIRLTeapot::desc() const
{
    return "SWIRLEntity [SWIRLTeapot]";
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
void SWIRLCamera::update( YTimeInterval dt )
{
    // interpolate relative camera position
    relativePosition.interp(dt);

    // Vector pointing in the look direction
    Vector3D lookVector = Globals::myAvatar->loc- Globals::myAvatar->refLoc;
    lookVector.normalize();
    
    // rotate relative position for absolute displacement
    float theta = lookVector.angleXZ() + 0.5*SWIRL_PI;
    Vector3D displacementVector = Vector3D(
        relativePosition.actual().x * cos(theta) - relativePosition.actual().z * sin(theta),
        relativePosition.actual().y,
        relativePosition.actual().x * sin(theta) + relativePosition.actual().z * cos(theta)
        );

    absLoc = Globals::myAvatar->loc + displacementVector;
    loc = relativePosition.actual();
}

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
void SWIRLCamera::togglePosition()
{
    static int position=1;

    if(position==3)
    {
      // third to first person
      relativePosition.update( Globals::firstPerson );
      position = 1;
    }
    else
    {
      // first to third person
      relativePosition.update( Globals::thirdPerson );
      position = 3;
    }
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLAvatar method: update( YTimeInterval dt )
// Desc: updates (interpolates) avatar position
//-----------------------------------------------------------------------------
void SWIRLAvatar::update( YTimeInterval dt )
{

    iLoc.update( goal );
    iRefLoc.update( refGoal );

    // interp
    iLoc.interp( dt );
    iRefLoc.interp( dt );

    // update
    loc    = iLoc.actual();
    refLoc = iRefLoc.actual();

    ori.x = 0.0f;
    ori.y = (-180.0f / 3.1415927) * (refLoc-loc).angleXZ() + 90.0f;
    ori.z = 0.0f;

    //Globals::waveform->loc = loc + (refLoc-loc)*0.1 + Vector3D(0.0f, 0.6f, 0.0f);
    //Globals::waveform->ori = ori;
}

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLAvatar::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );

    // enable state
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    // set vertex pointer
    glVertexPointer( 3, GL_FLOAT, 0, g_squareVertices );
    glNormalPointer( GL_FLOAT, 0, g_squareNormals );

    // push
    glPushMatrix();
    // scale
    glScalef( size.value, size.value, size.value );

    // draw it
    //glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 16, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 20, 4 );
    glutSolidTeapot( size.magnitude() );

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLAvatar constructor
// Desc: 
//-----------------------------------------------------------------------------
SWIRLAvatar::SWIRLAvatar( Vector3D startingLocation )
{
    size = Vector3D(1, 1, 1.0f); 
    loc  = startingLocation;
    refLoc = loc + Vector3D(0.0f, 0.0f, 10.0f);
    col = Globals::ourOrange;
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLAvatar method: move( amount )
// Desc: 
//-----------------------------------------------------------------------------
void SWIRLAvatar::move( float amount )
{
   Vector3D movementVector = refLoc - loc;
   movementVector.normalize();
   movementVector *= amount;

   goal    += movementVector;
   refGoal += movementVector;

   // TODO
   //swirl_send_message( "/move", amount );
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLAvatar method: turn( radAmount )
// Desc: 
//-----------------------------------------------------------------------------
void SWIRLAvatar::turn( float radAmount )
{
   Vector3D lookVector = refLoc - loc;
   float tmpRefX = lookVector.x;
   float tmpRefZ = lookVector.z;

   lookVector.x = tmpRefX * cos(radAmount)
                  - tmpRefZ * sin(radAmount);

   lookVector.z = tmpRefX * sin(radAmount)
                  + tmpRefZ * cos(radAmount);

   refGoal = loc + lookVector;
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLAvatar method: strafe( amount )
// Desc: 
//-----------------------------------------------------------------------------
void SWIRLAvatar::strafe( float amount )
{
   Vector3D movementVector = refLoc - loc;
   // Rotate movementVector by 90 degrees
   float tmpX = movementVector.x;
   float tmpZ = movementVector.z;

   movementVector.x = - tmpZ;
   movementVector.z = tmpX;

   movementVector.normalize();
   movementVector *= amount;

   refGoal += movementVector;
   goal    += movementVector;

   // TODO
   //swirl_send_message( "/strafe", -0.1f );
   //swirl_send_message( "/strafe", amount );

}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLBirdy constructor
// Desc: ...
//-----------------------------------------------------------------------------
SWIRLFluid::SWIRLFluid()
{
    printf("About to instantiate SWIRLFluid\n");
    // Instantiate fluidsynth
    synth = new GeXFluidSynth();
    // Init fluidsynth
    synth->init( SWIRL_SRATE, 32 );
    // Load the soundfont
    //synth->load( "data/soundfonts/birds.sf2", "" );
    synth->load( "data/soundfonts/CasioVL-1.sf2", "" );
    // Map program changes
    synth->programChange( 0, 0 );
    printf("Instantiated SWIRLFluid\n");
}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLBirdy method: tick()
// Desc: return a sample of audio from SWIRLBirdy
//-----------------------------------------------------------------------------
SAMPLE SWIRLFluid::tick( SAMPLE input )
{
  SAMPLE oneFrame[2];
  synth->synthesize2( oneFrame, 1);
  return 0.5*oneFrame[0];
}

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLCube::render()
{
    // enable state
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    // set vertex pointer
    glVertexPointer( 3, GL_FLOAT, 0, g_squareVertices );
    glNormalPointer( GL_FLOAT, 0, g_squareNormals );

    // push
    glPushMatrix();
    // scale
    glScalef( size.value, size.value, size.value );

    // draw it
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 16, 4 );
    glDrawArrays( GL_TRIANGLE_STRIP, 20, 4 );

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
}




//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLCube::update( YTimeInterval dt )
{
    // interp
    size.interp( dt );
}

//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLBirdCube::update( YTimeInterval dt )
{
    static int counter = 0;
    int timeout = 24;

    // interp
    size.interp( dt );

    if(counter<=0)
    {
        if( (loc - Globals::myAvatar->loc).magnitude() < size.magnitude() )
        {
            synth->noteOn(0, (float)XFun::rand2i(48,62), XFun::rand2i(50,100) );
            counter = timeout;
        }
    }
    else
    {
        counter -= 1;
    }
}
//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLBirdCube::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );

    // enable state
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    // set vertex pointer
    glVertexPointer( 3, GL_FLOAT, 0, g_squareVertices );
    glNormalPointer( GL_FLOAT, 0, g_squareNormals );

    // push
    glPushMatrix();
        // scale
        glScalef( size.value, size.value, size.value );

        // draw it
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 16, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 20, 4 );

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}
