#include <vector>

#include "swirl-pool.h"
#include "swirl-vectors.h"  //3d-vectors

using namespace std;

//TODO

std::string SWIRLBowl::desc() const
{
    return "Bowl";
}

void SWIRLBowl::render(void)
{
	float bowlheight = 0.2 + POOL_HEIGHT;
	float bowlwidth = 0.2;
    
	float TexBorderDistance = bowlwidth / (MAXX+2*bowlwidth);
    
    glColor3f(0.2, 0.2, 0.3);
    
    glBegin(GL_QUADS);
    
    float minX = -4.0;
    float minZ = -4.0;
    float maxX = 8.0;
    float maxZ = 8.0;
    //******************
    //ground
    //******************
    
    glNormal3f(0.0f,1.0f,0.0);
    glTexCoord2f(0.0,0.0);
    glVertex3f(minX,0.0,minZ);
    glTexCoord2f(1.0,0.0);
    glVertex3f(maxX,0.0,minZ);
    glTexCoord2f(1.0,1.0);
    glVertex3f(maxX,0.0,maxZ);
    glTexCoord2f(0.0,1.0);
    glVertex3f(minX,0.0,maxZ);
	glEnd();
    
    
    glColor3f(0.2,0.2,0.3);
    
	glBegin(GL_QUADS);
	
    //******************
    //top
    //******************
    glNormal3f(0.0f,1.0f,0.0);
    
    glTexCoord2f(TexBorderDistance,TexBorderDistance);
    glVertex3f(0.0f,bowlheight,0.0);
    glTexCoord2f(1.0-TexBorderDistance,TexBorderDistance);
    glVertex3f(MAXX,bowlheight,0.0);
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(MAXX,bowlheight,-bowlwidth);
    glTexCoord2f(TexBorderDistance,0.0);
    glVertex3f(0.0f,bowlheight,-bowlwidth);
    
    glTexCoord2f(TexBorderDistance,0.0);
    glVertex3f(0.0f,bowlheight,-bowlwidth);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(0.0,1.0-TexBorderDistance);
    glVertex3f(-bowlwidth,bowlheight,MAXZ);
    glTexCoord2f(TexBorderDistance,1.0-TexBorderDistance);
    glVertex3f(0.0f,bowlheight,MAXZ);
    
    glTexCoord2f(1.0,0.0);
    glVertex3f(MAXX+bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(MAXX,bowlheight,-bowlwidth);
    glTexCoord2f(1.0-TexBorderDistance,1.0-TexBorderDistance);
    glVertex3f(MAXX,bowlheight,MAXZ);
    glTexCoord2f(1.0,1.0-TexBorderDistance);
    glVertex3f(MAXX+bowlwidth,bowlheight,MAXZ);
    
    glTexCoord2f(1.0,1.0-TexBorderDistance);
    glVertex3f(MAXX+bowlwidth,bowlheight,MAXZ);
    glTexCoord2f(0.0,1.0-TexBorderDistance);
    glVertex3f(-bowlwidth,bowlheight,MAXZ);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-bowlwidth,bowlheight,MAXZ+bowlwidth);
    glTexCoord2f(1.0,1.0);
    glVertex3f(MAXX+bowlwidth,bowlheight,MAXZ+bowlwidth);
    
    //******************
    //front
    //******************
    glNormal3f(0.0f,0.0f,1.0f);
    
    glTexCoord2f(TexBorderDistance,TexBorderDistance);
    glVertex3f(0.0f,bowlheight,0.0);
    glTexCoord2f(1.0-TexBorderDistance,TexBorderDistance);
    glVertex3f(MAXX,bowlheight,0.0);
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(MAXX,0.0f,0.0);
    glTexCoord2f(TexBorderDistance,0.0);
    glVertex3f(0.0f,0.0f,0.0);
    
    
    glTexCoord2f(0.0,1.0-TexBorderDistance);
    glVertex3f(-bowlwidth,bowlheight,MAXZ+bowlwidth);
    glTexCoord2f(1.0,1.0-TexBorderDistance);
    glVertex3f(MAXX+bowlwidth,bowlheight,MAXZ+bowlwidth);
    glTexCoord2f(1.0,1.0);
    glVertex3f(MAXX+bowlwidth,0.0f,MAXZ+bowlwidth);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-bowlwidth,0.0f,MAXZ+bowlwidth);
    
    
    //******************
    //back
    //******************
    glNormal3f(0.0,0.0,-1.0f);
    
    glTexCoord2f(TexBorderDistance,TexBorderDistance);
    glVertex3f(0.0f,bowlheight,MAXZ);
    glTexCoord2f(1.0-TexBorderDistance,TexBorderDistance);
    glVertex3f(MAXX,bowlheight,MAXZ);
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(MAXX,0.0f,MAXZ);
    glTexCoord2f(TexBorderDistance,0.0);
    glVertex3f(0.0f,0.0f,MAXZ);
    
    glTexCoord2f(0.0,1.0-TexBorderDistance);
    glVertex3f(-bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(1.0,1.0-TexBorderDistance);
    glVertex3f(MAXX+bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(1.0,1.0);
    glVertex3f(MAXX+bowlwidth,0.0f,-bowlwidth);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-bowlwidth,0.0f,-bowlwidth);
    
    
    //******************
    //side
    //******************
    glNormal3f(-1.0,0.0,0.0);
    
    glTexCoord2f(1.0-TexBorderDistance,TexBorderDistance);
    glVertex3f(MAXX,bowlheight,0.0);
    glTexCoord2f(1.0,1.0-TexBorderDistance);
    glVertex3f(MAXX,bowlheight,MAXZ);
    glTexCoord2f(1.0-TexBorderDistance,1.0-TexBorderDistance);
    glVertex3f(MAXX,0.0f,MAXZ);
    glTexCoord2f(1.0,TexBorderDistance);
    glVertex3f(MAXX,0.0f,0.0);
    
    
    glTexCoord2f(0.0,0.0);
    glVertex3f(-bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(1.0,0.0);
    glVertex3f(-bowlwidth,bowlheight,MAXZ+bowlwidth);
    glTexCoord2f(1.0,TexBorderDistance);
    glVertex3f(-bowlwidth,0.0f,MAXZ+bowlwidth);
    glTexCoord2f(0.0,TexBorderDistance);
    glVertex3f(-bowlwidth,0.0f,-bowlwidth);
    
    glNormal3f(1.0,0.0,0.0);
    
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(0.0f,bowlheight,MAXZ);
    glTexCoord2f(TexBorderDistance,0.0);
    glVertex3f(0.0f,bowlheight,0.0);
    glTexCoord2f(TexBorderDistance,TexBorderDistance);
    glVertex3f(0.0f,0.0f,0.0);
    glTexCoord2f(1.0-TexBorderDistance,TexBorderDistance);
    glVertex3f(0.0f,0.0f,MAXZ);
    
    glTexCoord2f(1.0,1.0);
    glVertex3f(MAXX+bowlwidth,bowlheight,MAXZ+bowlwidth);
    glTexCoord2f(1.0,0.0);
    glVertex3f(MAXX+bowlwidth,bowlheight,-bowlwidth);
    glTexCoord2f(1.0-TexBorderDistance,0.0);
    glVertex3f(MAXX+bowlwidth,0.0f,-bowlwidth);
    glTexCoord2f(1.0-TexBorderDistance,1.0);
    glVertex3f(MAXX+bowlwidth,0.0f,MAXZ+bowlwidth);
    
	glEnd();
}

//////////////////////////////////
std::string SWIRLPool::desc() const
{
    cout << "POOL";
    return "Pool";
}


void SWIRLPool::Initialize(int xSize, int zSize,  float OscillatorDistance, float OscillatorWeight, float Damping, float TextureStretchX, float TextureStretchZ)
{
	//assign member variables
	m_xSize = xSize;
	m_zSize = zSize;
	m_NumOscillators = m_xSize*m_zSize;
	m_OscillatorDistance = OscillatorDistance;
	m_OscillatorWeight = OscillatorWeight;
	m_Damping = Damping;

	//temporary vector for indies:
	vector <GLuint> IndexVect;  //we first put the indices into this vector, then copy them to the array below
	

	m_Oscillators = new SWIRLOscillator[m_NumOscillators];
	IndexVect.clear();  //to be sure it is empty
	for (int xc = 0; xc < m_xSize; xc++) 
		for (int zc = 0; zc < m_zSize; zc++) 
		{
			m_Oscillators[xc+zc*m_xSize].x = m_OscillatorDistance*float(xc);
			m_Oscillators[xc+zc*m_xSize].y = 0.0f;
			m_Oscillators[xc+zc*m_xSize].z = m_OscillatorDistance*float(zc);

			m_Oscillators[xc+zc*m_xSize].nx = 0.0f;
			m_Oscillators[xc+zc*m_xSize].ny = 1.0f;
			m_Oscillators[xc+zc*m_xSize].nz = 0.0f;

			m_Oscillators[xc+zc*m_xSize].u = (float)xc / (float)m_xSize * TextureStretchX;
			m_Oscillators[xc+zc*m_xSize].v = 1.0f - (float)zc / (float)m_zSize * TextureStretchZ;

			m_Oscillators[xc+zc*m_xSize].UpSpeed = 0;

			//create two triangles:
			if ((xc < m_xSize-1) && (zc < m_zSize-1))
			{
				IndexVect.push_back(xc+zc*m_xSize);
				IndexVect.push_back((xc+1)+zc*m_xSize);
				IndexVect.push_back((xc+1)+(zc+1)*m_xSize);

				IndexVect.push_back(xc+zc*m_xSize);
				IndexVect.push_back((xc+1)+(zc+1)*m_xSize);
				IndexVect.push_back(xc+(zc+1)*m_xSize);
			}

		}

	//copy the indices:
	m_Indices = new GLuint[IndexVect.size()];  //allocate the required memory
	for (int i = 0; i < IndexVect.size(); i++)
	{
		m_Indices[i] = IndexVect[i];
	}


	m_NumIndices = IndexVect.size();
	IndexVect.clear();  //no longer needed, takes only memory
}

void SWIRLPool::Reset()
{
	for (int xc = 0; xc < m_xSize; xc++)
		for (int zc = 0; zc < m_zSize; zc++)
		{
			m_Oscillators[xc+zc*m_xSize].y = 0.0f;
			m_Oscillators[xc+zc*m_xSize].UpSpeed = 0.0f;
			m_Oscillators[xc+zc*m_xSize].nx = 0.0f;
			m_Oscillators[xc+zc*m_xSize].ny = 1.0f;
			m_Oscillators[xc+zc*m_xSize].nz = 0.0f;
		}
}

void SWIRLPool::AffectOscillator(int xPos, int zPos, float deltaY)
{
	if ((xPos >= 0) && (xPos < m_xSize) && (zPos >= 0) && (zPos < m_zSize))
	{
		if (m_Oscillators[xPos+zPos*m_xSize].y > -0.15)  //THIS LINE IS REQUIRED FOR FOUNTAINS WITH MANY DROPS!!!
			m_Oscillators[xPos+zPos*m_xSize].y+=deltaY;
	}
}


void SWIRLPool::update(YTimeInterval deltaTime)
{
//********
// Here we do the physical calculations: 
// The m_Oscillators are moved according to their neighbors.
// The parameter bEndIsFree indicates, whether the m_Oscillators in the edges can move or not.
// The new position may be assigned not before all calculations are done!

	//if we use two loops, it is a bit easier to understand what I do here.

    int xc,zc;

	for (xc = 0; xc < m_xSize; xc++) 
	{
		for (zc = 0; zc < m_zSize; zc++) 
		{
			int ArrayPos = xc+zc*m_xSize;

			m_Oscillators[ArrayPos].newY = m_Oscillators[ArrayPos].y;

			//check, if this oscillator is on an edge (=>end)
			if ((xc==0) || (xc==m_xSize-1) || (zc==0) || (zc==m_zSize-1))
				;//TBD: calculating m_Oscillators at the edge (if the end is free)
			else
			{
			  //calculate the new speed:
				

				//Change the speed (=accelerate) according to the oscillator's 4 direct neighbors:
				float AvgDifference = m_Oscillators[ArrayPos-1].y				//left neighbor
									 +m_Oscillators[ArrayPos+1].y				//right neighbor
									 +m_Oscillators[ArrayPos-m_xSize].y  //upper neighbor
									 +m_Oscillators[ArrayPos+m_xSize].y  //lower neighbor
									 -4*m_Oscillators[ArrayPos].y;				//subtract the pos of the current osc. 4 times	
				
				m_Oscillators[ArrayPos].UpSpeed += AvgDifference*(deltaTime/m_OscillatorWeight);

				m_Oscillators[ArrayPos].UpSpeed *= (1.0f-m_Damping);

			  //calculate the new position, but do not yet store it in "y" (this would affect the calculation of the other osc.s)
				m_Oscillators[ArrayPos].newY += m_Oscillators[ArrayPos].UpSpeed*deltaTime;
			  
				
				
			}
		}	
     
	}

	//copy the new position to y:
	for ( xc = 0; xc < m_xSize; xc++) 
	{
		for (int zc = 0; zc < m_zSize; zc++) 
		{
			m_Oscillators[xc+zc*m_xSize].y =m_Oscillators[xc+zc*m_xSize].newY;
		}
	}
	//calculate new normal vectors (according to the oscillator's neighbors):
	for ( xc = 0; xc < m_xSize; xc++) 
	{
		for (int zc = 0; zc < m_zSize; zc++) 
		{
			///
			//Calculating the normal:
			//Take the direction vectors 1.) from the left to the right neighbor 
			// and 2.) from the upper to the lower neighbor.
			//The vector orthogonal to these 

			SF3dVector u,v,p1,p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)

			if (xc > 0) p1 = F3dVector(m_Oscillators[xc-1+zc*m_xSize].x,
									   m_Oscillators[xc-1+zc*m_xSize].y,
									   m_Oscillators[xc-1+zc*m_xSize].z);
			else
						p1 = F3dVector(m_Oscillators[xc+zc*m_xSize].x,
									   m_Oscillators[xc+zc*m_xSize].y,
									   m_Oscillators[xc+zc*m_xSize].z);	
			if (xc < m_xSize-1) 
						p2 = F3dVector(m_Oscillators[xc+1+zc*m_xSize].x,
									   m_Oscillators[xc+1+zc*m_xSize].y,
									   m_Oscillators[xc+1+zc*m_xSize].z);
			else
						p2 = F3dVector(m_Oscillators[xc+zc*m_xSize].x,
									   m_Oscillators[xc+zc*m_xSize].y,
									   m_Oscillators[xc+zc*m_xSize].z);	
			u = p2-p1; //vector from the left neighbor to the right neighbor
			if (zc > 0) p1 = F3dVector(m_Oscillators[xc+(zc-1)*m_xSize].x,
									   m_Oscillators[xc+(zc-1)*m_xSize].y,
									   m_Oscillators[xc+(zc-1)*m_xSize].z);
			else
						p1 = F3dVector(m_Oscillators[xc+zc*m_xSize].x,
									   m_Oscillators[xc+zc*m_xSize].y,
									   m_Oscillators[xc+zc*m_xSize].z);	
			if (zc < m_zSize-1) 
						p2 = F3dVector(m_Oscillators[xc+(zc+1)*m_xSize].x,
									   m_Oscillators[xc+(zc+1)*m_xSize].y,
									   m_Oscillators[xc+(zc+1)*m_xSize].z);
			else
						p2 = F3dVector(m_Oscillators[xc+zc*m_xSize].x,
									   m_Oscillators[xc+zc*m_xSize].y,
									   m_Oscillators[xc+zc*m_xSize].z);	
			v = p2-p1; //vector from the upper neighbor to the lower neighbor
			//calculate the normal:
			SF3dVector normal = Normalize3dVector(CrossProduct(&u,&v));

			//assign the normal:
			if (normal.y > 0.0)   //normals always look upward!
			{
				m_Oscillators[xc+zc*m_xSize].nx = normal.x;
				m_Oscillators[xc+zc*m_xSize].ny = normal.y;
				m_Oscillators[xc+zc*m_xSize].nz = normal.z;
			}
			else
			{
				m_Oscillators[xc+zc*m_xSize].nx = -normal.x;
				m_Oscillators[xc+zc*m_xSize].ny = -normal.y;
				m_Oscillators[xc+zc*m_xSize].nz = -normal.z;
			}
		}
	}

}


void SWIRLPool::render()
{
	//There might be more vertex arrays. Thus, pass the pointers each time you use them:

	glVertexPointer(	3,   //3 components per vertex (x,y,z)
						GL_FLOAT,
						sizeof(SWIRLOscillator),
						m_Oscillators);
	glTexCoordPointer(  2,
						GL_FLOAT,
						sizeof(SWIRLOscillator),
						&m_Oscillators[0].u);


	glNormalPointer(	GL_FLOAT,
						sizeof(SWIRLOscillator),
						&m_Oscillators[0].nx);  //Pointer to the first normal

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	//Draw the array:
	glColor3f(1.0,1.0,1.0);
	glDrawElements(	GL_TRIANGLES, //mode
						m_NumIndices,  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						m_Indices);


}

float SWIRLPool::GetOscillatorDistance()
{
	return m_OscillatorDistance;
}


