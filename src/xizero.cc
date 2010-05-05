//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license

#include "CLinc.h"
#include "xizero.h"
#include "xizero.hh"

int main(int argc,char* argv[])
{
	//init sequence
	clglobal = CLglobal::instance(); 
	clglobal->clapp->init(argv);
	sprite* cursor = clglobal->clformat->loadxpm(CLxzcursor);
	clglobal->clwindow->setcursor(cursor);
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

