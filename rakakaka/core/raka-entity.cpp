//-----------------------------------------------------------------------------
// name: raka-entity.cpp
// desc: entities for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-entity.h"
#include "raka-globals.h"
#include "x-fun.h"
using namespace std;




// texture coordinates
static const GLshort g_coord[ ] = { 0, 0, 1, 0, 0, 1, 1, 1 };




//-----------------------------------------------------------------------------
// name: set()
// desc: start with this
//-----------------------------------------------------------------------------
void RAKASpark::set( int _texture, float _size, float _alpha )
{
    setTexture( _texture );
    setSize( _size );
    alpha = _alpha;
}




//-----------------------------------------------------------------------------
// name: setSize()
// desc: set size
//-----------------------------------------------------------------------------
void RAKASpark::setSize( float _size )
{
    _size *= 0.5f;
    vertices[0] = -_size; vertices[1] = -_size;
    vertices[2] = _size; vertices[3] = -_size;
    vertices[6] = _size; vertices[7] = _size;
    vertices[4] = -_size; vertices[5] = _size;
}




//-------------------------------------------------------------------------------
// name: setTexture()
// desc: ...
//-------------------------------------------------------------------------------
void RAKASpark::setTexture( int _texture )
{
    // set it
    texture = _texture;
}



//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void RAKASpark::update( YTimeInterval dt )
{
    // slew
    ALPHA.interp( dt );
    // set
    this->alpha = ALPHA.value;
}




//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void RAKASpark::render( )
{
    // disable depth
    glDisable( GL_DEPTH_TEST );
    // enable texture
    glEnable( GL_TEXTURE_2D );
    // set blend function
    glBlendFunc( GL_ONE, GL_ONE );
    // glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // enable blend
    glEnable( GL_BLEND );
    
    // bind to texture
    glBindTexture( GL_TEXTURE_2D, Globals::textures[this->texture] );
    
    // set color
    // glColor4f( col.x, col.y, col.z, alpha * ALPHA.value );
    
    // set vertex coordinates
    glVertexPointer( 2, GL_FLOAT, 0, this->vertices );
    glEnableClientState( GL_VERTEX_ARRAY );
    
    // set texture coordinates
    glTexCoordPointer( 2, GL_SHORT, 0, g_coord );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    // draw stuff!
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    
    // disable texture
    glDisable( GL_TEXTURE_2D );
    // disable blend
    glDisable( GL_BLEND );
    
    // disable client states
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}




//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void RAKATeapot::update( YTimeInterval dt )
{
    // do nothing for now
}




//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void RAKATeapot::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glutSolidTeapot( 1.0 );
    // disable lighting
    glDisable( GL_LIGHTING );
}
