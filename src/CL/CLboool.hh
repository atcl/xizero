///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLboool
#define HH_CLboool
///*

///includes
#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLmacros.hh"
///*

///header
/* class name:	CLboool
 * 
 * description:	A tri-state bool type
 * 
 * author:	atcl
 * 
 * notes:	test.
 * 
 * version: 0.1
 */
///*

///definitions
class boool
{
	private:
		CLmath* clmath;
		xlong b;
		
	public:
		boool();
		boool(boool& a);
		boool(bool a);
		boool(xlong a);
		~boool() { };
	
		inline boool& operator=(boool& a);
		inline boool& operator=(bool a);
		inline boool& operator=(xlong a);
		
		inline boool operator-() const;
		inline boool operator!() const;
		
		inline boool& operator++();
		inline boool& operator--();
		
		inline boool& operator+=(boool& a);
		inline boool& operator-=(boool& a);
		inline boool& operator*=(boool& a);
		inline boool& operator+=(bool a);
		inline boool& operator-=(bool a);
		inline boool& operator*=(bool a);
		inline boool& operator+=(xlong a);
		inline boool& operator-=(xlong a);
		inline boool& operator*=(xlong a);
		
		inline boool operator+(boool& a) const;
		inline boool operator-(boool& a) const;
		inline boool operator*(boool& a) const;
		inline boool operator+(bool a)   const;
		inline boool operator-(bool a)   const;
		inline boool operator*(bool a)   const;
		inline boool operator+(xlong a)  const;
		inline boool operator-(xlong a)  const;
		inline boool operator*(xlong a)  const;
		
		friend boool operator+(bool a,boool& b);
		friend boool operator-(bool a,boool& b);
		friend boool operator*(bool a,boool& b);
		friend boool operator+(xlong a,boool& b);
		friend boool operator-(xlong a,boool& b);
		friend boool operator*(xlong a,boool& b);
		
		inline bool operator&&(boool& a) const;
		inline bool operator||(boool& a) const;
		inline bool operator^(boool& a)  const;
		
		inline bool operator==(boool& a) const;
		inline bool operator!=(boool& a) const;
		inline bool operator<=(boool& a) const;
		inline bool operator>=(boool& a) const;
		inline bool operator<(boool& a)  const;
		inline bool operator>(boool& a)  const;
		inline bool operator==(bool a)   const;
		inline bool operator!=(bool a)   const;
		inline bool operator<=(bool a)   const;
		inline bool operator>=(bool a)   const;
		inline bool operator<(bool a)    const;
		inline bool operator>(bool a)    const;
		
		friend bool operator==(bool a,boool& b);
		friend bool operator!=(bool a,boool& b);
		friend bool operator<=(bool a,boool& b);
		friend bool operator>=(bool a,boool& b);
		friend bool operator<(bool a,boool& b);
		friend bool operator>(bool a,boool& b);

		inline operator bool()  const;		
		inline operator xlong() const;
};
///*

///implementation

//unassigning constructor
boool::boool() { this->b=0; } //! noncritical
//*

//boool assigning constructor
boool::boool(boool& a) { this->b=a.b; } //! noncritical
//*

//bool assigning constructor
boool::boool(bool a) { this->b=a; } //! noncritical
//*

//xlong assigning construcotr
boool::boool(xlong a) { this->b=clmath->sign(a); } //! noncritical
//*

//assign boool
boool& boool::operator=(boool& a) //! noncritical
{
	this->b = a.b;
	return *this;
}
//*

//assign bool
boool& boool::operator=(bool a) //! noncritical
{
	this->b = a;
	return *this;
}
//*

//assign xlong
boool& boool::operator=(xlong a) //! noncritical
{
	this->b = clmath->sign(a);
	return *this;
}
//*

//negate
boool boool::operator-() const //! noncritical
{
	boool temp(-(this->b));
	return temp;
}
//*

//invert
boool boool::operator!() const //! noncritical
{
	boool temp(-(this->b));
	return temp;
}
//*
	
//increment
boool& boool::operator++() //! noncritical
{
	if(this->b<1) this->b++;
	return *this;
}
//*

//decrement
boool& boool::operator--() //! noncritical
{
	if(this->b>-1) this->b--;
	return *this;
}
//*

//assignment addition with boool
boool& boool::operator+=(boool& a) //! noncritical
{
	this->b = clmath->sign(this->b + a.b);
	return *this;
}
//*

//assignment subtraction with boool
boool& boool::operator-=(boool& a) //! noncritical
{
	this->b = clmath->sign(this->b - a.b);
	return *this;
}
//*

//assignment multiplication with boool
boool& boool::operator*=(boool& a) //! noncritical
{
	this->b = clmath->sign(this->b * a.b);
	return *this;
}

//assignment addition with bool
boool& boool::operator+=(bool a) //! noncritical
{
	this->b = clmath->sign(this->b + xlong(a));
	return *this;
}
//*

//assignment subtraction with bool
boool& boool::operator-=(bool a) //! noncritical
{
	this->b = clmath->sign(this->b - xlong(a));
	return *this;
}
//*

//assignment multiplication with bool
boool& boool::operator*=(bool a) //! noncritical
{
	this->b = clmath->sign(this->b * xlong(a));
	return *this;
}
//*

