#!/usr/bin/env bash

set -e
set -u

rm -f mixer_bass && g++ -L. -lbass mixer_bass.cc -o mixer_bass
echo "!!!!!!!!!!!!"
