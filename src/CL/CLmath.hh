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
#define DEG2RAD M_PI/180
#define RAD2DEG 180/M_PI

class CLmath : public virtual CLcl, public CLsingle<CLmath>
{
	friend class CLsingle<CLmath>;
	
	private:
		float  fxpi;
		float  clpi;
		float* clsin;
		float* clcos;
		float* cltan;
		xlong* clarcsin;
		xlong* clarccos;
		xlong* clarctan; 
		CLmath();
		~CLmath();
	public:
		xlong sign(xlong x) const;
		xlong heaviside(xlong x) const;
		xlong delta(xlong x) const;
		template<typename T> T absolute(T x) const;
		template<typename T> T min(T a,T b) const;
		template<typename T> T max(T a,T b) const;
		template<typename T> T round(T x) const;
		template<typename T> T roundup(T x) const;
		template<typename T> T rounddown(T x) const;
		template<typename T> T sqrt(T x) const;
		template<typename T> T deg2rad(T d) const;
		template<typename T> T power(T b,xlong e) const;
		template<typename T> T factorial(T f) const;
		template<typename T> T doublefactorial(T f) const;
		float pi() const;
		float sin(xlong x) const;
		float cos(xlong x) const;
		float tan(xlong x) const;
		float arcsin(float x) const;
		float arccos(float x) const;
		float arctan(float x) const;
		float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k) const;
};

CLmath::CLmath()
{
	//precalucalte pi approximation
	fxpi = 355/113;
	//*
	
	//calc pi //very bad convergence!!!
	clpi = 1;
	float h = 3;
	float altsign = -1.0;
	for(uxlong i=0; i<10000; i++)
	{
		clpi += altsign / h;
		h += 2;
		altsign *= -1;
	}
	clpi *= 4;
	//*
	
	//calc arcsin/arccos/asrctan
	
	
	
	//*
	
	//calc sin/cos/tan arrays
	clsin = new float[360];
	clcos = new float[360];
	cltan = new float[360];
	float ii = 0;
	float k = 0;
	float l = 0;
	altsign = -1.0;
	for(uxlong i=0; i<360; i++)
	{
		ii = i * DEG2RAD;
		clsin[i] = ii;
		clcos[i] = 1;
		//cltan[i] = ...
		k = 2;
		l = 3;
		for(uxlong j=0; j<6; j++)
		{
			clsin[i] += altsign * power(ii,l) / factorial(l);
			clcos[i] += altsign * power(ii,k) / factorial(k);
			//cltan[i] = ...
			altsign *= -1.0;
			k+=2;
			l+=2;
		}
		altsign = -1.0;		
	}
	//*
	
	//~ //old sin/cos
	//~ sinarray = new float[360];
	//~ cosarray = new float[360];
	//~ 
	//~ for(xlong i=0; i<360; i++)
	//~ {
		//~ xsin[i] = std::sin(i*DEG2RAD);
		//~ xcos[i] = std::cos(i*DEG2RAD);
	//~ }
	//~ //*
}

CLmath::~CLmath() 
{
	delete[] clsin;
	delete[] clcos;
	delete[] cltan;	
}

xlong CLmath::sign(xlong x) const { return xlong(x!=0) | (xlong(x>=0)-1);  }


xlong CLmath::heaviside(xlong x) const { return xlong(x>0); }

template<>
xlong CLmath::absolute<xlong>(xlong x) const { return (xlong(x>0)-1 ^ x) + xlong(x<0); }

template<>
float CLmath::absolute<float>(float x) const
{
	__asm__ __volatile__ ("btrl $31,%%eax;" : "=a"(x) : "a"(x) );
	return x;
}

xlong CLmath::delta(xlong x) const
{
	__asm__ __volatile__ ("xorl %%ebx,%%ebx; orl $0,%%eax; setz %%bl ;" : "=b"(x) : "a"(x) );
	return x;
}

template<typename T>
T CLmath::min(T a,T b) const { return (a<b) ? a : b; }

template<typename T>
T CLmath::max(T a,T b) const { return (a>b) ? a : b; }

template<typename T>
T CLmath::round(T x) const { return ( (x-floor(x) ) > 0.5 ) ? ceil(x) : floor(x); }

template<typename T>
T CLmath::roundup(T x) const { return T(ceil(x)); }

template<typename T>
T CLmath::rounddown(T x) const { return T(floor(x)); }

template<typename T>
T CLmath::sqrt(T x) const
{
	if(x<=0) return 0;

	T num = x;
	T tmp = (x + 1) >> 1;

	for(int i=15;i>0;i--) { tmp = ((num/tmp) + tmp) >> 1; }

	return tmp;
}

template<>
float CLmath::sqrt<float>(float x) const
{
	if(x<=0) return 0;

	float num = x;
	float tmp = (x + 1)/2;

	for(int i=15;i>0;i--) { tmp = ((num/tmp) + tmp)/2; }

	return tmp;
}

template<typename T>
T CLmath::deg2rad(T d) const { return float(fxpi*d); }

template<typename T>
T CLmath::power(T b,xlong e) const
{
	T r = 1;
	for(xlong i=0;i<e;i++) { r *= b; }
	return r;
}

template<typename T>
T CLmath::factorial(T f) const
{
	T r = 1;
	for(xlong i=2;i<=f;i++) { r *= i; }
	return r;
}

template<typename T>
T CLmath::doublefactorial(T f) const
{
	T k = 0;
	T r = 0;
	T r1 = 1;
	T r2 = 1;
	T r3 = 1;
	
	k = f / 2;
	for(xlong i=0;i<k;i++) { r1 *= 2; }
	for(xlong i=2;i<=k;i++) { r2 *= i; }
	r = r1 * r2;
	
	if(f%2!=0)
	{
		k = f + 1;
		for(xlong i=2;i<=k;i++) { r3 *= i; }
		r = r3 / r;
	}

	return r;
}

float CLmath::pi() const { return fxpi; }

float CLmath::sin(xlong x) const
{
	if(x < 0) x -= 180;
	x = absolute(x);
	x %= 360;
	return clsin[x];
}

float CLmath::cos(xlong x) const
{
	x = absolute(x)%360;
	return clcos[x];
}

float CLmath::tan(xlong x) const
{

}

float CLmath::arcsin(float x) const
{ 
	return std::asin(x) * RAD2DEG; //!change to lookup table!
}

float CLmath::arccos(float x) const
{
	return std::acos(x) * RAD2DEG; //!change to lookup table!
}

float CLmath::arctan(float x) const
{

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

#endif

