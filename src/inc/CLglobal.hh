//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLGLOBAL
#define HH_CLGLOBAL
#warning "Compiling " __FILE__ " ! TODO: ..."


//variables
uxlong CLscreenwidth = xres;
uxlong CLscreenheight = yres;
uxlong CLscreendepth = 4;
uxlong CLscreenpixsize = xres * yres;
uxlong CLscreenmemsize = xres * yres * 4;

CLbuffer<xlong>* CLdoublebuffer;
CLbuffer<xlong>* CLstencilbuffer;
CLbuffer<float>* CLzbuffer;

CLlight*   amb;
CLmatrix*  ltm;
CLmatrix*  bsm;

//static classes
CLfps*    clfps;
CLformat* clformat;
CLmath*   clmath;
CLgfx1*   clgfx1;
CLgfx2*   clgfx2;

#endif
