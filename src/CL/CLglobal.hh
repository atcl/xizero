///license
//atCROSSLEVEL studios
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGLOBAL
#define HH_CLGLOBAL
///*

///includes
#include "CLtypes.hh"
#include "CLmatrix.hh"
///*

///declarations
//variables
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

//more temp:
uxlong screenbytewidth = XRES<<2;
uxlong screenbyteheight = YRES<<2;
uxlong screenbytedepth = 4;
uxlong screenbytesize = (XRES*YRES)<<2;
uxlong screenpixelwidth = XRES;
uxlong screenpixelheight = YRES;
uxlong screenpixelsize = XRES*YRES;

CLubuffer cldoublebuffer = *(new CLubuffer((XRES*YRES),0));
CLubuffer cltriplebuffer = *(new CLubuffer((XRES*YRES),0));
CLubuffer clstencilbuffer = *(new CLubuffer((XRES*YRES),0));
CLfbuffer clzbuffer = *(new CLfbuffer((XRES*YRES),ZRES));

CLfvector light;
CLfvector slight;
CLfvector plane;

//*

///*

#endif
