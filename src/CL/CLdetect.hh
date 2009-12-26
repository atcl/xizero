///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLDETECT
#define HH_CLDETECT
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLstring.hh"
#include "CLsingle.hh"
///*

///os-dep
#ifdef WIN32

#else //ifdef LINUX
	
#endif
///*

///header
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
///*

///declarations
//CPUID flags
#define FPUFLAG   0b00000000000000000000000000000001
#define MMXFLAG   0b00000000100000000000000000000000
#define SSEFLAG   0b00000010000000000000000000000000
#define SSE2FLAG  0b00000100000000000000000000000000
#define SSE3FLAG  0b00000000000000000000001000000000
#define SSE41FLAG 0b00000000000010000000000000000000
#define SSE42FLAG 0b00000000000100000000000000000000
#define SSE4aFLAG 0b00000000000000000000000001000000
#define SSE5aFLAG 0b00000000000000000000100000000000
#define HTTFLAG   0b00001000000000000000000000000000
#define PREFFLAG  0b00000000000000000000000010000000
//*
///*

///definitions
class CLdetect : public virtual CLcl, public CLsingle<CLdetect>
{
	friend class CLsingle<CLdetect>;
	
	private:
		xlong pcores;
		xlong l2c;
		bool havemmx;
		bool havesse;
		CLdetect();
		~CLdetect() { };
	public:
		xlong cores() const { return pcores; };
		xlong ram() const;
		xlong vram() const;
		xlong ps() const;
		xlong l2() const;
		
		bool mmx() const { return havemmx; };
		bool sse() const { return havesse; };
};
///*

///implementation
CLdetect::CLdetect() //! noncritical
{
	//cpu
	xlong c1 = 0;
	xlong c2 = 0;
	xlong c3 = 0;
	xlong c4 = 0;
	xlong c5 = 0;
	
	//issue cpuid
	__asm__ __volatile__ ("cpuid; nop;":"=d"(c2):"a"(0x00000001));
	__asm__ __volatile__ ("cpuid":"=c"(c3):"a"(0x80000001));
	__asm__ __volatile__ ("cpuid":"=a"(c4):"a"(0x00000004));
	__asm__ __volatile__ ("cpuid":"=c"(c5):"a"(0x80000008));
	//*
	
	//check ram //fix!
	CLfile* mem = clsystem->getfile("/proc/meminfo");
	xlong p = clstring->find(mem->text,"MemTotal:");
	xlong m = clstring->tolong(&(mem->text[p+9]));
	//*
	
	//process cuid results here so vars can find way back
	if( (c2 & MMXFLAG)!=0 ) { havemmx = 1; } else { havemmx = 0; }
	if( (c2 & SSEFLAG)!=0 ) { havesse = 1; } else { havesse = 0; }
	pcores = (c4>>25); // * (c5 & 255); //fix!
	//*
}

xlong CLdetect::ram() const { } //! noncritical

xlong CLdetect::vram() const { } //! noncritical

xlong CLdetect::ps() const { } //! noncritical
///*

#endif
