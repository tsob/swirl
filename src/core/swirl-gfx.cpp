//----------------------------------------------------------------------------
// Name: swirl-gfx.cpp
// Desc: Graphics stuff for swirl visualization
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#include "swirl-gfx.h"
#include "swirl-globals.h"
#include "swirl-sim.h"
#include "swirl-audio.h"
#include "swirl-networking.h"

#include "x-fun.h"
#include "x-gfx.h"
#include "x-loadlum.h"
#include "x-vector3d.h"

#include <iostream>
#include <vector>

//TODO networking -- see also swirl-networking
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"

#define ADDRESS "127.0.0.1"

using namespace std;

//-----------------------------------------------------------------------------
// Name: swirl_gfx_init( )
// Desc: graphics/interaction entry point
//-----------------------------------------------------------------------------
bool swirl_gfx_init( int argc, const char ** argv )
{

    //TODO - networking development
    int sendPort = atoi(argv[1]);
    int receivePort = atoi(argv[2]);

    if (sendPort == 6000)
    {
        Globals::app = 1;
        cout << "ONE\n";
    }
    else
    {
        Globals::app = 2;
        cout << "TWO\n";
    }

    UdpTransmitSocket* transmitSocket = getTransmitSocket( ADDRESS, sendPort);

    //TODO
    pthread_t listenerThread;
    pthread_create(&listenerThread, NULL, oscListener, &receivePort);


#ifdef __APPLE__
    // save working dir
    char * cwd = getcwd( NULL, 0 );
    // set path
    // #ifdef __VQ_RELEASE__
#if 1
    // HACK: use this hard-coded path
    Globals::path = "./";
#else
    // store this globally
    Globals::path = cwd;
#endif
    // compute the datapath
    Globals::datapath = Globals::path + Globals::relpath;
#endif

    // initialize GLUT
    glutInit( &argc, (char **)argv );

#ifdef __APPLE__
    //restore working dir
    chdir( cwd );
    free( cwd );
#endif


    // Print about
    swirl_about();
    swirl_endline();

    // Double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );

    // Set the window postion
    glutInitWindowPosition( 100, 100 );

    // Create the window
    glutCreateWindow( "SWIRL");

    // Full screen
    //TODO
    // if( Globals::fullscreen )
    //   glutFullScreen();

    // Set the idle function - called when idle
    glutIdleFunc( idleFunc );

    // Set the display function - called when redrawing
    glutDisplayFunc( displayFunc );

    // Set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );

    // Set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // Set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // For arrow keys, etc
    glutSpecialFunc (specialFunc );

    // Do our own initialization
    initialize_graphics();
    // Simulation
    initialize_simulation();
    // Do data
    if( !initialize_data() )
    {
        // done
        return false;
    }

    // print keys
    // swirl_endline();
    // swirl_keys();
    // swirl_line();
    // swirl_endline();

    return true;
}




//-----------------------------------------------------------------------------
// Name: swirl_gfx_loop( )
// Desc: hand off to graphics loop
//-----------------------------------------------------------------------------
void swirl_gfx_loop()
{
    // Let GLUT handle the current thread from here
    glutMainLoop();
}




//-----------------------------------------------------------------------------
// Name: initialize_graphics( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize_graphics()
{
    // log
    cerr << "[swirl]: initializing graphics system..." << endl;

    // reset time
    XGfx::resetCurrentTime();
    // set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // get the first
    XGfx::getCurrentTime( true );
    // random
    XFun::srand();

    // set the GL clear color - use when the color buffer is cleared
    glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, Globals::fillmode );
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    // material have diffuse and ambient lighting
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
    // normalize (for scaling)
    glEnable( GL_NORMALIZE );
    // line width
    glLineWidth( Globals::linewidth );

    // enable light 0
    glEnable( GL_LIGHT0 );

    // setup and enable light 1
    glLightfv( GL_LIGHT1, GL_AMBIENT, Globals::light1_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, Globals::light1_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, Globals::light1_specular );
    glEnable( GL_LIGHT1 );

    // load textures
    //loadTextures();

    // fog
    Globals::fog_mode[0] = 0;
    Globals::fog_mode[1] = GL_LINEAR;
    // fog_mode[1] = GL_EXP; fog_mode[2] = GL_EXP2;
    Globals::fog_filter = 0;
    Globals::fog_density = .04f;

    // fog color
    GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};
    // fog mode
    if( Globals::fog_filter ) glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
    // set fog color
    glFogfv( GL_FOG_COLOR, fogColor );
    // fog density
    glFogf( GL_FOG_DENSITY, Globals::fog_density );
    // fog hint
    glHint( GL_FOG_HINT, GL_DONT_CARE );
    // fog start depth
    glFogf( GL_FOG_START, 1.0f );
    // fog end depth
    glFogf( GL_FOG_END, 10.5f );
    // enable
    if( Globals::fog_filter ) glEnable( GL_FOG );

    // check global flag
    if( Globals::fog )
    {
        // fog mode
        glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
        // enable
        glEnable(GL_FOG);
    }
    else
    {
        // disable
        glDisable(GL_FOG);
    }

    // clear the color buffer once
    glClear( GL_COLOR_BUFFER_BIT );
}




