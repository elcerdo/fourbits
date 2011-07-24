#include "player.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
using std::endl;
using std::cerr;
using std::cout;

#define PI 3.14159265358979323846
#define TABLESIZE 2048
#define KEYS 17

int sinetable[TABLESIZE];

#define MAXVOL  4000    // higher value = longer fadeout
int vol[KEYS]={0},pos[KEYS];

DWORD CALLBACK WriteStream(HSTREAM, short *buffer, DWORD length, void *) {
    memset(buffer,0,length);
    for (int n=0;n<KEYS;n++) {
	if (!vol[n]) continue;
	float f=pow(2.0,(n+3)/12.0)*TABLESIZE*440.0/44100.0;
	for (DWORD c=0;c<length/4 && vol[n];c++) {
	    int s=sinetable[(int)((pos[n]++)*f)&(TABLESIZE-1)]*vol[n]/MAXVOL;
	    s+=(int)buffer[c*2];
	    if (s>32767) s=32767;
	    else if (s<-32768) s=-32768;
	    buffer[c*2+1]=buffer[c*2]=s; // left and right channels are the same
	    if (vol[n]<MAXVOL) vol[n]--;
	}
    }
    return length;
}

void Player::clearNotes() {
    for (int n=0;n<KEYS;n++) {
	setNote(n,false);
    }
}

void Player::setNote(int key,bool state) {
    if (state) {
	pos[key]=0;
	vol[key]=MAXVOL; 
    } else {
	vol[key]=0;
    }
}

void Player::playSample(int number) {
    cout << "playing sample " << number << endl;

    HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,0.5f);
    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_PAN,((rand()%201)-100)/100.f);

    BASS_ChannelPlay(handle,false);
}

void Player::fade() {
    for (int n=0;n<KEYS;n++) {
	if (vol[n]==MAXVOL) vol[n]--;
    }

}


Player::Player() {
    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

    if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,NULL,NULL)) {
	cerr << "can't init bass" << endl;
	exit(2);
    }

    for (int a=0; a<TABLESIZE; a++) sinetable[a]=(int)(sin(2.0*PI*(double)a/TABLESIZE)*7000.0);

    BASS_GetInfo(&info);
    BASS_SetConfig(BASS_CONFIG_BUFFER,10+info.minbuf);

    int number;
    number = 0;
    samples[number] = BASS_SampleLoad(false,"01/00.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    number = 1;
    samples[number] = BASS_SampleLoad(false,"01/01.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    number = 2;
    samples[number] = BASS_SampleLoad(false,"01/02.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    number = 3;
    samples[number] = BASS_SampleLoad(false,"01/03.wav",0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);

    stream = BASS_StreamCreate(44100,2,0,(STREAMPROC*)WriteStream,0);
    BASS_ChannelPlay(stream,TRUE);
}

Player::~Player() {
    BASS_StreamFree(stream);
    BASS_Free();
}
