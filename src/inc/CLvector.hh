//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
#warning "Compiling " __FILE__ " ! TODO: overload operators, template vector"

#include "CLtypes.hh"


struct vector
{
	xlong x;
	xlong y;
	xlong z;
	xlong l;

	//vector operator=(vector &a);
	//vector();
	//vector(xlong x,xlong y,xlong z);
	//vector(xlong x,xlong y,xlong z,xlong l);
	//~vector();
	xlong length();
	void unit();
	vector& operator+=(vector &a);
	vector& operator-=(vector &a);
	vector& operator+(vector &a);
	vector& operator-(vector &a);
	xlong operator*(vector &a);	//dot-product
	vector& operator*=(xlong &s);	//scalar multiplication
	xlong operator^(vector &a);	//cross-product
};

struct fvector
{
	float x;
	float y;
	float z;
	float l;

	//fvector operator=(fvector &a);
	//fvector();
	//fvector(float x,float y,float z);
	//fvector(float x,float y,float z,float l);
	//fvector(xlong x,xlong y,xlong z);
	//fvector(xlong x,xlong y,xlong z,xlong l);
	//~vector();
	xlong length();
	void unit();
	fvector& operator+=(fvector &a);
	fvector& operator-=(fvector &a);
	fvector& operator+(fvector &a);
	fvector& operator-(fvector &a);
	float operator*(fvector &a);	//dot-product
	fvector& operator*=(float &s);	//scalar multiplication
	float operator^(fvector &a);	//cross-product
};

//vector:

// vector vector::operator=(vector &a)
// {
// 	x = a.x;
// 	y = a.y;
// 	z = a.z;
// 	l = a.l;
// }
// 
// vector::vector()
// {
// 
// }
// 
// vector::vector(xlong vx,xlong vy,xlong vz)
// {
// 	x = vx;
// 	y = vy;
// 	z = vz;
// 	//calc vector length l;
// }
// 
// vector::vector(xlong vx,xlong vy,xlong vz,xlong vl)
// {
// 	x = vx;
// 	y = vy;
// 	z = vz;
// 	l = vl;
// }
// 
// vector::~vector() { }

xlong vector::length()
{
	
}

void vector::unit()
{

}

vector& vector::operator+=(vector &a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

vector& vector::operator-=(vector &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

vector& vector::operator+(vector &a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

vector& vector::operator-(vector &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

xlong vector::operator*(vector &a)
{
	return ( (x*a.x) + (y*a.y) + (z*a.z) );
}

vector& vector::operator*=(xlong &s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

xlong vector::operator^(vector &a)
{

}

//fvector:

// fvector fvector::operator=(fvector &a)
// {
// 	x = a.x;
// 	y = a.y;
// 	z = a.z;
// 	l = a.l;
// }
// 
// fvector::fvector()
// {
// 
// }
// 
// fvector::fvector(float vx,float vy,float vz)
// {
// 	x = vx;
// 	y = vy;
// 	z = vz;
// 	//calc vector length l
// }
// 
// fvector::fvector(float vx,float vy,float vz,float vl)
// {
// 	x = vx;
// 	y = vy;
// 	z = vz;
// 	l = vl;
// }
// 
// fvector::~fvector() { }

xlong fvector::length()
{
	
}

void fvector::unit()
{

}

fvector& fvector::operator+=(fvector &a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

fvector& fvector::operator-=(fvector &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

fvector& fvector::operator+(fvector &a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

fvector& fvector::operator-(fvector &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

float fvector::operator*(fvector &a)
{
	return ( (x*a.x) + (y*a.y) + (z*a.z) );
}

fvector& fvector::operator*=(float &s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

float fvector::operator^(fvector &a)
{

}

#endif
