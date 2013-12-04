//-----------------------------------------------------------------------------
// Name: swirl-entity.h
// Desc: entities for visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __SWIRL_ENTITY_H__
#define __SWIRL_ENTITY_H__

#include "y-entity.h"
//#include "y-waveform.h"
#include "y-fluidsynth.h"
#include "x-audio.h"
#include "x-fun.h"
#include "x-buffer.h"
//#include "swirl-audio.h"
#include "Stk.h"
//#include "swirl-globals.h"
#include <vector>

using namespace stk;

//-----------------------------------------------------------------------------
// Name: class SWIRLEntity
// Desc: Augments YEntity with audio-related functionality
//-----------------------------------------------------------------------------
class SWIRLEntity : public YEntity
{
public:
    // Input one sample to the filter and return one output.
    // -  Returns nothing for this superclass.
    virtual SAMPLE tick( SAMPLE input ) {};

    // Synthesize a whole buffer.
    // -  Returns nothing for this superclass.
    virtual void synthesize( SAMPLE * buffer, unsigned int numFrames ) {};

    // Get one frame from every child
    virtual void tickAll( SAMPLE * oneFrame, Vector3D listenerPosition );

    // Synthesize one buffer from this and every child.
    virtual void synthesizeAll(
            SAMPLE * buffer, unsigned int numFrames, Vector3D listenerPosition
            );

public:
    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLCamera
// Desc: The camera, meant to be attached to our avatar (i.e. an instance of
//       SWIRLAvatar).
//-----------------------------------------------------------------------------
class SWIRLCamera : public SWIRLEntity
{
public:
    // Update
    void update( YTimeInterval dt );
    // Render
    //void render();

public:
    // Relative position determines the camera's position in relation to the
    // avatar, e.g. for first vs. third person POV.
    iSlew3D relativePosition;

    // togglePosition() changes between predefined first- and tird-person
    // relative positions.
    void togglePosition();

public:
    // Absolute location, needed for calling the actual look() function.
    Vector3D absLoc;

public:
    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLAvatar
// Desc: the avatars
//-----------------------------------------------------------------------------
class SWIRLAvatar : public SWIRLEntity
{
public:
    SWIRLAvatar( int anId, Vector3D startingLocation ); // Constructor

<<<<<<< HEAD
    int id;
    
=======
public:
    // Augment SWIRLEntity with reference location and goals for slewing
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f
    Vector3D goal;
    Vector3D refGoal;
    Vector3D refLoc;

public:
    // Slewing vectors for movement
    iSlew3D iLoc;
    iSlew3D iRefLoc;

public:
    // Movement methods
    void move(   float amount );
    void turn(   float degAmount );
    void strafe( float amount );

public:
    // Update
    void update( YTimeInterval dt );
    // Render
    void render();

public:
    // description
    virtual std::string desc() const;

public:
    Vector3D size;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLMoon
// Desc: whoa... it's the moon
//-----------------------------------------------------------------------------
class SWIRLMoon : public SWIRLEntity
{
public:
    // update
    //void update( YTimeInterval dt );

    // render
    void render();

public:
    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLTeapot
// Desc: for testing
//-----------------------------------------------------------------------------
class SWIRLTeapot : public SWIRLEntity
{
public:
    // update
    //void update( YTimeInterval dt );
    // render
    void render();

public:
    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLFluid
// Desc: adds fluidsynth to SWIRLEntity
//-----------------------------------------------------------------------------
class SWIRLFluid : public SWIRLEntity
{
public:
    SWIRLFluid(); //Constructor

public:
    virtual SAMPLE tick( SAMPLE input );
    virtual void synthesize( SAMPLE * buffer, unsigned int numFrames );

public:
    // fluidsynth instance is a member of this class
    GeXFluidSynth * synth;

public:
    // description
    virtual std::string desc() const;
};

// TODO remove?
//-----------------------------------------------------------------------------
// Name: class SWIRLCube
// Desc: a cube SWIRLEntity
//-----------------------------------------------------------------------------
class SWIRLCube : public SWIRLEntity
{
public:
    SWIRLCube() : size( 1, 1, 1.0f ) { }

public:
    virtual void update( YTimeInterval dt );
    virtual void render();

public:
    Vector3D size;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLBirdCube
// Desc: a cube that makes fluidsynth sounds when avatar is close.
//-----------------------------------------------------------------------------
class SWIRLBirdCube : public SWIRLFluid
{
public:
    SWIRLBirdCube() : size( 1, 1, 1.0f ) { counter = 0; }

public:
    virtual void render();
    virtual void update( YTimeInterval dt );

public:
    // description
    virtual std::string desc() const;

    int counter;

public:
    Vector3D size;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLNoteSphere
// Desc: a sphere that makes a fluidsynth note when avatar is close.
//-----------------------------------------------------------------------------
class SWIRLNoteSphere : public SWIRLFluid
{
public:
    SWIRLNoteSphere() : size( 1, 1, 1.0f ){
      pitch   = XFun::rand2i(22,62);
      channel = rand() % 30;
      counter = 0;
    }

public:
    virtual void render();
    virtual void update( YTimeInterval dt );

public:
    // description
    virtual std::string desc() const;
    float pitch;
    int counter, channel;

public:
    Vector3D size;
};


//-----------------------------------------------------------------------------
// Name: pitch2color
// Desc: maps pitch to a color
//-----------------------------------------------------------------------------
Vector3D pitch2color( float pitch );

#endif
