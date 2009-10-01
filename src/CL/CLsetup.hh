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
#include "CLbench.hh"
#include "CLmatrix.hh"

//prototypes:
CLglobal* CLsetup();
void CLgarbage();
//*

CLglobal* CLsetup()
{
	CLglobal* clglobal = CLglobal::getinstance(); 
	
	clglobal->clwindow->init(XRES,YRES,CLTITLE);

	//init matrices
	linearM  = new CLmatrix(1);
	shadowM  = new CLmatrix(1);
	shadowM->shadow(clslight,clplane);
	//*
	
	leftside = new screenside[YRES];
	rightside = new screenside[YRES];
	
	return clglobal;
}

void CLgarbage()
{
	//free globals
	delete linearM;
	delete shadowM;
	delete leftside;
	delete rightside;
	//*
}

#endif
