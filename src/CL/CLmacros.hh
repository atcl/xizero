//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <iostream>

//todo: write prefetch macro and place at end of loops if useful

#include "CLtypes.hh"
#include "CLconsts.hh"

xlong inline x20(xlong f)  { return ( (f<<4) + (f<<2) ); } //fast multiplication with 20

xlong inline x40(xlong f)  { return ( (f<<5) + (f<<3) ); } //fast ultiplication with 40

xlong inline x180(xlong f) { return ( (f<<7) + (f<<5) + (f<<4) + (f<<2) ); } //fast multiplication with 180

xlong inline x360(xlong f) { return ( (f<<8) + (f<<6) + (f<<5) + (f<<3) ); } //fast multilication with 360

xlong inline x600(xlong f) { return ( (f<<9) + (f<<6) + (f<<4) + (f<<3) ); } //fast multiplication with 600

xlong inline x800(xlong f) { return ( (f<<9) + (f<<8) + (f<<5) ); } //fast multiplication with 800

void inline tty(const xchar* c="\n") { std::cout << c; } //console output without lineend

void inline say(const xchar* c="hi") { std::cout << c << std::endl; } //console output with lineend (cstring)

void inline say(xlong l) { std::cout << l << std::endl; } //console output with lineend (long)

void inline say(uxlong l) {	std::cout << l << std::endl; } //console output with lineend (ulong)

void inline say(float f) { std::cout << f << std::endl; } //console output with lineend (float)

void inline bye() { CLsystem::instance()->exit(0,0,__func__,"bye"); } //force program exit

xlong inline linearize(xlong x,xlong y,xlong w) { return ((y*w)+x); }

void inline CLprefetch(void* hint) { __asm__ __volatile__ ("prefetch %%0": :"r"(hint) ); } //use prefetcht1???

#endif

