//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLMATH
#define HH_CLMATH
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include <math.h>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLvector.hh"


//+templates
class CLmath : public virtual CLcl
{
	private:
		float* sinarray;
		float* cosarray;
		float qpi;
		float qpi180;

	public:
		CLmath();
		~CLmath();

		template<typename T> T sign(T x);
		template<typename T> T absolute(T x);
		template<typename T> T min(T a,T b);
		template<typename T> T min(T a,T b,T c,T d);
		template<typename T> T max(T a,T b);
		template<typename T> T max(T a,T b,T c,T d);
		template<typename T> T intsqrt(T x);
		float sinbyarray(xlong x); //!template
		float cosbyarray(xlong x); //!template
		float vectorlength(fvector v);
		xlong vectorlength(vector v);
		float dotproduct(fvector a,fvector b);
		xlong dotproduct(vector a,vector b);
		fvector crossproduct(fvector a,fvector b);
		vector crossproduct(vector a,vector b);

		float pi();
		float deg2rad(xlong d);
		xlong faculty(xlong f);
		xlong power(xlong b,xlong e);
		float odeeuler(float(*f)(float,float),float x0,float t0,float h,xlong k);

		xlong round(float x);
		xlong roundup(float x);
		xlong rounddown(float x);
};

CLmath::CLmath()
{
	qpi = 355/113;
	qpi180 = qpi / 180;

	float degtorad = M_PI/180;

	sinarray = new float[360];
	cosarray = new float[360];

	for(xlong i=0; i<360; i++)
	{
		sinarray[i] = sin(i*degtorad);
		cosarray[i] = cos(i*degtorad);
	}

	
}

CLmath::~CLmath()
{
	delete[] sinarray;
	delete[] cosarray;
}

template<typename T> T CLmath::sign(T x)
{
	if(x == 0) return 0;
	else if(x < 0) return -1;
	else if(x > 0) return 1;
}

template<typename T> T CLmath::absolute(T x)
{
	if(x<0) x *= -1;

	return x;
}

template<typename T> T CLmath::min(T a,T b)
{
	if(a<=b) return a;
	else return b;
}

template<typename T> T CLmath::min(T a,T b,T c,T d)
{
	T m = a;
	if(b<m) m=b;
	if(c<m) m=c;
	if(d<m) m=d;
	return m;
}

template<typename T> T CLmath::max(T a,T b)
{
	if(a>=b) return a;
	else return b;
}

template<typename T> T CLmath::max(T a,T b,T c,T d)
{
	T m = a;
	if(b>m) m=b;
	if(c>m) m=c;
	if(d>m) m=d;
	return m;
}

float CLmath::sinbyarray(xlong x)
{
	if(x < 0) x -= 180;
	xlong d = absolute(x)%360;
	return sinarray[d];
}

float CLmath::cosbyarray(xlong x)
{
	xlong d = absolute(x)%360;
	return cosarray[d];
}

template<typename T> T CLmath::intsqrt(T x)
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

float CLmath::vectorlength(fvector v)
{
	float t = ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	float u = sqrt(t);
	return u;
}

xlong CLmath::vectorlength(vector v)
{
	xlong t = ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	xlong u = intsqrt(t);
	return u;
}

float CLmath::dotproduct(fvector a,fvector b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

xlong CLmath::dotproduct(vector a,vector b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

fvector CLmath::crossproduct(fvector a,fvector b)
{
	fvector f;
	f.x = ( (a.y * b.z) - (a.z * b.y) );
	f.y = ( (a.z * b.x) - (a.x * b.z) );
	f.z = ( (a.x * b.y) - (a.y * b.x) );
	f.l = vectorlength(f);
}

vector CLmath::crossproduct(vector a,vector b)
{
	vector f;
	f.x = ( (a.y * b.z) - (a.z * b.y) );
	f.y = ( (a.z * b.x) - (a.x * b.z) );
	f.z = ( (a.x * b.y) - (a.y * b.x) );
	f.l = vectorlength(f);
}

float CLmath::pi()
{
	return qpi;
}

float CLmath::deg2rad(xlong d)
{
	return float(qpi*d);
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

float CLmath::odeeuler( float(*f)(float,float),float x0,float t0,float h,xlong k)
{
	float tk = t0;
	float xk = x0;

	for(int i=1;i<=k;i++)
	{
		xk = xk + (h * f(tk,xk) );
	}

	return xk;
}

xlong CLmath::round(float x)
{
	if(x-floor(x)>=0.5) return ceil(x);
	else return floor(x);
}

xlong CLmath::roundup(float x)
{
	return xlong(ceil(x));
}

xlong CLmath::rounddown(float x)
{
	return xlong(floor(x));
}

#endif
