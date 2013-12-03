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
#include "x-buffer.h"
//#include "swirl-audio.h"
#include "Stk.h"
//#include "swirl-globals.h"
#include <vector>

using namespace stk;

//-----------------------------------------------------------------------------
// Name: class SWIRLEntity
// Desc: extends YEntity for our stuff
//-----------------------------------------------------------------------------
class SWIRLEntity : public YEntity
{
public:

    //! Input one sample to the filter and return one output.
    virtual SAMPLE tick( SAMPLE input ) {};

    // Get one sample from every child
    virtual void tickAll( SAMPLE * oneFrame, Vector3D listenerPosition );

    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLCamera
// Desc: the camera
//-----------------------------------------------------------------------------
class SWIRLCamera : public SWIRLEntity
{
public:
    // update
    void update( YTimeInterval dt );
    // render
    //void render();

    iSlew3D relativePosition;
    void togglePosition();

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
    SWIRLAvatar( Vector3D startingLocation ); // Constructor

    Vector3D goal;
    Vector3D refGoal;
    Vector3D refLoc;

    iSlew3D iLoc;
    iSlew3D iRefLoc;

    // update
    void update( YTimeInterval dt );
    // render
    void render();

    // description
    virtual std::string desc() const;

    // movement methods
    void move(   float amount );
    void turn(   float degAmount );
    void strafe( float amount );

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

    // description
    virtual std::string desc() const;
};

//-----------------------------------------------------------------------------
// Name: class SWIRLFluid
// Desc:
//-----------------------------------------------------------------------------
class SWIRLFluid : public SWIRLEntity
{
public:
    SWIRLFluid(); //Constructor

    virtual SAMPLE tick( SAMPLE input );

    // fluidsynth
    GeXFluidSynth * synth;
};

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
// Name: class SWIRLFluid
// Desc:
//-----------------------------------------------------------------------------
class SWIRLBirdCube : public SWIRLFluid
{
public:
    SWIRLBirdCube() : size( 1, 1, 1.0f ) {  }

    virtual void render(); 
    virtual void update( YTimeInterval dt );

    // description
    virtual std::string desc() const;

public:
    Vector3D size;
};


#endif
