//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <iostream>

//todo: write prefetch macro and place at end of loops if useful

#include "CLtypes.hh"
#include "CLconsts.hh"

inline bool isoff(xlong x,xlong y) { return (x<0 || x>=XRES || y<0 || y>=YRES); } //check if point is off screen

inline bool isoff(xlong x1,xlong y1,xlong x2,xlong y2) { return ( (x1<0&&x2<0) || (x1>=XRES&&x2>=XRES) || (y1<0&&y2<0) || (y1>=YRES&&y2>=YRES) ); } //check if rectangle s off screen

inline void clip(xlong& x,xlong& y) { if(x<0) x=0; if(x>=XRES) x=XRES-1; if(y<0) y=0; if(y>=YRES) y=YRES-1; } //clip point against screen borders

xlong inline x20(xlong f)  { return ( (f<<4) + (f<<2) ); } //fast multiplication with 20

xlong inline x40(xlong f)  { return ( (f<<5) + (f<<3) ); } //fast ultiplication with 40

xlong inline x180(xlong f) { return ( (f<<7) + (f<<5) + (f<<4) + (f<<2) ); } //fast multiplication with 180

xlong inline x360(xlong f) { return ( (f<<8) + (f<<6) + (f<<5) + (f<<3) ); } //fast multilication with 360

xlong inline x600(xlong f) { return ( (f<<9) + (f<<6) + (f<<4) + (f<<3) ); } //fast multiplication with 600

xlong inline x800(xlong f) { return ( (f<<9) + (f<<8) + (f<<5) ); } //fast multiplication with 800

void inline tty(const xchar* c="\n") { std::cout << c; } //console output without lineend

void inline tty(xlong l) { std::cout << l; } //console output without lineend (long)

void inline tty(uxlong l) { std::cout << l; } //console output without lineend (ulong)

void inline say(const xchar* c="hi") { std::cout << c << std::endl; } //console output with lineend (cstring)

void inline say(xlong l) { std::cout << l << std::endl; } //console output with lineend (long)

void inline say(uxlong l) {	std::cout << l << std::endl; } //console output with lineend (ulong)

void inline say(float f) { std::cout << f << std::endl; } //console output with lineend (float)

void inline bye() { CLsystem::instance()->exit(0,0,__func__,"bye"); } //force program exit

xlong inline linear(xlong x,xlong y,xlong w) { return ((y*w)+x); }

uxlong inline byteadd(uxlong a,uxlong b)
{
	doubleword xa = { a };
	doubleword xb = { b };
	xb.db[0] = xa.db[0] + xb.db[0]; xb.db[0] = (uxchar(xb.db[0]<xa.db[0]))-1 | xb.db[0];
	xb.db[1] = xa.db[1] + xb.db[1]; xb.db[1] = (uxchar(xb.db[1]<xa.db[1]))-1 | xb.db[1];
	xb.db[2] = xa.db[2] + xb.db[2]; xb.db[2] = (uxchar(xb.db[2]<xa.db[2]))-1 | xb.db[2];
	xb.db[3] = xa.db[3] + xb.db[3]; xb.db[3] = (uxchar(xb.db[3]<xa.db[3]))-1 | xb.db[3];
	return xb.dd;
}

uxlong inline bytesub(uxlong a,uxlong b)
{
	doubleword xa = { a };
	doubleword xb = { b };
	xb.db[0] = xa.db[0] - xb.db[0]; xb.db[0] = (uxchar(xb.db[0]>xa.db[0]))-1 & xb.db[0];
	xb.db[1] = xa.db[1] - xb.db[1]; xb.db[1] = (uxchar(xb.db[1]>xa.db[1]))-1 & xb.db[1];
	xb.db[2] = xa.db[2] - xb.db[2]; xb.db[2] = (uxchar(xb.db[2]>xa.db[2]))-1 & xb.db[2];
	xb.db[3] = xa.db[3] - xb.db[3]; xb.db[3] = (uxchar(xb.db[3]>xa.db[3]))-1 & xb.db[3];
	return xb.dd;
}

uxlong inline bytemul(uxlong a,float s)
{
	doubleword xa = { a };
	
	return xa.dd;
}

void inline CLprefetch(void* hint) { __asm__ __volatile__ ("prefetch %%0": :"r"(hint) ); } //use prefetcht1???

#endif

