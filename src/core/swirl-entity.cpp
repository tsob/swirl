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
#include <utility>
#include "x-fun.h"
#include "x-thread.h"
#include "swirl-fountain.h"

using namespace std;
using namespace stk;

//XMutex g_SWiRLEntityMutex;

//-----------------------------------------------------------------------------
// Name: class: SWIRL* method: desc()
// Desc: Returns a description (usually type and name) of this object. Useful
//       for debugging stuff.
//-----------------------------------------------------------------------------

//TODO replace with typeid
std::string SWIRLFluid::getClassName() const
{
    return "SWIRLFluid ";
}
std::string SWIRLBirdCube::getClassName() const
{
    return "SWIRLBirdCube";
}
std::string SWIRLNoteSphere::getClassName() const
{
    return "SWIRLNoteSphere";
}
std::string SWIRLEntity::getClassName() const
{
    return "SWIRLEntity";
}
std::string SWIRLCamera::getClassName() const
{
    return "SWIRLCamera";
}
std::string SWIRLCube::getClassName() const
{
    return "SWIRLCube";
}
std::string SWIRLMoon::getClassName() const
{
    return "SWIRLMoon";
}
std::string SWIRLTeapot::getClassName() const
{
    return "SWIRLTeapot";
}
std::string SWIRLConeAvatar::getClassName() const
{
    return "SWIRLConeAvatar";
}
std::string SWIRLSphereAvatar::getClassName() const
{
    return "SWIRLSphereAvatar";
}
std::string SWIRLCubeAvatar::getClassName() const
{
    return "SWIRLCubeAvatar";
}
std::string SWIRLFountain::getClassName() const
{
    return className;
}

//-----------------------------------------------------------------------------
// name: g_squareVertices
// desc: vertices for a cube
//-----------------------------------------------------------------------------

SWIRLEntity* SWIRLEntityFactory::createEntity(const char* entityClassName, int id, Vector3D loc, Vector3D ori)
{
    SWIRLEntity* entity = NULL;
    
    cerr << "SWIRLEntityFactory::createEntity(" << entityClassName << ")" << endl;
    
    if (!strcmp(entityClassName, "SWIRLSphereAvatar"))
        entity = new SWIRLSphereAvatar(); //TODO we can have ori too
    else if (!strcmp(entityClassName, "SWIRLConeAvatar"))
        entity = new SWIRLConeAvatar(); //TODO we can have ori too
    else if (!strcmp(entityClassName, "SWIRLCubeAvatar"))
        entity = new SWIRLCubeAvatar(); //TODO we can have ori too
    else if (!strcmp(entityClassName, "SWIRLBirdCube")) {
        cerr << "creating SWIRLBirdCube" << endl;
        
        entity = new SWIRLBirdCube(); //TODO we can have ori too
        
        cerr << "SWIRLBirdCube created" << endl;

        
    }
    else if (!strcmp(entityClassName, "SWIRLNoteSphere"))
        entity = new SWIRLNoteSphere(); //TODO we can have ori too
    else if (strstr(entityClassName, "SWIRLFountain"))
    {
        cerr << "Creating fountain" << endl;
        
        //TODO destruct
        SWIRLPool* pool = new SWIRLPool();
        
        cerr << "-4";

        //pool->Initialize(NUM_X_OSCILLATORS,NUM_Z_OSCILLATORS,OSCILLATOR_DISTANCE,OSCILLATOR_WEIGHT, 0.05, 4.0, 4.0);
        //Globals::sim->root().addChild( pool );
        cerr << "-3";

        
        //pool->Reset(); //TODO
        
        cerr << "-1";

        entity = new SWIRLFountain();
        
        cerr << "0";

        if (!strcmp(entityClassName, "SWIRLFountain1")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 3,8,35,76,90,0.5,0.11);
            cerr << "1";

        } else if (!strcmp(entityClassName, "SWIRLFountain2")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 3,8,35,76,90,0.5,0.11);
            cerr << "2";

        } else if (!strcmp(entityClassName, "SWIRLFountain3")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 1,20,100,70,70,5.0,0.15);
            cerr << "3";

        } else if (!strcmp(entityClassName, "SWIRLFountain4")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 1,20,200,85,85,10,0.1);
            cerr << "4";

        } else if (!strcmp(entityClassName, "SWIRLFountain5")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 5,20,85,90,90,1.0,0.15);
            cerr << "5";

        } else if (!strcmp(entityClassName, "SWIRLFountain6")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 2,20,50,40,70,1.5,0.2);
            cerr << "6";

        } else if (!strcmp(entityClassName, "SWIRLFountain7")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 3,50,25,76,90,0.2,0.11);
            cerr << "7";

        } else if (!strcmp(entityClassName, "SWIRLFountain8")) {
            ((SWIRLFountain*)entity)->Initialize(entityClassName, pool, 4,100,45,76,90,0.2,0.11);
            cerr << "8";

        }


        ((SWIRLFountain*)entity)->Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f,
                                          POOL_HEIGHT,
                                          NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f);
        
        
        //place it in the center of the pool:
        ((SWIRLFountain*)entity)->Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f,
                                          POOL_HEIGHT,
                                          NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f);
    }
    
    
    entity->id = id;
    entity->loc = loc; //TODO call constructor
    entity->ori = ori;

    // slewing variables for location and orientation`
    entity->iLoc = iSlew3D( entity->loc, 5.0f );
    entity->iOri = iSlew3D( entity->ori, 5.0f );

    entity->goal = entity->loc;
    entity->oriGoal = entity->ori;
    
    cerr << "entity created" << endl;

    return entity;
}


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
    
    //g_SWiRLEntityMutex.acquire();
    // tick children
    for( vector<YEntity *>::iterator itr = children.begin();
         itr != children.end(); itr++ )
    {
        //TODO if (dynamic_cast<SWIRLEntity *>(*itr))
        //{
        //((SWIRLEntity*)*itr)->tickAll(oneFrame, listenerPosition);
        //}
        (*itr)->tickAll(oneFrame, listenerPosition);
    }
    //g_SWiRLEntityMutex.release();
    
    
}

