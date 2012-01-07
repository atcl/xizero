// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZvector.hh
// fixed point vector class

///guard
#ifndef HH_XZVECTOR
#define HH_XZVECTOR
///*

///includes
#include "XZfixed.hh"
#include "XZmath.hh"
///*

///declarations
class fvector;
///*

///definitions
class lvector
{
	public:
		long x;
		long y;
		long z;
		long e;
		INLINE lvector(long a=0,long b=0,long c=0,long d=1) : x(a),y(b),z(c),e(d) { ; }
		INLINE ~lvector() { ; }
		INLINE lvector  operator+(const lvector& a) const { return lvector(x+a.x,y+a.y,z+a.z); }
		INLINE lvector  operator-(const lvector& a) const { return lvector(x-a.x,y-a.y,z-a.z); }
		INLINE lvector  operator*(long a) const { return lvector(x*a,y*a,z*a); }
		INLINE lvector  operator-() const { return lvector(-x,-y,-z); }
		INLINE lvector& operator+=(const lvector& a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
		INLINE lvector& operator-=(const lvector& a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
		INLINE lvector& operator*=(long a) { x*=a; y*=a; z*=a; return *this; }
		INLINE lvector& operator=(const lvector& a) { x=a.x; y=a.y; z=a.z; e=a.e; return *this; }
		INLINE lvector& operator=(const fvector& a);
		INLINE lvector& operator=(long a) { x=a; y=a; z=a; return *this; }
		INLINE lvector  cross(const lvector& a) const { return lvector(y*a.z-z*a.y,z*a.x-x*a.z,x*a.y-y*a.x); }
		INLINE lvector  hadamard(const lvector& a) const { return lvector(x*a.x,y*a.y,z*a.z); }
		INLINE long     dot(const lvector& a) const { return x*a.x+y*a.y+z*a.z; }
		INLINE long     length() const { return math::sqr(x*x+y*y+z*z); }
		INLINE void     set(long a,long b,long c,long d=1) { x=a; y=b; z=c; e=d; }
};

class fvector
{
	public:
		fixed x;
		fixed y;
		fixed z;
		fixed e;
		INLINE fvector(fixed a=0,fixed b=0,fixed c=0,fixed d=1) : x(a),y(b),z(c),e(d) { ; }
		INLINE ~fvector() { ; }
		INLINE fvector  operator+(const fvector& a) const { return fvector(x+a.x,y+a.y,z+a.z); }
		INLINE fvector  operator-(const fvector& a) const { return fvector(x-a.x,y-a.y,z-a.z); }
		INLINE fvector  operator*(fixed a) const { return fvector(fx::mul(x,a),fx::mul(y,a),fx::mul(z,a)); }
		INLINE fvector  operator-() const { return fvector(-x,-y,-z); }
		INLINE fvector& operator+=(const fvector& a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
		INLINE fvector& operator-=(const fvector& a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
		INLINE fvector& operator*=(fixed a) { x=fx::mul(x,a); y=fx::mul(y,a); z=fx::mul(z,a); return *this; }
		INLINE fvector& operator=(const fvector& a) { x=a.x; y=a.y; z=a.z; e=a.e; return *this; }
		INLINE fvector& operator=(const lvector& a) { x=fx::l2f(a.x); y=fx::l2f(a.y); z=fx::l2f(a.z); return *this; }
		INLINE fvector& operator=(fixed a) { x=a; y=a; z=a; return *this; }
		INLINE fvector  cross(const fvector& a) const { return fvector(fx::mul(y,a.z)-fx::mul(z,a.y),fx::mul(z,a.x)-fx::mul(x,a.z),fx::mul(x,a.y)-fx::mul(y,a.x)); } 
		INLINE fvector  hadamard(const fvector& a) const { return fvector(fx::mul(x,a.x),fx::mul(y,a.y),fx::mul(z,a.z)); }
		INLINE fixed    dot(const fvector& a) const { return fx::mul(x,a.x)+fx::mul(y,a.y)+fx::mul(z,a.z); }
		INLINE fixed    length() const { return fx::sqr(fx::mul(x,x)+fx::mul(y,y)+fx::mul(z,z)); }
		INLINE void     set(fixed a,fixed b,fixed c,fixed d=1) { x=a; y=b; z=c; e=d; }
}; 
///*

///Implementation
lvector& lvector::operator=(const fvector& a) { x=fx::r2l(a.x); y=fx::r2l(a.y); z=fx::r2l(a.z); e=fx::r2l(a.e); return *this; }
///*

#endif

