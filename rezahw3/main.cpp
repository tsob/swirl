/**********************************************************************

  Foutain and Water Simulation


***********************************************************************/

#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <time.h>
#include <stdlib.h>


#include "Pool.h"
#include "AirFountain.h"
#include "Camera.h"

#include <iostream>

#include "chuck_fft.h"
#include "Thread.h"


#define MY_SRATE 44100
#define SAMPLE double
#define ZPF 1

using namespace std;

void displayFunc(); //TODO
Mutex g_mutex2; //TODO


#include "RtAudio.h"

const size_t MAX_TIME = 10000;

static float spec[MAX_TIME][512][2]; //x, y //TODO buffSize
static float maxAmp[MAX_TIME]; //x, y //TODO buffSize
static float maxFreq[MAX_TIME]; //x, y //TODO buffSize

static int now = 0, t = 0;


GLsizei g_width = 800;
GLsizei g_height = 600;

Mutex g_mutex;

float * g_fftBuff;
float * g_audioBuff;

float * g_window;
bool g_useWindow = false;
bool g_displayBars = false;
unsigned int g_buffSize = 512;


//-----------------------------------------------------------------------------
// Defines a point in a 3D space (coords x, y and z)
//-----------------------------------------------------------------------------
struct pt3d
{
    pt3d( GLfloat x, GLfloat y, GLfloat z ) : x(x), y(y), z(z) {};
    
    float x;
    float y;
    float z;
};


