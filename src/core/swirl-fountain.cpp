#include "stdlib.h"
#include <math.h>
#include "swirl-fountain.h"
#include "swirl-globals.h"

#define RandomFactor 2.0

GLfloat GetRandomFloat(GLfloat range)
{
	return (GLfloat)rand() / (GLfloat)RAND_MAX * range * RandomFactor;
}

SWIRLDrop::SWIRLDrop()
{
    col = Globals::ourWhite;
}

void SWIRLDrop::SetConstantSpeed(SF3dVector NewSpeed)
{
	ConstantSpeed = NewSpeed;
}

void SWIRLDrop::SetAccFactor (GLfloat NewAccFactor)
{
	AccFactor = NewAccFactor;
}

void SWIRLDrop::SetTime(GLfloat NewTime)
{
	time = NewTime;
}

void SWIRLDrop::GetNewPosition(SF3dVector * PositionVertex, float dtime, SWIRLPool * pPool, SWIRLFountain * pAirFountain)
{
	SF3dVector Position;
	time += dtime*20.0f;

	if (time > 0.0f)
	{
		Position.x = ConstantSpeed.x * time;
		Position.y = ConstantSpeed.y * time - AccFactor * time * time;
		Position.z = ConstantSpeed.z * time;
		PositionVertex->x = Position.x;
		PositionVertex->y = Position.y;
		PositionVertex->z = Position.z;
		if (Position.y < 0.0) 
		{
			/*the drop has fallen into the water. The problem is now, that we cannot
			set time to 0.0, because if there are more "DropsPerRay" than "TimeNeeded" (See InitFountain())
			several drops would be seen as one. Check it out.
			*/
			time = time - int(time);
			if (time > 0.0) time -= 1.0;

			//The drop creates a little wave in the pool:
            if (pPool) {
                int OscillatorX = (int)((Position.x+pAirFountain->Position.x)/pPool->GetOscillatorDistance());
                int OscillatorZ = (int)((Position.z+pAirFountain->Position.z)/pPool->GetOscillatorDistance());
                pPool->AffectOscillator(OscillatorX,
									OscillatorZ,
									-0.1);  //change this to make the waves stronger/weaker
            }

			
		
		}
	}
	else
	{
		PositionVertex->x = 0.0;
		PositionVertex->y = 0.0;
		PositionVertex->z = 0.0;
	}
}

/********************************************************************/

SWIRLFountain::SWIRLFountain()
{
    col = Globals::ourWhite;
    pool = NULL;
}

void SWIRLFountain::Initialize(std::string className, SWIRLPool* aPool, GLint Steps, GLint RaysPerStep, GLint DropsPerRay,
					GLfloat AngleOfDeepestStep, 
					GLfloat AngleOfHighestStep,
					GLfloat RandomAngleAddition,
					GLfloat AccFactor)
{
    this->className = className;
    pool = aPool;
    
    //This function needn't be and isn't speed optimized

	m_NumDropsComplete = Steps*RaysPerStep*DropsPerRay;

	FountainDrops = new SWIRLDrop [ m_NumDropsComplete ];
	FountainVertices = new SF3dVector [ m_NumDropsComplete ];
	SF3dVector NewSpeed;
	GLfloat DropAccFactor; //different from AccFactor because of the random change
	GLfloat TimeNeeded;
	GLfloat StepAngle; //Angle, which the ray gets out of the fountain with
	GLfloat RayAngle;	//Angle you see when you look down on the fountain
	GLint i,j,k;
	for (k = 0; k <Steps; k++)
	{
		for (j = 0; j < RaysPerStep; j++)
		{
			for (i = 0; i < DropsPerRay; i++)
			{
				DropAccFactor = AccFactor + GetRandomFloat(0.005);
				if (Steps > 1) 
					StepAngle = AngleOfDeepestStep + (AngleOfHighestStep-AngleOfDeepestStep) 
							* GLfloat(k) / (Steps-1) + GetRandomFloat(RandomAngleAddition);
				else
					StepAngle = AngleOfDeepestStep + GetRandomFloat(RandomAngleAddition);
			
				//This is the speed caused by the step:
				NewSpeed.x = cos ( StepAngle * PI / 180.0) * (0.2+0.04*k);
				NewSpeed.y = sin ( StepAngle * PI / 180.0) * (0.2+0.04*k);
				//This is the speed caused by the ray:
	
				RayAngle = (GLfloat)j / (GLfloat)RaysPerStep * 360.0 + 12.0;  //+12.0 causes a rotation (12°)
				//for the next computations "NewSpeed.x" is the radius. Care! Dont swap the two
				//lines, because the second one changes NewSpeed.x!
				NewSpeed.z = NewSpeed.x * sin ( RayAngle * PI /180.0);
				NewSpeed.x = NewSpeed.x * cos ( RayAngle * PI /180.0);

				NewSpeed.x*=3.0f;
				NewSpeed.y*=3.0f;
				NewSpeed.z*=3.0f;
				
				
				//Calculate how many steps are required, that a drop comes out and falls down again
				TimeNeeded = NewSpeed.y/ DropAccFactor;
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetConstantSpeed ( NewSpeed );
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetAccFactor (DropAccFactor);
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetTime(TimeNeeded * i / DropsPerRay);
			}
		}
	}

}

void SWIRLFountain::update(YTimeInterval dtime)
{
//TODO
    
	for (int i = 0; i < m_NumDropsComplete;i++)
		FountainDrops[i].GetNewPosition(&FountainVertices[i],dtime, pool, this);

}

void SWIRLFountain::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(	3,			//x,y,z-components
						GL_FLOAT,	//data type of SVertex
						0,			//the vertices are tightly packed
						FountainVertices);
	glPushMatrix();
	glTranslatef(Position.x,Position.y,Position.z);
	glDrawArrays(	GL_POINTS,
					0,
					m_NumDropsComplete);

	glPopMatrix();
 
}

void SWIRLFountain::Delete()
{
	delete[] FountainDrops;
	delete[] FountainVertices;
}
