//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLgfx2.hh"


class CLmsgbox : public virtual CLcl
{
	private:
		xlong* doublebuffer;
		void*  action;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,uxlong color2=0); //also for 3d frames around buttons
		
	public:
		CLmsgbox(xlong* db,void* a);
		~CLmsgbox();
		void draw();
		void onok();
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

void CLmsgbox::onok()
{
	//action();
}

#endif

