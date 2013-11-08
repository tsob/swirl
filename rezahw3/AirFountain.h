#include "Pool.h"
#include "Vectors.h"

class CAirFountain;

class CDrop
{
private:
	GLfloat time;
	SF3dVector ConstantSpeed;  
	GLfloat AccFactor;
public:
	void SetConstantSpeed (SF3dVector NewSpeed);
	void SetAccFactor(GLfloat NewAccFactor);
	void SetTime(GLfloat NewTime);
	void GetNewPosition(SF3dVector * PositionVertex, float dtime, CPool * pPool, CAirFountain * pAirFountain); 
};

class CAirFountain
{
protected:
	SF3dVector * FountainVertices;
	CDrop * FountainDrops;
public:
	SF3dVector Position;
	void Render();
	void Update(float dtime, CPool * pPool);
	GLint m_NumDropsComplete;
	void Initialize(GLint Steps, GLint RaysPerStep, GLint DropsPerRay, 
					GLfloat AngleOfDeepestStep, 
					GLfloat AngleOfHighestStep,
					GLfloat RandomAngleAddition,
					GLfloat AccFactor);
	void Delete();
};