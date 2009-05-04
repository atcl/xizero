//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include "CLtypes.hh"


xlong inline times800(xlong f)
{
	return (f<<9 + f<<8 +f<<5);
}

xlong inline times600(xlong f)
{
	return (f<<9 + f<<6 +f<<4 + f<<3);
}

void inline say(const xchar* c)
{
	CLprint_(c);
}

void inline bye()
{
	CLexit_(0,__func__,"bye");
}



#endif

