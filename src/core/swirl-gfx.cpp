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
#include "swirl-networking.h"

using namespace std;

//-----------------------------------------------------------------------------
// Name: swirl_gfx_init( )
// Desc: graphics/interaction entry point
//-----------------------------------------------------------------------------
bool swirl_gfx_init( int argc, const char ** argv )
{

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

    // Double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // Initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );

    // Set the window postion
    glutInitWindowPosition( 100, 100 );

    // Create the window
    glutCreateWindow("Swirl, the Sonic World");

    // Full screen
    //TODO
    if( Globals::fullscreen )
    {
      glutSetCursor(GLUT_CURSOR_NONE);
      glutFullScreen();
    }

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

    // Set the mouse movement function
    // TODO troubleshoot
    glutPassiveMotionFunc( motion );

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
    //swirl_endline();
    //swirl_keys();
    //swirl_line();
    //swirl_endline();

    // Debug: print scene graph //TODO remove
    swirl_line();
    cerr << "Scene graph:" << endl;
    Globals::sim->root().dumpSceneGraph(1);
    swirl_line();


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
    // Log
    cerr << "[swirl]: initializing graphics system..." << endl;

    // Reset time
    XGfx::resetCurrentTime();
    // Set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // Get the first
    XGfx::getCurrentTime( true );
    // Seed random number generator
    XFun::srand();

    // set the GL clear color - use when the color buffer is cleared
    glClearColor(
        Globals::bgColor.actual().x,
        Globals::bgColor.actual().y,
        Globals::bgColor.actual().z,
        1.0f
        );
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
    Globals::fog_density = .000882f;

    // fog color
    GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};
    // fog mode
    if( Globals::fog_filter )
      glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
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
    // seed rand
    XFun::srand();

    // instantiate simulation
    Globals::sim = new SWIRLSim();

    // add to simulation
    SWIRLAvatar* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;
    
    Globals::sim->root().addChild( myAvatar );
    Globals::sim->root().addChild( new SWIRLMoon  );

<<<<<<< HEAD
    myAvatar->iLoc = iSlew3D( myAvatar->loc, 5.0f );
    myAvatar->iRefLoc = iSlew3D( myAvatar->refLoc, 5.0f );
=======
    Globals::myAvatar->iLoc = iSlew3D( Globals::myAvatar->loc, 1.0f );
    Globals::myAvatar->iRefLoc = iSlew3D( Globals::myAvatar->refLoc, 1.0f );

    if (Globals::app == 1)
    {
        Globals::myAvatar->loc.z = -2;
        Globals::myAvatar->refLoc.z = 9;
    }
    else
    {
        Globals::myAvatar->loc.z = -1;
        Globals::myAvatar->refLoc.z = 10;
    }
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f

    myAvatar->goal = myAvatar->loc;
    myAvatar->refGoal = myAvatar->refLoc;

    Globals::camera->relativePosition = iSlew3D( Globals::firstPerson, 0.5f);

<<<<<<< HEAD
    myAvatar->addChild( Globals::camera );
    Globals::sim->root().addChild( myAvatar );

    Globals::sim->root().addChild( new SWIRLBirdCube() );

    // Put a cube in the environment
    //YCube* cube = new YCube();
    //cube->loc = Vector3D( 1.0f, 1.0f, 1.0f  );
    //Globals::sim->root().addChild( cube );
=======
    Globals::myAvatar->addChild( Globals::camera );
    Globals::sim->root().addChild( Globals::myAvatar );
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f
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
    SWIRLAvatar* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

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


    gluLookAt(
        Globals::camera->absLoc.x,
        Globals::camera->absLoc.y,
        Globals::camera->absLoc.z,
<<<<<<< HEAD
        myAvatar->refLoc.x,
        myAvatar->refLoc.y,
        myAvatar->refLoc.z,
=======
        Globals::myAvatar->iRefLoc.actual().x,
        Globals::myAvatar->iRefLoc.actual().y,
        Globals::myAvatar->iRefLoc.actual().z,
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f
        0.0f, 1.0f, 0.0f
        );

    // set the position of the lights
    glLightfv( GL_LIGHT0, GL_POSITION, Globals::light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, Globals::light1_pos );
}


