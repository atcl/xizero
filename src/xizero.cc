//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	clglobal = CLsetup();
	cldata = new CLar(BASEDIR"xizero.dat");
	clglobal->clgfx2->loadfonts(cldata->findbyname("fonts.a"));
	clglobal->clcursor->set(CLxzcursor);
	//*

	//intros
	CLintro* clintro = new CLintro(cldata->findbyname("intro.a"));
	clintro->atcrosslevel();
	clintro->xizero();
	//*
	
	//mainmenu
	sprite* xzmenuback = clglobal->clgfx2->savescreen();
	const xchar* xzmenubuttons[] = { "New Game","Load Game","Options","Exit Game" };
	void (*xzmenuactions[])() = { newgame,loadgame,options,exitgame };
	CLmainmenu* xzmainmenu = new CLmainmenu(xzmenuback,4,xzmenubuttons,xzmenuactions,"Xi Zero","Version "CLversion,0x00FFFFFF);
	clglobal->clcursor->show();
	while(clglobal->clwindow->run())
	{
		CLbutton::checkclick();
		xzmainmenu->draw();
		clglobal->clcursor->draw();
	}
	//*

	//no way leads here, but anyhow:	
	return 0;
	//*
}

