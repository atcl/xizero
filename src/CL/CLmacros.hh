///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMACROS
#define HH_CLMACROS
///*

///includes
#include <iostream>

#include "CLtypes.hh"
#include "CLconsts.hh"

///*

//todo: write prefetch macro and place at end of loops if useful

///implementation
inline bool isoff(xlong x,xlong y) { return (x<0 || x>=XRES || y<0 || y>=YRES); } //check if point is off screen

inline bool isoff(xlong x1,xlong y1,xlong x2,xlong y2) { return ( (x1<0&&x2<0) || (x1>=XRES&&x2>=XRES) || (y1<0&&y2<0) || (y1>=YRES&&y2>=YRES) ); } //check if rectangle s off screen

inline void clip(xlong& x,xlong& y) { if(x<0) x=0; if(x>=XRES) x=XRES-1; if(y<0) y=0; if(y>=YRES) y=YRES-1; } //clip point against screen borders

xlong inline x20(xlong f)  { return ( (f<<4) + (f<<2) ); } //fast multiplication with 20

xlong inline x40(xlong f)  { return ( (f<<5) + (f<<3) ); } //fast ultiplication with 40

xlong inline x180(xlong f) { return ( (f<<7) + (f<<5) + (f<<4) + (f<<2) ); } //fast multiplication with 180

xlong inline x360(xlong f) { return ( (f<<8) + (f<<6) + (f<<5) + (f<<3) ); } //fast multilication with 360

xlong inline x600(xlong f) { return ( (f<<9) + (f<<6) + (f<<4) + (f<<3) ); } //fast multiplication with 600

xlong inline x800(xlong f) { return ( (f<<9) + (f<<8) + (f<<5) ); } //fast multiplication with 800

template<typename T>
void inline tty(const T c) { std::cout << std::setprecision(4) << c; } //console output without lineend

template<typename T>
void inline say(const T c) { std::cout << std::setprecision(4) << c << std::endl; } //console output with lineend

void inline eol() { std::cout << std::endl; }

void inline err(const xchar* f,const xchar* m) { std::cout << f << m << std::endl; }

void inline bye() { std::cout << "bye" << std::endl; ::exit(0); } //force program exit

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
///*

#endif

