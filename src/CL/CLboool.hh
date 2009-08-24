//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLboool
#define HH_CLboool
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLmacros.hh"

#define boooltrue   1
#define booolfalse -1
#define booolnull   0


class boool
{
	private:
		xlong b;
	
	public:
		boool();
		~boool();
	
		boool& operator=(xlong a);
		
		boool& operator-();
		boool& operator!();
		
		boool& operator++();
		boool& operator--();
		
		boool operator+(boool a);
		boool operator-(boool a);
		boool operator+(bool a);
		boool operator-(bool a);
		boool operator+(xlong a);
		boool operator-(xlong a);
		boool operator*(xlong a);
		
		friend boool operator+(xlong a,boool b);
		friend boool operator-(xlong a,boool b);
		friend boool operator*(xlong a,boool b);
		
		boool operator&&(boool a);
		boool operator||(boool a);
		boool operator^(boool a);
		boool operator&&(bool a);
		boool operator||(bool a);
		boool operator^(bool a);
		
		bool operator==(boool a);
		bool operator!=(boool a);
		bool operator<=(boool a);
		bool operator>=(boool a);
		bool operator<(boool a);
		bool operator>(boool a);
		bool operator==(bool a);
		bool operator!=(bool a);
		bool operator<=(bool a);
		bool operator>=(bool a);
		bool operator<(bool a);
		bool operator>(bool a);
		
		friend bool operator==(bool a,boool b);
		friend bool operator!=(bool a,boool b);
		friend bool operator<=(bool a,boool b);
		friend bool operator>=(bool a,boool b);
		friend bool operator<(bool a,boool b);
		friend bool operator>(bool a,boool b);
};

boool::boool()
{
	this->b=0;
}

boool::~boool() { }

boool& boool::operator=(xlong a)
{
	this->b = CLmath::sign(a);
	return *this;
}

boool& boool::operator-()
{
	this->b = -(this->b); //CLneg(this->b);
	return *this;
}

boool& boool::operator!()
{
	this->b = -(this->b); //CLneg(this->b);
	return *this;
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

//friends:

boool operator+(xlong a,boool b)
{
	
}

boool operator-(xlong a,boool b)
{
	
}

boool operator*(xlong a,boool b)
{
	
}

bool operator==(bool a,boool b)
{
	
}

bool operator!=(bool a,boool b)
{
	
}

bool operator<=(bool a,boool b)
{
	
}

bool operator>=(bool a,boool b)
{
	
}

bool operator<(bool a,boool b)
{
	
}

bool operator>(bool a,boool b)
{
	
}

#endif
