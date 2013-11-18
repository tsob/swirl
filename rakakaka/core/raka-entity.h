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
// name: class RAKASpark
// desc: ...
//-----------------------------------------------------------------------------
class RAKASpark : public YEntity
{
public:
    // constructor
    RAKASpark() : texture(0), ALPHA( 1, 1, 1 ) { }
    
public:
    // set
    void set( int _texture, float _size, float _alpha );
    void setTexture( int _texture );
    void setSize( float _size );
    
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
    
public:
    // which ripple texture
    GLuint texture;
    // vertices
    GLfloat vertices[8];
    // alpha ramp
    Vector3D ALPHA;
};




//-----------------------------------------------------------------------------
// name: class RAKATeapot
// desc: for testing
//-----------------------------------------------------------------------------
class RAKATeapot : public YEntity
{
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
};




#endif