//-----------------------------------------------------------------------------
// Name: initialize_simulation( )
// Desc: simulation setup
//-----------------------------------------------------------------------------
void initialize_simulation()
{
    // instantiate simulation
    Globals::sim = new SWIRLSim();

    // add to simulation
    Globals::sim->root().addChild( getAvatar() );

    Globals::cameraEye.x = 0;
    Globals::cameraEye.y = 0;

    if (Globals::app == 1)
    {
        Globals::cameraEye.z = -2;
        Globals::cameraReference.z = 9;
    }
    else
    {
        Globals::cameraEye.z = -1;
        Globals::cameraReference.z = 10;
    }


    // Put a cube in the environment
    YCube* cube = new YCube();
    Globals::sim->root().addChild( cube );
}




//-----------------------------------------------------------------------------
// Name: initialize_data( )
// Desc: load initial data
//-----------------------------------------------------------------------------
bool initialize_data()
{
    //    // analysis
    //    Globals::analysis = new VQAnalysis();
    //
    //    // instantiate score reader/renderer
    //    Globals::skore = new VQSkore();
    //    // add
    //    Globals::sim->addChild( Globals::skore );
    //
    //    // read analysis
    //    if( !Globals::analysis->read( string("data/analysis/") +
    //                                 Globals::currentAnalysis + ".txt" ) )
    //    {
    //        // error out
    //        return false;
    //    }
    //
    //    // read MIDI
    //    Globals::skore->read( Globals::analysis->header().midi );
    //
    //    // fast forward
    //    if( Globals::fastForwardPoint > 0 )
    //        Globals::skore->fastforward( Globals::fastForwardPoint-1 );
    //
    //    // go ahead and start it
    //    Globals::skore->start();

    // done
    return true;
}




//-----------------------------------------------------------------------------
// Name: swirl_about()
// Desc: ...
//-----------------------------------------------------------------------------
void swirl_about()
{
    swirl_line();
    fprintf( stderr, "[swirl]: The Sonic World\n" );
    swirl_line();
    fprintf( stderr, "   | by Tim O'Brien\n" );
    fprintf( stderr, "   |    Reza Payami\n" );
    fprintf( stderr, "   |  & Haley Sayres\n" );
    fprintf( stderr, "   | Stanford University | CCRMA\n" );
    fprintf( stderr, "   | http://ccrma.stanford.edu/\n" );
    fprintf( stderr, "   | version: %s\n", Globals::version.c_str() );
}




//-----------------------------------------------------------------------------
// Name: swirl_keys()
// Desc: ...
//-----------------------------------------------------------------------------
// TODO update
void swirl_keys()
{
    swirl_line();
    fprintf( stderr, "[swirl]: run-time control\n" );
    swirl_line();
    fprintf( stderr, "  'h' - print this help message\n" );
    fprintf( stderr, "  's' - toggle fullscreen\n" );
    fprintf( stderr, "  'b' - toggle animated trails\n" );
    fprintf( stderr, "  'f' - toggle fog rendering\n" );
    fprintf( stderr, "  '[' and ']' - rotate automaton\n" );
    fprintf( stderr, "  '-' and '+' - zoom away/closer to center of automaton\n" );
    fprintf( stderr, "  'n' and 'm' - adjust amount of blending\n" );
    fprintf( stderr, "  '<' and '>' - adjust fog density\n" );
    fprintf( stderr, "  ''' - bg: white\n" );
    fprintf( stderr, "  ';' - bg: black\n" );
    fprintf( stderr, "  ',' - bg: blue\n" );
    fprintf( stderr, "  '.' - bg: gray\n" );
    fprintf( stderr, "  'q' - quit\n" );
}




