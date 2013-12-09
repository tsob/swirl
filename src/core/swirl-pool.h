#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#include "x-gfx.h"


//TODO
#include "swirl-entity.h"

//TODO

#define NUM_X_OSCILLATORS		400
#define NUM_Z_OSCILLATORS		250

#define OSCILLATOR_DISTANCE		0.015
#define OSCILLATOR_WEIGHT       0.0001


#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)

#define POOL_HEIGHT				0.3

void renderBowl();

struct SWIRLOscillator
{
	GLfloat x,y,z;     //position
	GLfloat nx,ny,nz;  //normal vector
	GLfloat u,v;	   //texture coordinates
    
	GLfloat UpSpeed;
	GLfloat newY;
};

class SWIRLBowl : public SWIRLEntity
{
public:
    virtual std::string desc() const;
    virtual void render();
};

class SWIRLPool : public SWIRLEntity
{
protected:
	//vertex data for the waves:
	SWIRLOscillator * m_Oscillators;
	GLuint * m_Indices;
    
	
	int m_NumOscillators;  //x*z
	int m_xSize,m_zSize;
    
	int m_NumIndices;
    
	float m_OscillatorDistance;
	float m_OscillatorWeight;
	float m_Damping;
    
public:
	float GetOscillatorDistance();
	void Initialize(int xSize, int zSize, float OscillatorDistance, float OscillatorWeight, float Damping, float TextureStretchX, float TextureStretchZ);
	void Reset();
	void AffectOscillator(int xPos, int zPos, float deltaY);
	virtual void update(YTimeInterval deltaTime);
	virtual void render();
    virtual std::string desc() const;
    
};


#endif