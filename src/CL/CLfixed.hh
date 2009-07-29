//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFIXED
#define HH_CLFIXED
//#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"

struct xshush
{
	xshort  num;
	uxshort frc;
};

union xfixed
{
	xlong  i;
	xshush p;

			xfixed& operator=(xfixed &a);
			xfixed& operator=(xshort& a);
			
			xfixed& operator-();
			
			xfixed& operator++();
			xfixed& operator--();
			
			xfixed& operator+=(xfixed& a);
			xfixed& operator+=(xshort& a);
			xfixed& operator-=(xfixed& a);
			xfixed& operator-=(xshort& a);
			xfixed& operator*=(xfixed& a);
			xfixed& operator*=(xshort& a);
			xfixed& operator/=(xfixed& a);
			xfixed& operator/=(xshort& a);
	
			xfixed operator+(xfixed& a);
			xfixed operator+(xshort& a);
	friend  xfixed operator+(short& a,xfixed& b);
			xfixed operator-(xfixed& a);
			xfixed operator-(xshort& a);
	friend  xfixed operator-(short& a,xfixed& b);
			xfixed operator*(xfixed& a);
			xfixed operator*(xshort& a);
	friend  xfixed operator*(short& a,xfixed& b);
			xfixed operator/(xfixed& a);
			xfixed operator/(xshort& a);
	friend  xfixed operator/(short& a,xfixed& b);
	
	
			bool operator==(xfixed& a);
			bool operator==(xshort& a);
	friend  bool operator==(short& a,xfixed& b);
			bool operator!=(xfixed& a);
			bool operator!=(xshort& a);
	friend  bool operator!=(short& a,xfixed& b);
			bool operator<=(xfixed& a);
			bool operator<=(xshort& a);
	friend  bool operator<=(short& a,xfixed& b);
			bool operator>=(xfixed& a);
			bool operator>=(xshort& a);
	friend  bool operator>=(short& a,xfixed& b);
			bool operator<(xfixed& a);
			bool operator<(xshort& a);
	friend  bool operator<(short& a,xfixed& b);
			bool operator>(xfixed& a);
			bool operator>(xshort& a);
	friend  bool operator>(short& a,xfixed& b);
};

xfixed& xfixed::operator=(xfixed &a)
{
	this->i = a.i;
	return *this;
}

xfixed& xfixed::operator=(xshort &a)
{
	this->p.num = a;
	this->p.frc = 0;
	return *this;
}

xfixed& xfixed::operator-()
{
	this->p.num = -this->p.num;
	return *this;
}

xfixed& xfixed::operator++()
{
	this->p.num++;
	return *this;
}

xfixed& xfixed::operator--()
{
	this->p.num--;
	return *this;
}

xfixed& xfixed::operator+=(xfixed& a)
{
	this->i += a.i;
	return *this;
}

xfixed& xfixed::operator+=(xshort& a)
{
	this->p.num += a;
	return *this;
}

xfixed& xfixed::operator-=(xfixed& a)
{
	this->i -= a.i;
	return *this;
}

xfixed& xfixed::operator-=(xshort& a)
{
	this->p.num -= a;
	return *this;
}

xfixed& xfixed::operator*=(xfixed& a)
{
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a.i) ); //swap eax,edx in shrd?

	return *this;
}

xfixed& xfixed::operator*=(xshort& a)
{
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a) ); //swap eax,edx in shrd?

	return *this;
}

xfixed& xfixed::operator/=(xfixed& a)
{
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}

//friends:

xfixed operator+(short& a,xfixed& b)
{
	xfixed temp;

	return temp;
}

xfixed operator-(short& a,xfixed& b)
{
	xfixed temp;

	return temp;
}

xfixed operator*(short& a,xfixed& b)
{
	xfixed temp;

	return temp;
}

xfixed operator/(short& a,xfixed& b)
{
	xfixed temp;

	return temp;	
}

bool operator==(short& a,xfixed& b)
{
	return 0;	
}

bool operator!=(short& a,xfixed& b)
{
	return 0;
}

bool operator<=(short& a,xfixed& b)
{
	return 0;
}

bool operator>=(short& a,xfixed& b)
{
	return 0;
}

bool operator<(short& a,xfixed& b)
{
	return 0;	
}

bool operator>(short& a,xfixed& b)
{
	return 0;
}

#endif