//-----------------------------------------------------------------------------
// Name: swirl_help()
// Desc: ...
//-----------------------------------------------------------------------------
void swirl_help()
{
    swirl_endline();
    swirl_keys();
    swirl_endline();
}




//-----------------------------------------------------------------------------
// Name: swirl_usage()
// Desc: ...
//-----------------------------------------------------------------------------
void swirl_usage()
{
    swirl_line();
    fprintf( stderr, "[swirl]: command line arguments\n" );
    swirl_line();
    fprintf( stderr, "usage: swirl --[options] [name]\n" );
    fprintf( stderr, "   [options] = help | fullscreen" );
}




//-----------------------------------------------------------------------------
// Name: swirl_endline()
// Desc: ...
//-----------------------------------------------------------------------------
void swirl_endline()
{
    fprintf( stderr, "\n" );
}




//-----------------------------------------------------------------------------
// Name: swirl_line()
// Desc: ...
//-----------------------------------------------------------------------------
void swirl_line()
{
    fprintf( stderr, "---------------------------------------------------------\n" );
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( int w, int h )
{
    // save the new window size
    Globals::windowWidth = w; Globals::windowHeight = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
}




//-----------------------------------------------------------------------------
// Name: look( )
// Desc: ...
//-----------------------------------------------------------------------------
void look( )
{
    // go
    Globals::fov.interp( XGfx::delta() );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( Globals::fov.value, (GLfloat)Globals::windowWidth / (GLfloat)Globals::windowHeight, .005, 500.0 );

    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity();
    // position the view point


    /*gluLookAt( 0.0f,
              Globals::viewRadius.x * sin( Globals::viewEyeY.x ),
              Globals::viewRadius.x * cos( Globals::viewEyeY.x ),
              0.0f, 0.0f, 0.0f,
              0.0f, ( cos( Globals::viewEyeY.x ) < 0 ? -1.0f : 1.0f ), 0.0f );*/

    gluLookAt( Globals::cameraEye.x, Globals::cameraEye.y,
              Globals::cameraEye.z,
              Globals::cameraReference.x, Globals::cameraReference.y, Globals::cameraReference.z,
              0.0f, 1.0f, 0.0f );
   /*
    gluLookAt( Globals::viewRadius.y ,0.0f,
              Globals::viewEyeY.y,
              0.0f, 0.0f, 0.0f,
              0.0f, ( cos( Globals::viewEyeY.x ) < 0 ? -1.0f : 1.0f ), 0.0f );
    */
    // set the position of the lights
    glLightfv( GL_LIGHT0, GL_POSITION, Globals::light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, Globals::light1_pos );
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{

    //TODO
    UdpTransmitSocket* transmitSocket = getTransmitSocket();

    char buffer[SWIRL_FRAMESIZE];
    osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

    // system keys (handled first)
    switch( key )
    {
        case 'q':
        {
            exit( 0 );
            break;
        }
        case 'r':
        {
            break;
        }
        case 'a':
        {
            break;
        }
        case 'b':
        {
            static GLfloat blendAlpha = 0.15f;
            Globals::blendScreen = !Globals::blendScreen;
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal = blendAlpha;
                Globals::blendAlpha.slew = .5f;
            }
            else
            {
                blendAlpha = Globals::blendAlpha.goal;
                Globals::blendAlpha.goal = 1;
            }
            fprintf( stderr, "[swirl]: blendscreen:%s\n", Globals::blendScreen ? "ON" : "OFF" );
            break;
        }
        case 'f':
        {
            Globals::fog = !Globals::fog;
            fprintf( stderr, "[swirl]: fog:%s\n", Globals::fog ? "ON" : "OFF" );
            if( Globals::fog )
            {
                // fog mode
                glFogi( GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter] );
                // enable
                glEnable( GL_FOG );
            }
            else
            {
                // disable
                glDisable( GL_FOG );
            }
            break;
        }
        case 'h':
        {
            //swirl_endline();
            //swirl_about();
            //swirl_endline();
            swirl_help();
            break;
        }
        case 'm':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal -= .005;
                if( Globals::blendAlpha.goal < 0 ) Globals::blendAlpha.goal = 0;
            }
            break;
        case 'n':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal += .01;
                if( Globals::blendAlpha.goal > 1 ) Globals::blendAlpha.goal = 1;
            }
            break;
        case 'M':
        case 'N':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal = .15f;
            }
            break;
        case 's':
        {
            if( !Globals::fullscreen )
            {
                Globals::lastWindowWidth = Globals::windowWidth;
                Globals::lastWindowHeight = Globals::windowHeight;
                glutFullScreen();
            }
            else
                glutReshapeWindow( Globals::lastWindowWidth, Globals::lastWindowHeight );

            Globals::fullscreen = !Globals::fullscreen;
            fprintf( stderr, "[swirl]: fullscreen:%s\n", Globals::fullscreen ? "ON" : "OFF" );
            break;
        }
        case '<':
            Globals::fog_density *= .95f;
            fprintf( stderr, "[swirl]: fog density:%f\n", Globals::fog_density );
            glFogf(GL_FOG_DENSITY, Globals::fog_density);
            break;
        case '>':
            Globals::fog_density *= 1.05f;
            fprintf( stderr, "[swirl]: fog density:%f\n", Globals::fog_density );
            glFogf(GL_FOG_DENSITY, Globals::fog_density);
            break;

            //        case 'A':
            //        case 'B':
            //        case 'C':
            //        case 'D':
            //        case 'E':
            //        case 'F':
            //        case 'G':
            //        case 'H':
            //        case 'I':
            //        case 'J':
            //            Globals::core->noteOn( Globals::selectedVoice, key - 'A' + 48 );
            //            break;
            //
            //        case '6':
            //        case '7':
            //        case '8':
            //        case '9':
            //            Globals::selectedVoice = key - '6';
            //            break;

            //        case 'k':
            //            for( int i = 0; i < Globals::hud->histoVoice().numBins(); i++ )
            //                Globals::hud->histoVoice().bin(i)->setValue( XFun::rand2f(.1,.9) );
            //            for( int i = 0; i < Globals::hud->histoPitch().numBins(); i++ )
            //                Globals::hud->histoPitch().bin(i)->setValue( XFun::rand2f(.1,.9) );
            //            break;

            //        case 'V':
            //            Globals::skore->start();
            //            break;
    }

    // check if something else is handling viewing
    bool handled = false;


    // post visualizer handling (if not handled)
    if( !handled )
    {
        switch( key )
        {
            case ']':
                turn_right();
                break;
            case '[':
                turn_left();
                break;
            case 'w':
                move_forward();
                break;
            case 'x':
                move_back();
                break;

            case 'd':
                strafe_right();
                break;
            case 'a':
                strafe_left();
                break;

            case '\'':
                Globals::bgColor.update( Vector3D( 1,1,1 ) );
                break;
            case ';':
                Globals::bgColor.update( Vector3D( 0,0,0 ) );
                break;
            case '.':
                Globals::bgColor.update( Vector3D( .5f, .5f, .5f ) );
                break;
            case ',':
                Globals::bgColor.update( Vector3D( .5f, .75f, 1.0f ) );
                break;
            case 'n':
                swirl_playNotes(60, 120);
                break;
        }
    }

    // do a reshape since viewEyeY might have changed
    reshapeFunc( Globals::windowWidth, Globals::windowHeight );

    // post redisplay
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    glutPostRedisplay( );
}

