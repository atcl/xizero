//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <iostream>

using namespace std;

#include "CLinc.h"


int main()
{
	//init sequence
	CLsetup();
	FLTKsetup();


	CLlevel *testlevel = new CLlevel("../dat/terrain/terrain.a","../dat/enemies/enemies.a","../dat/enemies/enemydat.a","../dat/player/player.a","../dat/player/player.a","../dat/level/level0.a",clformat,clmath,CLdoublebuffer,CLzbuffer,CLstencilbuffer);

	clfps->init();

	while(FLTKwait())
	{
		switch(FLTKgetturbokeystate())
		{
			case 0:
			break;
			case FL_Up:
				testlevel->subsmark(4);
			break;
			case FL_Down:
				testlevel->subsmark(-4);
			break;

			case '0':
				CLexit_(__func__,"xizero says: bye",0);
			break;
		}

		clbuffer->clear(0,CLdoublebuffer,CLscreenpixsize);
		clbuffer->clear(zmax,CLzbuffer,CLscreenpixsize);
		clbuffer->clear(0,CLstencilbuffer,CLscreenpixsize);

		testlevel->display();

		clfps->increment(); 
	}

	//xlong reval = CLsystem_("Xdialog --msgbox 'hi' 0 0");

	return 0;
}