//lighting:
GLfloat LightAmbient[]=		{ 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat LightDiffuse[]=		{ 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat LightPosition[]=	{ 1.0f, -0.5f, -0.5f, 0.0f };



#define NUM_X_OSCILLATORS		400
#define NUM_Z_OSCILLATORS		250


#define OSCILLATOR_DISTANCE		0.015
#define OSCILLATOR_WEIGHT       0.0001

#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)

#define POOL_HEIGHT				0.3


//Camera object:
CCamera Camera;

//The "pool" which represents the water within the fountain bowl
CPool Pool;

//water outside the bowl is in the air:
CAirFountain AirFountain1;
CAirFountain AirFountain2;
CAirFountain AirFountain3;



bool  g_bRain		   = true;	
bool  g_bFillModePoints = true;
bool  g_bLighting		= true;


void KeyDown(unsigned char key, int x, int y)
{	
	switch(key)
	{
	case 27:	//ESC
		exit(0);
		break;
	case 'a':		
		Camera.RotateY(1.0f);
		break;
	case 'd':		
		Camera.RotateY(-1.0f);
		break;
	case 'w':		
		Camera.MoveForwards(-0.05f ) ;
		break;
	case 's':		
		Camera.MoveForwards( 0.05f ) ;
		break;
	
    case 'y':
		Camera.RotateX(0.5f);
		break;
	case 'h':		
		Camera.RotateX(-0.5f);
		break;
	
    case 'j':
		Camera.StrafeRight(-0.05f);
		break;
	case 'l':
		Camera.StrafeRight(0.05f);
		break;
	case 'k':
		Camera.Move(F3dVector(0.0,-0.05,0.0));
		break;
	case 'i':
		Camera.Move(F3dVector(0.0,0.05,0.0));
		break;
	}
}

void RenderBowl(void)
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

void DrawScene(void)
{

	//Render the pool
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0.0,POOL_HEIGHT,0.0);
		Pool.Render();
	glPopMatrix();

	//Render the bowl
	RenderBowl();

	glDisable(GL_TEXTURE_2D);

	//Render the water in the air.
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
    glColor3f(0.5,0.5,0.5);
	AirFountain1.Render();
	AirFountain2.Render();
	AirFountain3.Render();
	
	glDisable(GL_BLEND);

}



// =========
// = Audio =
// =========
// Audio callback
int audioCallback( void * outputBuffer, void * inputBuffer,
                  unsigned int bufferSize, double streamTime,
                  RtAudioStreamStatus status, void * userData )
{
    static int now = 0;
    
    SAMPLE * out = (SAMPLE *)outputBuffer;
    SAMPLE * in = (SAMPLE *)inputBuffer;
    
    g_mutex.lock();
    
    // zero out the fft buffer (required only if ZPF > 1)
    memset( g_fftBuff, 0, ZPF * bufferSize * sizeof(float) );
    
    float maxAmp = 0;
    
    for(size_t i = 0; i < bufferSize; ++i)
    {
        out[i] = 0;
        
        g_fftBuff[i] = in[i];
        
    }
    
    // apply window to the buffer of audio
    if( g_useWindow )
        apply_window( g_fftBuff, g_window, g_buffSize );
    
    
    // compute the fft
    rfft( g_fftBuff, ZPF * bufferSize / 2, FFT_FORWARD );
    
    g_mutex.unlock();
    
    return 0;
}




// ========
// = Main =
// ========
// Entry point
//int main_old (int argc, char *argv[])
void setupAudio()
{
    
    // RtAudio config + init
    
    // pointer to RtAudio object
    RtAudio *  audio = NULL;
    
    // create the object
    try
    {
        audio = new RtAudio();
    }
    catch( RtError & err ) {
        err.printMessage();
        exit(1);
    }
    
    if( audio->getDeviceCount() < 1 )
    {
        // nopes
        cout << "no audio devices found!" << endl;
        exit( 1 );
    }
    
    // let RtAudio print messages to stderr.
    audio->showWarnings( true );
    
    // set input and output parameters
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = audio->getDefaultInputDevice();
    iParams.nChannels = 1;
    iParams.firstChannel = 0;
    oParams.deviceId = audio->getDefaultOutputDevice();
    oParams.nChannels = 1;
    oParams.firstChannel = 0;
    
    // create stream options
    RtAudio::StreamOptions options;
    
    // set the callback and start stream
    try
    {
        audio->openStream( &oParams, &iParams, RTAUDIO_FLOAT64, MY_SRATE, &g_buffSize, &audioCallback, NULL, &options);
        
        cerr << "Buffer size defined by RtAudio: " << g_buffSize << endl;
        
        // allocate the buffer for the fft
        g_audioBuff = new float[10000]; //TODO
        g_fftBuff = new float[g_buffSize * ZPF];
        
        if ( g_fftBuff == NULL ) {
            cerr << "Something went wrong when creating the fft buffers" << endl;
            exit (1);
        }
        
        // allocate the buffer for the time domain window
        g_window = new float[g_buffSize];
        if ( g_window == NULL ) {
            cerr << "Something went wrong when creating the window" << endl;
            exit (1);
        }
        
        // create a hanning window
        make_window( g_window, g_buffSize );
        
        // start the audio stream
        audio->startStream();
        
        // test RtAudio functionality for reporting latency.
        cout << "stream latency: " << audio->getStreamLatency() << " frames" << endl;
    }
    catch( RtError & err )
    {
        err.printMessage();
        goto cleanup;
    }
    
    // if we get here, stop!
    try
    {
        audio->stopStream();
    }
    catch( RtError & err )
    {
        err.printMessage();
    }
    
    // Clean up
cleanup:
    if(audio)
    {
        audio->closeStream();
        delete audio;
    }
    
    
    //return 0;
}



void Display(void)
{

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    
    const size_t MAX_TIME = 10000;
    static int now = 0, t = 0;
    
    static float spec[MAX_TIME][512][2]; //x, y 
    static float maxAmp[MAX_TIME]; //x, y 
    static float maxFreq[MAX_TIME]; //x, y 
    
    // cast to use the complex data type defined in chuck_fft
    complex * fft = (complex *)g_fftBuff;
    
    if( g_fftBuff )
    {
        
        g_mutex.lock();
        
        for (t = now; t >= 0; t--)
        {
            
            maxAmp[now] = 0;
            maxFreq[now] = 0;
            
            for(size_t f = 0; f < ZPF * g_buffSize / 2; ++f)
            {
                
                spec[now][f][0] = 5 * (float)f * 2.0 / (ZPF * (float)g_buffSize); // x
                spec[now][f][1] = 20 * 2 * ZPF * cmp_abs( fft[f] ); // y
                
                
                if (maxAmp[now] < spec[now][f][1])
                {
                    maxAmp[now] = spec[now][f][1];
                    maxFreq[now] = spec[now][f][0];
                    //cout << spec[now][f][1] << endl;

                }
                
            }
        }
        
        
        AirFountain1.Delete();
        AirFountain2.Render();
        AirFountain3.Delete();

        
        int val = maxAmp[now] * 5;
        
        if (val > 2)
            val = 2;
        
        AirFountain1.Initialize(1, //Steps
                               val,//maxAmp[now], //RaysPerStep
                               100,//DropsPerRay
                               3, //AngleOfDeepestStep
                               85, //AngleOfHighestStep
                               0.00, //RandomAngleAddition
                               0.40); //AccFactor

        

        AirFountain2.Initialize(5, //Steps
                               val,//maxAmp[now], //RaysPerStep
                               200,//DropsPerRay
                               3, //AngleOfDeepestStep
                               85, //AngleOfHighestStep
                               0.00, //RandomAngleAddition
                               0.10); //AccFactor
    
        AirFountain3.Initialize(1, //Steps
                                val,//maxAmp[now], //RaysPerStep
                                200,//DropsPerRay
                                3, //AngleOfDeepestStep
                                85, //AngleOfHighestStep
                                0.00, //RandomAngleAddition
                                3.2); //AccFactor*/

        g_mutex.unlock();
        
        now++;
    }
    
    Camera.Render();

	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
	//Turn two sided lighting on:
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	DrawScene();
	glFlush();			//Finish rendering
	glutSwapBuffers();
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.3
	//Far clipping plane distance: 50.0
	gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.3,50.0);
	glViewport(0,0,x,y);  //Use the whole window for rendering
	glMatrixMode(GL_MODELVIEW);
	
}

