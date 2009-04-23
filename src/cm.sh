#!/bin/sh

g++ -I/usr/include/freetype2 -o unittest unittest.cc -lfltk -Wno-multichar -Wno-write-strings
