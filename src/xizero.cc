//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <iostream>

//using namespace std;

#include "CLinc.h"
#include "xizero.hh"

int main()
{
	//init sequence
	CLsetup();
	FLTKsetup();


	CLlevel *testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemies/enemies.a","../dat/enemies/enemydat.a","../dat/player/player.a","../dat/level/level0.a",clformat,clmath,CLdoublebuffer,CLzbuffer,CLstencilbuffer);

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
// 			case FL_Up:
// 				testlevel->subsmark(4);
// 			break;
// 			case FL_Down:
// 				testlevel->subsmark(-4);
// 			break;

			case '0':
				CLexit_(0,0,__func__,"xizero says: bye");
			break;

			default:
				testlevel->update(input,turbo);
			break;
		}

		CLdoublebuffer->fastclear(0);
		CLzbuffer->clear(zmax);
		CLstencilbuffer->fastclear(0);

		testlevel->display();

		//drawzbuffer(CLzbuffer,CLdoublebuffer);

		clfps->increment(); 
	}

	//clean up:
	delete testlevel; //wrap clean up code in a func in the header file that is callable by a new CLexit func

	return 0;
}

