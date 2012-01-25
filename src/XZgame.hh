// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZgame.hh
// Game Library 

///guard
#ifndef HH_XZGAME
#define HH_XZGAME
///*

///includes
#include "XZmath.hh"
#include "XZvector.hh"
///*

///definition
namespace game
{
	/*OK*/ INLINE bool onscreen(long x,long y);
	/*OK*/ INLINE bool collision(long x,long y,long a,long b,long c,long d);
	/*OK*/ INLINE bool collision(long x,long y,long z,long w,long a,long b,long c,long d); 
	bool collision(const fvector& x,const box& y,const fvector& a,bool r);
	//bool collision(const fvector& x,const box& y,const fvector& a,const box& b,bool r);
	long angle(const fvector& y,const box& x);
}
///*

///implementation
bool game::onscreen(long x,long y)
{
	return (x>0)&&(y>0)&&(x<XRES)&&(y<YRES);
}

bool game::collision(long x,long y,long a,long b,long c,long d)
{
	return x>math::min(a,c)&&y>math::min(b,d)&&x<math::max(a,c)&&y<math::max(b,d);
}

bool game::collision(long x,long y,long z,long w,long a,long b,long c,long d)
{
	return ( (x>math::min(a,c))&&(y>math::min(b,d))&&(x<math::max(a,c))&&(y<math::max(b,d)) )||( (z>math::min(a,c))&&(w>math::min(b,d))&&(z<math::max(a,c))&&(w<math::max(b,d)) ); 
}

bool game::collision(const fvector& x,const box& y,const fvector& a,bool r)
{
	//add position to box
	const fvector m( (x.x+y[0].x), (x.y-y[0].y), (x.z+y[0].z) );
	const fvector n( (x.x+y[1].x), (x.y-y[0].y), (x.z+y[0].z) );
	//fvector o( (x.x+y.v[2].x), (x.y-y.v[2].y), (x.z+y.v[2].z) ); //not needed
	const fvector p( (x.x+y[1].x), (x.y-y[1].y), (x.z+y[0].z) );
	//*

	//if requested calcualte reusable intersections
	static fixed inter[6][2];
	static fixed coeff[6];
	if(r)
	{
		const fvector slope0 = n-m;
		const fvector slope1 = p-m;

		coeff[0] = fx::div(slope0.x,slope0.z-slope0.y);
		coeff[1] = fx::div(slope1.x,slope1.z-slope1.y);
		coeff[2] = fx::div(slope0.y,slope0.z-slope0.x);
		coeff[3] = fx::div(slope1.y,slope1.z-slope1.x);
		coeff[4] = fx::div(slope0.z,slope0.y-slope0.x);
		coeff[5] = fx::div(slope0.z,slope1.y-slope1.x);

		inter[0][0] = m.x + fx::mul(m.y - m.z,coeff[0]);
		inter[0][1] = p.x + fx::mul(p.y - p.z,coeff[0]);
		inter[1][0] = m.x + fx::mul(m.y - m.z,coeff[1]);
		inter[1][1] = n.x + fx::mul(n.y - n.z,coeff[1]);
		inter[2][0] = m.y + fx::mul(m.x - m.z,coeff[2]);
		inter[2][1] = p.y + fx::mul(p.x - p.z,coeff[2]);
		inter[3][0] = m.y + fx::mul(m.x - m.z,coeff[3]);
		inter[3][1] = n.y + fx::mul(n.x - n.z,coeff[3]);
		inter[4][0] = m.z + fx::mul(m.x - m.y,coeff[4]);
		inter[4][1] = p.z + fx::mul(p.x - p.y,coeff[4]);
		inter[5][0] = m.z + fx::mul(m.x - m.y,coeff[5]);
		inter[5][1] = n.z + fx::mul(n.x - n.y,coeff[5]);
	}
	//*

	//calculate intersections of a
	const fixed pre0 = a.x + a.y - a.z;
	const fixed loc0 = fx::mul(pre0,coeff[0]);
	const fixed loc1 = fx::mul(pre0,coeff[1]);
	const fixed pre1 = a.y + a.x - a.z;
	const fixed loc2 = fx::mul(pre1,coeff[2]);
	const fixed loc3 = fx::mul(pre1,coeff[3]);
	const fixed pre2 = a.z + a.x - a.y;
	const fixed loc4 = fx::mul(pre2,coeff[4]);
	const fixed loc5 = fx::mul(pre2,coeff[5]);
	//*

	//check overlap
	//! compares against loc0 correct?
	long v = -4;
	v += loc0 > math::min(inter[0][0],inter[0][1]) && loc0 < math::max(inter[0][0],inter[0][1]);
	v += loc1 > math::min(inter[1][0],inter[1][1]) && loc0 < math::max(inter[1][0],inter[1][1]);
	v += loc2 > math::min(inter[2][0],inter[2][1]) && loc0 < math::max(inter[2][0],inter[2][1]);
	v += loc3 > math::min(inter[3][0],inter[3][1]) && loc0 < math::max(inter[3][0],inter[3][1]);
	v += loc4 > math::min(inter[4][0],inter[4][1]) && loc0 < math::max(inter[4][0],inter[4][1]);
	v += loc5 > math::min(inter[5][0],inter[5][1]) && loc0 < math::max(inter[5][0],inter[5][1]);
	//*

	return v;
}

