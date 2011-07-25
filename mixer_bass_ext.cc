#include "player.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

int main(int argc,char * argv[])
{
    Player player;

    player.setNote(0,true);
    player.setNote(2,true);
    usleep (1000000);
    player.playSample(0);
    player.setNote(0,false);
    usleep (1000000);
    player.playSample(1);
    player.setNote(2,false);
    player.setNote(0,true);
    usleep(5000000);

    return 0;
}



