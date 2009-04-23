#!/bin/sh

g++ -I/usr/include/freetype2 -o xizero xizero.cc -lfltk -Wno-multichar -Wno-write-strings -O3
