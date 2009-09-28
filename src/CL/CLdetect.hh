//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDETECT
#define HH_CLDETECT
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLsingle.hh"

#ifdef WIN32

#else //ifdef LINUX
	
#endif

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
#define FPUFLAG   0b00000000000000000000000000000001
#define MMXFLAG   0b00000000100000000000000000000000
#define SSEFLAG   0b00000010000000000000000000000000
#define SSE2FLAG  0b00000100000000000000000000000000
#define SSE3FLAG  0b00000000000000000000001000000001
#define SSE41FLAG 0b00000000000010000000000000000000
#define SSE42FLAG 0b00000000000100000000000000000000
#define SSE4aFLAG 0b00000000000000000000000001000000
#define SSE5aFLAG 0b00000000000000000000100000000000
#define HTTFLAG   0b00001000000000000000000000000000
#define PREFFLAG  0b00000000000000000000000010000000
//*

//move all detection in ctor and all methods just get's and is's
class CLdetect : public virtual CLcl, public CLsingle<CLdetect>
{
	friend class CLsingle<CLdetect>;
	
	private:
		xlong pcpu;
		xlong pcores;
		xlong l2;
		CLdetect();
		~CLdetect() { };
	public:
		xlong cpu() const;
		xlong cores() const;
		xlong ram() const;
		xchar* gpu() const;
		xlong vram() const;
		xchar* sb() const;
		xlong ps() const;
};

CLdetect::CLdetect()
{
	//cpu
	xlong aa = 0;
	xlong cc = 0;
	xlong dd = 0;
	pcpu = 0;
	//~ __asm__ __volatile__ ("cpuid":"=c"(cc):"a"(1));
	//~ if(cc && SSE3FLAG) pcpu += SSE3;
	//~ if(cc && SSE41FLAG) pcpu += SSE41;
	//~ if(cc && SSE42FLAG) pcpu += SSE42;
	//~ __asm__ __volatile__ ("cpuid":"=d"(dd):"a"(1));
	//~ if(dd && FPUFLAG) pcpu += FPU;
	//~ if(dd && MMXFLAG) pcpu += MMX;
	//~ if(dd && SSEFLAG) pcpu += SSE;
	//~ if(dd && SSE2FLAG) pcpu += SSE2;
	//~ if(dd && HTTFLAG) pcpu += HTT;
	
	//~ cc=0;
	//~ __asm__ __volatile__ ("cpuid":"=c"(cc):"a"(0x80000001));
	//~ if(cc && SSE4aFLAG) pcpu += SSE4a;
	//~ if(cc && SSE5aFLAG) pcpu += SSE5a;
	//~ if(cc && PREFFLAG) pcpu += PREF;
//~ 
	//~ //cores
	//~ aa=0;
	//~ __asm__ __volatile__ ("cpuid":"=a"(aa):"a"(0x00000004));
	//~ cc=0;
	//~ __asm__ __volatile__ ("cpuid":"=c"(cc):"a"(0x80000008));
	//~ pcores = (aa>>25) * (cc & 255);

	//...	
}

xlong CLdetect::cpu() const { return pcpu; }

xlong CLdetect::cores() const { return pcores; }

#ifdef WIN32

xlong CLdetect::ram() const 
{
	
}

xchar* CLdetect::gpu() const 
{
	
}

xlong CLdetect::vram() const 
{
	
}

xchar* CLdetect::sb() const 
{
	
}

xlong CLdetect::ps() const 
{
	
}

#else //ifdef LINUX

xlong CLdetect::ram() const
{
	CLfile* mem = clsystem->getfile("/proc/meminfo");
	
	//seek line beginning with "MemTotal:"
	//seek end of line " kB"
	//number before is the total physical memory
}

xchar* CLdetect::gpu() const 
{
	
}

xlong CLdetect::vram() const 
{
	
}

xchar* CLdetect::sb() const 
{
	
}

xlong CLdetect::ps() const
{
	
}

#endif 


#endif
