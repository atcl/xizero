//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLTRIBOOL
#define HH_CLTRIBOOL
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmath.hh"
#include "CLmacros.hh"

#define tritrue   1
#define trifalse -1
#define trinull   0


struct tribool
{
	xlong b;
	
	tribool& operator=(xlong a);
	
	tribool& operator-();
	tribool& operator!();
	
	tribool& operator++();
	tribool& operator--();
	
	tribool& operator+=(xlong a);
	tribool& operator-=(xlong a);
	tribool& operator*=(xlong a);
	
	tribool operator+(xlong a);
	tribool operator-(xlong a);
	tribool operator*(xlong a);
	
	tribool operator&&(xlong a);
	tribool operator||(xlong a);
	tribool operator^(xlong a);
	
	bool operator==(xlong a);
	bool operator!=(xlong a);
	bool operator<=(xlong a);
	bool operator>=(xlong a);
	bool operator<(xlong a);
	bool operator>(xlong a);
	
	//tribool,bool as args
};

tribool& tribool::operator=(xlong a)
{
	this->b = CLmath::sign(a);
	return *this;
}

tribool& tribool::operator-()
{
	this->b = -(this->b); //CLneg(this->b);
	return *this;
}

tribool& tribool::operator!()
{
	this->b = -(this->b); //CLneg(this->b);
	return *this;
}
	
tribool& tribool::operator++()
{
	if(this->b<1) this->b++;
	return *this;
}

tribool& tribool::operator--()
{
	if(this->b>-1) this->b--;
	return *this;
}

#endif
