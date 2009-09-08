//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLboool
#define HH_CLboool
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLmacros.hh"

#define CLtrue   1
#define CLfalse -1
#define CLnull   0

class boool
{
	private:
		xlong b;
	
	public:
		boool();
		boool(boool& a);
		boool(bool a);
		boool(xlong a);
		~boool();
	
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
		inline boool operator+(bool a)  const;
		inline boool operator-(bool a)  const;
		inline boool operator*(bool a)  const;
		inline boool operator+(xlong a) const;
		inline boool operator-(xlong a) const;
		inline boool operator*(xlong a) const;
		
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
		inline bool operator==(bool a)  const;
		inline bool operator!=(bool a)  const;
		inline bool operator<=(bool a)  const;
		inline bool operator>=(bool a)  const;
		inline bool operator<(bool a)   const;
		inline bool operator>(bool a)   const;
		
		friend bool operator==(bool a,boool& b);
		friend bool operator!=(bool a,boool& b);
		friend bool operator<=(bool a,boool& b);
		friend bool operator>=(bool a,boool& b);
		friend bool operator<(bool a,boool& b);
		friend bool operator>(bool a,boool& b);

		inline operator bool()  const;		
		inline operator xlong() const;
};

//unassigning constructor
boool::boool()
{
	this->b=0;
}
//*

//boool assigning constructor
boool::boool(boool& a)
{
	this->b=a.b;
}
//*

//bool assigning constructor
boool::boool(bool a)
{
	this->b=a;
}
//*

//xlong assigning construcotr
boool::boool(xlong a)
{
	this->b=CLmath::sign(a);
}
//*

//defaut destructor
boool::~boool() { }
//*

//assign boool
boool& boool::operator=(boool& a)
{
	this->b = a.b;
}
//*

//assign bool
boool& boool::operator=(bool a)
{
	this->b = a;
	return *this;
}
//*

//assign xlong
boool& boool::operator=(xlong a)
{
	this->b = CLmath::sign(a);
	return *this;
}
//*

//negate
boool boool::operator-() const
{
	boool temp(-(this->b));
	return temp;
}
//*

//invert
boool boool::operator!() const
{
	boool temp(-(this->b));
	return temp;
}
//*
	
//increment
boool& boool::operator++()
{
	if(this->b<1) this->b++;
	return *this;
}
//*

//decrement
boool& boool::operator--()
{
	if(this->b>-1) this->b--;
	return *this;
}
//*

//assignment addition with boool
boool& boool::operator+=(boool& a)
{
	this->b = CLmath::sign(this->b + a.b);
	return *this;
}
//*

//assignment subtraction with boool
boool& boool::operator-=(boool& a)
{
	this->b = CLmath::sign(this->b - a.b);
	return *this;
}
//*

//assignment multiplication with boool
boool& boool::operator*=(boool& a)
{
	this->b = CLmath::sign(this->b * a.b);
	return *this;
}

//assignment addition with bool
boool& boool::operator+=(bool a)
{
	this->b = CLmath::sign(this->b + xlong(a));
	return *this;
}
//*

//assignment subtraction with bool
boool& boool::operator-=(bool a)
{
	this->b = CLmath::sign(this->b - xlong(a));
	return *this;
}
//*

//assignment multiplication with bool
boool& boool::operator*=(bool a)
{
	this->b = CLmath::sign(this->b * xlong(a));
	return *this;
}
//*

//assignment addition with xlong
boool& boool::operator+=(xlong a)
{
	this->b = CLmath::sign(this->b + a);
	return *this;
}
//*

//assignment subtraction with xlong
boool& boool::operator-=(xlong a)
{
	this->b = CLmath::sign(this->b - a);
	return *this;
}
//*

//assignment multiplication with xlong
boool& boool::operator*=(xlong a)
{
	this->b = CLmath::sign(this->b * a);
	return *this;
}
//*

//add with boool
boool boool::operator+(boool& a) const
{
	boool temp(CLmath::sign(this->b + a.b));
	return temp;
}
//*

//subtract with boool
boool boool::operator-(boool& a) const
{
	boool temp(CLmath::sign(this->b - a.b));
	return temp;
}

//multiplicate with bool
boool boool::operator*(boool& a) const
{
	boool temp(CLmath::sign(this->b * a.b));
	return temp;
}
//*

