//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <iostream>

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLsetup();

	CLlevel *testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemy/enemies.a","../dat/player/player.a","../dat/level/level0.a");

	clbench->init();

	xchar input = 0;
	xchar turbo = 0;
	xlong mode  = 1;
	xlong dis   = 0;
	bool running  = 1;

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

			default:
				testlevel->update(input,turbo);
			break;
		}

		CLdoublebuffer->fastclear(0);
		CLzbuffer->clear(zres); //optimze by: fastclear for float(reinterpret_cast), get rid pf clear translation 
		//CLstencilbuffer->fastclear(0);

		switch(mode)
		{
			case 1: 
				CLmisc3d::drawfloor(100,670,0x0000b0b0);
				testlevel->display();
				break;

			case 2:
				dis = ( testlevel->getmark() ) * xres;
				CLmisc3d::drawzbuffer(testlevel->getlandscape(),dis);
				(*testlevel->getlandscape())[ (xlong(tv.y)*xres) + xlong(tv.x) ] = 0x000FFFFFF; //temp
				(*testlevel->getlandscape())[ (xlong(lv.y)*xres) + xlong(lv.x) ] = 0x000FFFF00; //temp
				break;

			case 3:
				CLmisc3d::drawfloor(100,670,0x0000b0b0);
				testlevel->display();
				CLmisc3d::drawzbuffer();
				break;
		}
		
		clbench->inc();		
	}

	return 0;
}

