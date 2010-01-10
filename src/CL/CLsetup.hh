///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSETUP
#define HH_CLSETUP
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLmath.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLglobals.hh"
#include "CLbench.hh"
#include "CLmatrix.hh"
///*

///definitions
//prototypes:
CLglobal* CLsetup();
void CLgarbage();
//*
///*

///implementation
CLglobal* CLsetup() //! noncritical
{
	CLglobal* clglobal = CLglobal::instance(); 

	sprite* cursor = clglobal->clformat->loadxpm(CLxzcursor);
	clglobal->clwindow->setcursor(cursor);

	clglobal->clmath->pi();
	
	return clglobal;
}

void CLgarbage() //! noncritical
{

}
///*

#endif
