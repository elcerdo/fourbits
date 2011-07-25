#!/usr/bin/env bash

set -e
set -u

rm -f mixer_bass && g++ -L. -I. -lbass mixer_bass.cc -o mixer_bass
rm -f mixer_bass_ext && g++ -L. -I. -lbass player.cc mixer_bass_ext.cc -o mixer_bass_ext
echo "!!!!!!!!!!!!"
