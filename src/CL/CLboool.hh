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
	
		inline boool& operator=(boool a);
		inline boool& operator=(bool a);
		inline boool& operator=(xlong a);
		
		inline boool operator-() const;
		inline boool operator!() const;
		
		inline boool& operator++();
		inline boool& operator--();
		
		inline boool& operator+=(boool a);
		inline boool& operator-=(boool a);
		inline boool& operator*=(boool a);
		inline boool& operator+=(bool a);
		inline boool& operator-=(bool a);
		inline boool& operator*=(bool a);
		inline boool& operator+=(xlong a);
		inline boool& operator-=(xlong a);
		inline boool& operator*=(xlong a);
		
		inline boool operator+(boool a) const;
		inline boool operator-(boool a) const;
		inline boool operator*(boool a) const;
		inline boool operator+(bool a)  const;
		inline boool operator-(bool a)  const;
		inline boool operator*(bool a)  const;
		inline boool operator+(xlong a) const;
		inline boool operator-(xlong a) const;
		inline boool operator*(xlong a) const;
		
		friend boool operator+(xlong a,boool b);
		friend boool operator-(xlong a,boool b);
		friend boool operator*(xlong a,boool b);
		
		inline bool operator&&(boool a) const;
		inline bool operator||(boool a) const;
		inline bool operator^(boool a)  const;
		
		inline bool operator==(boool a) const;
		inline bool operator!=(boool a) const;
		inline bool operator<=(boool a) const;
		inline bool operator>=(boool a) const;
		inline bool operator<(boool a)  const;
		inline bool operator>(boool a)  const;
		inline bool operator==(bool a)  const;
		inline bool operator!=(bool a)  const;
		inline bool operator<=(bool a)  const;
		inline bool operator>=(bool a)  const;
		inline bool operator<(bool a)   const;
		inline bool operator>(bool a)   const;
		
		friend bool operator==(bool a,boool b);
		friend bool operator!=(bool a,boool b);
		friend bool operator<=(bool a,boool b);
		friend bool operator>=(bool a,boool b);
		friend bool operator<(bool a,boool b);
		friend bool operator>(bool a,boool b);

		inline operator bool()  const;		
		inline operator xlong() const;
};

boool::boool()
{
	this->b=0;
}

boool::boool(boool& a)
{
	this->b=a.b;
}

boool::boool(bool a)
{
	this->b=a;
}

boool::boool(xlong a)
{
	this->b=CLmath::sign(a);
}

boool::~boool() { }

boool& boool::operator=(boool a)
{
	this->b = a.b;
}

boool& boool::operator=(bool a)
{
	this->b = a;
	return *this;
}

boool& boool::operator=(xlong a)
{
	this->b = CLmath::sign(a);
	return *this;
}

boool boool::operator-() const
{
	boool temp(-(this->b));
	return temp;
}

boool boool::operator!() const
{
	boool temp(-(this->b));
	return temp;
}
	
boool& boool::operator++()
{
	if(this->b<1) this->b++;
	return *this;
}

boool& boool::operator--()
{
	if(this->b>-1) this->b--;
	return *this;
}

boool& boool::operator+=(boool a)
{
	
}

boool& boool::operator-=(boool a)
{
	
}

boool& boool::operator*=(boool a)
{
	
}

boool& boool::operator+=(bool a)
{
	
}

boool& boool::operator-=(bool a)
{
	
}

boool& boool::operator*=(bool a)
{
	
}

boool& boool::operator+=(xlong a)
{
	
}

boool& boool::operator-=(xlong a)
{
	
}

boool& boool::operator*=(xlong a)
{
	
}

boool boool::operator+(boool a) const
{
	boool temp(CLmath::sign(this->b + a.b));
	return temp;
}

boool boool::operator-(boool a) const
{
	boool temp(CLmath::sign(this->b - a.b));
	return temp;
}

boool boool::operator*(boool a) const
{
	boool temp(CLmath::sign(this->b * a.b));
	return temp;
}

boool boool::operator+(bool a)  const
{
	boool temp(CLmath::sign(this->b + xlong(a)));
	return temp;
}

boool boool::operator-(bool a)  const
{
	boool temp(CLmath::sign(this->b - xlong(a)));
	return temp;	
}

boool boool::operator*(bool a)  const
{
	boool temp(CLmath::sign(this->b * xlong(a)));
	return temp;	
}

boool boool::operator+(xlong a) const
{
	boool temp(CLmath::sign(this->b + a));
	return temp;	
}

boool boool::operator-(xlong a) const
{
	boool temp(CLmath::sign(this->b - a));
	return temp;	
}

boool boool::operator*(xlong a) const
{
	boool temp(CLmath::sign(this->b + a));
	return temp;
}
		
bool boool::operator&&(boool a) const
{
	
}

bool boool::operator||(boool a) const
{
	
}

bool boool::operator^(boool a)  const
{
	
}
		
bool boool::operator==(boool a) const
{
	
}

bool boool::operator!=(boool a) const
{
	
}

bool boool::operator<=(boool a) const
{
	
}

bool boool::operator>=(boool a) const
{
	
}

bool boool::operator<(boool a)  const
{
	
}

bool boool::operator>(boool a)  const
{
	
}

bool boool::operator==(bool a)  const
{
	
}

bool boool::operator!=(bool a)  const
{
	
}

bool boool::operator<=(bool a)  const
{
	
}

bool boool::operator>=(bool a)  const
{
	
}

bool boool::operator<(bool a)   const
{
	
}

bool boool::operator>(bool a)   const
{
	
}

boool::operator bool()  const
{
	return bool(this->b);
}
	
boool::operator xlong() const
{
	return b;
}

//friends:

inline boool operator+(xlong a,boool b)
{
	
}

inline boool operator-(xlong a,boool b)
{
	
}

inline boool operator*(xlong a,boool b)
{
	
}

inline bool operator==(bool a,boool b)
{
	
}

inline bool operator!=(bool a,boool b)
{
	
}

inline bool operator<=(bool a,boool b)
{
	
}

inline bool operator>=(bool a,boool b)
{
	
}

inline bool operator<(bool a,boool b)
{
	
}

inline bool operator>(bool a,boool b)
{
	
}

#endif
