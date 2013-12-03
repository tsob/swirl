//----------------------------------------------------------------------------
// Name: swirl-audio.cpp
// Desc: Audio stuff for swirl
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "swirl-audio.h"
#include <vector>

using namespace std;

// globals
double g_now;
double g_nextTime;
int g_prog = 0;



// HACK: vector of notes
vector<SwirlNote> g_notes;
int g_noteIndex = 0;
XMutex g_mutex;

//-----------------------------------------------------------------------------
// Name: class SwirlNote
// Desc: Constructor
//-----------------------------------------------------------------------------
SwirlNote::SwirlNote( int c, float p, float v, float d )
{
    channel  = c;
    pitch    = p;
    velocity = v;
    duration = d;
};

//-----------------------------------------------------------------------------
// Name: class SwirlNoteSequence
// Desc: Constructor
//-----------------------------------------------------------------------------
SwirlNoteSequence::SwirlNoteSequence( )
{
    // Clear vectors
    notes.clear();
    nextTime = 0.0; // Start immediately
    noteIndex = 0;
}
SwirlNoteSequence::SwirlNoteSequence( YTimeInterval startTime  )
{
    // Clear vectors
    notes.clear();
    nextTime = startTime;
    noteIndex = 0;
}

//-----------------------------------------------------------------------------
// Name: addNote [class SwirlNoteSequence]
// Desc: Add a note to the sequence
//-----------------------------------------------------------------------------
void SwirlNoteSequence::addNote( SwirlNote note )
{
    // Add note to sequence
    notes.push_back( note );
}

// Update
void SwirlNoteSequence::update( YTimeInterval dt )
{
    int noteIndex = 0;
    YTimeInterval timeAccumulation = 0.0;

    if( dt > nextTime )
    {
        // lock (to protect vector)
        m_mutex.acquire();
        // move down the vector
        if( noteIndex < notes.size() )
        {
            // temporary note pointer
            SwirlNote * n = &g_notes[g_noteIndex];
            // note on!
            Globals::synth->noteOn( n->channel, n->pitch, n->velocity * 120 );
            // check to see next time
            nextTime += n->duration;
            // move to next note for next time
            noteIndex++;
        }
        // release lock
        m_mutex.release();
    }
    else
    {
      nextTime -= dt;
    }
}

// Play some notes
void swirl_playNotes( float pitch, float velocity )
{
    // lock
    g_mutex.acquire();
    // clear notes
    g_notes.clear();

    for( int i = 0; i < 24; i++ )
    {
        // next notes
        g_notes.push_back( SwirlNote( 0, pitch + i*2, (1 - i/24.0), .05 + .15*(1 - i/24.0) ) );
    }
    // unlock
    g_mutex.release();

    // reset the index
    g_noteIndex = 0;

    // play now!
    g_nextTime = g_now;
}


//-----------------------------------------------------------------------------
// Name: swirl_pullNotes()
// Desc: Used by the audio callback to pull a note into the buffer when it's
//       time.
//-----------------------------------------------------------------------------
void swirl_pullNotes()
{



    // HACK: rough time keeping for next notes - this logic really should be
    // somewhere else: e.g., in its own class and not directly in the audio callback!
    if( g_now > g_nextTime )
    {
        // lock (to protect vector)
        g_mutex.acquire();
        // move down the vector
        if( g_noteIndex < g_notes.size() )
        {
            // temporary note pointer
            SwirlNote * n = &g_notes[g_noteIndex];
            // note on!
            Globals::synth->noteOn( n->channel, n->pitch, n->velocity * 120 );
            // HACK: with a major 3rd above!
            //Globals::synth->noteOn( n->channel, n->pitch + 4, n->velocity * 80 );
            // check to see next time
            g_nextTime += n->duration * SWIRL_SRATE;
            // move to next note for next time
            g_noteIndex++;
        }
        // release lock
        g_mutex.release();
    }

}


