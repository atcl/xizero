///license
//atCROSSLEVEL studios 2009,2010
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
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.1
 */
///*

///declarations
class CLparafile : CLbase<CLparafile,0>
{
	private:
		CLformat& clformat;
	protected:
		xchar** name;
		void**  value;
	public:
		CLparafile();
		~CLparafile();
		void* get(const xchar name);
};
///*

///implementation
CLparafile::CLparafile() //! noncritical
: clformat(CLformat::instance())
{
	
}

CLparafile::~CLparafile() //! noncritical
{
	delete[] name;
	delete[] value;
}

void* CLparafile::get(const xchar name) //! noncritical
{
	
}
///*

#endif
