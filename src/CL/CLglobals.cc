//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef CC_CLGLOBALS
#define CC_CLGLOBALS
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLwindow.hh"
#include "CLbench.hh"
#include "CLsystem.hh"
#include "CLformat.hh"
#include "CLutils.hh"
#include "CLmath.hh"
#include "CLgame.hh"
#include "CLmisc3d.hh"
#include "CLfloor.hh"
#include "CLfsprog.hh"
#include "CLdetect.hh"
#include "CLgamepad.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"
#include "CLscreen.hh"
#include "CLsound.hh"
#include "CLid.hh"
#include "CLcursor.hh"

#include "CLglobals.hh"

uxlong CLglobal::screenbytewidth   = xres<<2;
uxlong CLglobal::screenbyteheight  = xres<<2;
uxlong CLglobal::screenbytedepth   = 4;
uxlong CLglobal::screenbytesize    = (xres*yres)<<2;
uxlong CLglobal::screenpixelwidth  = xres;
uxlong CLglobal::screenpixelheight = yres;
uxlong CLglobal::screenpixelsize   = xres*yres;

CLlbuffer CLglobal::cldoublebuffer  = *(new CLlbuffer(xres*yres));
CLlbuffer CLglobal::clstencilbuffer = *(new CLlbuffer(xres*yres));
CLfbuffer CLglobal::clzbuffer       = *(new CLfbuffer(xres*yres));

CLwindow* CLglobal::clwindow = CLwindow::instance();
CLbench*  CLglobal::clbench  = CLbench::instance();
CLsystem* CLglobal::clsystem = CLsystem::instance();
CLformat* CLglobal::clformat = CLformat::instance();
CLutils*  CLglobal::clutils  = CLutils::instance();
CLmath*   CLglobal::clmath   = CLmath::instance();
CLgame*   CLglobal::clgame   = CLgame::instance();
CLmisc3d* CLglobal::clmisc3d = CLmisc3d::instance();
CLfloor*  CLglobal::clfloor  = CLfloor::instance();
CLdetect* CLglobal::cldetect = CLdetect::instance();
CLfsprogress* CLglobal::clfsprogress = CLfsprogress::instance();
CLgamepad* CLglobal::clgamepad = CLgamepad::instance();
CLgfx1*   CLglobal::clgfx1   = CLgfx1::instance();
CLgfx2*   CLglobal::clgfx2   = CLgfx2::instance();
CLtransitions* CLglobal::cltransitions = CLtransitions::instance();
CLsound*  CLglobal::clsound  = CLsound::instance();
CLsprites* CLglobal::clsprites = CLsprites::instance();
CLintro*  CLglobal::clintro  = CLintro::instance();
CLid*     CLglobal::clid     = CLid::instance();
CLcursor* CLglobal::clcursor = CLcursor::instance();

CLglobal* CLglobal::global = 0;

CLglobal* CLglobal::getinstance()
{
	//check if existing, if not create
	if(global==0) global = new CLglobal();
	return global;
	//*
}

#endif