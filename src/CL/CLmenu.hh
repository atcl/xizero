///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMENU
#define HH_CLMENU
///*

///sys includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLresource.hh"
#include "CLstruct.hh"
#include "CLutils.hh"
///*

///api includes
#include "CLbase.hh"
#include "CLformat.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
#include "CLgfx.hh"
#include "CLwindow.hh"
///*

///header
/* class name:	CLmenu
 * 
 * description:	Ingame menu bar
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definition
class CLmenu : public CLbase<CLmenu,1>
{
	friend class CLbase<CLmenu,1>;
	friend class CLglobal;
	
	private:
		CLformat& clformat;
		CLwindow& clwindow;
		CLgfx&    clgfx;
	protected:
		sprite*   icon;
		CLlabel*  title;
		CLbutton* exit;
		CLbutton* info;
		CLbutton* about;
		CLmenu();
		~CLmenu();
		static void exiting(void* c,void* d) { bye(); }
		static void infoing(void* c,void* d) { nfo(); }
		static void versing(void* c,void* d) { ver(); }
	public:
		void show();
		static void wrapper();
};
///*

///implementation
CLmenu::CLmenu() //! noncritical
: clformat(CLformat::instance()), clwindow(CLwindow::instance()), clgfx(CLgfx::instance())
{
	icon = clformat.loadxpm(CLicon);
	title = new CLlabel(0,0,XRES,20,0x00FFFFFF,0x00FF0000,0x00800000,"atCROSSLEVEL's XiZero",0);
	exit = new CLbutton(780,1,18,18,0,0x00C0C0C0,0,&exiting,"X",1);
	info = new CLbutton(2,21,100,18,0,0x00C0C0C0,0,&infoing,"Info",0);
	about = new CLbutton(102,21,100,18,0,0x00C0C0C0,0,&versing,"About",0);

	title->setvisible(0);
	exit->setvisible(0);
	info->setvisible(0);
	about->setvisible(0);
}

CLmenu::~CLmenu() //! noncritical
{
	delete icon;
	delete title;
	delete exit;
	delete info;
	delete about;	
}

void CLmenu::show() //! noncritical
{
	//save background
	sprite* back = clgfx.savescreen();
	//*
	
	//activate mouse cursor and activate buttons
	clwindow.showcursor(1);
	title->setvisible(1);
	exit->setvisible(1);
	info->setvisible(1);
	about->setvisible(1);
	//*
	
	while(clwindow.run())
	{
		if(clwindow.getinkey(1)==SPACE) { break; }
		CLbutton::checkclick(clwindow.getmouselb(),clwindow.getmousex(),clwindow.getmousey());
		clgfx.drawscreen(back);
		title->draw();
		clgfx.drawsprite(2,2,icon);
		exit->draw();
		clgfx.drawrectangle(0,20,XRES,40,0x00C0C0C0,1);
		info->draw();
		about->draw();
	}
	
	//activate mouse cursor and activate buttons
	clwindow.showcursor(0);
	title->setvisible(0);
	exit->setvisible(0);
	info->setvisible(0);
	about->setvisible(0);
	//*
}

void CLmenu::wrapper() { CLmenu::instance().show(); } //!noncritical
///*

#endif

