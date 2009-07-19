//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCONIO
#define HH_CLCONIO
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLgfx2.hh"


class CLconio : public virtual CLcl
{
	protected:
		CLbuffer<xlong>* doublebuffer;
		CLbuffer<xchar>* con;
		
	private:
		xlong width;
		xlong height;
		xlong size;
		xlong current;
		
	public:
		CLconio(xlong x,xlong y);
		~CLconio();

		void addch(xchar c,uxlong fc,uxlong bc);
		void addst(xchar* s,uxlong fc,uxlong bc);
		void locate(xlong x,xlong y);
		void clear();
		xchar kbhit();
		xchar getch();
		void flush();
};

CLconio::CLconio(xlong x,xlong y)
{
	width = x;
	height = y;
	size = x*y;

	con = new CLbuffer(size);
}

CLconio::~CLconio() { }

void CLconio::addch(xchar c,xlong x,xlong y,uxlong fc,uxlong bc)
{

}

void CLconio::addst(xchar* s,xlong x,xlong y,uxlong fc,uxlong bc)
{

}

void CLconio::locate(xlong x,xlong y)
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
