//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLGLOBAL
#define HH_CLGLOBAL
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLwindow.hh"
#include "CLmatrix.hh"
#include "CLbench.hh"

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

#endif
