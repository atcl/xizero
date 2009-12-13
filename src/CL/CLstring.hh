///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSTRING
#define HH_CLSTRING
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
///*

///header
/* class name:	CLstring
 * 
 * description:	Handles (c-)strings
 * 
 * author:	atcl
 * 
 * notes:	finish implementing.
 * 
 * version: 0.1
 */
///*

///definitions
class CLstring : public virtual CLcl, public CLsingle<CLstring>
{
	private:
		
	public:
		CLstring() { };
		~CLstring() { };
		xchar* replace(const xchar* s,xlong p,xchar c) const;
		xchar* left(const xchar* s,xlong p) const;
		xchar* right(const xchar* s,xlong p) const;
		xchar* mid(const xchar* s,xlong a,xlong e) const;
		xchar* concat(const xchar* s,const xchar* t) const;
		xlong length(const xchar* s) const;
		xchar* compare(const xchar* s,const xchar* t,bool f) const;
		xchar* unconst(const xchar* s) const;
		xchar charat(const xchar* s,xlong p) const;
		xlong getlinecount(const xchar* s) const;

		//operator =
		//operator +
		//operator []
};
///*

///implementation

///*

#endif

