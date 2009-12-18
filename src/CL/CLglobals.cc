///license
//atCROSSLEVEL studios
//licensed under zlib/libpng license
///*

///guard
#ifndef CC_CLGLOBALS
#define CC_CLGLOBALS
///*

///includes
#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLwindow.hh"
#include "CLbench.hh"
#include "CLsystem.hh"
#include "CLformat.hh"
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
#include "CLstring.hh"
#include "CLapp.hh"
#include "CLmsgbox.hh"
#include "CLintro.hh"

#include "CLglobals.hh"
///*

///definitions
uxlong CLglobal::screenbytewidth   = XRES<<2;
uxlong CLglobal::screenbyteheight  = YRES<<2;
uxlong CLglobal::screenbytedepth   = 4;
uxlong CLglobal::screenbytesize    = (XRES*YRES)<<2;
uxlong CLglobal::screenpixelwidth  = XRES;
uxlong CLglobal::screenpixelheight = YRES;
uxlong CLglobal::screenpixelsize   = (XRES*YRES);

CLubuffer CLglobal::cldoublebuffer  = *(new CLubuffer((XRES*YRES),0));
CLubuffer CLglobal::cltriplebuffer  = *(new CLubuffer((XRES*YRES),0));
CLubuffer CLglobal::clstencilbuffer = *(new CLubuffer((XRES*YRES),0));
CLfbuffer CLglobal::clzbuffer       = *(new CLfbuffer((XRES*YRES),ZRES));

CLwindow* CLglobal::clwindow = CLwindow::instance();
CLbench*  CLglobal::clbench  = CLbench::instance();
CLsystem* CLglobal::clsystem = CLsystem::instance();
CLformat* CLglobal::clformat = CLformat::instance();
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
CLid*     CLglobal::clid     = CLid::instance();
CLstring* CLglobal::clstring = CLstring::instance();
CLapp*    CLglobal::clapp    = CLapp::instance();
CLmsgbox* CLglobal::clmsgbox = CLmsgbox::instance();
CLintro*  CLglobal::clintro  = CLintro::instance();

CLglobal* CLglobal::global = 0;
///*

///implementation
CLglobal* CLglobal::getinstance() //! noncritical
{
	//check if existing, if not create
	if(global==0) global = new CLglobal();
	return global;
	//*
}
///*

#endif
