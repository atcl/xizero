//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSTRING
#define HH_CLSTRING
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


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

CLstring::CLstring()
{
	s = new xchar;
	*s = 0;
	l = 0;
}

CLstring::CLstring(const xchar* c)
{
	
}

CLstring::~CLstring()
{

}

void CLstring::replace(xlong p,xchar c)
{
	if(p<l)
	{
		s[p] = c;
	}
}

void CLstring::left(xlong p)
{
	if(p<l && p>0)
	{
		
	}
}

void CLstring::right(xlong p)
{
	if(p<l && p>0)
	{
		
	}
}

void CLstring::mid(xlong a,xlong z)
{
	if(z<l && a>=0)
	{
		
	}
}

void CLstring::concat(CLstring c)
{

}

void CLstring::concat(const xchar* c)
{

}

xlong CLstring::length()
{
	return l;
}

void CLstring::compare(CLstring c)
{
	
}

void CLstring::compare(const xchar* c)
{

}

#endif

