///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLVECTOR
#define HH_CLVECTOR
///*

///includes
#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLutils.hh"
///*

///header
/* class name:	CLvector
 * 
 * description:	A comprehensive vector class 
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
//curios base class
template<typename T,class clvector>
struct CLvectorbase
{
	public:
		inline clvector  operator+(const clvector& a) const { static_cast<clvector*>(this)->operator+(a); }
		inline clvector  operator-(const clvector& a) const { static_cast<clvector*>(this)->operator-(a); }
		inline clvector  operator-()                        { static_cast<clvector*>(this)->operator-(); }
		inline        T  operator/(const clvector& a) const { static_cast<clvector*>(this)->operator*(a); }
		inline clvector  operator*(T c) const               { static_cast<clvector*>(this)->operator*(c); }
		inline clvector  operator%(const clvector& a) const	{ static_cast<clvector*>(this)->operator^(a); }
		inline        T  operator!() const                  { static_cast<clvector*>(this)->operator!(); }
		inline clvector& operator+=(const clvector& a)      { static_cast<clvector*>(this)->operator+=(a); }
		inline clvector& operator-=(const clvector& a)      { static_cast<clvector*>(this)->operator-=(a); }
		inline clvector& operator*=(T c)                    { static_cast<clvector*>(this)->operator*=(c); }
		inline clvector& operator=(const clvector& a)       { static_cast<clvector*>(this)->operator=(a); }
		inline clvector& operator=(T c)                     { static_cast<clvector*>(this)->operator=(c); }
		inline        T  operator^(const clvector& a)       { static_cast<clvector*>(this)->operator%(a); }
	
					void print() const						{ static_cast<clvector*>(this)->print(); }
};
//*

template<typename T>
struct CLvector : public CLvectorbase<T,CLvector<T> >
{
	private:
		static CLmath& clmath;
	public:
		T x;
		T y;
		T z;
		T e; //for extra information

		CLvector() { x=y=z=e=0; }
		CLvector(T tx,T ty,T tz,T te=0) : x(tx) , y(ty) , z(tz), e(te) {  } 
		~CLvector() { }

		inline CLvector  operator+(const CLvector& a) const;	//vector addition
		inline CLvector  operator-(const CLvector& a) const;	//vector subtraction
		inline CLvector  operator-();							//vector negation
		inline CLvector  operator*(T c) const;					//scalar multiplication
		template<typename S> friend CLvector<S> operator*(S c,CLvector<S>& a);		//scalar multiplication friend
		inline CLvector  operator%(const CLvector& a) const;	//cross product
		inline        T  operator/(const CLvector& a) const;	//dot product
		inline        T  operator!() const;						//vector length		//change to unary +?
		
		inline CLvector& operator+=(const CLvector& a);			//vector addition
		inline CLvector& operator-=(const CLvector& a);			//vector subtraction
		inline CLvector& operator*=(T c);						//scalar multiplication

		inline CLvector& operator=(const CLvector& a);			//vector vector assignment
		inline CLvector& operator=(T c);						//scalar vector assignment
						 operator CLvector<float>() const;		//cast to float vector
						 operator CLvector<xlong>() const;		//cast to xlong vector
						 //operator CLvector<xfixed>() const;	//cast to fixed vector
				  
		inline		  T  operator^(const CLvector& a);			//angle between vectors

		void print() const;										//console output	
};

template<typename T>CLmath& CLvector<T>::clmath = CLmath::instance();
///*

///implementation
//vector addition:
template<typename T>
CLvector<T> CLvector<T>::operator+(const CLvector<T>& a) const //! noncritical
{
	return CLvector<T>( (this->x + a.x),(this->y + a.y),(this->z + a.z) );
}
//*

//vector subtraction:
template<typename T>
CLvector<T> CLvector<T>::operator-(const CLvector<T>& a) const //! noncritical
{
	return CLvector<T>( (this->x - a.x),(this->y - a.y),(this->z - a.z) );
}
//*

//dot product:
template<typename T>
T CLvector<T>::operator/(const CLvector<T>& a) const //! noncritical
{
	return ( (this->x * a.x) + (this->y * a.y) + (this->z * a.z) );
}
//*

//scalar multiplication:
template<typename T>
CLvector<T> CLvector<T>::operator*(T c) const //! noncritical
{
	return CLvector<T>( (this->x * c),(this->y * c),(this->z * c) );
}
//*

//cross product:
template<typename T>
CLvector<T> CLvector<T>::operator%(const CLvector<T>& a) const //! critical
{
	return CLvector<T>( ( (this->y * a.z) - (this->z * a.y) ),( (this->z * a.x) - (this->x * a.z) ),( (this->x * a.y) - (this->y * a.x) ) );
}
//*

//vector length:
template<typename T>
T CLvector<T>::operator!() const //! critical
{
	return clmath.sqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) );
}
//*

//vector addition:
template<typename T>
CLvector<T>& CLvector<T>::operator+=(const CLvector<T>& a) //! noncritical
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}
//*

//vector subtraction:
template<typename T>
CLvector<T>& CLvector<T>::operator-=(const CLvector<T>& a) //! noncritical
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
	return *this;
}
//*

//scalar multiplication:
template<typename T>
CLvector<T>& CLvector<T>::operator*=(T c) //! noncritical
{
	this->x *= c;
	this->y *= c;
	this->z *= c;
	return *this;
}
//*

//vector additive negation:
template<typename T>
CLvector<T> CLvector<T>::operator-() //! noncritical
{
	return CLvector<T>( -this->x,-this->y,-this->z );
}
//*

//vector vector assignment:
template<typename T>
CLvector<T>& CLvector<T>::operator=(const CLvector<T>& a) //! noncritical
{
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
	return *this;
}
//*

//scalar vector assignment
template<typename T>
CLvector<T>& CLvector<T>::operator=(T c) //! noncritical
{
	this->x = c;
	this->y = c;
	this->z = c;
	return *this;
}
//*

//vector cast CLvector<float>:
template<typename T>
CLvector<T>::operator CLvector<float>() const //! noncritical
{
	return CLvector<float>( float(this->x), float(this->y), float(this->z) );
}
//*

//vector cast CLvector<xlong>:
template<typename T>
CLvector<T>::operator CLvector<xlong>() const //! noncritical
{
	return CLvector<xlong>( xlong(this->x), xlong(this->y), xlong(this->z) );
}
//*

//~ //vector cast CLvector<xfixed>:
//~ template<typename T>
//~ CLvector<T>::operator CLvector<xfixed>() const
//~ {
	//~ return CLvector<xfixed>( xfixed(this->x), xfixed(this->y), xfixed(this->z) );
//~ }
//~ //*

//angle between vectors:
template<typename T>
T CLvector<T>::operator^(const CLvector& a) //! critical
{
	float c =        ( (this->x * a.x)    +  (this->y * a.y)    +  (this->z * a.z) ) /
		(clmath.sqrt( (this->x * this->x) + (this->y * this->y) + (this->z * this->z) ) * 
		 clmath.sqrt(     (a.x * a.x)     +     (a.y * a.y)     +     (a.z * a.z) ) );

	return(clmath.arccos(c));
}
//*

//console output
template<typename T>
void CLvector<T>::print() const //! noncritical
{
	tty(u8"( "); tty(x); tty(u8" , "); tty(y); tty(u8" , "); tty(z); say(u8" )");
}
//*

//friend:
//scalar multiplication from left:
template<typename T>
inline CLvector<T> operator*(T c,CLvector<T>& a) //! critical
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
		inline operator T() const { return l/r; }
		inline operator CLvector<T>() const { return l%r; }

};
//*

//container multiplication operator
template<typename T>
_CLvector<T> operator*(const CLvector<T>& l, const CLvector<T>& r) { return _CLvector<T>(l,r); } //! noncritical
//*
///*

///declarations
//typedefs:
typedef CLvector<xlong> CLlvector;
typedef CLvector<float> CLfvector;
//typedef CLvector<xfixed> CLxvector;
//*
///*

#endif

