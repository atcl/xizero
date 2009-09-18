//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLglobal* clglobal = CLsetup();

	CLlevel* testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemy/enemies.a","../dat/player/player.a","../dat/level/level0.a");
	clglobal->clfloor->init(100,670,0x0000b0b0,1);

	xchar input = 0;
	xchar turbo = 0;
	xlong mode  = 1;
	xlong dis   = 0;
	bool running  = 1;
	bool pause = 0;
	
	//CLfsprogress::instance()->set(20);
	
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
		
		if(!pause) testlevel->update(input,turbo,clglobal->clgamepad->getstate());

		clglobal->cldoublebuffer.ultraclear(0);
		clglobal->clzbuffer.clear(zres);
		clglobal->clstencilbuffer.ultraclear(0);

		switch(mode)
		{
			case 1: 
				clglobal->clfloor->draw();
				testlevel->display();
				break;

			case 2:
				dis = ( testlevel->getmark() ) * xres;
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

	//exit sequence
	delete testlevel;
	clglobal->clsound->exit();
	return 0;
	//*
}