//-----------------------------------------------------------------------------
// Name: mouseMoveFunc( )
// Desc: mouse movement handler
//-----------------------------------------------------------------------------
void mouseMoveFunc( int x, int y )
{
    SWIRLAvatar* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

    static float lastx = 0.0;
    static float lasty = 0.0;

    lastx = (float)x - lastx;
    lasty = (float)y - lasty;

    if ( (abs((int)lastx)>10) || (abs((int)lasty)>20) )
    {
        lastx = (float)x;
        lasty = (float)y;
        return;
    }

    if( (float)x > lastx )
        myAvatar->turn(lastx*0.01);
    else
        myAvatar->turn((-lastx)*0.01);
    if( (float)y > 0 )
        myAvatar->move(-lasty*0.01);
    else
        myAvatar->move(lasty*0.01);

    lastx = (float)x;
    lasty = (float)y;

    // do a reshape since viewEyeY might have changed
    reshapeFunc( Globals::windowWidth, Globals::windowHeight );
    // post redisplay
    glutPostRedisplay( );

}

//-----------------------------------------------------------------------------
// Name: motion( int x, int y )
// Desc: A mouse movement function adapted slightly from
//       http://en.wikibooks.org/wiki/OpenGL_Programming/Glescraft_4
//-----------------------------------------------------------------------------
void motion(int x, int y) {
    static bool wrap = false;

    if(!wrap) {
        int ww = glutGet(GLUT_WINDOW_WIDTH);
        int wh = glutGet(GLUT_WINDOW_HEIGHT);

        int dx = x - ww / 2;
        int dy = y - wh / 2;

        // Do something with dx and dy here
        Globals::myAvatar->move(dy*-0.01);
        Globals::myAvatar->turn(dx*0.01);
        reshapeFunc( Globals::windowWidth, Globals::windowHeight );
        glutPostRedisplay( );

        // move mouse pointer back to the center of the window
        wrap = true;
        glutWarpPointer(ww / 2, wh / 2);
    }
    else
    {
        wrap = false;
    }
}

