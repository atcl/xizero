//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFIXED
#define HH_CLFIXED
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"

struct xshush
{
	xshort  num;
	uxshort frc;
};

union fixedp
{
	xlong  i;
	xshush p;

			fixedp& operator=(fixedp &a);
			fixedp& operator=(xshort& a);
			
			fixedp operator-();
			
			fixedp& operator++();
			fixedp& operator--();
			
			fixedp& operator+=(fixedp& a);
			fixedp& operator+=(xshort& a);
			fixedp& operator-=(fixedp& a);
			fixedp& operator-=(xshort& a);
			fixedp& operator*=(fixedp& a);
			fixedp& operator*=(xshort& a);
			fixedp& operator/=(fixedp& a);
			fixedp& operator/=(xshort& a);
	
			fixedp operator+(fixedp& a);
			fixedp operator+(xshort& a);
	friend  fixedp operator+(short& a,fixedp& b);
			fixedp operator-(fixedp& a);
			fixedp operator-(xshort& a);
	friend  fixedp operator-(short& a,fixedp& b);
			fixedp operator*(fixedp& a);
			fixedp operator*(xshort& a);
	friend  fixedp operator*(short& a,fixedp& b);
			fixedp operator/(fixedp& a);
			fixedp operator/(xshort& a);
	friend  fixedp operator/(short& a,fixedp& b);
	
	
			bool operator==(fixedp& a);
			bool operator==(xshort& a);
	friend  bool operator==(short& a,fixedp& b);
			bool operator!=(fixedp& a);
			bool operator!=(xshort& a);
	friend  bool operator!=(short& a,fixedp& b);
			bool operator<=(fixedp& a);
			bool operator<=(xshort& a);
	friend  bool operator<=(short& a,fixedp& b);
			bool operator>=(fixedp& a);
			bool operator>=(xshort& a);
	friend  bool operator>=(short& a,fixedp& b);
			bool operator<(fixedp& a);
			bool operator<(xshort& a);
	friend  bool operator<(short& a,fixedp& b);
			bool operator>(fixedp& a);
			bool operator>(xshort& a);
	friend  bool operator>(short& a,fixedp& b);
};

fixedp& fixedp::operator=(fixedp &a)
{
	this->i = a.i;
	return *this;
}

fixedp& fixedp::operator=(xshort &a)
{
	this->p.num = a;
	this->p.frc = 0;
	return *this;
}

//friends:

fixedp operator+(short& a,fixedp& b)
{
	fixedp temp;

	return temp;
}

fixedp operator-(short& a,fixedp& b)
{
	fixedp temp;

	return temp;
}

fixedp operator*(short& a,fixedp& b)
{
	fixedp temp;

	return temp;
}

fixedp operator/(short& a,fixedp& b)
{
	fixedp temp;

	return temp;	
}

bool operator==(short& a,fixedp& b)
{
	return 0;	
}

bool operator!=(short& a,fixedp& b)
{
	return 0;
}

bool operator<=(short& a,fixedp& b)
{
	return 0;
}

bool operator>=(short& a,fixedp& b)
{
	return 0;
}

bool operator<(short& a,fixedp& b)
{
	return 0;	
}

bool operator>(short& a,fixedp& b)
{
	return 0;
}

#endif
