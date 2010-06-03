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
class CLmenu : public CLbase<CLmenu,1>
{
	friend class CLbase<CLmenu,1>;
	friend class CLglobal;
	
	private:
		CLformat& clformat;
		CLwindow& clwindow;
		CLgfx&    clgfx;
	protected:
		uxchar    syskey;
		sprite*   icon;
		CLlabel*  title;
		CLbutton* exit;
		CLbutton* info;
		CLbutton* about;
		CLmenu();
		~CLmenu();
	public:
		void show();
		static void wrapper();
		uxchar getsyskey() const { return syskey; };
};
///*

///implementation
CLmenu::CLmenu() //! noncritical
: clformat(CLformat::instance()), clwindow(CLwindow::instance()), clgfx(CLgfx::instance())
{
	syskey = '^';
	icon = clformat.loadxpm(CLicon);
	title = new CLlabel(0,0,XRES,20,0x00FFFFFF,0x00FF0000,0x00800000,"atCROSSLEVEL's XiZero",0);
	exit = new CLbutton(780,1,18,18,0,0x00C0C0C0,0,&bye,"X",1);
	info = new CLbutton(2,21,100,18,0,0x00C0C0C0,0,&nfo,"Info",1);
	about = new CLbutton(102,21,100,18,0,0x00C0C0C0,0,&nfo,"About",1);
	
	exit->setvisible(0);
	info->setvisible(0);
	about->setvisible(0);
	
	clwindow.setsyskey(syskey,&wrapper);
}

CLmenu::~CLmenu() //! noncritical
{
	//clwindow->setsyskey();
	//~ delete icon;
	//~ delete title;
	//~ delete exit;
	//~ delete info;
	//~ delete about;	
}

void CLmenu::show() //! noncritical
{
	//save background
	sprite* back = clgfx.savescreen();
	//*
	
	//activate mouse cursor and activate buttons
	clwindow.showcursor(1);
	exit->setvisible(1);
	info->setvisible(1);
	about->setvisible(1);
	//*
	
	while(clwindow.run())
	{
		if(clwindow.getinkey()==SPACE) { break; }
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
	exit->setvisible(0);
	info->setvisible(0);
	about->setvisible(0);
	//*
}

void CLmenu::wrapper() { CLmenu::instance().show(); } //!noncritical
///*

#endif

