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
#include "XZpolygon.hh"
///*

///definition
namespace game
{
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	/*OK*/ pure inline bool inside(sint x,sint y,sint a,sint b,sint c,sint d);
	                   bool collision(const fvector& x,fixed b,const fvector& a);
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

bool game::collision(const fvector& x,fixed b,const fvector& a)
{
	const fvector t = x-a;
	return fx::sqr(fx::mul(t.x,t.x)+fx::mul(t.y,t.y))<b;
}

sint game::angle(const fvector& x,const fvector& t,const fvector& m)
{
	return 0;
}

///*

#endif