//-------------------------------------------------------------------------------
// Name: class: SWIRLEntity method: tickAll()
// Desc: Get one audio frame from this and every child
//-------------------------------------------------------------------------------
void SWIRLEntity::synthesizeAll( SAMPLE * buffer,
                                 unsigned int numFrames,
                                 Vector3D listenerPosition
                               )
{
    
    // Stop if this entity is not active.
    if( !active )
        return;

    // render self if not hidden
    if( !hidden )
    {
        SWIRLEntity* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

        // For now, just do mono without spatialization
        SAMPLE tmpBuffer[numFrames*2];
        memset(tmpBuffer,0,sizeof(SAMPLE)*numFrames*2);
        this->synthesize( tmpBuffer, numFrames );

        for (int j = 0; j < numFrames; ++j)
        {
          for (int i = 0; i < SWIRL_NUMCHANNELS; ++i)
          {
            if(i<2)
            {
              // perform distance scaling and add to mono buffer
              if(myAvatar==NULL)
                buffer[j*SWIRL_NUMCHANNELS+i] += tmpBuffer[j*SWIRL_NUMCHANNELS+i]
                   / max((this->loc).magnitude(), 1.0f);
              else
                buffer[j*SWIRL_NUMCHANNELS+i] += tmpBuffer[j*SWIRL_NUMCHANNELS+i]
                   / max((this->loc - myAvatar->loc).magnitude(), 1.0f);
              //buffer[j*SWIRL_NUMCHANNELS + i] =  buffer[j*SWIRL_NUMCHANNELS];
            }
          }
        }
    }
    
    //g_SWiRLEntityMutex.acquire();
    
    // synthesize children
    for( vector<YEntity *>::iterator itr = children.begin();
         itr != children.end(); itr++ )
    {
        //TODO if (dynamic_cast<SWIRLEntity *>(*itr))
        //{
        //cerr << "going to child...";
        //((SWIRLEntity*)*itr)->synthesizeAll(buffer,numFrames,listenerPosition);
        //}
        (*itr)->synthesizeAll(buffer,numFrames,listenerPosition);
    }
    //g_SWiRLEntityMutex.release();
    
    
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
// Name: class: SWIRLCamera method: update
// Desc:
//-----------------------------------------------------------------------------
void SWIRLCamera::update( YTimeInterval dt )
{
    if (!dynamic_cast<SWIRLClient *>(Globals::application)) //if application is not "Client"
        return;

    SWIRLEntity* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

    // interpolate relative camera position
    relativePosition.interp(dt);
    loc = relativePosition.actual();

    // Vector pointing in the look direction
    Vector3D displacementVector =
      rotateVector( relativePosition.actual(), ori );

    absLoc = myAvatar->loc + displacementVector;
    loc = relativePosition.actual();
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLCamera method: togglePosition()
// Desc: Toggles between first- and third-person view.
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
// Name: class: SWIRLEntity method: update( YTimeInterval dt )
// Desc: updates (interpolates) entity position
//-----------------------------------------------------------------------------
void SWIRLEntity::update( YTimeInterval dt )
{
    // interp
    //iLoc.interp( dt );
    //iOri.interp( dt );

    // update goals
    iLoc.update( goal );
    iOri.update( oriGoal );

    // interp
    iLoc.interp( dt );
    iOri.interp( dt );

    // get current positions
    loc = iLoc.actual();
    ori = iOri.actual();
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------

SWIRLEntity::SWIRLEntity( int anId, Vector3D startingLocation) : id(anId)
{
    loc  = startingLocation;
    size = Vector3D(1, 1, 1.0f);
    ori  = Vector3D(0.0f, 0.0f, 0.0f);
    col  = Globals::ourOrange;
}
//-----------------------------------------------------------------------------
// Name: class: SWIRLEntity method: move( amount )
// Desc:
//-----------------------------------------------------------------------------
void SWIRLEntity::move( float amount )
{
   Vector3D movementVector = rotateVector( Vector3D(1.0f, 0.0f, 0.0f), ori);
   //movementVector.normalize(); //not needed
   movementVector *= amount;

    cerr << "MOV VEC " << movementVector.x << " " << movementVector.y << " " << movementVector.z << endl;

   goal    += movementVector;
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLEntity method: turn( radAmount )
// Desc:
//-----------------------------------------------------------------------------
void SWIRLEntity::turn( float radAmount )
{
   oriGoal.y -= rad2deg(radAmount);
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLEntity method: strafe( amount )
// Desc:
//-----------------------------------------------------------------------------
void SWIRLEntity::strafe( float amount )
{
   Vector3D movementVector = rotateVector( Vector3D(0.0f, 0.0f, -1.0f), ori);

   movementVector.normalize(); // maybe not necessary
   movementVector *= amount;

   goal    += movementVector;
}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLBirdy constructor
// Desc: ...
//-----------------------------------------------------------------------------
SWIRLFluid::SWIRLFluid()
{
    printf("About to instantiate SWIRLFluid\n");
    XFun::srand();
    // Instantiate fluidsynth
    synth = new GeXFluidSynth();
    // Init fluidsynth
    synth->init( SWIRL_SRATE, 32 );
    // Load the soundfont
    //synth->load( "data/soundfonts/birds.sf2", "" );
    //synth->load( "data/soundfonts/CasioVL-1.sf2", "" );
    synth->load( "data/soundfonts/chorium.sf2", "" );
    // Map program changes
    //synth->programChange( 0, XFun::rand2i(1,13)-1 );

    synth->programChange( 0, 11 ); //vibraphone
    synth->programChange( 1, 46 ); //harp
    synth->programChange( 2, 47 ); //timpani
    synth->programChange( 3, 32 ); //bass
    synth->programChange( 4, 34 ); //another bass
    synth->programChange( 5, 77 ); //shakuhachi
    synth->programChange( 6, 82 ); //synth calliope
    synth->programChange( 7, 104); //sitar
    synth->programChange( 8, 108); //Kalimba

    for (int i = 9; i < 30; ++i)
    {
        synth->programChange( i, rand() % 127 );
    }

    //synth->programChange( 1, 1 );
    //synth->programChange( 2, 2 );
    //synth->programChange( 3, 3 );
    printf("Instantiated SWIRLFluid\n");
}

// TODO
//-----------------------------------------------------------------------------
// Name: class: SWIRLFluid method: tick()
// Desc: return a sample of audio from SWIRLFluid
//-----------------------------------------------------------------------------
SAMPLE SWIRLFluid::tick( SAMPLE input )
{
  SAMPLE oneFrame[2];
  synth->synthesize2( oneFrame, 1);
  return oneFrame[0];
}

//-----------------------------------------------------------------------------
// Name: class: SWIRLFluid method: synthesize
// Desc: fills a stereo buffer with synthesized audio from the fluidsynth obj
//-----------------------------------------------------------------------------
void SWIRLFluid::synthesize( SAMPLE * buffer, unsigned int numFrames )
{
    synth->synthesize2( (float*)buffer, numFrames);
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
    //TODO check everywhere
    if (!dynamic_cast<SWIRLClient *>(Globals::application)) //if application is not "Client"
        return;

    SWIRLClient * client = (SWIRLClient*)Globals::application;
    SWIRLEntity* myAvatar = client->myAvatar;
    
    std::map<int, SWIRLEntity*>::iterator entitiesIter;
    SWIRLEntity* entity;

    //first add all the entities we have so far to the client
    for (entitiesIter = client->entities.begin(); entitiesIter != client->entities.end(); entitiesIter++) {
        entity = entitiesIter->second;

        if(entity->id == id) break;
        cerr << "Entity ID: " << entity->id << endl;
        cerr << "Class name: " << entity->getClassName() << endl;

        static vector< pair<int,int> > noteChanPitch;

        //static int counter = 0;
        int timeout = 24;

        // interp
        size.interp( dt );

        if(counter<=0)
        {
            if( (loc - entity->loc).magnitude() < size.magnitude() )
            {
                int pitch = XFun::rand2i(48,62);
                int channel = rand() % 9;
                synth->noteOn(channel, (float)pitch, (rand() % 40) + 80 );
                counter = timeout;
                noteChanPitch.push_back( make_pair(channel, (int)pitch) );
                //col = Vector3D(XFun::rand2f(0,1),XFun::rand2f(0,1),XFun::rand2f(0,1));
                col = pitch2color( (float)pitch );
            }
        }
        else
        {
            counter -= 1;
            if(counter<=0)
            {
                pair<int, int> myNoteOff = noteChanPitch.back();
                noteChanPitch.pop_back();
                synth->noteOff( myNoteOff.first, myNoteOff.second );
            }
        }
    }
    col *= powf(0.2,XGfx::delta());
}
//-----------------------------------------------------------------------------
// name: update()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLNoteSphere::update( YTimeInterval dt )
{
    if (!dynamic_cast<SWIRLClient *>(Globals::application)) //if application is not "Client"
        return;

    SWIRLEntity* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

    static vector< pair<int,int> > noteChanPitch;
    static int timeout = 512;

    // interp
    size.interp( dt );

    if(counter<=0)
    {
        if( (loc - myAvatar->loc).magnitude() < size.magnitude() )
        {
            synth->noteOn(channel, (float)pitch,  (rand() % 40) + 60 );
            counter = timeout;
            noteChanPitch.push_back( make_pair(channel, (int)pitch) );
            //col = Vector3D(XFun::rand2f(0,1),XFun::rand2f(0,1),XFun::rand2f(0,1));
            col = pitch2color( (float)pitch );
        }
    }
    else
    {
        counter--;
        if(counter<=0)
        {
            pair<int, int> myNoteOff = noteChanPitch.back();
            noteChanPitch.pop_back();
            synth->noteOff( myNoteOff.first, myNoteOff.second );
        }
    }
    col *= powf(1.1,XGfx::delta());
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

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLNoteSphere::render()
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

        glutSolidSphere( 0.5, 24, 24  );

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
// name:
// desc:
//-----------------------------------------------------------------------------


SWIRLConeAvatar::SWIRLConeAvatar()
{
    col = Globals::ourRed;
}

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLConeAvatar::render()
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

    //glColor3b(0, 0, 1);
    // scale
        glScalef( size.value, size.value, size.value );
       // glRotatef( -90.0f, 1.0f, 0.0f, 0.0f);
    /*
        glColor3f(100, 100, 255);


        glTranslatef( -0.2f, -0.2f, 0.4f);
        glutSolidSphere( 0.2, 24, 24  );

        glTranslatef( 0.4f, 0.0f, 0.0f);
        glutSolidSphere( 0.2, 24, 24  );

        glTranslatef( 0.0f, 0.2f, -0.4f);

        glColor3f(col.x, col.y, col.z);
   */
        glutSolidCone( 1.0f, 2.0f, 24, 24 );

        // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
// name:
// desc:
//-----------------------------------------------------------------------------


SWIRLSphereAvatar::SWIRLSphereAvatar()
{
    col = Globals::ourOrange;
}

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLSphereAvatar::render()
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

    //glColor3b(0, 0, 1);
    // scale
    glScalef( size.value, size.value, size.value );

    glPushMatrix();
    // scale
        glScalef( size.value, size.value, size.value );

        glutSolidSphere( 0.6, 24, 24  );

        glColor3f(100, 100, 255);

        glTranslatef( -0.2f, 0.3f, 0.4f);
        glutSolidSphere( 0.2, 24, 24  );

        glTranslatef( 0.4f, 0.0f, 0.0f);
        glutSolidSphere( 0.2, 24, 24  );
    glPopMatrix();

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}


//-----------------------------------------------------------------------------
// name:
// desc:
//-----------------------------------------------------------------------------


SWIRLCubeAvatar::SWIRLCubeAvatar()
{
    col = Globals::ourBlue;
}

//-----------------------------------------------------------------------------
// name: render()
// desc: ...
//-----------------------------------------------------------------------------
void SWIRLCubeAvatar::render()
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

    //glColor3b(0, 0, 1);
    // scale
    glScalef( size.value, size.value, size.value );

    glPushMatrix();
    // scale
    glScalef( size.value, size.value, size.value );

    glutSolidCube( 0.6);

    glColor3f(100, 100, 255);

    glTranslatef( -0.2f, 0.2f, 0.4f);
    glutSolidCube( 0.2);

    glTranslatef( 0.4f, 0.0f, 0.0f);
    glutSolidCube( 0.2 );
    glPopMatrix();

    // pop
    glPopMatrix();

    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    // disable lighting
    glDisable( GL_LIGHTING );
}


//-----------------------------------------------------------------------------
// Name: pitch2color
// Desc: maps pitch to a color
//-----------------------------------------------------------------------------
Vector3D pitch2color( float pitch ){
    float r, g, b;

    int pitchClass = (int)pitch % 12;

    r = pitchClass/11.0f;
    g = sin( pitchClass * SWIRL_PI / 12.0 )*0.5 + 0.5;
    b = 1.0f - pitchClass/11.0f;

    return Vector3D(r, g, b);
}


//-----------------------------------------------------------------------------
// Name: deg2rad
// Desc: gets radians from degrees
//-----------------------------------------------------------------------------
float deg2rad( float deg )
{
    return deg*SWIRL_PI/180.0;
}

//-----------------------------------------------------------------------------
// Name: rad2deg
// Desc: gets degrees from radians
//-----------------------------------------------------------------------------
float rad2deg( float rad )
{
    return rad*180.0/SWIRL_PI;
}


//-----------------------------------------------------------------------------
// Name: rotateVector
// Desc: rotate a vector with a given orientation in degrees
//-----------------------------------------------------------------------------
Vector3D rotateVector( Vector3D vector, Vector3D ori )
{
    Vector3D outputVector = vector;
    float theta, tmpX, tmpY, tmpZ;

    // Rotate about x
    theta = deg2rad(ori.x);
    tmpY = outputVector.y;
    tmpZ = outputVector.z;
    outputVector.y = tmpY * cos(theta) - tmpZ * sin(theta);
    outputVector.z = tmpY * sin(theta) + tmpZ * cos(theta);

    // Rotate about y
    theta = deg2rad(ori.y);
    tmpX = outputVector.x;
    tmpZ = outputVector.z;
    outputVector.z = tmpX * cos(theta) - tmpZ * sin(theta);
    outputVector.x = tmpX * sin(theta) + tmpZ * cos(theta);

    // Rotate about z
    theta = deg2rad(ori.z);
    tmpX = outputVector.x;
    tmpY = outputVector.y;
    outputVector.x = tmpX * cos(theta) - tmpY * sin(theta);
    outputVector.y = tmpX * sin(theta) + tmpY * cos(theta);

    return outputVector;
}
