#include "swirl-pool.h"
#include "swirl-vectors.h"

#include "swirl-entity.h"

class SWIRLFountain;

class SWIRLDrop : public SWIRLEntity
{
private:
	GLfloat time;
	SF3dVector ConstantSpeed;  
	GLfloat AccFactor;
public:
	SWIRLDrop();
    void SetConstantSpeed (SF3dVector NewSpeed);
	void SetAccFactor(GLfloat NewAccFactor);
	void SetTime(GLfloat NewTime);
	void GetNewPosition(SF3dVector * PositionVertex, float dtime, SWIRLPool * pPool, SWIRLFountain * pAirFountain);
};

class SWIRLFountain : public SWIRLEntity
{
protected:
	SF3dVector * FountainVertices;
	SWIRLDrop * FountainDrops;
    SWIRLPool* pool;
    
    std::string className;
    
public:
	SF3dVector Position;
	virtual void render();
	void update(YTimeInterval dtime);
	GLint m_NumDropsComplete;
	SWIRLFountain();
    void Initialize(std::string className, SWIRLPool* aPool, GLint Steps, GLint RaysPerStep, GLint DropsPerRay,
					GLfloat AngleOfDeepestStep, 
					GLfloat AngleOfHighestStep,
					GLfloat RandomAngleAddition,
					GLfloat AccFactor);
	void Delete();
    virtual std::string getClassName() const;

};