//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSETUP
#define HH_CLSETUP
//#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLmath.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLfps.hh"
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
	
	win = new CLwindow(xres,yres,"atCROSSLEVEL",CLdoublebuffer->getbuffer());

	clbench    = new CLbench(3,COUT_FPS);

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
	delete clbench;
	delete linearM;
	delete shadowM;
	delete leftside;
	delete rightside;
}

#endif
