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
#include "x-buffer.h"
#include <vector>

//-----------------------------------------------------------------------------
// Name: class SWIRLCamera
// Desc: the camera
//-----------------------------------------------------------------------------
class SWIRLCamera : public YEntity
{
public:
    // update
    void update( YTimeInterval dt );
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

#endif


