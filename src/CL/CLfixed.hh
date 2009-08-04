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

//inline all methods and friends!
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

xfixed& xfixed::operator/=(xshort& a)
{
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}

xfixed xfixed::operator+(xfixed& a)
{
	xfixed r = *this;
	r.i += a.i;
	return r;
}

xfixed xfixed::operator+(xshort& a)
{
	xfixed r = *this;
	r.p.num += a;
	return r;
}

xfixed xfixed::operator-(xfixed& a)
{
	xfixed r = *this;
	r.i -= a.i;
	return r;
}

xfixed xfixed::operator-(xshort& a)
{
	xfixed r = *this;
	r.p.num -= a;
	return r;
}

xfixed xfixed::operator*(xfixed& a)
{
	xfixed r = *this;
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a.i) ); //swap eax,edx in shrd?
	return r;
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
