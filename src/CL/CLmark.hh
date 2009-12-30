///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLMARK
#define HH_CLMARK
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLsystem.hh"
#include "CLlist.hh"
#include "CLsprites.hh"
#include "CLmath.hh"
///*

///header
/* class name:	CLmark
 * 
 * description:	This class manages all floor markings.
 * 
 * author:	atcl
 * 
 * notes:
 * 
 * version: 0.1
 */
///*

///definitions
struct CLmark
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
};

typedef CLlist<CLmark> CLmarklist;

class CLmarkmanager : public CLbase<CLmarkmanager,0>
{
	private:
		static CLsprites* clsprites;
		static CLsystem*  clsystem;
		static CLgame*    clgame;
		static CLmath*    clmath;
	protected:
		CLmarklist* marklist;
		CLmark** marktype;
		xlong marktypecount;
		xlong lastupdate;
		xlong* lmark;
	public:
		CLmarkmanager(xlong mtc,xlong* mts,xlong* lm);
		~CLmarkmanager();
		void mark(uxlong marktype,const CLfvector& markposition);
		void update();
		void display() const;
		void pause();
};

CLsprites* CLammomanager::clsprites = CLsprites::instance();
CLsystem*  CLammomanager::clsystem  = CLsystem::instance();
CLgame*    CLammomanager::clgame    = CLgame::instance();
CLmath*    CLammomanager::clmath    = CLmath::instance();
///*

///implementation

///*

#endif
