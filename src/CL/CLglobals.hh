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
uxlong nolight = 0x00303030;

CLmatrix* shadowM;

///*

#endif
