///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///includes
#include "xizero.hh"
///*

///mplementation
int main(int argc,char* argv[])
{
	CLar* cldata = 0;
	
	//init sequence
	tty("Loading: ");
	CLglobal clglobal(argv[0]);
	sprite* cursor = clglobal.clformat.loadxpm(CLxzcursor);
	clglobal.clwindow.setcursor(cursor);
	if(clglobal.clsystem.existsfile("xizero.dat")==1) { cldata = new CLar("xizero.dat"); }
	else { cldata = new CLar("/usr/share/xizero/xizero.dat"); }
	clglobal.clgfx.loadfonts(cldata->findbyname("fonts.a"));
	//*

	//intros
	clglobal.clintro.atcrosslevel(cldata->findbyname("intro.a"));
	sprite* xzmenuback = clglobal.clintro.xizero(cldata->findbyname("intro.a"),1);
	//*
	
	//mainmenu
	const xchar* xzmenubuttons[] = { "New Game","Load Game","Options","Exit Game" };
	void (*xzmenuactions[])(void*,void*) = { newgame,loadgame,options,exitgame };
	CLmainmenu* xzmainmenu = new CLmainmenu(xzmenuback,4,xzmenubuttons,xzmenuactions,"Xi Zero","Version "VERSION,0x00FFFFFF);
	clglobal.clwindow.showcursor(1);
	while(clglobal.clwindow.run())
	{
		CLbutton::checkclick(clglobal.clwindow.getmouselb(),clglobal.clwindow.getmousex(),clglobal.clwindow.getmousey(),&clglobal,cldata);
		xzmainmenu->draw();
	}
	//*

	//no way leads here, but anyhow:
	return 0;
	//*
}
///*

