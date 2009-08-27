//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCL
#define HH_CLCL
//#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include "CLtypes.hh"
#include "CLversion.hh"


class CLcl
{
	protected:
		static xlong version;
		xlong id;
		const xchar* name;

	public:
		CLcl();
		 virtual ~CLcl();

		xlong getversion();
};

xlong CLcl::version = CLmajor<<24 + CLminor<<16 + CLbuild<<8 + CLextra;

CLcl::CLcl()
{ 
	name = __func__; //needs to be changed to name of derived constructor __func__ !
}

CLcl::~CLcl() { }

xlong CLcl::getversion()
{
	return version;
}

#endif

