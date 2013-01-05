///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZvector.hh
// integer and fixed point vector classes
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZfixed.hh"
#include "XZmath.hh"
///</include>

///<declare>
struct fvector;

typedef union{
    sint v vector;
    sint u[4];
} quad;
///</declare>

///<define>
struct lvector
{
	sint x;
	sint y;
	sint z;
	sint e;
	lvector(sint a=0,sint b=0,sint c=0,sint d=1) : x(a),y(b),z(c),e(d) { ; }
	lvector(const fvector& a);
	//lvector(lvector&& a) { x=a.x; y=a.y; z=a.z; e=a.e; a.x={}; a.y={}; a.z={}; a.e={}; }
	inline lvector  operator-() const { return lvector(-x,-y,-z); }
	inline lvector  operator+(const lvector& a) const { return lvector(x+a.x,y+a.y,z+a.z); }
	inline lvector  operator-(const lvector& a) const { return lvector(x-a.x,y-a.y,z-a.z); }
	inline lvector  operator*(sint a) const { return lvector(x*a,y*a,z*a); }
	inline lvector& operator+=(const lvector& a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
	inline lvector& operator-=(const lvector& a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
	inline lvector& operator*=(sint a) { x*=a; y*=a; z*=a; return *this; }
	inline lvector& operator=(const lvector& a) { x=a.x; y=a.y; z=a.z; e=a.e; return *this; }
	inline lvector& operator=(const fvector& a);
	inline lvector& operator=(sint a) { x=a; y=a; z=a; return *this; }
	inline lvector  cross(const lvector& a) const { return lvector(y*a.z-z*a.y,z*a.x-x*a.z,x*a.y-y*a.x); }
	inline lvector  hadamard(const lvector& a) const { return lvector(x*a.x,y*a.y,z*a.z); }
	inline sint     dot(const lvector& a) const { return x*a.x+y*a.y+z*a.z; }
	inline sint     length() const { return math::sqr(x*x+y*y+z*z); }
	inline void     set(sint a,sint b,sint c,sint d=1) { x=a; y=b; z=c; e=d; }
};

struct fvector
{
	fixed x;
	fixed y;
	fixed z;
	fixed e;
	fvector(fixed a=0,fixed b=0,fixed c=0,fixed d=1) : x(a),y(b),z(c),e(d) { ; }
	fvector(const fvector& a) : x(a.x),y(a.y),z(a.z),e(a.e) { ; }
	fvector(const lvector& a) : x(fx::l2f(a.x)),y(fx::l2f(a.y)),z(fx::l2f(a.z)),e(fx::l2f(a.e)) { ; }
	//fvector(fvector&& a) { x=a.x; y=a.y; z=a.z; e=a.e; a.x={}; a.y={}; a.z={}; a.e={}; }
	inline fvector  operator-() const { return fvector(-x,-y,-z); }
	inline fvector  operator+(const fvector& a) const { return fvector(x+a.x,y+a.y,z+a.z); }
	inline fvector  operator-(const fvector& a) const { return fvector(x-a.x,y-a.y,z-a.z); }
	inline fvector  operator*(fixed a) const { return fvector(fx::mul(x,a),fx::mul(y,a),fx::mul(z,a)); }
	inline fvector& operator+=(const fvector& a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
	inline fvector& operator-=(const fvector& a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
	inline fvector& operator*=(fixed a) { x=fx::mul(x,a); y=fx::mul(y,a); z=fx::mul(z,a); return *this; }
	inline fvector& operator=(const fvector& a) { x=a.x; y=a.y; z=a.z; e=a.e; return *this; }
	inline fvector& operator=(const lvector& a) { x=fx::l2f(a.x); y=fx::l2f(a.y); z=fx::l2f(a.z); return *this; }
	inline fvector& operator=(fixed a) { x=a; y=a; z=a; return *this; }
	inline fvector  cross(const fvector& a) const { return fvector(fx::mul(y,a.z)-fx::mul(z,a.y),fx::mul(z,a.x)-fx::mul(x,a.z),fx::mul(x,a.y)-fx::mul(y,a.x)); } 
	inline fvector  hadamard(const fvector& a) const { return fvector(fx::mul(x,a.x),fx::mul(y,a.y),fx::mul(z,a.z)); }
	inline fixed    dot(const fvector& a) const { return fx::mul(x,a.x)+fx::mul(y,a.y)+fx::mul(z,a.z); }
	inline fixed    length() const { return fx::sqr(fx::mul(x,x)+fx::mul(y,y)+fx::mul(z,z)); }
	inline void     set(fixed a,fixed b,fixed c,fixed d=1) { x=a; y=b; z=c; e=d; }
}; 
///</define>

///<code>
inline lvector& lvector::operator=(const fvector& a) { x=fx::r2l(a.x); y=fx::r2l(a.y); z=fx::r2l(a.z); e=fx::r2l(a.e); return *this; }
                lvector::lvector(const fvector& a) : x(fx::r2l(a.x)),y(fx::r2l(a.y)),z(fx::r2l(a.z)),e(fx::r2l(a.e)) { ; }
///</code>

