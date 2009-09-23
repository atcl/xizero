//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLMATH
#define HH_CLMATH
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <cmath>

#include "CLtypes.hh"
#include "CLsystem.hh"
#include "CLmacros.hh"
#include "CLsingle.hh"

/* class name:	CLmath
 * 
 * description:	A math library with assembly support
 * 
 * author:	atcl
 * 
 * notes:	implement own arccos,arcsin
 * 
 * version: 0.1
 */

//BIT flags
#define SIGNBIT "$0b10000000000000000000000000000000"

//degree to radian and vice versa
#define DEGTORAD M_PI/180
#define RADTODEG 180/M_PI

class CLmath : public virtual CLcl, public CLsingle<CLmath>
{
	friend class CLsingle<CLmath>;
	
	private:
		float  fxpi;
		float* sinarray;
		float* cosarray;
		xlong* arcsinarray;
		xlong* arccosarray;
		CLmath();
		~CLmath();
	public:
		template<typename T> T sign(T x) const;
		template<typename T> T heaviside(T x) const;
		template<typename T> T absolute(T x) const;
		template<typename T> T delta(T x) const;
		template<typename T> T min(T a,T b) const;
		template<typename T> T max(T a,T b) const;
		template<typename T> T round(T x) const;
		template<typename T> T roundup(T x) const;
		template<typename T> T rounddown(T x) const;
		template<typename T> T sqrt(T x) const;
		template<typename T> T deg2rad(T d) const;
		xlong faculty(xlong f) const;
		xlong power(xlong b,xlong e) const;
		float pi() const;
		float sin(xlong x) const;
		float cos(xlong x) const;
		float arcsin(float x) const;
		float arccos(float x) const;
		float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k) const;
};

CLmath::CLmath()
{
	//precalucalte pi approximation
	fxpi = 355/113;
	//*

	//fill look up tables for sine and cosine
	sinarray = new float[360];
	cosarray = new float[360];

	for(xlong i=0; i<360; i++)
	{
		sinarray[i] = std::sin(i*DEGTORAD);
		cosarray[i] = std::cos(i*DEGTORAD);
	}
	//*
	
	//fill look up tables for arcsine and arccosine
	//~ arcsinarray = new xlong[360];
	//~ arccosarray = new xlong[360];
	//~ 
	//~ for(xlong i=0; i<360; i++)
	//~ {
		//~ arcsinarray[i] = 0;
		//~ arccosarray[i] = 0;
	//~ }
	//*
}

CLmath::~CLmath() 
{
	delete[] sinarray;
	delete[] cosarray;	
}

template<typename T>
T CLmath::sign(T x) const
{
	//only xlong!
	__asm__ __volatile__ ("cdq; cmpl $0,%%eax; seta %%al; orb %%al,%%dl" : "=d"(x) : "a"(x) );
	return x; 
}

template<typename T>
T CLmath::heaviside(T x) const
{
	//only xlong!
	__asm__ __volatile__ ("cdq; incl %%edx;" : "=d"(x) : "a"(x) );
	return x;
}

template<typename T>
T CLmath::absolute(T x) const
{
	__asm__ __volatile__ ("cdq; xorl %%edx,%%eax; btl $31,%%ebx; adcl $0,%%eax;" : "=a"(x) : "a"(x),"b"(x) );
	return x;
}

template<>
float CLmath::absolute<float>(float x) const
{
	__asm__ __volatile__ ("btrl $31,%%eax;" : "=a"(x) : "a"(x) );
	return x;
}

template<typename T>
T CLmath::delta(T x) const
{
	//only xlong
	__asm__ __volatile__ ("xorl %%ebx,%%ebx; orl $0,%%eax; setz %%bl ;" : "=b"(x) : "a"(x) );
	return x;
}

template<typename T>
T CLmath::min(T a,T b) const
{
	return (a<b) ? a : b;
}

template<typename T>
T CLmath::max(T a,T b) const
{
	return (a>b) ? a : b;
}

template<typename T>
T CLmath::round(T x) const
{
	return ( (x-floor(x) ) > 0.5 ) ? ceil(x) : floor(x);
}

template<typename T>
T CLmath::roundup(T x) const
{
	return T(ceil(x));
}

template<typename T>
T CLmath::rounddown(T x) const
{
	return T(floor(x));
}

template<typename T>
T CLmath::sqrt(T x) const
{
	if(x<=0) return 0;

	T num = x;
	T tmp = (x + 1) >> 1;

	for(int i=15;i>0;i--)
	{
		tmp = ((num/tmp) + tmp) >> 1;
	}

	return tmp;
}

template<>
float CLmath::sqrt<float>(float x) const
{
	if(x<=0) return 0;

	float num = x;
	float tmp = (x + 1)/2;

	for(int i=15;i>0;i--)
	{
		tmp = ((num/tmp) + tmp)/2;
	}

	return tmp;
}

template<typename T>
T CLmath::deg2rad(T d) const
{
	return float(fxpi*d);
}

xlong CLmath::faculty(xlong f) const
{
	xlong r = 1;

	for(uxlong i=2;i<=f;i++)
	{
		r *= i;
	}

	return r;
}

xlong CLmath::power(xlong b,xlong e) const
{
	xlong r = 1;

	for(uxlong i=1;i<=e;i++)
	{
		r *= b;
	}

	return r;
}

float CLmath::pi() const
{
	return fxpi;
}

float CLmath::sin(xlong x) const
{
	if(x < 0) x -= 180;
	x = absolute(x);
	x %= 360;
	return sinarray[x];
}

float CLmath::cos(xlong x) const
{
	x = absolute(x)%360;
	return cosarray[x];
}

float CLmath::arcsin(float x) const
{ 
	return std::asin(x) * RADTODEG; //!change to lookup table!
}

float CLmath::arccos(float x) const
{
	return std::acos(x) * RADTODEG; //!change to lookup table!
}

float CLmath::odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k) const
{
	float tk = t0;
	float xk = x0;

	for(int i=1;i<=k;i++)
	{
		xk = xk + (h * f(tk,xk) );
	}

	return xk;
}

//temp
namespace { CLmath* clmath = CLmath::instance(); };
//*

#endif

