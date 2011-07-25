#include "player.h"

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <string>
//#include <iostream>
//using std::endl;
//using std::cerr;
//using std::cout;

#define PI 3.14159265358979323846
#define TABLESIZE 2048
#define KEYS 17

int sinetable[TABLESIZE];

#define MAXVOL  4000    // higher value = longer fadeout
int vol[KEYS] = {0},pos[KEYS];

int gamme[16] = {
    -24, -22, -20, -19, -17, -15, -14, -12, -10, -8, -7, -5, -3, -2, 0, 2
};


DWORD CALLBACK WriteStream(HSTREAM, short *buffer, DWORD length, void *) {
    memset(buffer,0,length);
    for (int n=0; n<KEYS; n++) {
        if (!vol[n]) continue;
        float f=pow(2.0,gamme[n]/12.0+1)*TABLESIZE*440.0/44100.0;
        for (DWORD c=0; c<length/4 && vol[n]; c++) {
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
    for (int n=0; n<KEYS; n++) {
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
    assert(samplesWorking);
    //cout << "playing sample " << number << endl;

    HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,1.);
    if (number==0) {
        BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,.1);
    }

    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_PAN,((rand()%201)-100)/100.f);

    BASS_ChannelPlay(handle,false);
}

void Player::fade() {
    for (int n=0; n<KEYS; n++) {
        if (vol[n]==MAXVOL) vol[n]--;
    }

}


Player::Player() :
    samplesWorking(false) {
    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

    if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,NULL,NULL)) {
        //cerr << "can't init bass" << endl;
        exit(2);
    }

    for (int a=0; a<TABLESIZE; a++) sinetable[a]=(int)(sin(2.0*PI*(double)a/TABLESIZE)*7000.0);

    BASS_GetInfo(&info);
    BASS_SetConfig(BASS_CONFIG_BUFFER,10+info.minbuf);

    stream = BASS_StreamCreate(44100,2,0,(STREAMPROC*)WriteStream,0);
    BASS_ChannelPlay(stream,TRUE);

    loadSamplesFromDirectory("andrew");
}

void Player::loadSamplesFromDirectory(const std::string &directory) {

    int number;
    number = 0;
    samples[number] = BASS_SampleLoad(false,(directory+"/1.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    {
        HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
        BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,.01);
    }
    number = 1;
    samples[number] = BASS_SampleLoad(false,(directory+"/2.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    {
        HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
        BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,1.);
    }
    number = 2;
    samples[number] = BASS_SampleLoad(false,(directory+"/3.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    {
        HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
        BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,1.);
    }
    number = 3;
    samples[number] = BASS_SampleLoad(false,(directory+"/4.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    assert(samples[number]);
    {
        HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
        BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,1.);
    }

    samplesWorking = true;
}

Player::~Player() {
    BASS_StreamFree(stream);
    BASS_Free();
}
