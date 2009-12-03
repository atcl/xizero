///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMENU
#define HH_CLMENU
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLgfx2.hh"
///*

///header
/* class name:	CLmenu
 * 
 * description:	Ingame menu bar
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definition
class CLmenu : public virtual CLcl
{
	private:
		char* title;
		xlong* icon;
		void* actionptr;
		CLbutton* exit;
		CLbutton* info;
		CLbutton* about;
		
	public:
		CLmenu();
		~CLmenu();
		void draw();
		void action();
		static void invoke();
};
///*

///implementation
CLmenu::CLmenu() //! noncritical
{

}

CLmenu::~CLmenu() { } //! noncritical

void CLmenu::draw() //! noncritical
{
	//drawrectangle(0,0,xres,?,grey);	//win
	//drawrectangle(2,2,xres,?/2,red);	//title
	//drawrectangle(?,?,?,?,grey);		//close
}

void CLmenu::action() //! critical
{

}
///*

#endif

