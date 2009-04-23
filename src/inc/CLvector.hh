//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
#pragma message "Compiling " __FILE__ " ! TODO: overload operators"


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
	vector& operator+=(vector &a);
	vector& operator-=(vector &a);
	vector& operator+(vector &a);
	vector& operator-(vector &a);
	xlong operator*(vector &a);
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
	fvector& operator+=(fvector &a);
	fvector& operator-=(fvector &a);
	fvector& operator+(fvector &a);
	fvector& operator-(fvector &a);
	float operator*(fvector &a);
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

#endif
