//-----------------------------------------------------------------------------
// name: raka-entity.h
// desc: entities for visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_ENTITY_H__
#define __RAKA_ENTITY_H__

#include "y-entity.h"
#include "x-buffer.h"
#include <vector>

//-----------------------------------------------------------------------------
// name: class RAKATeapot
// desc: for testing
//-----------------------------------------------------------------------------
class RAKATeapot : public YEntity
{
public:
    // update
    //void update( YTimeInterval dt );
    // render
    void render();
};

#endif