//assignment addition with xlong
boool& boool::operator+=(xlong a) //! noncritical
{
	this->b = clmath->sign(this->b + a);
	return *this;
}
//*

//assignment subtraction with xlong
boool& boool::operator-=(xlong a) //! noncritical
{
	this->b = clmath->sign(this->b - a);
	return *this;
}
//*

//assignment multiplication with xlong
boool& boool::operator*=(xlong a) //! noncritical
{
	this->b = clmath->sign(this->b * a);
	return *this;
}
//*

//add with boool
boool boool::operator+(boool& a) const //! noncritical
{
	boool temp(clmath->sign(this->b + a.b));
	return temp;
}
//*

//subtract with boool
boool boool::operator-(boool& a) const //! noncritical
{
	boool temp(clmath->sign(this->b - a.b));
	return temp;
}

//multiplicate with bool
boool boool::operator*(boool& a) const //! noncritical
{
	boool temp(clmath->sign(this->b * a.b));
	return temp;
}
//*

//add with bool
boool boool::operator+(bool a)  const //! noncritical
{
	boool temp(clmath->sign(this->b + xlong(a)));
	return temp;
}
//*

//subtract with bool
boool boool::operator-(bool a)  const //! noncritical
{
	boool temp(clmath->sign(this->b - xlong(a)));
	return temp;	
}
//*

//multiplicate with bool
boool boool::operator*(bool a)  const //! noncritical
{
	boool temp(clmath->sign(this->b * xlong(a)));
	return temp;	
}
//*

//add with xlong
boool boool::operator+(xlong a) const //! noncritical
{
	boool temp(clmath->sign(this->b + a));
	return temp;	
}
//*

//subtract with xlong
boool boool::operator-(xlong a) const //! noncritical
{
	boool temp(clmath->sign(this->b - a));
	return temp;	
}
//*

//multiplicate with xlong
boool boool::operator*(xlong a) const //! noncritical
{
	boool temp(clmath->sign(this->b + a));
	return temp;
}
//*
	
//logical and with boool	
bool boool::operator&&(boool& a) const { return (this->b && a); } //! noncritical
//*

//logical or vboool
bool boool::operator||(boool& a) const { return (this->b || a); } //! noncritical
//*

//logical xor with boool
bool boool::operator^(boool& a)  const { return (this->b ^ a); } //! noncritical
//*

//equality with boool		
bool boool::operator==(boool& a) const { return (this->b == a.b); } //! noncritical
//*

//inequality with boool
bool boool::operator!=(boool& a) const { return (this->b != a.b); } //! noncritical
//*

//less or equal with boool
bool boool::operator<=(boool& a) const { return (this->b <= a.b); } //! noncritical
//*

//greater or equal with boool
bool boool::operator>=(boool& a) const { return (this->b >= a.b); } //! noncritical
//*

//less with boool
bool boool::operator<(boool& a)  const { return (this->b < a.b); } //! noncritical
//*

//greater with boool
bool boool::operator>(boool& a)  const { return (this->b > a.b); } //! noncritical

//equality with bool
bool boool::operator==(bool a)  const { return (this->b == a); } //! noncritical
//*

//inequality with bool
bool boool::operator!=(bool a)  const { return (this->b == a); } //! noncritical
//*

//less or equal with bool
bool boool::operator<=(bool a)  const { return (this->b <= a); } //! noncritical
//*

//greater or equal with bool
bool boool::operator>=(bool a)  const { return (this->b >= a); } //! noncritical
//*

//less with bool
bool boool::operator<(bool a)   const { return (this->b < a); } //! noncritical
//*

//greater with bool
bool boool::operator>(bool a)   const { return (this->b > a); } //! noncritical
//*

//cast to bool
boool::operator bool()  const { return bool(this->b); } //! noncritical
//*

//cast to xlong
boool::operator xlong() const { return b; } //! noncritical
//*

//friends:

//add with xlong from left
inline boool operator+(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(a+b.b);
	return temp;
}
//*

//subtract with xlong from left
inline boool operator-(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(a-b.b);
	return temp;
}
//*

//multiplicate with xlong from left
inline boool operator*(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(a*b.b);
	return temp;
}
//*

//add with bool from left
inline boool operator+(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(xlong(a)+b.b);
	return temp;
}
//*

//subtract with bool from left
inline boool operator-(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(xlong(a)-b.b);
	return temp;
}
//*

//multiplicate with bool from left
inline boool operator*(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = clmath->sign(xlong(a)*b.b);
	return temp;
}
//*

//equality with bool from left
inline bool operator==(bool a,boool& b) { return (xlong(a) == b.b); } //! noncritical
//*

//inequality with bool from left
inline bool operator!=(bool a,boool& b) { return (xlong(a) != b.b); } //! noncritical
//*

//less or equal with bool from left
inline bool operator<=(bool a,boool& b) { return (xlong(a) <= b.b); } //! noncritical
//*

//greater or equal with bool from left
inline bool operator>=(bool a,boool& b) { return (xlong(a) >= b.b); } //! noncritical
//*

//less with bool from left
inline bool operator<(bool a,boool& b) { return (xlong(a) < b.b); } //! noncritical
//*

//greater with bool from left
inline bool operator>(bool a,boool& b) { return (xlong(a) > b.b); } //! noncritical
//*

///*

#endif
