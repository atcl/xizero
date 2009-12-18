///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMENU
#define HH_CLMENU
///*

///sys includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLresource.hh"
#include "CLstruct.hh"
///*

///api includes
#include "CLformat.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
#include "CLgfx1.hh"
#include "CLsystem.hh"
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
		sprite*   icon;
		CLlabel*  title;
		CLbutton* exit;
		CLbutton* info;
		CLbutton* about;
		void (*pause)();
		void (*e)();
		void (*i)();
		void (*a)();
	public:
		CLmenu(void (*p)());
		~CLmenu();
		void draw();
};
///*

///implementation
CLmenu::CLmenu(void (*p)()) //! noncritical
{
	pause = p;
	icon = clformat->loadxpm(CLicon);
	title = new CLlabel(20,2,756,16,0x00FFFFFF,0x00FF0000,0x00800000,"atCROSSLEVEL's XiZero",1);
	exit = new CLbutton(20,20,80,20,0,0x00C0C0C0,0,p,"Exit",0);
	info = new CLbutton(100,20,80,20,0,0x00C0C0C0,0,p,"Info",0);
	about = new CLbutton(180,20,80,20,0,0x00C0C0C0,0,p,"About",1);
	clwindow->setsyskey('^',pause);
}

CLmenu::~CLmenu() //! noncritical
{ 
	delete icon;
	delete title;
	delete exit;
	delete info;
	delete about;	
}

void CLmenu::draw() //! noncritical
{
	//pause();
	clgfx1->drawrectangle(0,0,XRES,40,0x00C0C0C0,1);
	clgfx1->drawsprite(2,2,icon);
	title->draw();
	exit->draw();
	info->draw();
	about->draw();
}
///*

#endif

