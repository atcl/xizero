// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
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

///declarations
typedef fvector box[2];
///*

///definition
namespace game
{
	/*OK*/ inline bool onscreen(sint x,sint y) CONST;
	/*OK*/ inline bool inside(sint x,sint y,sint a,sint b,sint c,sint d) CONST;
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
	//add position to box
	const fvector m( (x.x+y[0].x), (x.y-y[0].y), (x.z+y[0].z) );
	const fvector n( (x.x+y[1].x), (x.y-y[0].y), (x.z+y[0].z) );
	//const fvector o( (x.x+y[0].x), (x.y-y[1].y), (x.z+y[0].z) ); //not needed
	const fvector p( (x.x+y[1].x), (x.y-y[1].y), (x.z+y[0].z) );
	//*

	//if requested calcualte reusable intersections
	static fixed inter[4][2];
	static fixed coeff[4];
	if(r)
	{
		const fvector slope0 = n-m;
		const fvector slope1 = p-m;

		coeff[0] = fx::div(slope0.x,slope0.y); //crashes here
		coeff[1] = fx::div(slope1.x,slope1.y);
		coeff[2] = fx::div(slope0.y,slope0.x);
		coeff[3] = fx::div(slope1.y,slope1.x);

		inter[0][0] = m.x - fx::mul(m.y,coeff[0]);
		inter[0][1] = p.x - fx::mul(p.y,coeff[0]);
		inter[1][0] = m.x - fx::mul(m.y,coeff[1]);
		inter[1][1] = n.x - fx::mul(n.y,coeff[1]);
		inter[2][0] = m.y - fx::mul(m.x,coeff[2]);
		inter[2][1] = p.y - fx::mul(p.x,coeff[2]);
		inter[3][0] = m.y - fx::mul(m.x,coeff[3]);
		inter[3][1] = n.y - fx::mul(n.x,coeff[3]);
	}
	//*

	//calculate intersections of a
	const fixed loc0 = a.x - fx::mul(a.y,coeff[0]);
	const fixed loc1 = a.x - fx::mul(a.y,coeff[1]);
	const fixed loc2 = a.y - fx::mul(a.x,coeff[2]);
	const fixed loc3 = a.y - fx::mul(a.x,coeff[3]);
	//*

	//check overlap
	sint v = -4;
	v += (loc0 > math::min(inter[0][0],inter[0][1])) && (loc0 < math::max(inter[0][0],inter[0][1]));
	v += (loc1 > math::min(inter[1][0],inter[1][1])) && (loc1 < math::max(inter[1][0],inter[1][1]));
	v += (loc2 > math::min(inter[2][0],inter[2][1])) && (loc2 < math::max(inter[2][0],inter[2][1]));
	v += (loc3 > math::min(inter[3][0],inter[3][1])) && (loc3 < math::max(inter[3][0],inter[3][1]));
	//*

	return v;
}

sint game::angle(const fvector& x,const box& y,const fvector& m)
{
	return 0;
}

///*

#endif

