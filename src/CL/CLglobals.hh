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

///*

#endif
