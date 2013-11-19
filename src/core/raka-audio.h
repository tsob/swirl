//-----------------------------------------------------------------------------
// name: raka-audio.h
// desc: audio stuff
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_AUDIO_H__
#define __RAKA_AUDIO_H__

#include "x-audio.h"

// init audio
bool raka_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool raka_audio_start();

// play some notes
void raka_playNotes( float pitch, float velocity );


#endif
