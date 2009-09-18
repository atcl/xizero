//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSETUP
#define HH_CLSETUP
#pragma message "Compiling " __FILE__ " ! TODO: ..."

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
//*


void CLsetup()
{
	//init core buffers
	CLdoublebuffer  = new CLbuffer<xlong>(xres*yres);
	CLstencilbuffer = new CLbuffer<xlong>(xres*yres);
	CLzbuffer       = new CLbuffer<float>(xres*yres);
	//*
	
	//open window
	win = CLwindow::instance();
	win->init(xres,yres,CLTITLE,CLdoublebuffer->getbuffer());
	//*

	//init benchmarking
	CLbench::instance()->init(3,COUT_FPS);
	//*

	//init matrices
	linearM  = new CLmatrix(1);
	shadowM  = new CLmatrix(1);
	shadowM->shadow(clslight,clplane);
	//*
	
	leftside = new screenside[yres];
	rightside = new screenside[yres];
}

void CLgarbage()
{
	//free globals
	delete CLdoublebuffer;
	delete CLstencilbuffer;
	delete CLzbuffer;
	delete linearM;
	delete shadowM;
	delete leftside;
	delete rightside;
	//*
}

#endif
