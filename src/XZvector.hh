///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Vector Class + Library ( XZvector.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZfixed.hh"
///</include>

///<define>

union vector
{
	typedef fixed vec __attribute__ ((vector_size(16)));

	alignas(16) vec v;
	struct
	{
		fixed x;
		fixed y;
		fixed z;
		fixed e;
	};

	inline vector operator-() const { return vector{-v}; }

	inline vector operator+(const vector& w) const { return vector{v+w.v}; }
	inline vector operator-(const vector& w) const { return vector{v-w.v}; }

	inline vector& operator+=(const vector& w) { v += w.v; return *this; }
	inline vector& operator-=(const vector& w) { v -= w.v; return *this; }
	
	//vector operator>>(xint d) { return vector{x>>d,y>>d,z>>d,e>>d}; }
};

namespace fx
{
	pure vector l2f(const vector& v)
	{
		return vector{fx::l2f(v.x),fx::l2f(v.y),fx::l2f(v.z),fx::l2f(v.e)};
	}
	
	pure vector f2l(const vector& v)
	{
		return vector{fx::f2l(v.x),fx::f2l(v.y),fx::f2l(v.z),fx::f2l(v.e)};
	}
	
	pure vector mul(const vector& a,fixed x)
	{
		return vector{fx::mul(a.x,x),fx::mul(a.y,x),fx::mul(a.z,x),a.e};
	}

	pure vector cross(const vector& a,const vector& b)
	{
		return vector{fx::mul(a.y,b.z)-fx::mul(a.z,b.y),
		              fx::mul(a.z,b.x)-fx::mul(a.x,b.z),
		              fx::mul(a.x,b.y)-fx::mul(a.y,b.x),0};
	}

	pure fixed dot(const vector& a,const vector& b)
	{
		return fx::mul(a.x,b.x)+fx::mul(a.y,b.y)+fx::mul(a.z,b.z)+fx::mul(a.e,b.e);
	}
	
	inline pure fixed len(const vector& a)
	{
		return fx::sqr(dot(a,a));
	}
	
	pure vector unormal(const vector& a,const vector& b)
	{
		vector r = fx::cross(a,b);
		r = fx::mul(r,FXCEN);
		const fixed l = fx::len(r);
		if(l!=0) r = fx::mul(r,fx::div(FXONE,l));
		return r;
	}
}
///</define>

