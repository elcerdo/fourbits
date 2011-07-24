#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bass.h"


class Player {
    public:
	Player();
	~Player();
	void setNote(int note, bool state);
	void clearNote();
    protected:
	BASS_INFO info;
	HSTREAM stream;
};

#endif
