///license
//atCROSSLEVEL studios 2009,2010
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
#include "CLbase.hh"
///*

///header
/* class name:	CLdetect
 * 
 * description:	This class determines the system hardware capabilities.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
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
class CLdetect : public CLbase<CLdetect,1>
{
	friend class CLbase<CLdetect,1>;
	friend class CLglobal;
	
	private:
		CLsystem& clsystem;
		CLstring& clstring;
	protected:
		uxlong tram;
		uxlong vram;
		uxlong pcores;
		uxlong l2c;
		xlong psv;
		bool havemmx;
		bool havesse;
		CLdetect();
		~CLdetect() { };
	public:
		uxlong cores() const     { return pcores; }
		uxlong totalram() const  { return tram; }
		uxlong freeram();
		uxlong videoram() const  { return vram; }
		uxlong l2() const { return l2c; }
		xlong ps() const  { return psv; }
		bool mmx() const  { return havemmx; };
		bool sse() const  { return havesse; };
};
///*

///implementation
CLdetect::CLdetect() //! noncritical
: clsystem(CLsystem::instance()), clstring(CLstring::instance())
{
	//issue cpuid
	//~ xlong c1 = 0;
	//~ xlong c2 = 0;
	//~ xlong c3 = 0;
	//~ xlong c4 = 0;
	//~ xlong c5 = 0;
	//~ __asm__ __volatile__ ("cpuid; nop;":"=d"(c2):"a"(0x00000001));
	//~ __asm__ __volatile__ ("cpuid":"=c"(c3):"a"(0x80000001));
	//~ __asm__ __volatile__ ("cpuid":"=a"(c4):"a"(0x00000004));
	//~ __asm__ __volatile__ ("cpuid":"=c"(c5):"a"(0x80000008));
	//*
	
	//check ram
	xlong p = 0;
	CLfile* mem = clsystem.getfile("/proc/meminfo");
	p = clstring.find(mem->text,"MemTotal:");
	tram = clstring.tolong(&(mem->text[p+9])) / 1024;
	//*
	
	//process cuid results here so vars can find way back
	//if( (c2 & MMXFLAG)!=0 ) { havemmx = 1; } else { havemmx = 0; }
	//if( (c2 & SSEFLAG)!=0 ) { havesse = 1; } else { havesse = 0; }
	//pcores = (c4>>25) * (c5 & 255); //fix! and include ht
	//l2c = 0; //fix!
	
	havemmx = 0;
	havesse = 0;
	pcores = 1;
	//*
	
	//detect vram
	
	//*
	
	//detect pixel shader
	
	//*
}

uxlong CLdetect::freeram()
{
	CLfile* mem = clsystem.getfile("/proc/meminfo");
	xlong p = clstring.find(mem->text,"MemFree:");
	return clstring.tolong(&(mem->text[p+8])) / 1024;
}
///*

#endif