//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
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
        case 'u':
        {
            //// add some cubes
            //for( int i = 0; i < 10; i++ )
            //{
                //Globals::sim->root().addChild( new YCube );
            //}
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
                glutSetCursor(GLUT_CURSOR_NONE);
                glutFullScreen();
            }
            else
            {
                glutSetCursor(GLUT_CURSOR_INHERIT);
                glutReshapeWindow( Globals::lastWindowWidth, Globals::lastWindowHeight );
            }

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

        case '1':
            dropRandCube();
            break;
        case '2':
            dropNoteSphere();
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
        SWIRLServerProxy* serverProxy = ((SWIRLClient*)Globals::application)->serverProxy;
        SWIRLAvatar* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

        switch( key )
        {
            case ']':
                // turn right
<<<<<<< HEAD
                myAvatar->turn(0.1f);
                serverProxy->perform( myAvatar->id, "/turn", 0.1f );
                break;
            case '[':
                // turn left
                myAvatar->turn(-0.1f);
                serverProxy->perform( myAvatar->id, "/turn", -0.1f );
                break;
            case 'w':
                // move forward
                myAvatar->move(0.2f);
                serverProxy->perform( myAvatar->id, "/move", 0.2f );
                break;
            case 'x':
                // move back
                myAvatar->move(-0.2f);
                serverProxy->perform( myAvatar->id, "/move", -0.2f );
                break;
            case 'd':
                //strafe right
                myAvatar->strafe(0.1f);
                serverProxy->perform( myAvatar->id, "/strafe", 0.1f );
                break;
            case 'a':
                //strafe left
                myAvatar->strafe(-0.1f);
                serverProxy->perform( myAvatar->id, "/strafe", -0.1f );
=======
                Globals::myAvatar->turn(0.4f);
                swirl_send_message( "/turn", 0.1f );
                break;
            case '[':
                // turn left
                Globals::myAvatar->turn(-0.4f);
                swirl_send_message( "/turn", -0.1f );
                break;
            case 'w':
                // move forward
                Globals::myAvatar->move(0.4f);
                swirl_send_message( "/move", 0.2f );
                break;
            case 'x':
                // move back
                Globals::myAvatar->move(-0.4f);
                swirl_send_message( "/move", -0.2f );
                break;
            case 'd':
                //strafe right
                Globals::myAvatar->strafe(0.4f);
                swirl_send_message( "/strafe", 0.1f );
                break;
            case 'a':
                //strafe left
                Globals::myAvatar->strafe(-0.4f);
                swirl_send_message( "/strafe", -0.1f );
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f
                break;
            case 'c':
                // toggle camera position
                Globals::camera->togglePosition();
                break;
            case '\'':
                Globals::bgColor.update( Globals::nightSky );
                break;
            case ';':
                Globals::bgColor.update( Globals::skyBlue );
                break;
            case 'l':
                Globals::bgColor.update( Globals::sunset );
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

    // TODO necessary?
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
<<<<<<< HEAD
    SWIRLAvatar* myAvatar = ((SWIRLClient*)Globals::application)->myAvatar;

    SWIRLBirdCube * nextBirdCube = new SWIRLBirdCube;
    nextBirdCube->loc = myAvatar->loc;
    nextBirdCube->col = Vector3D( XFun::rand2f(0,1), XFun::rand2f(0,1), XFun::rand2f(0,1));
    Globals::sim->root().addChild( nextBirdCube );
=======
>>>>>>> 597465cb69cc9ed9d72943c175d7f4211cd9fe5f

    // debug TODO remove
    cout << "camera absolute position: " <<
      Globals::camera->absLoc.x << ", " <<
      Globals::camera->absLoc.y << ", " <<
      Globals::camera->absLoc.z << ", " <<
      endl;

    cout << "camera relative position: " <<
      Globals::camera->loc.x << ", " <<
      Globals::camera->loc.y << ", " <<
      Globals::camera->loc.z << ", " <<
      endl;

    cout << "My avatar position: " <<
      Globals::myAvatar->loc.x << ", " <<
      Globals::myAvatar->loc.y << ", " <<
      Globals::myAvatar->loc.z << ", " <<
      endl;

    cout << "My avatar reference position: " <<
      Globals::myAvatar->refLoc.x << ", " <<
      Globals::myAvatar->refLoc.y << ", " <<
      Globals::myAvatar->refLoc.z << ", " <<
      endl;

    Vector3D lookDirection = Globals::myAvatar->loc - Globals::myAvatar->refLoc;
    cout << "Look direction from avatar: " <<
      lookDirection.x << ", " <<
      lookDirection.y << ", " <<
      lookDirection.z << ", " <<
      endl;
    lookDirection = Globals::camera->absLoc - Globals::myAvatar->refLoc;
    cout << "Look direction from camera: " <<
      lookDirection.x << ", " <<
      lookDirection.y << ", " <<
      lookDirection.z << ", " <<
      endl;


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

    look();

    // save state
    glPushMatrix();

    // slew
    Globals::viewEyeY.interp( XGfx::delta());
    Globals::viewRadius.interp( XGfx::delta() );

    // draw the floor
    renderBackground();

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
    float backgroundRadius = 200.0;
    // save the current matrix
    glPushMatrix( );

        glColor4f(
            Globals::darkMossGreen.x,
            Globals::darkMossGreen.y,
            Globals::darkMossGreen.z,
            1.0f
            ); //ground color

        glBegin(GL_TRIANGLE_FAN);//start drawing triangles
            glVertex3f(-backgroundRadius,-1.0f,-backgroundRadius);
            glVertex3f( backgroundRadius,-1.0f,-backgroundRadius);
            glVertex3f( backgroundRadius,-1.0f, backgroundRadius);
            //drawing a new triangle to complete the rectangle
            glVertex3f( backgroundRadius,-1.0f, backgroundRadius);
            glVertex3f(-backgroundRadius,-1.0f, backgroundRadius);
            glVertex3f(-backgroundRadius,-1.0f,-backgroundRadius);
        glEnd();//end drawing of triangles

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
// Name: dropRandCube
// Desc: Drops a rand-pitch cube at the user's location
//-----------------------------------------------------------------------------
void dropRandCube()
{
    SWIRLBirdCube * nextCube = new SWIRLBirdCube;
    nextCube->loc = Globals::myAvatar->loc;
    nextCube->col = Vector3D( XFun::rand2f(0,1), XFun::rand2f(0,1), XFun::rand2f(0,1));
    Globals::sim->root().addChild( nextCube );

    glutPostRedisplay( );
}
//-----------------------------------------------------------------------------
// Name: dropNoteSphere
// Desc: 
//-----------------------------------------------------------------------------
void dropNoteSphere()
{
    SWIRLNoteSphere * nextObj = new SWIRLNoteSphere;
    nextObj->loc = Globals::myAvatar->loc;
    Globals::sim->root().addChild( nextObj );

    glutPostRedisplay( );
}
