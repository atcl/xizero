// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZgame.hh
// Game Library 

///guard
#ifndef HH_XZGAME
#define HH_XZGAME
//#pragma message "Compiling " __FILE__ "..." " TODO: 2D Collision Detection"
///*

///includes
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZvector.hh"
///*

///definition
namespace game
{
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	            inline bool bounds(const fvector& x,fixed r,fixed m);
	            inline bool collision(const fvector& x,const fvector& a,fixed r);
	                   sint angle(const fvector& x,const fvector& t,const fvector& m);
}
///*

///implementation
bool game::onscreen(sint x,sint y)
{
	return (x>0) && (y>0) && (x<XRES) && (y<YRES);
}

bool game::bounds(const fvector& x,fixed r,fixed m)
{
	return (x.x-r)>0&&(x.x+r)<XRES&&(x.y-r)>0&&(x.y+r)<m;
}

bool game::collision(const fvector& x,const fvector& a,fixed r)
{
	return (a.x>x.x-r)&&(a.x<x.x+r)&&(a.y>x.y-r)&&(a.y<x.y+r); 	//box works
	//const fvector t(a-x);						//component-wise?
	//return t.dot(t)<r*r;						//circle doesn't
}

sint game::angle(const fvector& x,const fvector& t,const fvector& m)
{
	return 0;
}
///*

#endif

