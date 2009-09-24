//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLglobal* clglobal = CLsetup();
	//*

	//atcrosslevel intro
	clglobal->clintro->atcrosslevel();
	//*
	
	//xizero intro
	clglobal->clintro->xizero();
	//*
	
	//mainmenu
	sprite* xzmenuback = clglobal->clgfx2->savescreen();
	const xchar* xzmenubuttons[] = { "New Game","Load Game","Options","Exit Game" };
	void (*xzmenuactions[])() = { newgame,loadgame,options,exitgame };
	CLmainmenu* xzmainmenu = new CLmainmenu(xzmenuback,4,xzmenubuttons,xzmenuactions,"Xi Zero","Version 0.1",0x00FFFFFF);
	//while(clglobal->clwindow->run()) { xzmainmenu->draw(); }
	//*


	xchar** lfn = clglobal->clformat->loadlvl("../dat/levels/level0.lvl");
	CLlevel* testlevel = new CLlevel(lfn[0],lfn[1],lfn[2],lfn[3],lfn[4]);
	clglobal->clfloor->init(100,670,0x0000b0b0,1);

	xchar input = 0;
	xchar turbo = 0;
	xlong mode  = 1;
	xlong dis   = 0;
	bool running  = 1;
	bool pause = 0;
	xlong gamestate = 0;
	
	while(running && clglobal->clwindow->run()) 
	{
		turbo = clglobal->clwindow->getturbo();
		input = clglobal->clwindow->getinkey();
		clglobal->clgamepad->handle();

		switch(turbo)
		{
			case '0':
				running = 0;
				delete testlevel;
				clglobal->clsystem->exit(0,0,__func__,"xizero says: bye");
			break;
			
			case '1':
				mode = 1;
			break;
			
			case '2':
				mode = 2;
			break;
			
			case '3':
				mode = 3;
			break;
		}
		switch(input)
		{
			case 'p':
				pause = !pause;
			break;
			
			case '4':
				testlevel->pause();
			break;
		}
		
		if(!pause) { gamestate = testlevel->update(input,turbo,clglobal->clgamepad->getstate()); }

		clglobal->cldoublebuffer.ultraclear(0);
		clglobal->clzbuffer.clear(ZRES);
		clglobal->clstencilbuffer.ultraclear(0);

		if(gamestate!=0) break; 

		switch(mode)
		{
			case 1: 
				clglobal->clfloor->draw();
				testlevel->display();
			break;

			case 2:
				dis = ( testlevel->getmark() ) * XRES;
				clglobal->clmisc3d->drawzbuffer(testlevel->getlandscape(),dis);
				testlevel->getplayer()->showbox();
			break;

			case 3:
				clglobal->clfloor->draw();
				testlevel->display();
				clglobal->clmisc3d->drawzbuffer();
			break;
		}
		
		clglobal->clbench->inc();		
	}

	//game done:
	sprite* overscreen = 0;
	if(gamestate==1) overscreen = clglobal->clformat->loadtga("../dat/screens/gamewon.tga");
	else             overscreen = clglobal->clformat->loadtga("../dat/screens/gameover.tga"); 
	clglobal->clgfx1->drawscreen(overscreen);
	clglobal->clwindow->draw();
	clglobal->clsystem->wait(9000);
	//*

	//exit sequence
	delete testlevel;
	clglobal->clsound->exit();
	return 0;
	//*
}

