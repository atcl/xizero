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
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///declarations
class CLapp;
class CLid;
class CLmath;
class CLscreen;
class CLcomspr;
class CLstring;
class CLsystem;
class CLcamera;
class CLdetect;
class CLformat;
class CLfsprog;
class CLgame;
class CLsound;
class CLgfx;
class CLmisc3d;
class CLwindow;
class CLintro;
class CLmenu;
class CLmsgbox;
class CLtrans;
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
{ clapp.setexename(argv); }

///*

#endif
