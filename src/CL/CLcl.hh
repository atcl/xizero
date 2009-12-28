///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLCL
#define HH_CLCL
///*

///includes
#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLglobals.hh"
///*

///header
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
///*

///defintions
class CLcl : public CLglobal
{
	protected:
		static xlong version;
		static xlong id;
	public:
		CLcl();
		 virtual ~CLcl() { };
		xlong getversion() const;
};

xlong CLcl::version = 0;

xlong CLcl::id = 0;
///*

///implementations
CLcl::CLcl() { id++; } //! noncritical

xlong CLcl::getversion() const { return CLversion; } //! noncritical
///*

#endif

