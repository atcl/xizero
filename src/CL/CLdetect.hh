//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDETECT
#define HH_CLDETECT
//#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"

//cpucaps constants
#define FPU   0b00000000000000000000000000000001
#define MMX   0b00000000000000000000000000000010
#define SSE   0b00000000000000000000000000000100
#define SSE2  0b00000000000000000000000000000000
#define SSE3  0b00000000000000000000000000000000
#define SSE41 0b00000000000000000000000000000000
#define SSE42 0b00000000000000000000000000000000
#define SSE5a 0b00000000000000000000000000000000

//*

namespace CLdetect
{
	xlong cpu();
	xlong ram();
	xchar* gpu();
	xlong vram();
	xchar* sb();
	xchar ps();
};

xlong CLdetect::cpu()
{
	xlong dd;
	
	__asm__ __volatile__ ("cpuid":"=d"(dd):"a"(1));
	
	xlong r = 0;
	
	if(dd && MMXFLAG) r += MMX;
	if(dd && SSEFLAG) r += SSE;
	
	return r;
}

#ifdef WIN32

xlong CLdetect::ram()
{
	
}

xchar* CLdetect::gpu()
{
	
}

xlong CLdetect::vram()
{
	
}

xchar* CLdetect::sb()
{
	
}

xlong CLdetect::ps()
{
	
}

#else //ifdef LINUX

xlong CLdetect::ram()
{
	
}

xchar* CLdetect::gpu()
{
	
}

xlong CLdetect::vram()
{
	
}

xchar* CLdetect::sb()
{
	
}

xlong CLdetect::ps()
{
	
}

#endif 


#endif
