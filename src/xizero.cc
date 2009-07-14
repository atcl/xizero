//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <iostream>

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLsetup();
	FLTKsetup();

	CLlevel *testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemies/enemies.a","../dat/enemies/enemydat.a","../dat/player/player.a","../dat/level/level0.a");

	clfps->init();

	xchar input = 0;
	xchar turbo = 0;
	xlong mark  = 0;
	xlong mode  = 1;
	xlong dis   = 0;

	while(FLTKwait())
	{
		turbo = FLTKgetturbokeystate();
		input = FLTKgetkeystate();

		switch(turbo)
		{
			case '0':
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
				CLmisc3d::drawfloor(100,670,0x0000b0b0,CLzbuffer,CLdoublebuffer);
				testlevel->display();
				break;

			case 2:
				dis = ( testlevel->getmark() ) * xres;
				CLmisc3d::drawzbuffer(testlevel->getlandscape(),CLdoublebuffer,dis);
				(*testlevel->getlandscape())[ (xlong(tv.y)*xres) + xlong(tv.x) ] = 0x000FFFFFF;
				(*testlevel->getlandscape())[ (xlong(lv.y)*xres) + xlong(lv.x) ] = 0x000FFFF00; 
				break;

			case 3:
				CLmisc3d::drawfloor(100,670,0x0000b0b0,CLzbuffer,CLdoublebuffer);
				testlevel->display();
				CLmisc3d::drawzbuffer(CLzbuffer,CLdoublebuffer);
				break;
		}
		
		clfps->increment(); 
	}

	return 0;
}

