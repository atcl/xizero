//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCONIO
#define HH_CLCONIO
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"


class CLconio : public virtual CLcl
{
	protected:
		CLbuffer<xlong>* doublebuffer;
		CLbuffer<xchar>* con;
	private:
		xlong width;
		xlong height;
		xlong size;
	public:
		CLconio(xlong s);
		~CLconio();

		void setup(xlong x,xlong y);
		void addch(xchar c,xlong x,xlong y,uxlong fc,uxlong bc);
		void addst(xchar* s,xlong x,xlong y,uxlong fc,uxlong bc);
		void clear();
		xchar kbhit();
		xchar getch();
		void flush();
};

CLconio::CLconio(xlong s)
{
	width = x;
	height = y;
	size = x*y;

	con = new CLbuffer(size);
}

CLconio::~CLconio() { }

void CLconio::setup(xlong x,xlong y)
{
	width = x;
	height = y;
	size = x*y;
	
	con->reassign(size);
}

void CLconio::addch(xchar c,xlong x,xlong y,uxlong fc,uxlong bc)
{

}

void CLconio::addst(xchar* s,xlong x,xlong y,uxlong fc,uxlong bc)
{

}

void CLconio::clear()
{

}

xchar CLconio::kbhit()
{

}

xchar CLconio::getch()
{

}

void CLconio::flush()
{

}

#endif 
