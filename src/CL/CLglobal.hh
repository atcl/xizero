///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGLOBALS
#define HH_CLGLOBALS
///*

///common includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLbase.hh"
#include "CLutils.hh"
///*

///singleton includes
#include "CLapp.hh"
#include "CLid.hh"
#include "CLmath.hh"
#include "CLscreen.hh"
#include "CLsprites.hh"
#include "CLstring.hh"
#include "CLsystem.hh"
#include "CLcamera.hh"
#include "CLdetect.hh"
#include "CLformat.hh"
#include "CLfsprog.hh"
#include "CLgame.hh"
#include "CLsound.hh"
#include "CLmisc3d.hh"
#include "CLgfx.hh"
#include "CLwindow.hh"
#include "CLintro.hh"
#include "CLmenu.hh"
#include "CLmsgbox.hh"
#include "CLtrans.hh"
///*

///dynamic includes
#include "CLfixed.hh"
#include "CLboool.hh"
#include "CLvector.hh"
#include "CLfifo.hh"
#include "CLlist.hh"
#include "CLtree.hh"
#include "CLbuffer.hh"
#include "CLar.hh"
#include "CLmainmenu.hh"
#include "CLlight.hh"
#include "CLlevel.hh"
#include "CLgame.hh"
#include "CLvoxels.hh"
#include "CLxml.hh"
//#include "CLxmlgui.hh"
///*

///header
/* class name:	CLglobal
 * 
 * description:	base class containing all singletons and global variables.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLglobal
{
	private:
		CLapp    app;
		CLid     id;
		CLmath   math;
		CLscreen screen;
		CLcomspr comspr;
		CLstring string;
		CLsystem system;
		//CLcamera camera;
		CLdetect detect;
		CLformat format;
		CLfsprog fsprog;
		CLgame   game;
		CLsound  sound;
		CLgfx    gfx;
		CLmisc3d misc3d;
		CLwindow window;
		CLintro  intro;
		CLmenu   menu;
		CLmsgbox msgbox;	
		CLtrans  trans;
	public:
		CLglobal(xchar* argv);
		~CLglobal() { };
		CLapp&    clapp;
		CLid&     clid;
		CLmath&   clmath;
		CLscreen& clscreen;
		CLcomspr& clcomspr;
		CLstring& clstring;
		CLsystem& clsystem;
		//CLcamera& clcamera
		CLdetect& cldetect;
		CLformat& clformat;
		CLfsprog& clfsprog;
		CLgame&   clgame;
		CLsound&  clsound;
		CLgfx&    clgfx;
		CLmisc3d& clmisc3d;
		CLwindow& clwindow;
		CLintro&  clintro;
		CLmenu&   clmenu;
		CLmsgbox& clmsgbox;
		CLtrans&  cltrans;
};
///*

///implementation
CLglobal::CLglobal(xchar* argv) :	app(CLapp::instance()), //! noncritical
									id(CLid::instance()),
									math(CLmath::instance()),
									screen(CLscreen::instance()),
									comspr(CLcomspr::instance()),
									string(CLstring::instance()),
									system(CLsystem::instance()),
									//camera(CLcamera::instance()),
									detect(CLdetect::instance()),
									format(CLformat::instance()),
									fsprog(CLfsprog::instance()),
									game(CLgame::instance()),
									sound(CLsound::instance()),
									gfx(CLgfx::instance()),
									misc3d(CLmisc3d::instance()),
									window(CLwindow::instance()),
									intro(CLintro::instance()),
									menu(CLmenu::instance()),
									msgbox(CLmsgbox::instance()),
									trans(CLtrans::instance()),
									
									clapp(app),
									clid(id),
									clmath(math),
									clscreen(screen),
									clcomspr(comspr),
									clstring(string),
									clsystem(system),
									//clcamera(camera),
									cldetect(detect),
									clformat(format),
									clfsprog(fsprog),
									clgame(game),
									clsound(sound),
									clgfx(gfx),
									clmisc3d(misc3d),
									clwindow(window),
									clintro(intro),
									clmenu(menu),
									clmsgbox(msgbox),
									cltrans(trans)
{ say("OK"); clapp.setexename(argv); }
///*

#endif
