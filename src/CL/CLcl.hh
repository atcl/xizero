//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCL
#define HH_CLCL
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <typeinfo>
#include <iostream>

#include "CLtypes.hh"
#include "CLversion.hh"

template<class T>
T* CLinit(T* singleton);

class CLutils;

class CLcl
{
	protected:
		CLutils* utils2;
		static xlong version;
		static xlong id;
		const xchar* name;
	public:
		CLcl();
		 virtual ~CLcl();
		xlong getversion() const;
};

xlong CLcl::version = CLmajor<<24 + CLminor<<16 + CLbuild<<8 + CLextra;

xlong CLcl::id = 0;

CLcl::CLcl()
{
	//~ utils2 = CLinit(utils2);
	//~ type_info t = typeid(T);
	//~ name = t.name();
	id++;
}

CLcl::~CLcl() { }

xlong CLcl::getversion() const { return version; }

#endif

