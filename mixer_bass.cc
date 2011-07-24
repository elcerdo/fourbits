#include "bass.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
using std::endl;
using std::cerr;
using std::cout;

#define PI 3.14159265358979323846
#define TABLESIZE 2048
#define KEYS 17

BASS_INFO info;
HSTREAM stream;

int sinetable[TABLESIZE];

const unsigned int keys[KEYS]={
    'Q','2','W','3','E','R','5','T','6','Y','7','U',
    'I','9','O','0','P'
};

#define MAXVOL  4000    // higher value = longer fadeout
int vol[KEYS]={0},pos[KEYS];

DWORD CALLBACK WriteStream(HSTREAM handle, short *buffer, DWORD length, void *user) {
    int n,s;
    DWORD c;
    float f;
    memset(buffer,0,length);
    for (n=0;n<KEYS;n++) {
	if (!vol[n]) continue;
	f=pow(2.0,(n+3)/12.0)*TABLESIZE*440.0/44100.0;
	for (c=0;c<length/4 && vol[n];c++) {
	    s=sinetable[(int)((pos[n]++)*f)&(TABLESIZE-1)]*vol[n]/MAXVOL;
	    s+=(int)buffer[c*2];
	    if (s>32767) s=32767;
	    else if (s<-32768) s=-32768;
	    buffer[c*2+1]=buffer[c*2]=s; // left and right channels are the same
	    if (vol[n]<MAXVOL) vol[n]--;
	}
    }
    return length;
}

void setNote(int key,bool state) {
    if (state) {
	pos[key]=0;
	vol[key]=MAXVOL; 
    } else {
	vol[key]--;
    }
}


int main(int argc,char * argv[])
{
    cout << "START" << endl;

    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

    if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,NULL,NULL)) {
	cerr << "can't init bass" << endl;
	exit(2);
    }

    for (int a=0; a<TABLESIZE; a++) sinetable[a]=(int)(sin(2.0*PI*(double)a/TABLESIZE)*7000.0);

    BASS_GetInfo(&info);
    BASS_SetConfig(BASS_CONFIG_BUFFER,10+info.minbuf);

    stream = BASS_StreamCreate(44100,2,0,(STREAMPROC*)WriteStream,0);
    BASS_ChannelPlay(stream,TRUE);

    setNote(0,true);
    usleep (1000000);
    setNote(0,false);
    usleep (1000000);
    setNote(0,true);
    usleep(5000000);

    cout << "END" << endl;


    BASS_Free();
}