//-----------------------------------------------------------------------------
// Name: audio_callback
// Desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    // keep track of current time in samples
    g_now += numFrames;

    // Play notes if it's time
    swirl_pullNotes();


    // sum
    SAMPLE sum = 0;
    // num channels
    unsigned int channels = Globals::lastAudioBufferChannels;

    // Zero out buffers
    memset( Globals::lastAudioBuffer, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames*channels );
    memset( Globals::lastAudioBufferMono, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames );

    // Copy to global buffer
    memcpy( Globals::lastAudioBuffer, buffer,
           sizeof(SAMPLE)*numFrames*channels );


    // synthesize it
    Globals::synth->synthesize2( buffer, numFrames );

    // Cascade audio for simulation
    Globals::sim->audioCascade( buffer, numFrames );

    // echo it
    Globals::echo->synthesize2( buffer, numFrames );

    // copy to mono buffer
    for( int i = 0; i < numFrames; i++ )
    {
        // zero out
        sum = 0;
        // loop over channels
        for( int j = 0; j < channels; j++ )
        {
            // sum
            sum += buffer[i*channels + j];
        }
        // set
        Globals::lastAudioBufferMono[i] = sum / channels;
    }

    // window it for taper in visuals
    for( int i = 0; i < numFrames; i++ )
    {
        // multiply
        Globals::lastAudioBufferMono[i] *= Globals::audioBufferWindow[i];
    }

    // set in the wave
    Globals::waveform->set( Globals::lastAudioBufferMono, numFrames );
}




//-----------------------------------------------------------------------------
// Name: swirl_audio_init()
// Desc: initialize audio system
//-----------------------------------------------------------------------------
bool swirl_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    // log
    cerr << "[swirl]: initializing audio system..." << endl;

    // initialize
    if( !XAudioIO::init(
          (unsigned int)0,
          (unsigned int)0,
          (unsigned int)srate,
          (unsigned int &)frameSize,
          (unsigned int)channels,
          (XAudioCallback)audio_callback,
          (void*)0 )
      )
    {
        // done
        return false;
    }

    // Instantiate fluidsynth
    Globals::synth = new GeXFluidSynth();
    // Init fluidsynth
    Globals::synth->init( srate, 32 );
    // Load the soundfont
    Globals::synth->load( "data/soundfonts/birds.sf2", "" );
    // Map program changes
    Globals::synth->programChange( 0, 0 );
    //Globals::synth->programChange( 1, 79 );
    //Globals::synth->programChange( 2, 4 );
    //Globals::synth->programChange( 3, 10 );
    //Globals::synth->programChange( 4, 13 );

    // Allocate echo
    Globals::echo = new YEcho( srate );
    Globals::echo->setDelay( 0, .25 );
    Globals::echo->setDelay( 1, .5 );

    // make a note
    //g_note = makeNote( 0, 60, .9, .5, 0 );

    // Allocate audio buffers
    Globals::lastAudioBuffer     = new SAMPLE[frameSize*channels];
    Globals::lastAudioBufferMono = new SAMPLE[frameSize];
    Globals::audioBufferWindow   = new SAMPLE[frameSize];

    // Set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames   = frameSize;

    // Set num channels
    Globals::lastAudioBufferChannels = channels;

    // Compute the window
    hanning( Globals::audioBufferWindow, frameSize );

    // Create waveform
    Globals::waveform = new YWaveform();
    Globals::waveform->loc.y = 0.5f;          // Place it
    Globals::waveform->loc.z = 2.5f;          // Place it
    Globals::waveform->col = Globals::ourRed; // Color it
    Globals::waveform->setWidth(  2.5f );     // Set the width
    Globals::waveform->setHeight( 1.0f );     // Set the height
    Globals::waveform->init( frameSize );     // Initialize it
    Globals::waveform->active = true;         // Active?
    // Add waveform to sim
    Globals::camera->addChild( Globals::waveform );


    return true;
}




//-----------------------------------------------------------------------------
// Name: swirl_audio_start()
// Desc: start audio system
//-----------------------------------------------------------------------------
bool swirl_audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
// Name: swirl_audio_stop()
// Desc: stop audio system
//-----------------------------------------------------------------------------
void swirl_audio_stop()
{
    // stop the audio
    XAudioIO::stop();
}
