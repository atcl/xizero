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
		CLglobal() { };
		~CLglobal() { };
	public:
		static CLscreen* clscreen;		
		static CLwindow* clwindow;
		static CLbench*  clbench;
		static CLsystem* clsystem;
		static CLformat* clformat;
		static CLmath*   clmath;
		static CLgame*   clgame;
		static CLmisc3d* clmisc3d;
		static CLfloor*  clfloor;
		static CLdetect* cldetect;
		static CLfsprogress* clfsprogress;
		static CLgamepad* clgamepad;
		static CLgfx1*   clgfx1;
		static CLgfx2*   clgfx2;
		static CLtransitions* cltransitions;
		static CLsound*  clsound;
		static CLsprites* clsprites;
		static CLid*     clid;
		static CLstring* clstring;
		static CLapp*    clapp;
		static CLmsgbox* clmsgbox;
		static CLintro*  clintro;
};

CLscreen* CLglobal::clscreen = CLscreen::instance();
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
///*

#endif
