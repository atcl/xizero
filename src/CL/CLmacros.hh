//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
//#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <iostream>

#include "CLtypes.hh"
#include "CLconsts.hh"


xlong inline x800(xlong f)
{
	return ( (f<<9) + (f<<8) + (f<<5) );
}

xlong inline x600(xlong f)
{
	return ( (f<<9) + (f<<6) + (f<<4) + (f<<3) );
}

void inline tty(const xchar* c="\n")
{
	std::cout << c;
}

void inline say(const xchar* c="hi")
{
	std::cout << c << std::endl;
}

void inline say(xlong l)
{
	std::cout << l << std::endl;
}

void inline say(uxlong l)
{
	std::cout << l << std::endl;
}

void inline say(float f)
{
	std::cout << f << std::endl;
}

void inline debug(const xchar* c,xlong v)
{
	std::cout << c << ": " << v << std::endl;
}

void inline bye()
{
	CLsystem::exit(0,0,__func__,"bye");
}

void inline CLneg(xlong& x)
{
	 __asm__ __volatile__("notl %%eax, inc %%eax;":"=a"(x) :"a"(x):"%eax");
}

void inline CLstosd(xlong* dst,xlong val)
{
	 __asm__ __volatile__("stosd": :"a"(val),"D"(dst):"%eax","%edi");
}

void inline CLlodsd(xlong* src,xlong& var)
{
	__asm__ __volatile__("lodsd":"=a"(var):"S"(src):"%eax","%esi");
}

//move to CLdetect
void detectCPU(bool& mmx,bool& sse)
{
	xlong dd;
	
	__asm__ __volatile__ ("cpuid":"=d"(dd):"a"(1));
	
	if(dd && MMXFLAG) mmx=1;
	if(dd && SSEFLAG) sse=1;
}
//*

#endif

