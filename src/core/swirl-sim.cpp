//-----------------------------------------------------------------------------
// Name: swirl-sim.cpp
// Desc: visualization simulation
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//-----------------------------------------------------------------------------
#include "swirl-sim.h"
#include <iostream>
using namespace std;

// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)

//-------------------------------------------------------------------------------
// name: SWIRLSim()
// desc: constructor
//-------------------------------------------------------------------------------
SWIRLSim::SWIRLSim()
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
// name: ~SWIRLSim()
// desc: destructor
//-------------------------------------------------------------------------------
SWIRLSim::~SWIRLSim()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------
// name: systemCascade()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void SWIRLSim::systemCascade()
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
// name: audioCascade()
// desc: ..
//-------------------------------------------------------------------------------
void SWIRLSim::audioCascade(SAMPLE * buffer, unsigned int numFrames)
{

    // Below is to tick all sample by sample...
/*
 *    SAMPLE oneFrame[SWIRL_NUMCHANNELS];
 *    // Zero out to start
 *    memset(oneFrame, 0, sizeof(SAMPLE)*SWIRL_NUMCHANNELS);
 *
 *    for (int i = 0; i < numFrames; ++i)
 *    {
 *         m_gfxRoot.tickAll( oneFrame, Globals::camera->loc );
 *
 *        for (int j = 0; j < SWIRL_NUMCHANNELS; ++j)
 *        {
 *            buffer[i* SWIRL_NUMCHANNELS+j] = oneFrame[j];
 *        }
 *    }
 */

    // Instead we synthesize all per buffer.

    // Zero out to start
    memset(buffer, 0, sizeof(SAMPLE)*SWIRL_NUMCHANNELS*numFrames);

    m_gfxRoot.synthesizeAll( buffer, numFrames, Globals::camera->loc );
}


//-------------------------------------------------------------------------------
// Pause the simulation
//-------------------------------------------------------------------------------
void SWIRLSim::pause() { m_isPaused = true; }

//-------------------------------------------------------------------------------
// Resume the simulation
//-------------------------------------------------------------------------------
void SWIRLSim::resume() { m_isPaused = false; }

//-------------------------------------------------------------------------------
// Get is paused
//-------------------------------------------------------------------------------
bool SWIRLSim::isPaused() const { return m_isPaused; }

//-------------------------------------------------------------------------------
// Set desired frame rate
//-------------------------------------------------------------------------------
void SWIRLSim::setDesiredFrameRate( double frate )
{
  m_desiredFrameRate = frate;
}

//-------------------------------------------------------------------------------
// Get desired frame rate
//-------------------------------------------------------------------------------
double SWIRLSim::getDesiredFrameRate() const { return m_desiredFrameRate; }

//-------------------------------------------------------------------------------
// Get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval SWIRLSim::delta() const { return m_lastDelta; }
