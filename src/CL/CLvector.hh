//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
#pragma message "Compiling " __FILE__  "! TODO: "

#include  <iostream>
#include "CLtypes.hh"
#include "CLmath.hh"


template<typename T>
struct CLvector
{
	T x;
	T y;
	T z;
	T e;

	CLvector() { x=y=z=0; }
	CLvector(T tx,T ty,T tz) { x=tx; y=ty; z=tz; }
	~CLvector() { }

	CLvector operator+(const CLvector& a) const;	//vector addition
	CLvector operator-(const CLvector& a) const;	//vector subtraction
	CLvector operator-();							//vector additive negation
	       T operator*(const CLvector& a) const;	//dot product
	CLvector operator*(T c) const;					//scalar multiplication
	template<typename S> friend CLvector<S> operator*(S c,CLvector<S>& a);		//scalar multiplication friend
	CLvector operator^(const CLvector& a) const;	//cross product
	       T operator!() const;						//vector length
	
	CLvector& operator+=(const CLvector& a);		//vector addition
	CLvector& operator-=(const CLvector& a);		//vector subtraction
	CLvector& operator*=(T c);						//scalar multiplication

	CLvector& operator=(const CLvector a);			//vector vector assignment //?argument as reference?
	CLvector& operator=(T c);						//scalar vector assignment
		      operator CLvector<float>() const;		//cast
			  
			T operator%(const CLvector& a);			//angle between vectors

	void print();									//console output	
};


//vector addition:
template<typename T>
CLvector<T> CLvector<T>::operator+(const CLvector<T>& a) const
{
	return CLvector<T>( (this->x + a.x),(this->y + a.y),(this->z + a.z) );
}
//*

//vector subtraction:
template<typename T>
CLvector<T> CLvector<T>::operator-(const CLvector<T>& a) const
{
	return CLvector<T>( (this->x - a.x),(this->y - a.y),(this->z - a.z) );
}
//*

//dot product:
template<typename T>
T CLvector<T>::operator*(const CLvector<T>& a) const
{
	return ( (this->x * a.x) + (this->y * a.y) + (this->z * a.z) );
}
//*

//scalar multiplication:
template<typename T>
CLvector<T> CLvector<T>::operator*(T c) const
{
	return ( (this->x * c) + (this->y * c) + (this->z * c) );
}
//*

//cross product:
template<typename T>
CLvector<T> CLvector<T>::operator^(const CLvector<T>& a) const
{
	return CLvector( ( (this->y * a.z) - (this->z * a.y) ),( (this->z * a.x) - (this->x * a.z) ),( (this->x * a.y) - (this->y * a.x) ) );
}
//*

//vector length:
template<typename T>
T CLvector<T>::operator!() const
{
	return CLmath::intsqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) );
}
//*

//vector addition:
template<typename T>
CLvector<T>& CLvector<T>::operator+=(const CLvector<T>& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}
//*

//vector subtraction:
template<typename T>
CLvector<T>& CLvector<T>::operator-=(const CLvector<T>& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
	return *this;
}
//*

//scalar multiplication:
template<typename T>
CLvector<T>& CLvector<T>::operator*=(T c)
{
	this->x *= c;
	this->y *= c;
	this->z *= c;
	return *this;
}
//*

//vector additive negation:
template<typename T>
CLvector<T> CLvector<T>::operator-()
{
	return CLvector<T>( -this->x,-this->y,-this->z );
}
//*

//vector vector assignment:
template<typename T>
CLvector<T>& CLvector<T>::operator=(const CLvector<T> a)
{
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
	return *this;
}
//*

//scalar vector assignment
template<typename T>
CLvector<T>& CLvector<T>::operator=(T c)
{
	this->x = c;
	this->y = c;
	this->z = c;
	return *this;
}
//*

//vector assignment:
template<typename T>
CLvector<T>::operator CLvector<float>() const
{
	return CLvector<float>( float(this->x), float(this->y), float(this->z) );
}
//*

//angle between vectors:
template<typename T>
T CLvector<T>::operator%(const CLvector& a)
{
	float c =           ( (this->x * a.x)     + (this->y * a.y)     + (this->z * a.z) ) /
		(CLmath::intsqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) ) * 
		 CLmath::intsqrt(     (a.x * a.x)     +     (a.y * a.y)     +     (a.z * a.z) ) );

	return(CLmath::arccos(c));
}
//*

//console output
template<typename T>
void CLvector<T>::print()
{
	std::cout << "( " << x << " , " << y << " , " << z << " )" << std::endl; 
}
//*

//scalar multiplication from left:
template<typename T>
CLvector<T> operator*(T c,CLvector<T>& a)
{
	return ( (a.x * c) + (a.y * c) + (a.z * c) );
}
//*

//typedefs:
typedef CLvector<xlong> CLlvector;
typedef CLvector<float> CLfvector;
//typedef CLvector<fixed> CLxvector;

#endif

