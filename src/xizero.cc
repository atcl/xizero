//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <iostream>

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLsetup();

	CLlevel* testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemy/enemies.a","../dat/player/player.a","../dat/level/level0.a");
	CLfloor::init(100,670,0x0000b0b0,1);

	clbench->init();

	xchar input = 0;
	xchar turbo = 0;
	xlong mode  = 1;
	xlong dis   = 0;
	bool running  = 1;
	bool pause = 0;
	
	while(running && win->run()) 
	{
		turbo = win->getturbo();
		input = win->getkey();

		switch(turbo)
		{
			case '0':
				running = 0;
				delete testlevel;
				CLsystem::exit(0,0,__func__,"xizero says: bye");
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
			
			case 'p':
				pause = !pause;
			break;
		}
		
		if(!pause) testlevel->update(input,turbo);

		CLdoublebuffer->fastclear(0);
		CLzbuffer->fastclear(zres);
		CLstencilbuffer->fastclear(0);

		switch(mode)
		{
			case 1: 
				CLfloor::draw();
				testlevel->display();
				break;

			case 2:
				dis = ( testlevel->getmark() ) * xres;
				CLmisc3d::drawzbuffer(testlevel->getlandscape(),dis);
				testlevel->getplayer()->showbox();
				break;

			case 3:
				CLfloor::draw();
				testlevel->display();
				CLmisc3d::drawzbuffer();
				break;
		}
		
		clbench->inc();		
	}

	delete testlevel;

	return 0;
}

