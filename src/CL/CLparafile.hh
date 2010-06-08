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
 * version: 	0.2
 */
///*

///declarations
class CLparafile : CLbase<CLparafile,0>
{
	private:

	protected:
		xchar** name;
		void**  value;
	public:
		CLparafile(CLfile* f);
		~CLparafile();
		void* get(const xchar* name,bool t) const;
};
///*

///implementation
CLparafile::CLparafile(CLfile* f) //! noncritical
: clformat(CLformat::instance())
{
	
}

CLparafile::~CLparafile() //! noncritical
{
	delete[] name;
	delete[] value;
}

void* CLparafile::get(const xchar* name,bool t) const //! noncritical
{
	
}
///*

#endif
