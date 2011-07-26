#!/usr/bin/env bash

set -e
set -u

arch="32b"
cat /proc/cpuinfo | grep flags | grep -q lm && arch="64b"

rm -f mixer_bass && g++ -L. -I. -lbass.${arch} mixer_bass.cc -o mixer_bass
rm -f mixer_bass_ext && g++ -L. -lbass.${arch} -I. player.cc mixer_bass_ext.cc -o mixer_bass_ext
echo "!!!!!!!!!!!!"
