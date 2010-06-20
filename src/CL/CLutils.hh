///license
//atCROSSLEVEL studios 2009,2010
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
#include "CLstruct.hh"
///*

///header
/* class name:	CLutils
 * 
 * description:	A rainbow of useful routines
 * 
 * author:		atcl
 * 
 * notes:		enclose in namespace?
 * 
 * version: 	0.2
 */
///*

///definitions
//namespace CLutils
//{
	inline bool isoff(xlong x,xlong y);								//check if point is off screen
	inline bool isoff(xlong x1,xlong y1,xlong x2,xlong y2);			//check if rectangle s off screen
	inline void clip(xlong& x,xlong& y);							//clip point against screen borders
	template<typename T>inline void tty(const T c);					//console output without lineend
	template<typename T>inline void say(const T c);					//console output with lineend
	inline void eol();												//output line end character to console
	inline bool eof(xchar c);										//test if is DOS end of file character
	inline void err(const xchar* c,const xchar* f,const xchar* m);	//console output error "in,at,with"
	inline void bye();												//force program exit
	inline void nfo();												//console output of id string
	inline void ver();												//version output
	inline xlong lt(const xchar* a,const xchar* b);					//compare cstrings
	inline uxlong endian(uxlong l);									//invert endianess
	inline uxlong byteadd(uxlong a,uxlong b);
	inline uxlong bytesub(uxlong a,uxlong b);
	inline uxlong ror(uxlong x);									//rotate right
	inline uxlong rol(uxlong x);									//rotate left
	inline void prefetch(void* hint);								//prefetch memory at hint
//}
///*

///implementation
bool isoff(xlong x,xlong y) { return (x<0 || x>=XRES || y<0 || y>=YRES); } 

bool isoff(xlong x1,xlong y1,xlong x2,xlong y2) { return ( (x1<0&&x2<0) || (x1>=XRES&&x2>=XRES) || (y1<0&&y2<0) || (y1>=YRES&&y2>=YRES) ); } 

void clip(xlong& x,xlong& y) { if(x<0) { x=0; } if(x>=XRES) { x=XRES-1; } if(y<0) { y=0; } if(y>=YRES) { y=YRES-1; } } 

template<typename T>void tty(const T c) { std::cout << std::setprecision(4) << c; } 

template<typename T>void say(const T c) { std::cout << std::setprecision(4) << c << std::endl; } 

void eol() { std::cout << std::endl; }

bool eof(xchar c) { return (c==0x1A); }

void err(const xchar* c,const xchar* f,const xchar* m) { std::cout << "ERROR in " << c << " at " << f << " with: " << m << std::endl; }

void bye() { std::cout << u8"bye" << std::endl; ::exit(0); } 

void nfo() { std::cout << u8"atCROSSLEVEL's XiZero Version " << VERSION << std::endl; }

void ver() { std::cout << u8"Version: " << (CLversion>>24) << "." << ((CLversion<<8)>>24) << "." << ((CLversion<<16)>>24) << "." << ((CLversion<<24)>>24) << std::endl; }

xlong lt(const xchar* a,const xchar* b)
{
	xlong i = 0;
	while(a[i]!=0 && b[i]!=0 && a[i]==b[i]) { i++; }
	return xlong(a[i]-b[i]);
}

uxlong endian(uxlong l)
{
	doubleword tl = { l };
	tl.db[0] ^= tl.db[3] ^= tl.db[0] ^= tl.db[3];
	tl.db[1] ^= tl.db[2] ^= tl.db[1] ^= tl.db[2];
	return tl.dd;
}

uxlong byteadd(uxlong a,uxlong b)
{
	doubleword xa = { a };
	doubleword xb = { b };
	xb.db[0] = xa.db[0] + xb.db[0]; xb.db[0] = ((uxchar(xb.db[0]<xa.db[0]))-1) | xb.db[0];
	xb.db[1] = xa.db[1] + xb.db[1]; xb.db[1] = ((uxchar(xb.db[1]<xa.db[1]))-1) | xb.db[1];
	xb.db[2] = xa.db[2] + xb.db[2]; xb.db[2] = ((uxchar(xb.db[2]<xa.db[2]))-1) | xb.db[2];
	xb.db[3] = xa.db[3] + xb.db[3]; xb.db[3] = ((uxchar(xb.db[3]<xa.db[3]))-1) | xb.db[3];
	return xb.dd;
}

uxlong bytesub(uxlong a,uxlong b)
{
	doubleword xa = { a };
	doubleword xb = { b };
	xb.db[0] = xa.db[0] - xb.db[0]; xb.db[0] = ((uxchar(xb.db[0]>xa.db[0]))-1) & xb.db[0];
	xb.db[1] = xa.db[1] - xb.db[1]; xb.db[1] = ((uxchar(xb.db[1]>xa.db[1]))-1) & xb.db[1];
	xb.db[2] = xa.db[2] - xb.db[2]; xb.db[2] = ((uxchar(xb.db[2]>xa.db[2]))-1) & xb.db[2];
	xb.db[3] = xa.db[3] - xb.db[3]; xb.db[3] = ((uxchar(xb.db[3]>xa.db[3]))-1) & xb.db[3];
	return xb.dd;
}

uxlong ror(uxlong x,xlong i) { return (x>>i)|(x<<(sizeof(uxlong)-i)); }

uxlong rol(uxlong x,xlong i) { return (x<<i)|(x>>(sizeof(uxlong)-i)); }

void prefetch(void* hint) { __asm__ __volatile__ ("prefetch %%0": :"r"(hint) ); }
///*

#endif

