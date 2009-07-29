//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
//#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLgfx2.hh"


class CLmsgbox : public virtual CLcl
{
	private:
		xlong* doublebuffer;
		void*  action;
		
	public:
		CLmsgbox(xlong* db,void* a);
		~CLmsgbox();
		void draw();
		void onclick();
};

CLmsgbox::CLmsgbox(xlong* db,void* a)
{
	doublebuffer = db;
	action = a;
}

CLmsgbox::~CLmsgbox() { }

void CLmsgbox::draw()
{

}

void CLmsgbox::onclick()
{
	//action();
}

#endif

