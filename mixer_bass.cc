#include "bass.h"
#include <iostream>
#include <cstdlib>
using std::endl;
using std::cerr;
using std::cout;

int main(int argc,char * argv[])
{
    if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,NULL,NULL)) {
	cerr << "can't init bass" << endl;
	exit(2);
    }

    BASS_Free();
}



