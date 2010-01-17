///license
//atCROSSLEVEL studios
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGLOBALS
#define HH_CLGLOBALS
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLscreen.hh"
#include "CLwindow.hh"
#include "CLsystem.hh"
#include "CLformat.hh"
#include "CLmath.hh"
#include "CLgame.hh"
#include "CLmisc3d.hh"
#include "CLfloor.hh"
#include "CLfsprog.hh"
#include "CLdetect.hh"
#include "CLgfx.hh"
#include "CLtrans.hh"
#include "CLsound.hh"
#include "CLid.hh"
#include "CLstring.hh"
#include "CLapp.hh"
#include "CLmsgbox.hh"
#include "CLintro.hh"
///*

///header
/* class name:	CLglobal
 * 
 * description:	base class containing all singletons and global variables.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///declarations

///*

///definitions
class CLglobal : public CLbase<CLglobal,1>
{
	friend class CLbase<CLglobal,1>; 
	
	protected:
		CLglobal();
		~CLglobal();
	public:
		CLscreen* clscreen;		
		CLwindow* clwindow;
		CLsystem* clsystem;
		CLformat* clformat;
		CLmath*   clmath;
		CLgame*   clgame;
		CLmisc3d* clmisc3d;
		CLfloor*  clfloor;
		CLdetect* cldetect;
		CLfsprogress* clfsprogress;
		CLgfx*    clgfx;
		CLtransitions* cltransitions;
		CLsound*  clsound;
		CLsprites* clsprites;
		CLid*     clid;
		CLstring* clstring;
		CLapp*    clapp;
		CLmsgbox* clmsgbox;
		CLintro*  clintro;
};
///*

///implementation
CLglobal::CLglobal() //! noncritical
{
	clscreen = CLscreen::instance();
	clwindow = CLwindow::instance();
	clsystem = CLsystem::instance();
	clformat = CLformat::instance();
	clmath   = CLmath::instance();
	clgame   = CLgame::instance();
	clmisc3d = CLmisc3d::instance();
	clfloor  = CLfloor::instance();
	cldetect = CLdetect::instance();
	clfsprogress = CLfsprogress::instance();
	clgfx    = CLgfx::instance();
	cltransitions = CLtransitions::instance();
	clsound  = CLsound::instance();
	clsprites = CLsprites::instance();
	clid     = CLid::instance();
	clstring = CLstring::instance();
	clapp    = CLapp::instance();
	clmsgbox = CLmsgbox::instance();
	clintro  = CLintro::instance();
}
	
CLglobal::~CLglobal() //! noncritical
{
	
}
///*

#endif
