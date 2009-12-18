///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLUTILS
#define HH_CLUTILS
///*

///sys includes
#include <iostream>
#include <iomanip>
#include <stdlib.h>
///*

///idp includes
#include "CLtypes.hh"
///*

///header
/* class name:	CLutils
 * 
 * description:	A rainbow of useful routines
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions

///*

///implementation
inline bool isoff(xlong x,xlong y) { return (x<0 || x>=XRES || y<0 || y>=YRES); } //check if point is off screen

inline bool isoff(xlong x1,xlong y1,xlong x2,xlong y2) { return ( (x1<0&&x2<0) || (x1>=XRES&&x2>=XRES) || (y1<0&&y2<0) || (y1>=YRES&&y2>=YRES) ); } //check if rectangle s off screen

inline void clip(xlong& x,xlong& y) { if(x<0) x=0; if(x>=XRES) x=XRES-1; if(y<0) y=0; if(y>=YRES) y=YRES-1; } //clip point against screen borders

template<typename T>
void inline tty(const T c) { std::cout << std::setprecision(4) << c; } //console output without lineend

template<typename T>
void inline say(const T c) { std::cout << std::setprecision(4) << c << std::endl; } //console output with lineend

void inline eol() { std::cout << std::endl; }

bool inline eof(xchar c) { return (c==0x1A); }

void inline err(const xchar* f,const xchar* m) { std::cout << f << ": " << m << std::endl; }

void inline bye() { std::cout << u8"bye" << std::endl; ::exit(0); } //force program exit

xlong inline lt(const xchar* a,const xchar* b)
{
	xlong i = 0;
	while(a[i]!=0 && b[i]!=0 && a[i]==b[i]) { i++; }
	return xlong(a[i]-b[i]);
}

uxlong inline endian(xlong l)
{
	doubleword tl = { l };
	tl.db[0] ^= tl.db[3] ^= tl.db[0] ^= tl.db[3];
	tl.db[1] ^= tl.db[2] ^= tl.db[1] ^= tl.db[2];
	return tl.dd;
}

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