/*bool game::collision(const fvector& x,const box& y,const fvector& a,const box& b,bool r)
{
	//add positions to boxes
	fvector m( (x.x + y.v[0].x), (x.y - y.v[0].y), (x.z + y.v[0].z) );
	fvector n( (x.x + y.v[1].x), (x.y - y.v[1].y), (x.z + y.v[1].z) );
	//fvector o( (x.x + y.v[2].x), (x.y - y.v[2].y), (x.z + y.v[2].z) );
	fvector p( (x.x + y.v[3].x), (x.y - y.v[3].y), (x.z + y.v[3].z) );

	fvector q( (a.x + b.v[0].x), (a.y - b.v[0].y), (a.z + b.v[0].z) );
	//fvector r( (a.x + b.v[1].x), (a.y - b.v[1].y), (a.z + b.v[1].z) );
	fvector s( (a.x + b.v[2].x), (a.y - b.v[2].y), (a.z + b.v[2].z) );
	//fvector t( (a.x + b.v[3].x), (a.y - b.v[3].y), (a.z + b.v[3].z) );
	//*

	//if requested calcualte reusable intersections 
	static fixed inter[6][2];
	static fvector slope[2];
	if(r)
	{
		slope[0] = n-m;
		slope[1] = p-m;

		inter[0][0] = m.x + ( (m.y - m.z) / (slope[0].z-slope[0].y) ) * slope[0].x;
		inter[0][1] = p.x + ( (p.y - p.z) / (slope[0].z-slope[0].y) ) * slope[0].x;
		inter[1][0] = m.x + ( (m.y - m.z) / (slope[1].z-slope[1].y) ) * slope[1].x;
		inter[1][1] = n.x + ( (n.y - n.z) / (slope[1].z-slope[1].y) ) * slope[1].x;
		inter[2][0] = m.y + ( (m.x - m.z) / (slope[0].z-slope[0].x) ) * slope[0].y;
		inter[2][1] = p.y + ( (p.x - p.z) / (slope[0].z-slope[0].x) ) * slope[0].y;
		inter[3][0] = m.y + ( (m.x - m.z) / (slope[1].z-slope[1].x) ) * slope[1].y;
		inter[3][1] = n.y + ( (n.x - n.z) / (slope[1].z-slope[1].x) ) * slope[1].y;
		inter[4][0] = m.z + ( (m.x - m.y) / (slope[0].y-slope[0].x) ) * slope[0].z;
		inter[4][1] = p.z + ( (p.x - p.y) / (slope[0].y-slope[0].x) ) * slope[0].z;
		inter[5][0] = m.z + ( (m.x - m.y) / (slope[1].y-slope[1].x) ) * slope[1].z;
		inter[5][1] = n.z + ( (n.x - n.y) / (slope[1].y-slope[1].x) ) * slope[1].z;
	}
	//*

	//calc intersections of b at a
	fixed locint[6][2];
	locint[0][0] = q.x + ( (q.y - q.z) / (slope[0].z-slope[0].y) ) * slope[0].x;
	locint[0][1] = s.x + ( (s.y - s.z) / (slope[0].z-slope[0].y) ) * slope[0].x;
	locint[1][0] = q.x + ( (q.y - q.z) / (slope[1].z-slope[1].y) ) * slope[1].x;
	locint[1][1] = s.x + ( (s.y - s.z) / (slope[1].z-slope[1].y) ) * slope[1].x;
	locint[2][0] = q.y + ( (q.x - q.z) / (slope[0].z-slope[0].x) ) * slope[0].y;
	locint[2][1] = s.y + ( (s.x - s.z) / (slope[0].z-slope[0].x) ) * slope[0].y;
	locint[3][0] = q.y + ( (q.x - q.z) / (slope[1].z-slope[1].x) ) * slope[1].y;
	locint[3][1] = s.y + ( (s.x - s.z) / (slope[1].z-slope[1].x) ) * slope[1].y;
	locint[4][0] = q.z + ( (q.x - q.y) / (slope[0].y-slope[0].x) ) * slope[0].z;
	locint[4][1] = s.z + ( (s.x - s.y) / (slope[0].y-slope[0].x) ) * slope[0].z;
	locint[5][0] = q.z + ( (q.x - q.y) / (slope[1].y-slope[1].x) ) * slope[1].z;
	locint[5][1] = s.z + ( (s.x - s.y) / (slope[1].y-slope[1].x) ) * slope[1].z;
	//*
		
	//check if intersection intervals overlap
	long v = -4;
	v += locint[0][0]>math::min(inter[0][0],inter[0][1]) && locint[0][0]<math::max(inter[0][0],inter[0][1]) && locint[0][1]>math::min(inter[0][0],inter[0][1]) && locint[0][1]<math::max(inter[0][0],inter[0][1]); 
	v += locint[1][0]>math::min(inter[1][0],inter[1][1]) && locint[0][0]<math::max(inter[1][0],inter[1][1]) && locint[1][1]>math::min(inter[1][0],inter[1][1]) && locint[0][1]<math::max(inter[1][0],inter[1][1]); 
	v += locint[2][0]>math::min(inter[2][0],inter[2][1]) && locint[0][0]<math::max(inter[2][0],inter[2][1]) && locint[2][1]>math::min(inter[2][0],inter[2][1]) && locint[0][1]<math::max(inter[2][0],inter[2][1]); 
	v += locint[3][0]>math::min(inter[3][0],inter[3][1]) && locint[0][0]<math::max(inter[3][0],inter[3][1]) && locint[3][1]>math::min(inter[3][0],inter[3][1]) && locint[0][1]<math::max(inter[3][0],inter[3][1]); 
	v += locint[4][0]>math::min(inter[4][0],inter[4][1]) && locint[0][0]<math::max(inter[4][0],inter[4][1]) && locint[4][1]>math::min(inter[4][0],inter[4][1]) && locint[0][1]<math::max(inter[4][0],inter[4][1]); 
	v += locint[5][0]>math::min(inter[5][0],inter[5][1]) && locint[0][0]<math::max(inter[5][0],inter[5][1]) && locint[5][1]>math::min(inter[5][0],inter[5][1]) && locint[0][1]<math::max(inter[5][0],inter[5][1]); 
	//*

	return v;
}*/

long game::angle(const fvector& y,const box& x)
{
	return 0;
}

///*

#endif

