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
		xlong b;
		inline xlong sgn(xlong x) const { return xlong(x!=0) | (xlong(x>=0)-1);  };
		
	public:
		boool() { this->b=0; };
		boool(boool& a) { this->b=a.b; };
		boool(bool a) { this->b=a; };
		boool(xlong a) { this->b=sgn(a); };
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
		
		inline bool operator&&(boool& a) const { return (this->b && a); };
		inline bool operator||(boool& a) const { return (this->b || a); };
		inline bool operator^(boool& a)  const { return (this->b ^ a); };
		
		inline bool operator==(boool& a) const { return (this->b == a.b); };
		inline bool operator!=(boool& a) const { return (this->b != a.b); };
		inline bool operator<=(boool& a) const { return (this->b <= a.b); };
		inline bool operator>=(boool& a) const { return (this->b >= a.b); };
		inline bool operator<(boool& a)  const { return (this->b < a.b); };
		inline bool operator>(boool& a)  const { return (this->b > a.b); };
		inline bool operator==(bool a)   const { return (this->b == a); };
		inline bool operator!=(bool a)   const { return (this->b == a); };
		inline bool operator<=(bool a)   const { return (this->b <= a); };
		inline bool operator>=(bool a)   const { return (this->b >= a); };
		inline bool operator<(bool a)    const { return (this->b < a); };
		inline bool operator>(bool a)    const { return (this->b > a); };
		
		friend bool operator==(bool a,boool& b) { return (xlong(a) == b.b); };
		friend bool operator!=(bool a,boool& b) { return (xlong(a) != b.b); };
		friend bool operator<=(bool a,boool& b) { return (xlong(a) <= b.b); };
		friend bool operator>=(bool a,boool& b) { return (xlong(a) >= b.b); };
		friend bool operator<(bool a,boool& b) { return (xlong(a) < b.b); };
		friend bool operator>(bool a,boool& b) { return (xlong(a) > b.b); };

		inline operator bool()  const { return bool(this->b); };		
		inline operator xlong() const { return b; };
};
///*

///implementation
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
	this->b = sgn(a);
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
	this->b = sgn(this->b + a.b);
	return *this;
}
//*

//assignment subtraction with boool
boool& boool::operator-=(boool& a) //! noncritical
{
	this->b = sgn(this->b - a.b);
	return *this;
}
//*

//assignment multiplication with boool
boool& boool::operator*=(boool& a) //! noncritical
{
	this->b = sgn(this->b * a.b);
	return *this;
}

//assignment addition with bool
boool& boool::operator+=(bool a) //! noncritical
{
	this->b = sgn(this->b + xlong(a));
	return *this;
}
//*

//assignment subtraction with bool
boool& boool::operator-=(bool a) //! noncritical
{
	this->b = sgn(this->b - xlong(a));
	return *this;
}
//*

//assignment multiplication with bool
boool& boool::operator*=(bool a) //! noncritical
{
	this->b = sgn(this->b * xlong(a));
	return *this;
}
//*

//assignment addition with xlong
boool& boool::operator+=(xlong a) //! noncritical
{
	this->b = sgn(this->b + a);
	return *this;
}
//*

//assignment subtraction with xlong
boool& boool::operator-=(xlong a) //! noncritical
{
	this->b = sgn(this->b - a);
	return *this;
}
//*

//assignment multiplication with xlong
boool& boool::operator*=(xlong a) //! noncritical
{
	this->b = sgn(this->b * a);
	return *this;
}
//*

//add with boool
boool boool::operator+(boool& a) const //! noncritical
{
	boool temp(sgn(this->b + a.b));
	return temp;
}
//*

//subtract with boool
boool boool::operator-(boool& a) const //! noncritical
{
	boool temp(sgn(this->b - a.b));
	return temp;
}

//multiplicate with bool
boool boool::operator*(boool& a) const //! noncritical
{
	boool temp(sgn(this->b * a.b));
	return temp;
}
//*

//add with bool
boool boool::operator+(bool a)  const //! noncritical
{
	boool temp(sgn(this->b + xlong(a)));
	return temp;
}
//*

//subtract with bool
boool boool::operator-(bool a)  const //! noncritical
{
	boool temp(sgn(this->b - xlong(a)));
	return temp;	
}
//*

//multiplicate with bool
boool boool::operator*(bool a)  const //! noncritical
{
	boool temp(sgn(this->b * xlong(a)));
	return temp;	
}
//*

//add with xlong
boool boool::operator+(xlong a) const //! noncritical
{
	boool temp(sgn(this->b + a));
	return temp;	
}
//*

//subtract with xlong
boool boool::operator-(xlong a) const //! noncritical
{
	boool temp(sgn(this->b - a));
	return temp;	
}
//*

//multiplicate with xlong
boool boool::operator*(xlong a) const //! noncritical
{
	boool temp(sgn(this->b + a));
	return temp;
}
//*

/*
//friends:

//add with xlong from left
inline boool operator+(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(a+b.b);
	return temp;
}
//*

//subtract with xlong from left
inline boool operator-(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(a-b.b);
	return temp;
}
//*

//multiplicate with xlong from left
inline boool operator*(xlong a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(a*b.b);
	return temp;
}
//*

//add with bool from left
inline boool operator+(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(xlong(a)+b.b);
	return temp;
}
//*

//subtract with bool from left
inline boool operator-(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(xlong(a)-b.b);
	return temp;
}
//*

//multiplicate with bool from left
inline boool operator*(bool a,boool& b) //! noncritical
{
	CLmath* clmath;
	boool temp;
	temp.b = sgn(xlong(a)*b.b);
	return temp;
}
//*
*/
///*

#endif
