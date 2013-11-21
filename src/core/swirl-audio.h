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

#include "x-audio.h"

// Initialize audio
bool swirl_audio_init(
    unsigned int srate,
    unsigned int frameSize,
    unsigned channels
    );

// Start and stop the audio pipeline
bool swirl_audio_start();
void swirl_audio_stop();

// Music-related
void swirl_playNotes( float pitch, float velocity );


#endif