//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: handles arrow stuff
//-----------------------------------------------------------------------------
void specialFunc( int key, int x, int y )
{
    // check
    bool handled = false;

    // if not handled
    if( !handled )
    {
        switch( key )
        {
            case GLUT_KEY_LEFT:
                break;
            case GLUT_KEY_RIGHT:
                break;
            case GLUT_KEY_UP:
                break;
            case GLUT_KEY_DOWN:
                break;
        }
    }
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    // update time
    XGfx::getCurrentTime( TRUE );

    // update
    Globals::bgColor.interp( XGfx::delta() );
    Globals::blendAlpha.interp( XGfx::delta() );

    // clear or blend
    if( Globals::blendScreen && Globals::blendAlpha.value > .0001 )
    {
        // clear the depth buffer
        glClear( GL_DEPTH_BUFFER_BIT );
        // blend screen
        blendPane();
    }
    else
    {
        // set the GL clear color - use when the color buffer is cleared
        glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
        // clear the color and depth buffers
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    // enable depth test
    glEnable( GL_DEPTH_TEST );

    // save state
    glPushMatrix();

    // slew
    Globals::viewEyeY.interp( XGfx::delta());
    Globals::viewRadius.interp( XGfx::delta() );
    look();

    // cascade simulation
    Globals::sim->systemCascade();

    // pop state
    glPopMatrix();

    //    // draw any HUD here
    //    Globals::hud->project();
    //    Globals::hud->updateAll( Globals::sim->delta() );
    //    Globals::hud->drawAll();

    // flush gl commands
    glFlush();
    // swap the buffers
    glutSwapBuffers();
}




//-----------------------------------------------------------------------------
// Name: blendPane()
// Desc: blends a pane into the current scene
//-----------------------------------------------------------------------------
void blendPane()
{
    // enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // disable lighting
    glDisable( GL_LIGHTING );
    // disable depth test
    glDisable( GL_DEPTH_TEST );
    // blend in a polygon
    glColor4f( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, Globals::blendAlpha.value );
    // glColor4f( Globals::blendRed, Globals::blendRed, Globals::blendRed, Globals::blendAlpha );
    // reduce the red component
    // Globals::blendRed -= .02f;
    // if( Globals::blendRed < 0.0f ) Globals::blendRed = 0.0f;

    GLfloat h = 10;
    GLfloat d = -1;

    // draw the polyg
    glBegin( GL_QUADS );
    glVertex3f( -h, -h, d );
    glVertex3f( h, -h, d );
    glVertex3f( h, h, d );
    glVertex3f( -h, h, d );
    glEnd();

    // enable lighting
    glEnable( GL_LIGHTING );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // disable blending
    glDisable( GL_BLEND );
}




//-----------------------------------------------------------------------------
// Name: renderBackground()
// Desc: ...
//-----------------------------------------------------------------------------
void renderBackground()
{
    // save the current matrix
    glPushMatrix( );

    // restore
    glPopMatrix( );
}




//-------------------------------------------------------------------------------
// Name: loadTexture()
// Desc: load textures
//-------------------------------------------------------------------------------
/*
 *void loadTextures()
 *{
 *    char filename[256];
 *    GLenum minFilter, maxFilter;
 *    int i;
 *
 *    // log
 *    fprintf( stderr, "[swirl]: loading textures...\n" );
 *
 *    // set store alignment
 *    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
 *
 *    // set filter types
 *    minFilter = GL_LINEAR;
 *    maxFilter = GL_LINEAR;
 *
 *    // load tng flares
 *    for( i = SWIRL_TEX_FLARE_TNG_1; i <= SWIRL_TEX_FLARE_TNG_5; i++ )
 *    {
 *        glGenTextures( 1, &(Globals::textures[i]) );
 *        sprintf( filename, "%sflare-tng-%d.bw", Globals::datapath.c_str(), i - SWIRL_TEX_FLARE_TNG_1 + 1 );
 *        setupTexture( filename, Globals::textures[i], minFilter, maxFilter );
 *    }
 *}
 */




//-------------------------------------------------------------------------------
// Name: swirl_loadTexture()
// Desc: load texture
//-------------------------------------------------------------------------------
/*
 *XTexture * swirl_loadTexture( const string & filename )
 *{
 *    // instantiate image data
 *    XTexture * tex = new XTexture;
 *
 *    // load the texture
 *    if( !swirl_initTexture( filename, tex ) )
 *    {
 *        delete tex;
 *        return NULL;
 *    }
 *
 *    return tex;
 *}
 */




//-------------------------------------------------------------------------------
// Name: swirl_loadTexture()
// Desc: load texture
//-------------------------------------------------------------------------------
/*
 *bool swirl_initTexture( const string & filename, XTexture * tex )
 *{
 *    // set desired resize
 *    tex->resizeWidth = 512;
 *    tex->resizeHeight = 512;
 *
 *    // generate the texture
 *    glGenTextures( 1, &tex->name );
 *    // bind the texture
 *    glBindTexture( GL_TEXTURE_2D, tex->name );
 *    // select modulate to mix texture with color for shading
 *    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
 *    // setting parameters
 *    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
 *    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 *
 *    // load the texture
 *    if( !XGfx::loadTexture( filename, tex ) )
 *    {
 *        cerr << "[swirl]: error - cannot load texture '" << filename.c_str() << "'..." << endl;
 *        return false;
 *    }
 *
 *    return true;
 *}
 */




//--------------------------------------------------------------------------------
// Name: setupTexture()
// Desc: ...
//--------------------------------------------------------------------------------
/*
 *void setupTexture( const char * filename, GLuint texobj,
 *                  GLenum minFilter, GLenum maxFilter, int useMipMaps )
 *{
 *    unsigned char * buf = NULL;
 *    int width = 0, height = 0, components = 0;
 *
 *    glBindTexture( GL_TEXTURE_2D, texobj );
 *    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
 *    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
 *    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter );
 *
 *    // log
 *    // fprintf( stderr, "[swirl]: loading %s...\n", filename );
 *
 *    // load luminance
 *    buf = loadLuminance( filename, &width, &height, &components );
 *
 *    // log
 *    // fprintf( stderr, "[swirl]: '%s' : %dx%dx%d\n", filename, width, height, components);
 *
 *    // build mip maps
 *    if( useMipMaps )
 *    {
 *        gluBuild2DMipmaps( GL_TEXTURE_2D, 1, width, height,
 *                          GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
 *    }
 *    else
 *    {
 *        glTexImage2D( GL_TEXTURE_2D, 0, 1, width, height, 0,
 *                     GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
 *    }
 *
 *    free(buf);
 *}
 */




//-----------------------------------------------------------------------------
// Name: checkTexDim( )
// Desc: checks to see if a dim is a valid opengl texture dimension
//-----------------------------------------------------------------------------
/*
 *bool checkTexDim( int dim )
 *{
 *    if( dim < 0 )
 *        return false;
 *
 *    int i, count = 0;
 *
 *    // count bits
 *    for( i = 0; i < 31; i++ )
 *        if( dim & ( 0x1 << i ) )
 *            count++;
 *
 *    // this is true only if dim is power of 2
 *    return count == 1;
 *}
 */


//-----------------------------------------------------------------------------
// Name: strafe_left( )
// Desc: move me left
//-----------------------------------------------------------------------------
void strafe_left()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraEye.x -= 0.1;
   Globals::cameraReference.x -= 0.1;

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraEyeX" )
   << Globals::cameraEye.x << osc::EndMessage
   << osc::EndBundle;
   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}


