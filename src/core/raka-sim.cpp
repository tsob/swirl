//-----------------------------------------------------------------------------
// Name: raka-sim.cpp
// Desc: visualization simulation
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//-----------------------------------------------------------------------------
#include "raka-sim.h"
#include <iostream>
using namespace std;

// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)

//-------------------------------------------------------------------------------
// name: RAKASim()
// desc: constructor
//-------------------------------------------------------------------------------
RAKASim::RAKASim()
{
    m_desiredFrameRate = 60;
    m_useFixedTimeStep = false;
    m_timeLeftOver     = 0;
    m_simTime          = 0;
    m_lastDelta        = 0;
    m_first            = true;
    m_isPaused         = false;
}

//-------------------------------------------------------------------------------
// name: ~RAKASim()
// desc: destructor
//-------------------------------------------------------------------------------
RAKASim::~RAKASim()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------
// name: systemCascade()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void RAKASim::systemCascade()
{
    // Get current time (once per frame)
    XGfx::getCurrentTime( true );
    
    // Timing loop
    YTimeInterval timeElapsed = XGfx::getCurrentTime() - m_simTime;
    m_simTime  += timeElapsed;
    
    // Special case: first update
    if( m_first )
    {
        timeElapsed = STEPTIME; // Set time just enough for one update
        m_first = false;        // Set flag
    }
    
    // Clamp time elapsed
    if( timeElapsed > SIM_SKIP_TIME )
        timeElapsed = SIM_SKIP_TIME;
    
    // Update it as long as we aren't paused
    if( !m_isPaused )
    {
        // Update the world with a fixed timestep
        m_gfxRoot.updateAll( timeElapsed );
    }
    
    // Redraw
    m_gfxRoot.drawAll();
    
    // Set
    m_lastDelta = timeElapsed;
}

//-------------------------------------------------------------------------------
// Pause the simulation
//-------------------------------------------------------------------------------
void RAKASim::pause() { m_isPaused = true; }

//-------------------------------------------------------------------------------
// Resume the simulation
//-------------------------------------------------------------------------------
void RAKASim::resume() { m_isPaused = false; }

//-------------------------------------------------------------------------------
// Get is paused
//-------------------------------------------------------------------------------
bool RAKASim::isPaused() const { return m_isPaused; }

//-------------------------------------------------------------------------------
// Set desired frame rate
//-------------------------------------------------------------------------------
void RAKASim::setDesiredFrameRate( double frate )
{
  m_desiredFrameRate = frate;
}

//-------------------------------------------------------------------------------
// Get desired frame rate
//-------------------------------------------------------------------------------
double RAKASim::getDesiredFrameRate() const { return m_desiredFrameRate; }

//-------------------------------------------------------------------------------
// Get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval RAKASim::delta() const { return m_lastDelta; }
