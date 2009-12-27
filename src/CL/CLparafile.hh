///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLPARAFILE
#define HH_CLPARAFILE
///*

///includes
#include "CLtypes.hh"
#include "CLxml.hh"
///*

///header
/* class name:	CLparafile
 * 
 * description:	This class manages xml parameter files.
 * 
 * author:	atcl
 * 
 * notes:
 * 
 * version: 0.1
 */
///*

///declarations
class CLparafile : public virtual CLcl
{
	private:
	
	public:
		CLparafile();
		~CLparafile();
		get(const xchar name,xlong value);
};
///*

///implementation
CLparafile::CLparafile() //! noncritical
{
	
}

CLparafile::~CLparafile() //! noncritical
{
	
}

CLparafile::get(const xchar name,xlong value) //! noncritical
{
	
}
///*

#endif
