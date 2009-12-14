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
		xchar* copy(const xchar* s,xlong l=0) const;
		xlong length(const xchar* s) const;
		xlong compare(const xchar* s,const xchar* t,bool f) const;
		xlong tolong(const xchar* s) const;
		xlong linecount(const xchar* s) const;
		xlong find(const xchar* s,const xchar* f,xlong p=0) const;
		
		xlong linecount(CLfile* s) const { linecount(s->text); };
		xlong find(CLfile* s,const xchar* f,xlong p=0) const { find(s->text,f,p); };
};
///*

///implementation
xchar* CLstring::copy(const xchar* s,xlong l) const
{
	if(l==0) { while (s[l]) { l++; } }
	xchar* r = new xchar[l+1];
	for(uxlong i=0; i<l; i++) { r[i] = s[i]; }
	r[l] = 0;
	return r;
}

xlong CLstring::length(const xchar* s) const
{
	xlong l = 0;
	while (s[l]) { l++; }
	return l;
}

xlong CLstring::compare(const xchar* s,const xchar* t,bool f) const
{
	
}

xlong CLstring::tolong(const xchar* s) const
{
	
}

xlong CLstring::linecount(const xchar* s) const
{
	xlong c = 1;
	xlong l = length(s);
	for(uxlong i=0; i<l; i++) { if( s[i] == '\n' ) c++; }
	return c;
}

xlong CLstring::find(const xchar* s,const xchar* f,xlong p) const
{
	
}
///*

#endif

