//-----------------------------------------------------------------------------
// name: swirl-audio.h
// desc: audio stuff
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __SWIRL_AUDIO_H__
#define __SWIRL_AUDIO_H__

#include "x-audio.h"

// init audio
bool swirl_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool swirl_audio_start();
void swirl_audio_stop();
void swirl_playNotes( float pitch, float velocity );


#endif
