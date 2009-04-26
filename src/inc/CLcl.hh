//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCL
#define HH_CLCL
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include "CLtypes.hh"


class CLcl
{
	private:
		static xlong version;

	public:
		CLcl();
		 virtual ~CLcl();

		xlong getversion();
};

xlong CLcl::version = 0x00010000;

CLcl::CLcl() { }

CLcl::~CLcl() { } //virtual

xlong CLcl::getversion()
{
	return version;
}

#endif

