//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMENU
#define HH_CLMENU
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLgfx2.hh"

class CLmenu : public virtual CLcl
{
	private:
		xlong* doublebuffer;
		char* title;
		xlong* icon;
		void* actionptr;
		
	public:
		CLmenu(xlong* db,void* a);
		~CLmenu();
		void draw();
		void onclick();
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
	//drawrectangle(0,0,xres,?,grey);	//win
	//drawrectangle(2,2,xres,?/2,red);	//title
	//drawrectangle(?,?,?,?,grey);		//close
}

void CLmenu::action()
{

}

#endif

