//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLTRIBOOL
#define HH_CLTRIBOOL
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmath.hh"

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
	
	bool operator==(xlong a);
	bool operator!=(xlong a);
	bool operator<=(xlong a);
	bool operator>=(xlong a);
	bool operator<(xlong a);
	bool operator>(xlong a);
};

tribool& tribool::operator=(xlong a)
{
	b = CLmath::sign(a);
}

#endif
