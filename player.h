#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bass.h"
#include <string>

#define NSAMPLES 4

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
    protected:
	HSTREAM stream;
	HSAMPLE samples[NSAMPLES];
	float samplesVolumes[NSAMPLES];
	bool samplesWorking;
};

#endif
