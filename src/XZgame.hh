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

///declarations
typedef fvector box[4];
///*

///definition
namespace game
{
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	/*OK*/ pure inline bool inside(sint x,sint y,sint a,sint b,sint c,sint d);
	              bool collision(const fvector& x,const box& y,const fvector& a,bool r=1);
	              sint angle(const fvector& x,const box& y,const fvector& m);
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

bool game::collision(const fvector& x,const box& y,const fvector& a,bool r)
{
	//if requested calculate reusable intersections
	static fixed coeff[4];
	static fixed inter[8];
	ifu(r)
	{
		//add position to box
		const fvector m( (x.x+y[0].x), (x.y-y[0].y), 0 );
		const fvector n( (x.x+y[1].x), (x.y-y[1].y), 0 );
		//const fvector o( (x.x+y[2].x), (x.y-y[2].y), 0 ); //not needed
		const fvector p( (x.x+y[3].x), (x.y-y[3].y), 0 );
		//*

		const fvector slope0 = n-m;
		const fvector slope1 = p-m;

		coeff[0] = math::set(fx::div(slope0.x,slope0.y+(slope0.y==0)),slope0.y!=0);
		coeff[1] = math::set(fx::div(slope1.x,slope1.y+(slope1.y==0)),slope1.y!=0);
		coeff[2] = math::set(fx::div(slope0.y,slope0.x+(slope0.x==0)),slope0.x!=0);
		coeff[3] = math::set(fx::div(slope1.y,slope1.x+(slope1.x==0)),slope1.x!=0);

		const fixed mxy0 = m.x - fx::mul(m.y,coeff[0]);
		const fixed pxy0 = p.x - fx::mul(p.y,coeff[0]);
		const fixed mxy1 = m.x - fx::mul(m.y,coeff[1]);
		const fixed nxy1 = n.x - fx::mul(n.y,coeff[1]);
		const fixed myx2 = m.y - fx::mul(m.x,coeff[2]);
		const fixed pyx2 = p.y - fx::mul(p.x,coeff[2]);
		const fixed myx3 = m.y - fx::mul(m.x,coeff[3]);
		const fixed nyx3 = n.y - fx::mul(n.x,coeff[3]);

		inter[0] = math::min(mxy0,pxy0);
		inter[1] = math::max(mxy0,pxy0);
		inter[2] = math::min(mxy1,nxy1);
		inter[3] = math::max(mxy1,nxy1);
		inter[4] = math::min(myx2,pyx2);
		inter[5] = math::max(myx2,pyx2);
		inter[6] = math::min(myx3,nyx3);
		inter[7] = math::max(myx3,nyx3);
	}
	//*

	//calculate intersections of a
	const fixed loc0 = a.x - fx::mul(a.y,coeff[0]);
	const fixed loc1 = a.x - fx::mul(a.y,coeff[1]);
	const fixed loc2 = a.y - fx::mul(a.x,coeff[2]);
	const fixed loc3 = a.y - fx::mul(a.x,coeff[3]);
	//*

	//check overlap
	return (loc0>=inter[0]) && (loc0<=inter[1]) && (loc1>=inter[2]) && (loc1<=inter[3]) && (loc2>=inter[4]) && (loc2<=inter[5]) && (loc3>=inter[6]) && (loc3<=inter[7]); 
	//*
}

sint game::angle(const fvector& x,const box& y,const fvector& m)
{
	return 0;
}

///*

#endif

