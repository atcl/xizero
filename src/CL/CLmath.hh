///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMATH
#define HH_CLMATH
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLmath
 * 
 * description:	A math library with assembly support
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///declarations
//degree to radian and vice versa
#define PI 3.14159265358979
#define DEG2RAD PI/180
#define RAD2DEG 180/PI
#define SEED 22695477
///*

///definitions
class CLmath : public CLbase<CLmath,1>
{
	friend class CLbase<CLmath,1>;
	friend class CLglobal;
	
	private:
	
	protected:
		xlong seed;
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
		template<typename T> T sign(T x) const;
		template<typename T> T heaviside(T x) const;
		template<typename T> T delta(T x) const;
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
		xlong arcsin(float x) const;
		xlong arccos(float x) const;
		xlong arctan(float x) const;
		float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k) const;
		xlong random(uxlong range);
};
///*

///implementation
CLmath::CLmath() //! noncritical
{
	seed = SEED;
	
	//precalucalte pi approximation
	fxpi = 355.0/113.0;
	//*
	
	//calc pi (arctan method) //very bad convergence!!!
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
	
	//calc arcsin/arccos
	clarcsin = new xlong[200];
	clarccos = new xlong[200];
	float ii = 0;
	float k = 0;
	float l = PI / 2;
	float m = 0;
	for(uxlong i=0; i<200; i++)
	{
		ii = (i-100) / 100;
		k = 2;
		m = ii;
		for(uxlong j=0; j<6; j++)
		{
			m += doublefactorial(k-1) / doublefactorial(k) * power(ii,k+1) / (k+1);
			k+=2;
		}
		clarcsin[i] = xlong(m);
		clarccos[i] = xlong(l - m);
	}	
	//*
	
	//calc sin/cos/tan arrays
	clsin = new float[360];
	clcos = new float[360];
	cltan = new float[360];
	for(uxlong i=0; i<360; i++)
	{
		altsign = -1.0;
		ii = i * DEG2RAD;
		clsin[i] = ii;
		clcos[i] = 1;
		k = 2;
		l = 3;
		for(uxlong j=0; j<6; j++)
		{
			clcos[i] += altsign * power(ii,k) / factorial(k);
			clsin[i] += altsign * power(ii,l) / factorial(l);
			altsign *= -1.0;
			k+=2;
			l+=2;
		}
		cltan[i] = clsin[i] / clcos[i];
	}
	//*
}

CLmath::~CLmath() //! noncritical
{
	//~ delete[] clsin;
	//~ delete[] clcos;
	//~ delete[] cltan;
	//~ delete[] clarcsin;
	//~ delete[] clarccos;
}

template<typename T>
T CLmath::sign(T x) const { return T(x>0) - T(x<0);  } //! critical

template<typename T>
T CLmath::heaviside(T x) const { return T(x>0); } //! noncritical

template<>
xlong CLmath::absolute<xlong>(xlong x) const { return ((xlong(x>0)-1) ^ x) + xlong(x<0); } //! critical

template<>
float CLmath::absolute<float>(float x) const //! critical
{
	xlong c = (*(xlong*) &x) & 0x7FFFFFFF;
	return *(float*)&c;
}

template<typename T>
T CLmath::delta(T x) const { return (x==0); } //! critical

template<typename T>
T CLmath::min(T a,T b) const { return ((a<b) ? a : b); } //! critical

template<typename T>
T CLmath::max(T a,T b) const { return ((a>b) ? a : b); } //! critical

template<>
xlong CLmath::min(xlong a,xlong b) const { return (a + ( ((b-a)>>31) & (b-a) )); } //! critical

template<>
xlong CLmath::max(xlong a,xlong b) const { return (a - ( ((a-b)>>31) & (a-b) )); } //! critical

template<typename T>
T CLmath::round(T x) const { return ( (x - T(xlong(x))) > 0.5) ? T(xlong(x)+xlong(x>0)) : T(xlong(x)-xlong(x<0)); } //! noncritical

template<typename T>
T CLmath::roundup(T x) const { return T(xlong(x)+xlong(x>0 && (x - T(xlong(x))!=0))-xlong(x<0 && (x - T(xlong(x))!=0))); } //! noncritical

template<typename T>
T CLmath::rounddown(T x) const { return T(xlong(x)); } //! noncritical

template<>
xlong CLmath::sqrt<xlong>(xlong x) const //! critical
{
	if(x<=0) { return 0; }
	xlong num = x;
	xlong tmp = (x + 1) >> 1;
	for(int i=15;i>0;i--) { tmp = ((num/tmp) + tmp) >> 1; }
	return tmp;
}

template<typename T>
T CLmath::sqrt(T x) const //! critical
{
	if(x<=0) { return 0; }
	T num = x;
	T tmp = (x + 1)/2;
	for(int i=15;i>0;i--) { tmp = ((num/tmp) + tmp)/2; }
	return tmp;
}

template<typename T>
T CLmath::deg2rad(T d) const { return float(fxpi*d); } //! noncritical

template<typename T>
T CLmath::power(T b,xlong e) const //! critical
{
	T r = 1;
	for(xlong i=0;i<e;i++) { r *= b; }
	return r;
}

template<typename T>
T CLmath::factorial(T f) const //! critical
{
	T r = 1;
	for(xlong i=2;i<=f;i++) { r *= i; }
	return r;
}

template<typename T>
T CLmath::doublefactorial(T f) const //! critical
{
	xlong ff = xlong(f);
	T k = 0;
	T r = 0;
	T r1 = 1;
	T r2 = 1;
	T r3 = 1;
	
	k = ff / 2;
	for(xlong i=0;i<k;i++) { r1 *= 2; }
	for(xlong i=2;i<=k;i++) { r2 *= i; }
	r = r1 * r2;
	
	if(ff%2!=0)
	{
		k = ff + 1;
		for(xlong i=2;i<=k;i++) { r3 *= i; }
		r = r3 / r;
	}

	return r;
}

float CLmath::pi() const { return fxpi; }

float CLmath::sin(xlong x) const //! noncritical
{
	if(x < 0) { x -= 180; }
	x = absolute(x) % 360;
	return clsin[x];
}

float CLmath::cos(xlong x) const //! noncritical
{
	x = absolute(x) % 360;
	return clcos[x];
}

float CLmath::tan(xlong x) const //! noncritical
{
	if(x < 0) { x -= 180; }
	x = absolute(x) % 360;
	return cltan[x];
}

xlong CLmath::arcsin(float x) const //! noncritical
{
	if(x<-1 || x>1) { x = sign(x); }
	return clarcsin[xlong(x*100.0)];
}

xlong CLmath::arccos(float x) const //! noncritical
{
	if(x<-1 || x>1) { x = sign(x); }
	return clarccos[xlong(100.0*x)];
}

xlong CLmath::arctan(float x) const //! critical
{
	float r = x;
	float l = 3;
	float altsign = -1.0;
	for(uxlong j=0; j<6; j++)
	{
		r += altsign * power(x,l) / factorial(l);
		altsign *= -1.0;
		l+=2;
	}
	return xlong(r);
}

float CLmath::odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k) const //! noncritical
{
	float tk = t0;
	float xk = x0;
	for(int i=1;i<=k;i++) { xk = xk + (h * f(tk,xk) ); }
	return xk;
}

xlong CLmath::random(uxlong range) //! noncritical
{
	//xor-shift random number generator
	seed++;
	seed ^= (seed<<15);
	seed ^= (seed>>21);
	seed ^= (seed<<4);
	return seed % range;
	//*
}
///*

#endif

