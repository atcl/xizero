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
class CLscreen;
class CLwindow;
class CLsystem;
class CLformat;
class CLmath;
class CLgame;
class CLmisc3d;
class CLfsprog;
class CLdetect;
class CLgfx;
class CLtrans;
class CLid;
class CLstring;
class CLapp;
class CLmsgbox;
class CLintro;
class CLsound;
class CLcomspr;
class CLmenu;
///*

///definitions
class CLglobal
{
	private:
		CLscreen screen;		
		CLwindow window;
		CLsystem system;
		CLformat format;
		CLmath   math;
		CLgame   game;
		CLmisc3d misc3d;
		CLdetect detect;
		CLfsprog fsprog;
		CLgfx    gfx;
		CLtrans  trans;
		CLsound  sound;
		CLcomspr comspr;
		CLid     id;
		CLstring string;
		CLapp    app;
		CLmsgbox msgbox;
		CLintro  intro;
		CLmenu   menu;
	public:
		CLglobal(xchar* argv);
		~CLglobal() { };
		CLscreen& clscreen;
		CLwindow& clwindow;
		CLsystem& clsystem;
		CLformat& clformat;
		CLmath&   clmath;
		CLgame&   clgame;
		CLmisc3d& clmisc3d;
		CLdetect& cldetect;
		CLfsprog& clfsprog;
		CLgfx&    clgfx;
		CLtrans&  cltrans;
		CLsound&  clsound;
		CLcomspr& clcomspr;
		CLid&     clid;
		CLstring& clstring;
		CLapp&    clapp;
		CLmsgbox& clmsgbox;
		CLintro&  clintro;
		CLmenu&   clmenu;
};
///*

///implementation
CLglobal::CLglobal(xchar* argv) : screen(CLscreen::instance()), window(CLwindow::instance()), system(CLsystem::instance()),
								  format(CLformat::instance()), math(CLmath::instance()),     game(CLgame::instance()),
								  misc3d(CLmisc3d::instance()), detect(CLdetect::instance()), fsprog(CLfsprog::instance()),
								  gfx(CLgfx::instance()),       trans(CLtrans::instance()),   sound(CLsound::instance()),
								  comspr(CLcomspr::instance()), id(CLid::instance()),         string(CLstring::instance()),
								  app(CLapp::instance()),       msgbox(CLmsgbox::instance()), intro(CLintro::instance()),
								  menu(CLmenu::instance()),
								  
								  clscreen(screen), clwindow(window), clsystem(system),
								  clformat(format), clmath(math),     clgame(game),
								  clmisc3d(misc3d), cldetect(detect), clfsprog(fsprog),
								  clgfx(gfx),       cltrans(trans),   clsound(sound),
								  clcomspr(comspr), clid(id),         clstring(string),
								  clapp(app),       clmsgbox(msgbox), clintro(intro),
								  clmenu(menu) { clapp.setexename(argv); } //! noncritical

///*

#endif