void Idle(void)
{
	//Do the physical calculation for one step:
	float dtime = 0.006;
	AirFountain1.Update(dtime, &Pool);
	AirFountain2.Update(dtime, &Pool);
    AirFountain3.Update(dtime, &Pool);
	Pool.Update(dtime);

	//render the scene:
	Display();
}


/////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{

    // RtAudio config + init
    
    // pointer to RtAudio object
    RtAudio *  audio = NULL;
    
    // create the object
    try
    {
        audio = new RtAudio();
    }
    catch( RtError & err ) {
        err.printMessage();
        exit(1);
    }
    
    if( audio->getDeviceCount() < 1 )
    {
        // nopes
        cout << "no audio devices found!" << endl;
        exit( 1 );
    }
    
    // let RtAudio print messages to stderr.
    audio->showWarnings( true );
    
    // set input and output parameters
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = audio->getDefaultInputDevice();
    iParams.nChannels = 1;
    iParams.firstChannel = 0;
    oParams.deviceId = audio->getDefaultOutputDevice();
    oParams.nChannels = 1;
    oParams.firstChannel = 0;
    
    // create stream options
    RtAudio::StreamOptions options;
    
    // set the callback and start stream
    try
    {
        audio->openStream( &oParams, &iParams, RTAUDIO_FLOAT64, MY_SRATE, &g_buffSize, &audioCallback, NULL, &options);
        
        cerr << "Buffer size defined by RtAudio: " << g_buffSize << endl;
        
        // allocate the buffer for the fft
        g_audioBuff = new float[10000]; //TODO
        g_fftBuff = new float[g_buffSize * ZPF];
        
        if ( g_fftBuff == NULL ) {
            cerr << "Something went wrong when creating the fft buffers" << endl;
            exit (1);
        }
        
        // allocate the buffer for the time domain window
        g_window = new float[g_buffSize];
        if ( g_window == NULL ) {
            cerr << "Something went wrong when creating the window" << endl;
            exit (1);
        }
        
        // create a hanning window
        make_window( g_window, g_buffSize );
        
        // start the audio stream
        audio->startStream();
        
        // test RtAudio functionality for reporting latency.
        cout << "stream latency: " << audio->getStreamLatency() << " frames" << endl;
    }
    catch( RtError & err )
    {
        err.printMessage();
        goto cleanup;
    }
    
    //Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(600,600);
	//Create a window with rendering context and everything else we need
	glutCreateWindow("Fountain with simulated water");
	//compute the vertices and indices
	Pool.Initialize(NUM_X_OSCILLATORS,NUM_Z_OSCILLATORS,OSCILLATOR_DISTANCE,OSCILLATOR_WEIGHT, 0.05, 4.0, 4.0);
	//init the airfountain: (look at KeyDown() to see more possibilities of initialization)
	AirFountain1.Initialize(3,8,35,76,90,0.5,0.11);
    AirFountain2.Initialize(3,8,35,76,90,0.5,0.11);
    AirFountain3.Initialize(3,8,35,76,90,0.5,0.11);
	
	
	//place it in the center of the pool:
	AirFountain1.Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f,
									 POOL_HEIGHT,
								 	 NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE/2.0f);
    
    AirFountain2.Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE/3.0f,
									 POOL_HEIGHT,
								 	 NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE/3.0f);

    AirFountain3.Position = F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE/1.5f,
                                      POOL_HEIGHT,
                                      NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE/1.5f);

	//initialize camera: 
	Camera.Move(F3dVector(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE / 2.0,3.0,NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE+4.0));
	Camera.RotateX(-20);

	//Enable the vertex array functionality:
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	
	//Switch on solid rendering:
	g_bFillModePoints = false;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	//Initialize lighting:
	g_bLighting = true;
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);								

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	//Some general settings:
	//
    //glClearColor(0.2,0.2,0.7,0.0);
    glClearColor(0.3,0.3,0.4,0.0);
    glutFullScreen();

	glFrontFace(GL_CCW);   //Tell OGL which orientation shall be the front face
	glShadeModel(GL_SMOOTH);
	

	//Initialize blending:
	glEnable(GL_BLEND);
	glPointSize(3.0);
	glEnable(GL_POINT_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//initialize generation of random numbers:
	srand((unsigned)time(NULL));
	
	//Assign the two used Msg-routines
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutIdleFunc(Idle);
	//Let GLUT get the msgs
	glutMainLoop();

	//Clean up:
	AirFountain1.Delete();
 	AirFountain2.Delete();
 	AirFountain3.Delete();
   
    
    try
    {
        audio->stopStream();
    }
    catch( RtError & err )
    {
        err.printMessage();
    }
    
    // Clean up
cleanup:
    if(audio)
    {
        audio->closeStream();
        delete audio;
    }
    
    

	return 0;
}
