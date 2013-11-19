//-----------------------------------------------------------------------------
// Name: raka-sim.h
// Desc: visualization simulation
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_SIM_H__
#define __RAKA_SIM_H__

#include "raka-entity.h"

//-----------------------------------------------------------------------------
// name: class RAKASim
// desc: simulation class
//-----------------------------------------------------------------------------
class RAKASim
{
public:
    RAKASim();
    virtual ~RAKASim();
    
public:
    // cascade timestep simulation through system (as connected to this)
    void systemCascade();
    
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
    YEntity & root() { return m_gfxRoot; }
    
protected:
    YEntity m_gfxRoot;
    
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
