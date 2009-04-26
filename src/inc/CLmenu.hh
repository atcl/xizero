//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMENU
#define HH_CLMENU
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLgfx2.hh"


class CLmenu : public virtual CLcl
{
	private:
		xlong* doublebuffer;
		char* title;
		xlong* icon;
		void* actionptr;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,uxlong color2=0); //also for 3d frames
		
	public:
		CLmenu(xlong* db,void* a);
		~CLmenu();
		void draw();
		void action();
};

CLmenu::CLmenu(xlong* db,void* a)
{
	doublebuffer = db;
	actionptr = a;
}

CLmenu::~CLmenu() { }

void CLmenu::draw()
{

}

void CLmenu::action()
{

}

#endif

