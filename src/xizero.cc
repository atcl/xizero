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

	CLlevel *testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemies/enemies.a","../dat/enemies/enemydat.a","../dat/player/player.a","../dat/level/level0.a",CLdoublebuffer,CLzbuffer,CLstencilbuffer);

// 	testlevel->display();	

	clfps->init();

	xchar input = 0;
	xchar turbo = 0;
	xlong mark  = 0;

	while(FLTKwait())
	{
		turbo = FLTKgetturbokeystate();
		input = FLTKgetkeystate();

		switch(turbo)
		{
			case '0':
				delete testlevel;
				CLexit_(0,0,__func__,"xizero says: bye");
			break;

			default:
				testlevel->update(input,turbo);
			break;
		}

		CLdoublebuffer->fastclear(0);
		CLzbuffer->clear(zres-1);
		CLstencilbuffer->fastclear(0);

		//drawfloor(105,670,0x0000b0b0,CLzbuffer,CLdoublebuffer);

		testlevel->display();

		//drawzbuffer(CLzbuffer,CLdoublebuffer);

		clfps->increment(); 
	}

	return 0;
}

