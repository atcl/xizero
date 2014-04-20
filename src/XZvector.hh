///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// integer and fixed point vector classes ( XZvector.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZfixed.hh"
#include "XZmath.hh"
///</include>

///<define>

/*
typedef fixed vec __attribute__ ((vector_size(16)));

typedef alignas(16) union vector
{
    vec   v;
    fixed x[4];

} vector;

namespace fx
{
	pure vector mul(const vector& a,fixed x)
	{
		return vector{fx::mul(a.x[0],x),fx::mul(a.x[1],x),fx::mul(a.x[2],x)};
	}

	pure vector cross(const vector& a,const vector& b)
	{
		return vector{fx::mul(a.x[1],b.x[2])-fx::mul(a.x[2],b.x[1]),
		              fx::mul(a.x[2],b.x[1])-fx::mul(a.x[0],b.x[2]),
		              fx::mul(a.x[0],b.x[1])-fx::mul(a.x[1],b.x[0])};
	}

	pure fixed dot(const vector& a,const vector& b)
	{
		return fx::mul(a.x[0],b.x[0])+fx::mul(a.x[1],b.x[1])+fx::mul(a.x[2],a.x[2]);
	}

	inline pure fixed len(const vector& a)
	{
		return fx::sqr(dot(a,a));
	}
}
*/


struct alignas(16) vector
{
	fixed x;
	fixed y;
	fixed z;
	fixed e;

	vector(fixed a=0,fixed b=0,fixed c=0,fixed d=1) : x(a),y(b),z(c),e(d) { ; }
	vector(const vector& a) : x(a.x),y(a.y),z(a.z),e(a.e) { ; }
	vector(const tuple& a)  : x(fx::l2f(a.x)),y(fx::l2f(a.y)),z(fx::l2f(a.z)),e(fx::l2f(a.e)) { ; }

	inline vector  operator-() const { return vector(-x,-y,-z); }

	inline vector  operator+(const vector& a) const { return vector(x+a.x,y+a.y,z+a.z); }
	inline vector  operator-(const vector& a) const { return vector(x-a.x,y-a.y,z-a.z); }
	inline vector  operator*(const vector& a) const { return vector(fx::mul(x,a.x),fx::mul(y,a.y),fx::mul(z,a.z)); }
	inline vector  operator*(fixed a) const { return vector(fx::mul(x,a),fx::mul(y,a),fx::mul(z,a)); }


	inline vector& operator+=(const vector& a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
	inline vector& operator-=(const vector& a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
	inline vector& operator*=(fixed a) { x=fx::mul(x,a); y=fx::mul(y,a); z=fx::mul(z,a); return *this; }

	inline vector& operator=(const vector& a) { x=a.x; y=a.y; z=a.z; e=a.e; return *this; }
	inline vector& operator=(fixed a) { x=a; y=a; z=a; return *this; }

	inline vector  cross(const vector& a) const { return vector(fx::mul(y,a.z)-fx::mul(z,a.y),fx::mul(z,a.x)-fx::mul(x,a.z),fx::mul(x,a.y)-fx::mul(y,a.x)); } 

	inline fixed   dot(const vector& a) const { return fx::mul(x,a.x)+fx::mul(y,a.y)+fx::mul(z,a.z); }
	inline fixed   len() const { return fx::sqr(fx::mul(x,x)+fx::mul(y,y)+fx::mul(z,z)); }
};


///</define>

