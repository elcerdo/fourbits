#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bass.h"


class Player {
    public:
	Player();
	~Player();
	void setNote(int note, bool state);
	void clearNotes();
	void fade();
	void playSample(int number);
    protected:
	BASS_INFO info;
	HSTREAM stream;
	HSAMPLE samples[4];
	bool fading;
};

#endif
