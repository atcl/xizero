//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLGLOBAL
#define HH_CLGLOBAL
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLwindow.hh"
#include "CLmatrix.hh"
#include "CLfps.hh"
#include "CLsingle.hh"

class CLglobal : public CLsingle<CLglobal>
{
	private:
		CLglobal();
		~CLglobal();	
	public:
		uxlong CLscreenbytewidth;
		uxlong CLscreenbyteheight;
		uxlong CLscreenbytedepth;
		uxlong CLscreenbytesize;
		uxlong CLscreenpixelwidth;
		uxlong CLscreenpixelheight;
		uxlong CLscreenpixelsize;
		
		CLlbuffer* CLdoublebuffer;
		CLlbuffer* CLstencilbuffer;
		CLfbuffer* CLzbuffer;
		
		CLwindow* window;
		CLbench*  bench;
		
		CLfvector light;
		CLfvector slight;
		CLfvector plane;
		
		//~ CLsystem* system;
		//~ CLformat* format;
		//~ CLutils*  utils;
};

//variables
uxlong CLscreenwidth = xres;
uxlong CLscreenheight = yres;
uxlong CLscreendepth = 4;
uxlong CLscreenpixsize = xres * yres;
uxlong CLscreenmemsize = xres * yres * 4;

CLlbuffer* CLdoublebuffer;
CLlbuffer* CLstencilbuffer;
CLfbuffer* CLzbuffer;

CLwindow* win;

CLfvector cllight(1,1,1);
CLfvector clplane(0,0.1,1);
CLfvector clslight(0,0.4,1.1);

CLmatrix* linearM;
CLmatrix* shadowM;

//temp:
xlong xmark;
CLlvector tv(0,0,0);
CLlvector lv(0,0,0);
//

#endif