//add with bool
boool boool::operator+(bool a)  const
{
	boool temp(CLmath::sign(this->b + xlong(a)));
	return temp;
}
//*

//subtract with bool
boool boool::operator-(bool a)  const
{
	boool temp(CLmath::sign(this->b - xlong(a)));
	return temp;	
}
//*

//multiplicate with bool
boool boool::operator*(bool a)  const
{
	boool temp(CLmath::sign(this->b * xlong(a)));
	return temp;	
}
//*

//add with xlong
boool boool::operator+(xlong a) const
{
	boool temp(CLmath::sign(this->b + a));
	return temp;	
}
//*

//subtract with xlong
boool boool::operator-(xlong a) const
{
	boool temp(CLmath::sign(this->b - a));
	return temp;	
}
//*

//multiplicate with xlong
boool boool::operator*(xlong a) const
{
	boool temp(CLmath::sign(this->b + a));
	return temp;
}
//*
	
//logical and with boool	
bool boool::operator&&(boool& a) const
{
	return (this->b && a);
}
//*

//logical or vboool
bool boool::operator||(boool& a) const
{
	return (this->b || a);
}
//*

//logical xor with boool
bool boool::operator^(boool& a)  const
{
	return (this->b ^ a);
}
//*

//equality with boool		
bool boool::operator==(boool& a) const
{
	return (this->b == a.b);
}
//*

//inequality with boool
bool boool::operator!=(boool& a) const
{
	return (this->b != a.b);
}
//*

//less or equal with boool
bool boool::operator<=(boool& a) const
{
	return (this->b <= a.b);
}
//*

//greater or equal with boool
bool boool::operator>=(boool& a) const
{
	return (this->b >= a.b);
}
//*

//less with boool
bool boool::operator<(boool& a)  const
{
	return (this->b < a.b);
}
//*

//greater with boool
bool boool::operator>(boool& a)  const
{
	return (this->b > a.b);
}

//equality with bool
bool boool::operator==(bool a)  const
{
	return (this->b == a);
}
//*

//inequality with bool
bool boool::operator!=(bool a)  const
{
	return (this->b == a);
}
//*

//less or equal with bool
bool boool::operator<=(bool a)  const
{
	return (this->b <= a);
}
//*

//greater or equal with bool
bool boool::operator>=(bool a)  const
{
	return (this->b >= a);
}
//*

//less with bool
bool boool::operator<(bool a)   const
{
	return (this->b < a);
}
//*

//greater with bool
bool boool::operator>(bool a)   const
{
	return (this->b > a);
}
//*

//cast to bool
boool::operator bool()  const
{
	return bool(this->b);
}
//*

//cast to xlong
boool::operator xlong() const
{
	return b;
}
//*

//friends:

//add with xlong from left
inline boool operator+(xlong a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(a+b.b);
	return temp;
}
//*

//subtract with xlong from left
inline boool operator-(xlong a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(a-b.b);
	return temp;
}
//*

//multiplicate with xlong from left
inline boool operator*(xlong a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(a*b.b);
	return temp;
}
//*

//add with bool from left
inline boool operator+(bool a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(xlong(a)+b.b);
	return temp;
}
//*

//subtract with bool from left
inline boool operator-(bool a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(xlong(a)-b.b);
	return temp;
}
//*

//multiplicate with bool from left
inline boool operator*(bool a,boool& b)
{
	boool temp;
	temp.b = CLmath::sign(xlong(a)*b.b);
	return temp;
}
//*

//equality with bool from left
inline bool operator==(bool a,boool& b)
{
	return (xlong(a) == b.b);
}
//*

//inequality with bool from left
inline bool operator!=(bool a,boool& b)
{
	return (xlong(a) != b.b);
}
//*

//less or equal with bool from left
inline bool operator<=(bool a,boool& b)
{
	return (xlong(a) <= b.b);
}
//*

//greater or equal with bool from left
inline bool operator>=(bool a,boool& b)
{
	return (xlong(a) >= b.b);
}
//*

//less with bool from left
inline bool operator<(bool a,boool& b)
{
	return (xlong(a) < b.b);
}
//*

//greater with bool from left
inline bool operator>(bool a,boool& b)
{
	return (xlong(a) > b.b);	
}
//*

#endif
