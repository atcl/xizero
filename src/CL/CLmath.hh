//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLMATH
#define HH_CLMATH
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <cmath>

#include "CLtypes.hh"
#include "CLapi.hh"

namespace CLmath
{
	float  fxpi;
	float* sinarray;
	float* cosarray;
	float* arcsinarray;
	float* arccosarray;

	void init();
	template<typename T> T sign(T x);
	template<typename T> T heaviside(T x);
	template<typename T> T absolute(T x);
	template<typename T> T min(T a,T b);
	template<typename T> T max(T a,T b);
	template<typename T> T round(T x);
	template<typename T> T roundup(T x);
	template<typename T> T rounddown(T x);
	template<typename T> T intsqrt(T x);
	template<typename T> T deg2rad(xlong d);
	xlong faculty(xlong f);
	xlong power(xlong b,xlong e);
	float pi();
	float sin(xlong x);
	float cos(xlong x);
	float arcsin(float x);
	float arccos(float x);
	float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k);
};


void CLmath::init()
{
	fxpi = 355/113;

	float degtorad = M_PI/180;

	sinarray = new float[360];
	cosarray = new float[360];
	arcsinarray = new float[360];
	arccosarray = new float[360];

	for(xlong i=0; i<360; i++)
	{
		sinarray[i] = std::sin(i*degtorad);
		cosarray[i] = std::cos(i*degtorad);
		arcsinarray[i] = 0;
		arccosarray[i] = 0;
	}
}

template<typename T>
T CLmath::sign(T x)
{
	return (x==0) ? 0 : (x<0 ? -1 : 1);
}

template<typename T>
T CLmath::heaviside(T x)
{
	return (x<=0) ? 0 : 1;
}

template<typename T>
T CLmath::absolute(T x)
{
	return (x<0) ? -x : x;
}

template<typename T>
T CLmath::min(T a,T b)
{
	return (a<b) ? a : b;
}

template<typename T>
T CLmath::max(T a,T b)
{
	return (a>b) ? a : b;
}

template<typename T>
T CLmath::round(T x)
{
	return ( (x-floor(x) ) > 0.5 ) ? ceil(x) : floor(x);
}

template<typename T>
T CLmath::roundup(T x)
{
	return xlong(ceil(x));
}

template<typename T>
T CLmath::rounddown(T x)
{
	return xlong(floor(x));
}

template<typename T>
T CLmath::intsqrt(T x)
{
//!replace div by 2 with >> (first create >> operators in fixed, and specialize for float)

	if(x<=0) return 0;

	T num = x;
	T tmp = (x + 1)/2;

	for(int i=15;i>0;i--)
	{
		tmp = ((num/tmp) + tmp)/2;
	}

	return tmp;
}

template<typename T>
T CLmath::deg2rad(T d)
{
	return float(fxpi*d);
}

xlong CLmath::faculty(xlong f)
{
	xlong r = 1;

	for(int i=2;i<=f;i++)
	{
		r *= i;
	}

	return r;
}

xlong CLmath::power(xlong b,xlong e)
{
	xlong r = 1;

	for(int i=1;i<=e;i++)
	{
		r *= b;
	}

	return r;
}

float CLmath::pi()
{
	return fxpi;
}

float CLmath::sin(xlong x)
{
	if(x < 0) x -= 180;
	x = absolute(x)%360;
	return sinarray[x];
}

float CLmath::cos(xlong x)
{
	x = absolute(x)%360;
	return cosarray[x];
}

float CLmath::arcsin(float x)
{ 
	return std::asin(x) * 180 / M_PI; //!change to lookup table!
}

float CLmath::arccos(float x)
{
	return std::acos(x) * 180 / M_PI; //!change to lookup table!
}

float CLmath::odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k)
{
	float tk = t0;
	float xk = x0;

	for(int i=1;i<=k;i++)
	{
		xk = xk + (h * f(tk,xk) );
	}

	return xk;
}

#endif
