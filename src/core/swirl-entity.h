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
    SWIRLEntity( int anId = 0, Vector3D startingLocation = Vector3D(0, 0, 0));

public:
    int id;
    virtual std::string desc() const { return "SWIRLEntity"; }

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
    // Augment SWIRLEntity with reference location and goals for slewing
    Vector3D goal;
    Vector3D oriGoal;

public:
    // Slewing vectors for movement
    iSlew3D iLoc;
    iSlew3D iOri;

public:
    // Movement methods
    void move(   float amount );
    void turn(   float degAmount );
    void strafe( float amount );

public:
    // Update
    void update( YTimeInterval dt );

public:
    virtual std::string getClassName() const;

public:
    Vector3D size;
};


//-----------------------------------------------------------------------------
// Name: class SWIRLFluid
// Desc: adds fluidsynth to SWIRLEntity
//-----------------------------------------------------------------------------
class SWIRLFluid : public SWIRLEntity //TODO SWIRLAudibleEntity
{
public:
    SWIRLFluid();

public:
    virtual SAMPLE tick( SAMPLE input );
    virtual void synthesize( SAMPLE * buffer, unsigned int numFrames );

public:
    // fluidsynth instance is a member of this class
    GeXFluidSynth * synth;

public:
    // description
<<<<<<< HEAD
    virtual std::string getClassName() const;
=======
    virtual std::string desc() const;
    
    Vector3D eye;
    Vector3D reference;
>>>>>>> origin/reza
};

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------

class SWIRLEntityFactory
{
public:
    static SWIRLEntity* createEntity(const char* entityClassName, int id, Vector3D loc, Vector3D ori);
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
    virtual std::string getClassName() const;
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

    virtual std::string getClassName() const;
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

    virtual std::string getClassName() const;
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

public:
    virtual std::string getClassName() const;

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

    virtual std::string getClassName() const;

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
        srand(time(NULL));
        pitch   = (rand() % 60) + 22;
        channel = rand() % 9;
        counter = 0;
    }

public:
    virtual void render();
    virtual void update( YTimeInterval dt );

public:
    float pitch;
    int counter, channel;

public:
    Vector3D size;

    virtual std::string getClassName() const;

};

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
class SWIRLConeAvatar : public SWIRLEntity
{
public:
    SWIRLConeAvatar();

    virtual void render();

    virtual std::string getClassName() const;

};

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
class SWIRLSphereAvatar : public SWIRLEntity
{
public:
    SWIRLSphereAvatar();

    virtual void render();

    virtual std::string getClassName() const;

};

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
class SWIRLCubeAvatar : public SWIRLEntity
{
public:
    SWIRLCubeAvatar();

    virtual void render();

    virtual std::string getClassName() const;

};

//-----------------------------------------------------------------------------
// Name: pitch2color
// Desc: maps pitch to a color
//-----------------------------------------------------------------------------
Vector3D pitch2color( float pitch );

float deg2rad( float deg );
float rad2deg( float rad );
Vector3D rotateVector( Vector3D vector, Vector3D ori );

#endif
