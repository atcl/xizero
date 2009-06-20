//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSETUP
#define HH_CLSETUP
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLfps.hh"

#include "CLlight.hh"
#include "CLmatrix.hh"


//prototypes:
void CLsetup();
void CLgarbage();
//


void CLsetup()
{
	CLdoublebuffer  = new CLbuffer<xlong>(xres*yres);
	CLstencilbuffer = new CLbuffer<xlong>(xres*yres);
	CLzbuffer       = new CLbuffer<float>(xres*yres);

	clfps    = new CLfps(3,true);

	CLmath::init();

	linearM  = new CLmatrix(1);
	shadowM  = new CLmatrix(1);

	shadowM->shadow(cllight,clplane);

	leftside = new screenside[yres];
	rightside = new screenside[yres]; 
}

void CLgarbage()
{
	delete CLdoublebuffer;
	delete CLstencilbuffer;
	delete CLzbuffer;

	delete clfps;

	delete linearM;
	delete shadowM;

	delete leftside;
	delete rightside;
}

#endif
