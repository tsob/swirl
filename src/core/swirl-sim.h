//-----------------------------------------------------------------------------
// Name: swirl-sim.h
// Desc: visualization simulation
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//-----------------------------------------------------------------------------
#ifndef __SWIRL_SIM_H__
#define __SWIRL_SIM_H__

#include "swirl-entity.h"
#include "swirl-globals.h"

//-----------------------------------------------------------------------------
// name: class SWIRLSim
// desc: simulation class
//-----------------------------------------------------------------------------
class SWIRLSim
{
public:
    SWIRLSim();
    virtual ~SWIRLSim();

public:
    // cascade timestep simulation through system (as connected to this)
    void systemCascade();
    // Generate audio buffers from all audio objects in sim
    void audioCascade(SAMPLE * buffer, unsigned int numFrames);

public:
    // pause the simulation
    void pause();
    // resume the simulation
    void resume();
    // get is paused
    bool isPaused() const;

public:
    // set desired frame rate
    void setDesiredFrameRate( double frate );
    // get it
    double getDesiredFrameRate() const;
    // get the timestep in effect (fixed or dynamic)
    YTimeInterval delta() const;

public:
    // get the root
    SWIRLEntity & root() { return m_gfxRoot; }

protected:
    SWIRLEntity m_gfxRoot;

public:
    double m_desiredFrameRate;
    bool   m_useFixedTimeStep;

public:
    YTimeInterval m_timeLeftOver;
    YTimeInterval m_simTime;
    YTimeInterval m_lastDelta;
    bool          m_first;
    bool          m_isPaused;
};

#endif
