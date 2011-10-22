#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bass.h"
#include <string>

#define NSAMPLES 4

struct NoteData {
    static NoteData standard();
    static const int nstates = 16;
    static const int volume_max = 4000;
    int notes[nstates];
    int volumes[nstates];
    int positions[nstates];
};

class Player {
    public:
	Player();
	~Player();
	void setNote(int note, bool state);
	bool loadSamplesFromDirectory(const std::string &directory);
	void clearNotes();
	void fade();
	void playSample(int sample);
	void setSampleVolume(int sample, float volume);
	void setSynthVolume(float volume);
        NoteData& getNoteData();
    protected:
	HSTREAM stream;
	HSAMPLE samples[NSAMPLES];
	float samplesVolumes[NSAMPLES];
	bool samplesWorking;
};

#endif
