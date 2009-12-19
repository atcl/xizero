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
		void draw() const;
};
///*

///implementation
CLmenu::CLmenu(void (*p)()) //! noncritical
{
	pause = p;
	icon = clformat->loadxpm(CLicon);
	title = new CLlabel(0,0,XRES,20,0x00FFFFFF,0x00FF0000,0x00800000,"atCROSSLEVEL's XiZero",0);
	exit = new CLbutton(781,1,18,18,0,0x00C0C0C0,0,p,"X",1);
	info = new CLbutton(2,21,100,18,0,0x00C0C0C0,0,p,"Info",1);
	about = new CLbutton(102,21,100,18,0,0x00C0C0C0,0,p,"About",1);
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

void CLmenu::draw() const //! noncritical
{
	//pause();
	title->draw();
	clgfx1->drawsprite(2,2,icon);
	exit->draw();
	clgfx1->drawrectangle(0,20,XRES,40,0x00C0C0C0,1);
	info->draw();
	about->draw();
}
///*

#endif

