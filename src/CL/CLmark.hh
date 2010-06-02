///license
//atCROSSLEVEL studios 2009,2010
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
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
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
		CLmath&   clmath;
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
///*

///implementation

///*

#endif
