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
	/*OK*/ pure inline bool inside(sint x,sint y,sint a,sint b,sint c,sint d);
	            inline bool bounds(const fvector& x,const fvector& m);
	            inline bool collision(const fvector& x,const fvector& r,const fvector& a);
	                   sint angle(const fvector& x,const fvector& t,const fvector& m);
}
///*

///implementation
bool game::onscreen(sint x,sint y)
{
	return (x>0) && (y>0) && (x<XRES) && (y<YRES);
}

bool game::inside(sint x,sint y,sint a,sint b,sint c,sint d)
{
	return (x>math::min(a,c)) && (y>math::min(b,d)) && (x<math::max(a,c)) && (y<math::max(b,d));
}

bool game::bounds(const fvector& x,const fvector& m)
{
	return 0;
}

bool game::collision(const fvector& x,const fvector& r,const fvector& a)
{
	const fvector t(a.x-x.x-r.x,a.y-x.y-r.y,0);
	//const fvector t(a-(x+r));
	return t.length()<r.e;
}

sint game::angle(const fvector& x,const fvector& t,const fvector& m)
{
	return 0;
}
///*

#endif

