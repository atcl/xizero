//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include "CLinc.h"
#include "xizero.h"
#include "xizero.hh"

int main()
{
	//init sequence
	clglobal = CLsetup();
	cldata = new CLar(BASEDIR"xizero.dat");
	clglobal->clgfx->loadfonts(cldata->findbyname("fonts.a"));
	//*

	//intros
	clglobal->clintro->atcrosslevel(cldata->findbyname("intro.a"));
	clglobal->clintro->xizero(cldata->findbyname("intro.a"));
	//*
	
	//mainmenu
	sprite* xzmenuback = clglobal->clgfx->savescreen();
	const xchar* xzmenubuttons[] = { "New Game","Load Game","Options","Exit Game" };
	void (*xzmenuactions[])() = { newgame,loadgame,options,exitgame };
	CLmainmenu* xzmainmenu = new CLmainmenu(xzmenuback,4,xzmenubuttons,xzmenuactions,"Xi Zero","Version "VERSION,0x00FFFFFF);
	clglobal->clwindow->showcursor(1);
	while(clglobal->clwindow->run())
	{
		CLbutton::checkclick();
		xzmainmenu->draw();
	}
	//*

	//no way leads here, but anyhow:	
	return 0;
	//*
}

