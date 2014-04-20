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
};

namespace fx
{
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
}
///</define>