//-----------------------------------------------------------------------------
// Name: strafe_right( )
// Desc: move me right
//-----------------------------------------------------------------------------
void strafe_right()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraEye.x += 0.1;
   Globals::cameraReference.x += 0.1;

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraEyeX" )
   << Globals::cameraEye.x << osc::EndMessage
   << osc::EndBundle;
   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}

//-----------------------------------------------------------------------------
// Name: move_forward( )
// Desc: move me forward
//-----------------------------------------------------------------------------
void move_forward()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraEye.z += 0.1;
   Globals::cameraReference.z += 0.1;

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraEyeZ" )
   << Globals::cameraEye.z << osc::EndMessage
   << osc::EndBundle;
   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}

//-----------------------------------------------------------------------------
// Name: move_back( )
// Desc: move me backward
//-----------------------------------------------------------------------------
void move_back()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraEye.z -= 0.1;
   Globals::cameraReference.z -= 0.1;

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraEyeZ" )
   << Globals::cameraEye.z << osc::EndMessage
   << osc::EndBundle;
   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}

//-----------------------------------------------------------------------------
// Name: turn_left( )
// Desc: turn me left
//-----------------------------------------------------------------------------
void turn_left()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraReference.x = Globals::cameraReference.x * cos(0.1)
                              - Globals::cameraReference.z * sin(0.1);

   Globals::cameraReference.z = Globals::cameraReference.x * sin(0.1)
                              + Globals::cameraReference.z * cos(0.1);

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraReferenceX" )
   << 0.1f << osc::EndMessage
   << osc::EndBundle;

   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}

//-----------------------------------------------------------------------------
// Name: turn_left( )
// Desc: turn me left
//-----------------------------------------------------------------------------
void turn_right()
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOuttream( buffer, SWIRL_FRAMESIZE);

   Globals::cameraReference.x = Globals::cameraReference.x * cos(-0.1)
                              - Globals::cameraReference.z * sin(-0.1);

   Globals::cameraReference.z = Globals::cameraReference.x * sin(-0.1)
                              + Globals::cameraReference.z * cos(-0.1);

   oscOuttream << osc::BeginBundleImmediate
   << osc::BeginMessage( "/cameraReferenceX" )
   << -0.1f << osc::EndMessage
   << osc::EndBundle;

   transmitSocket->Send( oscOuttream.Data(), oscOuttream.Size() );
}
