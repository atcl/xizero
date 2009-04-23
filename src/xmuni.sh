#!/bin/sh

g++ -I/usr/include/freetype2 -I/u/c_himp01/prog/fltk-1.1.9 -o xizero xizero.cc -L/u/c_himp01/prog/fltk-1.1.9/lib -lfltk -lX11 -Wno-multichar -Wno-write-strings

