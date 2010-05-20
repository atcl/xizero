///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFIXED
#define HH_CLFIXED
///*

///includes
#include "CLtypes.hh"
///*

///header
/* class name:	fixed
 * 
 * description:	fixed point type
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
//numerator and fraction container
struct xshush
{
	xshort  num;
	uxshort frc;
};
//*

union fixed
{
	xlong  i;
	xshush p;

	fixed() { this->i = 0; };
	fixed(xlong a) { this->p.num = xshort(a); this->p.frc = 0; };
	fixed(xshort a) { this->p.num = a; this->p.frc = 0; };
	fixed(float a);

	inline fixed& operator=(fixed& a);
	inline fixed& operator=(xshort& a);
	
	inline fixed operator-() const;
	
	inline fixed operator<<(xlong a) const;
	inline fixed operator>>(xlong a) const;
	
	inline fixed& operator++();
	inline fixed& operator--();
	
	inline fixed& operator+=(fixed& a);
	inline fixed& operator-=(fixed& a);
	inline fixed& operator*=(fixed& a);
	inline fixed& operator/=(fixed& a);
	
	inline fixed& operator+=(xshort& a);
	inline fixed& operator-=(xshort& a);
	inline fixed& operator*=(xshort& a);
	inline fixed& operator/=(xshort& a);
	
	inline fixed operator+(fixed& a) const;
	inline fixed operator-(fixed& a) const;
	inline fixed operator*(fixed& a) const;
	inline fixed operator/(fixed& a) const;
	
	inline fixed operator+(xshort& a) const;
	inline fixed operator-(xshort& a) const;
	inline fixed operator*(xshort& a) const;
	inline fixed operator/(xshort& a) const;
	
	friend fixed operator+(xshort& a,fixed& b);
	friend fixed operator-(xshort& a,fixed& b);
	friend fixed operator*(xshort& a,fixed& b);
	friend fixed operator/(xshort& a,fixed& b);
	
	inline bool operator==(fixed& a) const { return !(this->i ^ a.i); };
	inline bool operator!=(fixed& a) const { return (this->i ^ a.i); };
	inline bool operator<=(fixed& a) const { return (this->i<=a.i); };
	inline bool operator>=(fixed& a) const { return (this->i>=a.i); };
	inline bool operator<(fixed& a) const { return (this->i<a.i); };
	inline bool operator>(fixed& a) const { return (this->i>a.i); };
	
	inline bool operator==(xshort& a) const { return !(this->i ^ (xlong(a)<<16) ); };
	inline bool operator!=(xshort& a) const { return (this->i ^ (xlong(a)<<16) ); };
	inline bool operator<=(xshort& a) const { return (this->i<=xlong(a)<<16); };
	inline bool operator>=(xshort& a) const { return (this->i>=xlong(a)<<16); };
	inline bool operator<(xshort& a) const { return (this->i<xlong(a)<<16); };
	inline bool operator>(xshort& a) const { return (this->i>xlong(a)<<16); };
	
	friend bool operator==(xshort& a,fixed& b) { return !((xlong(a)<<16) ^ b.i); };
	friend bool operator!=(xshort& a,fixed& b) { return ((xlong(a)<<16) ^ b.i); };
	friend bool operator<=(xshort& a,fixed& b) { return ((xlong(a)<<16)<=b.i); };
	friend bool operator>=(xshort& a,fixed& b) { return ((xlong(a)<<16)>=b.i); };
	friend bool operator<(xshort& a,fixed& b) { return ((xlong(a)<<16)<b.i); };
	friend bool operator>(xshort& a,fixed& b) { return ((xlong(a)<<16)>b.i); };
	
	operator xshort() const;
	operator float() const;
};
///*

///implementation
//float assigning constructor
fixed::fixed(float a) //! noncritical
{
	this->p.num = xshort(a);
	this->p.frc = xshort( (a - float(xshort(a))) * 65536.0);
}
//*

//assign fixed
fixed& fixed::operator=(fixed& a) //! noncritical
{
	this->i = a.i;
	return *this;
}
//*

//assign xshort
fixed& fixed::operator=(xshort& a) //! noncritical
{
	this->p.num = a;
	this->p.frc = 0;
	return *this;
}
//*

//negate fixed
fixed fixed::operator-() const //! critical
{
	fixed r = *this;
	r.p.num = -this->p.num;
	return r;
}
//*

//increment fixed
fixed& fixed::operator++() //! critical
{
	this->p.num++;
	return *this;
}
//*

//decrement fixed
fixed& fixed::operator--() //! critical
{
	this->p.num--;
	return *this;
}
//*

//assignment addition fixed
fixed& fixed::operator+=(fixed& a) //! critical
{
	this->i += a.i;
	return *this;
}
//*

//assignment addition xshort
fixed& fixed::operator+=(xshort& a) //! critical
{
	this->p.num += a;
	return *this;
}
//*

//assignment subtraction fixed
fixed& fixed::operator-=(fixed& a) //! critical
{
	this->i -= a.i;
	return *this;
}
//*

//assignment subtraction xshort
fixed& fixed::operator-=(xshort& a) //! critical
{
	this->p.num -= a;
	return *this;
}
//*

//assignment multiplication fixed
fixed& fixed::operator*=(fixed& a) //! critical
{
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a.i) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment multiplication xshort
fixed& fixed::operator*=(xshort& a) //! critical
{
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment division fixed
fixed& fixed::operator/=(fixed& a) //! critical
{
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//assignment division xshort
fixed& fixed::operator/=(xshort& a) //! critical
{
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//add with fixed
fixed fixed::operator+(fixed& a) const //! critical
{
	fixed r = *this;
	r.i += a.i;
	return r;
}
//*

//add with xshort
fixed fixed::operator+(xshort& a) const //! critical
{
	fixed r = *this;
	r.p.num += a;
	return r;
}
//*

//subtract with fixed
fixed fixed::operator-(fixed& a) const //! critical
{
	fixed r = *this;
	r.i -= a.i;
	return r;
}
//*

//subtract with xshort
fixed fixed::operator-(xshort& a) const //! critical
{
	fixed r = *this;
	r.p.num -= a;
	return r;
}
//*

//multiplicate with fixed
fixed fixed::operator*(fixed& a) const //! critical
{
	fixed r = *this;
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a.i) ); //swap eax,edx in shrd?
	return r;
}
//*

//multiplicate with short
fixed fixed::operator*(xshort& a) const //! critical
{
	fixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//divide by fixed
fixed fixed::operator/(fixed& a) const //! critical
{
	fixed r = *this;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//divide by xshort
fixed fixed::operator/(xshort& a) const //! critical
{
	fixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//shift left
fixed fixed::operator<<(xlong a) const //! critical
{
	fixed temp((this->i)<<a);
	return temp;
}
//*

//shift right
fixed fixed::operator>>(xlong a) const //! critical
{
	fixed temp((this->i)>>a);
	return temp;
}
//*

//cast to xshort
fixed::operator xshort() const //! noncritical
{
	xshort o = this->p.frc >> 15;
	xshort r = this->p.num + o;
	return r; 
}
//*

//cast to float
fixed::operator float() const //! noncritical
{
	float r = (float(this->p.num)) + (float(this->p.frc)/65536);
	return r; 
}
//*

//friends:

//add with xshort from left
inline fixed operator+(xshort& a,fixed& b) //! critical
{
	fixed r = b;
	r.p.num += a;
	return r;
}
//*

//subtract with xshort from left
inline fixed operator-(xshort& a,fixed& b)  //! critical
{
	fixed r;
	r.i = 0;
	r.p.num = a;
	r.i -= b.i;
	return r;
}
//*

//multiplicate with xshort from left
inline fixed operator*(xshort& a,fixed& b)  //! critical
{
	fixed r = b;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//xshort divided by fixed
inline fixed operator/(xshort& a,fixed& b)  //! critical
{
	fixed r;
	r.i = 0;
	r.p.num = a;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(b) );
	return r;
}
//*
///*

#endif
