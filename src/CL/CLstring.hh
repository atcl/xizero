///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSTRING
#define HH_CLSTRING
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
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
 * version: 0.2
 */
///*

///definitions
#define __class__ CLstring
class CLstring : public CLbase<CLstring,1>
{
	friend class CLbase<CLstring,1>;
	
	protected:
		CLstring() { };
		~CLstring() { };
	public:
		xchar* copy(const xchar* s,xlong l=0) const;
		xchar* concat(const xchar* a,const xchar* b) const;
		xlong length(const xchar* s) const;
		xlong compare(const xchar* s,const xchar* t,uxlong n=0) const;
		xlong tolong(const xchar* s) const;
		xchar* toascii(xlong v) const;
		xlong linecount(const xchar* s) const;
		xlong find(const xchar* s,const xchar* f,xlong p=0) const;
		uxlong hex(const xchar* s) const;
		
		xlong linecount(CLfile* s) const { return linecount(s->text); };
		xlong find(CLfile* s,const xchar* f,xlong p=0) const { return find(s->text,f,p); };
};
///*

///implementation
xchar* CLstring::copy(const xchar* s,xlong l) const //! critical
{
	if(l==0) { while (s[l]) { l++; } }
	xchar* r = new xchar[l+1];
	for(xlong i=0; i<l; i++) { r[i] = s[i]; }
	r[l] = 0;
	return r;
}

xchar* CLstring::concat(const xchar* a,const xchar* b) const //!noncritical
{
	xlong la = length(a);
	xlong lb = length(b);
	xchar* r = new xchar[la+lb+1];
	for(xlong i=0; i<la; i++) { r[i] = a[i]; }
	for(xlong i=0; i<lb; i++) { r[la+i] = b[i]; }
	r[la+lb] = 0;
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

xchar* CLstring::toascii(xlong v) const //! noncritical
{
	xchar* r = new xchar[12];
	if(v==0) { r[0]='0'; r[1]=0; return r; }
	r[0] = '-';
	bool s = (v<0);
	if(s) { v*=-1; }
	xlong i = 0;
	xlong t = 0;
	bool u = 0;
	for(xlong j=1000000000; j>0; j/=10)
	{
		t = ((v/j)%10) + '0';
		r[i+s] = t;
		if(t!='0' || u!=0 ) { i++; u=1; }
	}
	r[i+s] = 0;
	return r;
}

xlong CLstring::linecount(const xchar* s) const //! noncritical
{
	xlong c = 1;
	xlong l = length(s);
	for(xlong i=0; i<l; i++) { if( s[i] == '\n' ) c++; }
	return c;
}

xlong CLstring::find(const xchar* s,const xchar* f,xlong p) const //! noncritical
{
	xlong l = length(s);
	xlong m = length(f);
	xlong t = 0;
	xlong r = -1;
	
	if( (p>l) || (m>l) || (l-m<p) ) { return -1; }
	
	for(xlong i=p; i<l; i++)
	{
		t = i;
		for(xlong j=0; j<m; j++,t++) { if(s[t]!=f[j]) break; }
		if(t==i+m) { r = i; }
	}
	
	return r;
}

uxlong CLstring::hex(const xchar* s) const //!  noncritical
{
	xlong i = 0;
	xlong r = 0;
	while(s[i]==' ') { i++; }
	
	for(xlong j=i; j<i+8; j++)
	{
		if(s[j]>='0' && s[j]<='9') { r += (s[j]-'0'); }
		else if(s[j]>='A' && s[j]<='F') { r += (s[j]-'A'+10); }
		else if(s[j]>='a' && s[j]<='f') { r += (s[j]-'a'+10); }
		else { break; }
		r <<= 4;
	}

	return (r>>4);
}
///*

#undef __class__
#endif

