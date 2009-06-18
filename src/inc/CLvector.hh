//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
#pragma message "Compiling " __FILE__  "! TODO: "

#include  <iostream>
#include "CLtypes.hh"


//CLvector definition:
template<typename T>
struct CLvector
{
	T x;
	T y;
	T z;

	CLvector() { x=y=z=0; }
	CLvector(T tx,T ty,T tz) { x=tx; y=ty; z=tz; }
	~CLvector() { }

	CLvector operator+(CLvector& a);		//vector addition
	CLvector operator-(CLvector& a);		//vector subtraction
	       T operator*(CLvector& a);		//dot product
	CLvector operator*(T c);			//scalar multiplication
	CLvector operator^(CLvector& a);		//cross product
	       T operator!();				//vector length
	
	CLvector& operator+=(CLvector& a);		//vector addition
	CLvector& operator-=(CLvector& a);		//vector subtraction
	CLvector& operator*=(T c);			//scalar multiplication
	CLvector& operator^=(CLvector& a);		//cross product

	CLvector& operator=(CLvector& a);		//vector assignment
	CLvector& operator=(CLvector a);		//vector assignment

	void print();					//console output	
};
//*

//CLvector implementation:

//vector addition:
template<typename T>
CLvector<T> CLvector<T>::operator+(CLvector<T>& a)
{
	return CLvector( this->x + a.x,this->y + a.y,this->z + a.z );
}
//*

//vector subtraction:
template<typename T>
CLvector<T> CLvector<T>::operator-(CLvector<T>& a)
{
	return CLvector( this->x - a.x,this->y - a.y,this->z - a.z );
}
//*

//dot product:
template<typename T>
T CLvector<T>::operator*(CLvector<T>& a)
{
	return ( (this->x * a.x) + (this->y * a.y) + (this->z * a.z) );
}
//*

//scalar multiplication:
template<typename T>
CLvector<T> CLvector<T>::operator*(T c)
{
	return ( (this->x * c) + (this->y * c) + (this->z * c) );
}
//*

//cross product:
template<typename T>
CLvector<T> CLvector<T>::operator^(CLvector<T>& a)
{
	return CLvector( ( (this->y * a.z) - (this->z * a.y) ),( (this->z * a.x) - (this->x * a.z) ),( (this->x * a.y) - (this->y * a.x) ) );
}
//*

//vector length:
template<typename T>
T CLvector<T>::operator!()
{
	return ( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) ); //! add square root
}
//*

//vector addition:
template<typename T>
CLvector<T>& CLvector<T>::operator+=(CLvector<T>& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}
//*

//vector subtraction:
template<typename T>
CLvector<T>& CLvector<T>::operator-=(CLvector<T>& a)
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

//cross product:
template<typename T>
CLvector<T>& CLvector<T>::operator^=(CLvector<T>& a)
{
	this->x = ( (this->y * a.z) - (this->z * a.y) );
	this->y = ( (this->z * a.x) - (this->x * a.z) );
	this->z = ( (this->x * a.y) - (this->y * a.x) );
	return *this;
}
//*

//vector assignment:
template<typename T>
CLvector<T>& CLvector<T>::operator=(CLvector<T>& a)
{
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
}
//*

//vector assignment:
template<typename T>
CLvector<T>& CLvector<T>::operator=(CLvector<T> a)
{
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
}
//*

//console output
template<typename T>
void CLvector<T>::print()
{
	std::cout << "( " << x << " , " << y << " , " << z << " )" << std::endl; 
}

//*


#endif

