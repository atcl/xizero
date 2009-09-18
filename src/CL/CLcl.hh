//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCL
#define HH_CLCL
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <typeinfo>

#include "CLtypes.hh"
#include "CLversion.hh"

//~ template<class T>
class CLcl
{
	protected:
		static xlong version;
		static xlong id;
		const xchar* name;
	public:
		CLcl();
		 virtual ~CLcl();
		xlong getversion() const;
};

//~ template<class T>
xlong CLcl::version = CLmajor<<24 + CLminor<<16 + CLbuild<<8 + CLextra;

//~ template<class T>
xlong CLcl::id = 0;

//~ template<class T>
CLcl::CLcl()
{
	//~ type_info t = typeid(T);
	//~ name = t.name();
	id++;
}

//~ template<class T>
CLcl::~CLcl() { }

//~ template<class T>
xlong CLcl::getversion() const { return version; }

#endif

