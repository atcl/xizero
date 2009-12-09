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
class CLstring : public virtual CLcl
{
	private:
		xchar* s;
		xlong l;
		
	public:
		CLstring();
		CLstring(const xchar* c);
		~CLstring();
		void replace(xlong p,xchar c);
		void left(xlong p);
		void right(xlong p);
		void mid(xlong a,xlong e);
		void concat(CLstring c);
		void concat(const xchar* c);
		xlong length();
		void compare(CLstring c);
		void compare(const xchar* c);

		//operator =
		//operator +
		//operator []
};
///*

///implementation
CLstring::CLstring() //! noncritical
{
	s = new xchar;
	*s = 0;
	l = 0;
}

CLstring::CLstring(const xchar* c) //! noncritical
{
	
}

CLstring::~CLstring() //! noncritical
{

}

void CLstring::replace(xlong p,xchar c) //! critical
{
	if(p<l)
	{
		s[p] = c;
	}
}

void CLstring::left(xlong p) //! critical
{
	if(p<l && p>0)
	{
		
	}
}

void CLstring::right(xlong p) //! critical
{
	if(p<l && p>0)
	{
		
	}
}

void CLstring::mid(xlong a,xlong z) //! critical
{
	if(z<l && a>=0)
	{
		
	}
}

void CLstring::concat(CLstring c) //! critical
{

}

void CLstring::concat(const xchar* c)
{

}

xlong CLstring::length() //! critical
{
	return l;
}

void CLstring::compare(CLstring c) //! critical
{
	
}

void CLstring::compare(const xchar* c) //! critical
{

}
///*

#endif

