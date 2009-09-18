//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCL
#define HH_CLCL
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <typeinfo>
#include <iostream>

#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLglobals.hh"

/* class name:	CLcl
 * 
 * description:	The base class of all other classes
 * 
 * author:	atcl
 * 
 * notes:	set derived type name in name.
 * 
 * version: 0.1
 */

class CLutils;

class CLcl : public CLglobal
{
	protected:
		static xlong version;
		static xlong id;
		const xchar* name;
	public:
		CLcl();
		 virtual ~CLcl() { };
		xlong getversion() const;
		const xchar* getname() const;
};

xlong CLcl::version = (CLmajor<<24) + (CLminor<<16) + (CLbuild<<8) + CLextra;

xlong CLcl::id = 0;

CLcl::CLcl()
{
	name = typeid(*this).name();
	id++;
}

xlong CLcl::getversion() const { return version; }

const xchar* CLcl::getname() const { return name; }

#endif

