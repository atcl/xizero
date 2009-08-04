//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDETECT
#define HH_CLDETECT
//#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"


namespace CLdetect
{
	void detectCPU(bool& mmx,bool& sse);
};

void detectCPU(bool& mmx,bool& sse)
{
	xlong dd;
	
	__asm__ __volatile__ ("cpuid":"=d"(dd):"a"(1));
	
	if(dd && MMXFLAG) mmx=1;
	if(dd && SSEFLAG) sse=1;
}


#endif
