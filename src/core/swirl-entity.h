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

    Stk * unitGenerator;

    //! Input one sample to the filter and return one output.
    SAMPLE tick( SAMPLE input );

    // Get one sample from every child
    virtual void tickAll( SAMPLE * oneFrame, Vector3D listenerPosition );
};

//-----------------------------------------------------------------------------
// Name: class SWIRLCamera
// Desc: the camera
//-----------------------------------------------------------------------------
class SWIRLCamera : public YEntity
{
public:
    // slew
    iSlew3D iLoc;
    iSlew3D iRefLoc;

    Vector3D refLoc;

    // update
    void update( YTimeInterval dt );
    // render
    void render();

    // description
    virtual std::string desc() const;
};


//-----------------------------------------------------------------------------
// Name: class SWIRLMoon
// Desc: whoa... it's the moon
//-----------------------------------------------------------------------------
class SWIRLMoon : public YEntity
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
class SWIRLTeapot : public YEntity
{
public:
    // update
    //void update( YTimeInterval dt );
    // render
    void render();

    // description
    virtual std::string desc() const;
};

#endif
