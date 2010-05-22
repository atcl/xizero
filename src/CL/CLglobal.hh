///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGLOBALS
#define HH_CLGLOBALS
///*

///includes
#include "CLtypes.hh"
#include "CLid.hh"
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
class CLscreen;
class CLwindow;
class CLsystem;
class CLformat;
class CLmath;
class CLgame;
class CLmisc3d;
class CLfsprogress;
class CLdetect;
class CLgfx;
class CLtransitions;
class CLid;
class CLstring;
class CLapp;
class CLmsgbox;
class CLintro;
class CLsound;
class CLsprites;
class CLmenu;
///*

///definitions
class CLglobal
{
	public:
		CLglobal(xchar* argv);
		~CLglobal();
		CLscreen* clscreen;		
		CLwindow* clwindow;
		CLsystem* clsystem;
		CLformat* clformat;
		CLmath*   clmath;
		CLgame*   clgame;
		CLmisc3d* clmisc3d;
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
		CLmenu*   clmenu;
};
///*

///implementation
CLglobal::CLglobal(xchar* argv) //! noncritical
{
	clscreen      = CLscreen::instance();
	clwindow      = CLwindow::instance();
	clsystem      = CLsystem::instance();
	clformat      = CLformat::instance();
	clmath        = CLmath::instance();
	clgame        = CLgame::instance();
	clmisc3d      = CLmisc3d::instance();
	cldetect      = CLdetect::instance();
	clfsprogress  = CLfsprogress::instance();
	clgfx         = CLgfx::instance();
	cltransitions = CLtransitions::instance();
	clsound       = CLsound::instance();
	clsprites     = CLsprites::instance();
	clid          = CLid::instance();
	clstring      = CLstring::instance();
	clapp         = CLapp::instance();
	clmsgbox      = CLmsgbox::instance();
	clintro       = CLintro::instance();
	clmenu        = CLmenu::instance();
}
	
CLglobal::~CLglobal() //! noncritical
{
	delete clsound;
}
///*

#endif
