//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
//#pragma message "Compiling " __FILE__  "! TODO: "

#include  <iostream>
#include "CLtypes.hh"
#include "CLmath.hh"

//curios base class
template<typename T,class clvector>
struct CLvectorbase
{
	public:
		clvector  operator+(const clvector& a) const { static_cast<clvector*>(this)->operator+(a); }
		clvector  operator-(const clvector& a) const { static_cast<clvector*>(this)->operator-(a); }
		clvector  operator-()                        { static_cast<clvector*>(this)->operator-(); }
		       T  dot(const clvector& a) const { static_cast<clvector*>(this)->operator*(a); }
		clvector  operator*(T c) const               { static_cast<clvector*>(this)->operator*(c); }
		//does the friend need to be crtp'ed?
		clvector  cross(const clvector& a) const { static_cast<clvector*>(this)->operator^(a); }
		       T  operator!() const                  { static_cast<clvector*>(this)->operator!(); }
		clvector& operator+=(const clvector& a)      { static_cast<clvector*>(this)->operator+=(a); }
		clvector& operator-=(const clvector& a)      { static_cast<clvector*>(this)->operator-=(a); }
		clvector& operator*=(T c)                    { static_cast<clvector*>(this)->operator*=(c); }
		
		clvector& operator=(const clvector& a)       { static_cast<clvector*>(this)->operator=(a); }
		clvector& operator=(T c)                     { static_cast<clvector*>(this)->operator=(c); }
		//does the cast need to be crtp'ed
		       T  operator%(const clvector& a)       { static_cast<clvector*>(this)->operator%(a); }
	
		void print() const { static_cast<clvector*>(this)->print(); }
};
//*

//inline all methods and friends!
template<typename T>
struct CLvector : public CLvectorbase<T,CLvector<T> >
{
	T x;
	T y;
	T z;
	T e;

	CLvector() { x=y=z=0; }
	CLvector(T tx,T ty,T tz) : x(tx) , y(ty) , z(tz) { ; } 
	~CLvector() { }

	CLvector operator+(const CLvector& a) const;	//vector addition
	CLvector operator-(const CLvector& a) const;	//vector subtraction
	CLvector operator-();							//vector additive negation
	       T dot(const CLvector& a) const;	//dot product
	CLvector operator*(T c) const;					//scalar multiplication
	template<typename S> friend CLvector<S> operator*(S c,CLvector<S>& a);		//scalar multiplication friend
	CLvector cross(const CLvector& a) const;	//cross product
	       T operator!() const;						//vector length
	
	CLvector& operator+=(const CLvector& a);		//vector addition
	CLvector& operator-=(const CLvector& a);		//vector subtraction
	CLvector& operator*=(T c);						//scalar multiplication

	CLvector& operator=(const CLvector& a);			//vector vector assignment
	CLvector& operator=(T c);						//scalar vector assignment
		      operator CLvector<float>() const;		//cast to float
			  operator CLvector<xlong>() const;		//cast to xlong
			  //operator CLvector<xfixed>() const;	//cast to fixed
			  
			T operator%(const CLvector& a);			//angle between vectors

			void print() const;							//console output	
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
T CLvector<T>::dot(const CLvector<T>& a) const
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
CLvector<T> CLvector<T>::cross(const CLvector<T>& a) const
{
	return CLvector( ( (this->y * a.z) - (this->z * a.y) ),( (this->z * a.x) - (this->x * a.z) ),( (this->x * a.y) - (this->y * a.x) ) );
}
//*

//vector length:
template<typename T>
T CLvector<T>::operator!() const
{
	return CLmath::sqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) );
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
CLvector<T>& CLvector<T>::operator=(const CLvector<T>& a)
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

//vector assignment:
template<typename T>
CLvector<T>::operator CLvector<xlong>() const
{
	return CLvector<xlong>( xlong(this->x), xlong(this->y), xlong(this->z) );
}
//*

//angle between vectors:
template<typename T>
T CLvector<T>::operator%(const CLvector& a)
{
	float c =           ( (this->x * a.x)     + (this->y * a.y)     + (this->z * a.z) ) /
		(CLmath::sqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) ) * 
		 CLmath::sqrt(     (a.x * a.x)     +     (a.y * a.y)     +     (a.z * a.z) ) );

	return(CLmath::arccos(c));
}
//*

//console output
template<typename T>
void CLvector<T>::print() const
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

//return type overloading container 
template<typename T>
struct _CLvector
{
	template<typename S> friend _CLvector<S> operator*(const CLvector<S>& l, const CLvector<S>& r);
	
	private:
		const CLvector<T>& l;
		const CLvector<T>& r;
		
		_CLvector(const CLvector<T>& L, const CLvector<T>& R) : l(L), r(R) { ; } //inline
	
	public:
		operator T() const { return l.dot(r); }
		operator CLvector<T>() const { return l.cross(r); }

};

//inline
template<typename T>
_CLvector<T> operator*(const CLvector<T>& l, const CLvector<T>& r)
{
    return _CLvector<T>(l,r);
}
//*

//typedefs:
typedef CLvector<xlong> CLlvector;
typedef CLvector<float> CLfvector;
//typedef CLvector<xfixed> CLxvector;

#endif

