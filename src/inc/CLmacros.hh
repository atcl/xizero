//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <string.h>

#include "CLtypes.hh"


xlong inline x800(xlong f)
{
	return (f<<9 + f<<8 + f<<5);
}

xlong inline x600(xlong f)
{
	return (f<<9 + f<<6 + f<<4 + f<<3);
}

void inline say(const xchar* c)
{
	
}

void inline bye()
{
	CLexit_(0,__func__,"bye");
}

void inline CLstosd(xlong* dst,xlong val,xlong cnt)
{
	memset(dst,val,cnt);
}

void inline CLmovsd(xlong* dst,void* src,xlong cnt)
{
	memcpy(dst,src,cnt);
}

void inline qq(const xchar m)
{
	CLprint_(c);
}

#endif

