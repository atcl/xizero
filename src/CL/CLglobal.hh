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

CLfvector clplane(0,0,1);
CLfvector cllight(1,1,-1);

CLmatrix* linearM;
CLmatrix* shadowM;

CLbench*    clbench;

//temp:
xlong xmark;
CLlvector tv(0,0,0);
CLlvector lv(0,0,0);
//

#endif
