//----------------------------------------------------------------------------
// Name: swirl-audio.h
// Desc: Audio stuff for swirl
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------
#ifndef __SWIRL_AUDIO_H__
#define __SWIRL_AUDIO_H__

#include "swirl-globals.h"
#include "swirl-sim.h"
#include "x-audio.h"
#include "x-thread.h"
#include "y-fft.h"
#include "y-waveform.h"
#include "y-fluidsynth.h"
#include "y-echo.h"
#include <iostream>
#include <vector>

using namespace std;

// Initialize audio
bool swirl_audio_init(
    unsigned int srate,
    unsigned int frameSize,
    unsigned channels
    );

// Start and stop the audio pipeline
bool swirl_audio_start();
void swirl_audio_stop();


//-----------------------------------------------------------------------------
// Name: class SwirlNote
// Desc: a note event
//-----------------------------------------------------------------------------
class SwirlNote
{
public:
    SwirlNote( int c, float p, float v, float d );

public:
    int   channel;
    float pitch;
    float velocity;
    float duration; // in seconds
    // add more stuff?
};

//-----------------------------------------------------------------------------
// Name: class SwirlNoteSequence
// Desc: a sequence of notes
//-----------------------------------------------------------------------------
class SwirlNoteSequence : public YEntity
{
public:
    SwirlNoteSequence(); // constructor
    SwirlNoteSequence( YTimeInterval startTime );
    // update
    void update( YTimeInterval dt );
    // render
    //void render();

    void addNote( SwirlNote note );

public:
    vector<SwirlNote> notes;    // The sequence of notes
    YTimeInterval nextTime;
    int noteIndex;

protected:
    XMutex m_mutex;
};



// Music-related
void swirl_playNotes( float pitch, float velocity );

void swirl_pullNotes();

#endif
