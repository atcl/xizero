//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDETECT
#define HH_CLDETECT
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLsingle.hh"

/* class name:	CLdetect
 * 
 * description:	This class determines the system hardware capabilities.
 * 
 * author:	atcl
 * 
 * notes:	finish implementing.
 * 
 * version: 0.1
 */

//CPUID flags
#define MMXFLAG 0b00000000100000000000000000000000
#define SSEFLAG 0b00000010000000000000000000000000
//*

//move all detection in ctor and all methods just get's and is's
class CLdetect : public virtual CLcl, public CLsingle<CLdetect>
{
	friend class CLsingle<CLdetect>;
	
	private:
		xlong cores;
		xlong l2;
		CLdetect() { };
		~CLdetect() { };
	public:
		xlong cpu();
		xlong ram();
		xchar* gpu();
		xlong vram();
		xchar* sb();
		xlong ps();
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
	CLfile* mem = clsystem->getfile("/proc/meminfo");
	
	//seek line beginning with "MemTotal:"
	//seek end of line " kB"
	//number before is the total physical memory
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
