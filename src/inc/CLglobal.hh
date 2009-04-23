//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLGLOBAL
#define HH_CLGLOBAL
#pragma message "Compiling " __FILE__ " ! TODO: ..."


//variables
uxlong CLscreenwidth = xres;
uxlong CLscreenheight = yres;
uxlong CLscreendepth = 4;
uxlong CLscreenpixsize = xres * yres;
uxlong CLscreenmemsize = xres * yres * 4;

xlong* CLdoublebuffer;
xlong* CLstencilbuffer;
float* CLzbuffer;

CLlight*   amb;
CLmatrix*  ltm;
CLmatrix*  bsm;

//static classes
CLfps*    clfps;
CLformat* clformat;
CLbuffer* clbuffer;
CLmath*   clmath;
CLgfx1*   clgfx1;
CLgfx2*   clgfx2;

#endif
