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
		xlong compare(const xchar* s,const xchar* t,uxlong n=0) const;
		xlong tolong(const xchar* s) const;
		xlong linecount(const xchar* s) const;
		xlong find(const xchar* s,const xchar* f,xlong p=0) const;
		
		xlong linecount(CLfile* s) const { linecount(s->text); };
		xlong find(CLfile* s,const xchar* f,xlong p=0) const { find(s->text,f,p); };
};
///*

///implementation
xchar* CLstring::copy(const xchar* s,xlong l) const //! critical
{
	if(l==0) { while (s[l]) { l++; } }
	xchar* r = new xchar[l+1];
	for(uxlong i=0; i<l; i++) { r[i] = s[i]; }
	r[l] = 0;
	return r;
}

xlong CLstring::length(const xchar* s) const //! critical
{
	xlong l = 0;
	while (s[l]) { l++; }
	return l;
}

xlong CLstring::compare(const xchar* s,const xchar* t,uxlong n) const //! noncritical
{
	uxlong sl = length(s);
	uxlong tl = length(t);
	uxlong i = 0;
	while(i<sl && i<tl && (i<n ^ n==0) ) { if(s[i]!=t[i]) { i=0; break; } i++; }
	return i;
}

xlong CLstring::tolong(const xchar* s) const //! critical
{
	xlong i = 0;
	xlong j = 0;
	xlong r = 0;
	xlong t = 1;
	xlong u = 1;
	
	while(s[i]==' ') { i++; }
	if(s[i]=='-') { u = -1; i++; }
	j = i;
	while(s[j]>='0' && s[j]<='9') { j++; }
	j -= i;
	for(; j>0; j--,i++)
	{
		for(xlong k=1; k<j; k++) t *= 10;
		r += (s[i]-'0') * t;
		t = 1;
	}
	r *= u;
	return r;
}

xlong CLstring::linecount(const xchar* s) const //! noncritical
{
	xlong c = 1;
	xlong l = length(s);
	for(uxlong i=0; i<l; i++) { if( s[i] == '\n' ) c++; }
	return c;
}

xlong CLstring::find(const xchar* s,const xchar* f,xlong p) const //! noncritical
{
	
}
///*

#endif

