#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

struct SOscillator
{
	GLfloat x,y,z;     //position
	GLfloat nx,ny,nz;  //normal vector
	GLfloat u,v;	   //texture coordinates

	GLfloat UpSpeed;
	GLfloat newY;
};

class CPool
{
protected:
	//vertex data for the waves:
	SOscillator * m_Oscillators;
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
	void Update(float deltaTime);
	void Render();
};

#endif