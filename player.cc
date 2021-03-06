#include "player.h"

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <string>
#include <iostream>
using std::endl;
using std::cerr;
using std::cout;

#define PI 3.14159265358979323846
#define TABLESIZE 2048
int sinetable[TABLESIZE];

NoteData NoteData::standard() {
    static const int default_notes[16] = {-24, -22, -20, -19, -17, -15, -14, -12, -10, -8, -7, -5, -3, -2, 0, 2};
    NoteData data;
    for (int kk=0; kk<NoteData::nstates; kk++) {
        data.notes[kk] = default_notes[kk];
        data.volumes[kk] = 0;
        data.positions[kk] = 0;
    }
    return data;
}

static NoteData noteData = NoteData::standard();

DWORD CALLBACK WriteStream(HSTREAM, short *buffer, DWORD length, void *) {
    memset(buffer,0,length);

    for (int kk=0; kk<NoteData::nstates; kk++) {
        const int& note = noteData.notes[kk];
        int& volume = noteData.volumes[kk];
        int& position = noteData.positions[kk];

        if (volume == 0) continue;

        const float f = pow(2.0,note/12.0+1)*TABLESIZE*440.0/44100.0;

        for (DWORD c=0; c<length/4; c++) {
            int s = sinetable[static_cast<int>(position*f) % TABLESIZE]*volume/NoteData::volume_max;
            s += static_cast<int>(buffer[c*2]);
            if (s>32767) s=32767;
            else if (s<-32768) s=-32768;

            buffer[c*2+1] = buffer[c*2] = s; // left and right channels are the same

            position++;
            if (volume>0 && volume<NoteData::volume_max) volume--;
        }
    }

    return length;
}

NoteData& Player::getNoteData()
{
    return noteData;
}

void Player::clearNotes() {
    for (int n=0; n<NoteData::nstates; n++) {
        setNote(n,false);
    }
}

void Player::setNote(int key,bool state) {
    if (state) {
        noteData.positions[key] = 0;
        noteData.volumes[key] = NoteData::volume_max;
    } else {
        noteData.volumes[key] = 0;
    }
}

void Player::setSampleVolume(int number, float volume) {
    assert(number>=0);
    assert(number<NSAMPLES);
    samplesVolumes[number] = volume;
}

void Player::playSample(int number) {
    assert(number>=0);
    assert(number<NSAMPLES);
    if (!samplesWorking) return;

    HSAMPLE handle = BASS_SampleGetChannel(samples[number],FALSE);
    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_VOL,samplesVolumes[number]);
    BASS_ChannelSetAttribute(handle,BASS_ATTRIB_PAN,((rand()%201)-100)/100.f);

    BASS_ChannelPlay(handle,false);
}

void Player::fade() {
    for (int n=0; n<NoteData::nstates; n++) {
        int& volume = noteData.volumes[n];
        if (volume == NoteData::volume_max) volume--;
    }
}

void displayInfo()
{
    BASS_INFO info;
    BASS_GetInfo(&info);
    cout << "*******************" << endl;
    cout << "minbuf " << info.minbuf << endl;
    cout << "latency " << info.latency << endl;
    cout << "buf " << BASS_GetConfig(BASS_CONFIG_BUFFER) << endl;
    cout << "hwbuf " << BASS_GetConfig(BASS_CONFIG_DEV_BUFFER) << endl;
    cout << "updateperiod " << BASS_GetConfig(BASS_CONFIG_UPDATEPERIOD) << endl;
    cout << "updatethread " << BASS_GetConfig(BASS_CONFIG_UPDATETHREADS) << endl;
    cout << "*******************" << endl;
}

Player::Player() :
samplesWorking(false) {

    if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,NULL,NULL)) {
        //cerr << "can't init bass" << endl;
        exit(2);
    }

    for (int a=0; a<TABLESIZE; a++) sinetable[a]=(int)(sin(2.0*PI*(double)a/TABLESIZE)*7000.0);

    bool ok = false;
    displayInfo();
    ok = BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,12);
    cout << ok << " updateperiod " << BASS_GetConfig(BASS_CONFIG_UPDATEPERIOD) << endl;
    ok = BASS_SetConfig(BASS_CONFIG_BUFFER,0);
    cout << ok << " buf " << BASS_GetConfig(BASS_CONFIG_BUFFER) << endl;
    ok = BASS_SetConfig(BASS_CONFIG_DEV_BUFFER,0);
    cout << ok << " hwbuf " << BASS_GetConfig(BASS_CONFIG_DEV_BUFFER) << endl;
    ok = BASS_SetConfig(BASS_CONFIG_UPDATETHREADS,2);
    cout << ok << " updatethread " << BASS_GetConfig(BASS_CONFIG_UPDATETHREADS) << endl;
    displayInfo();

    stream = BASS_StreamCreate(44100,2,0,(STREAMPROC*)WriteStream,0);
    ok = BASS_ChannelSetAttribute(stream,BASS_ATTRIB_NOBUFFER,true);
    float aa;
    BASS_ChannelGetAttribute(stream,BASS_ATTRIB_NOBUFFER,&aa);
    cout << ok << " streamnobuffer " << aa << endl;
    BASS_ChannelPlay(stream,TRUE);

    for (int kk=0; kk<NSAMPLES; kk++) {
	samples[kk] = 0;
	samplesVolumes[kk] = 1.f;
    }
    //loadSamplesFromDirectory("andrew");
}

void Player::setSynthVolume(float volume) {
    BASS_ChannelSetAttribute(stream,BASS_ATTRIB_VOL,volume);
}

bool Player::loadSamplesFromDirectory(const std::string &directory) {
    HSAMPLE local_samples[NSAMPLES];

    int number;
    number = 0;
    local_samples[number] = BASS_SampleLoad(false,(directory+"/1.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    if (local_samples[number]==0) {
	return false;
    }

    number = 1;
    local_samples[number] = BASS_SampleLoad(false,(directory+"/2.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    if (local_samples[number]==0) {
	BASS_SampleFree(local_samples[0]);
	return false;
    }

    number = 2;
    local_samples[number] = BASS_SampleLoad(false,(directory+"/3.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    if (local_samples[number]==0) {
	BASS_SampleFree(local_samples[0]);
	BASS_SampleFree(local_samples[1]);
	return false;
    }

    number = 3;
    local_samples[number] = BASS_SampleLoad(false,(directory+"/4.wav").c_str(),0,0,3,BASS_SAMPLE_OVER_POS);
    assert(local_samples[number]);
    if (local_samples[number]==0) {
	BASS_SampleFree(local_samples[0]);
	BASS_SampleFree(local_samples[1]);
	BASS_SampleFree(local_samples[2]);
	return false;
    }

    for (int kk=0; kk<NSAMPLES; kk++) {
	samples[kk] = local_samples[kk];
    }
    samplesWorking = true;
    return true;
}

Player::~Player() {
    BASS_StreamFree(stream);
    BASS_Free();
}
