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
	private:
		xchar** con;
		xlong width;
		xlong height;
	public:
		CLconio();
		~CLconio();

		void setup(xlong x,xlong y);
		void addch(xchar c,xlong x,xlong y,uxlong fc,uxlong bc);
		void addst(xchar* s,xlong x,xlong y,uxlong fc,uxlong bc);
		void clear();
		xchar kbhit();
		xchar getch();
		void flush();
};

#endif 
