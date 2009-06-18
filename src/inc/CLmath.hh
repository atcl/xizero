//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLMATH
#define HH_CLMATH
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <math.h>

#include "CLtypes.hh"

namespace CLmath
{

//globals:
float  fxpi;
float* sinarray;
float* cosarray;
//*

//prototypes:
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
float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k);
//*

//implementation:
void init()
{
	fxpi = 355/113;

	float degtorad = M_PI/180;

	sinarray = new float[360];
	cosarray = new float[360];

	for(xlong i=0; i<360; i++)
	{
		sinarray[i] = sin(i*degtorad);
		cosarray[i] = cos(i*degtorad);
	}
}

template<typename T>
T sign(T x)
{
	return (x==0) ? 0 : (x<0 ? -1 : 1);
}

template<typename T>
T heaviside(T x)
{
	return (x<=0) ? 0 : 1;
}

template<typename T>
T absolute(T x)
{
	return (x<0) ? -x : x;
}

template<typename T>
T min(T a,T b)
{
	return (a<b) ? a : b;
}

template<typename T>
T max(T a,T b)
{
	return (a>b) ? a : b;
}

template<typename T>
T round(T x)
{
	return ( (x-floor(x) ) > 0.5 ) ? ceil(x) : floor(x);
}

template<typename T>
T roundup(T x)
{
	return xlong(ceil(x));
}

template<typename T>
T rounddown(T x)
{
	return xlong(floor(x));
}

template<typename T>
T intsqrt(T x)
{
	if(x<=0) return 0;

	T num = x;
	T tmp = (x + 1)>>1;

	for(int i=15;i>0;i--)
	{
		tmp = ((num/tmp) + tmp)>>1;
	}

	return tmp;
}

template<typename T>
T deg2rad(T d)
{
	return float(fxpi*d);
}

xlong faculty(xlong f)
{
	xlong r = 1;

	for(int i=2;i<=f;i++)
	{
		r *= i;
	}

	return r;
}

xlong power(xlong b,xlong e)
{
	xlong r = 1;

	for(int i=1;i<=e;i++)
	{
		r *= b;
	}

	return r;
}

float pi()
{
	return fxpi;
}

float sin(xlong x)
{
	if(x < 0) x -= 180;
	xlong d = absolute(x)%360;
	return sinarray[d];
}

float cos(xlong x)
{
	xlong d = absolute(x)%360;
	return cosarray[d];
}

float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k)
{
	float tk = t0;
	float xk = x0;

	for(int i=1;i<=k;i++)
	{
		xk = xk + (h * f(tk,xk) );
	}

	return xk;
}

//*

} //namespace end

#endif

