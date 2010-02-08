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
/* class name:	xfixed
 * 
 * description:	fixed point type
 * 
 * author:	atcl
 * 
 * notes:	test.
 * 
 * version: 0.1
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

union xfixed
{
	xlong  i;
	xshush p;

	xfixed() { this->i = 0; };
	xfixed(xlong a) { this->p.num = xshort(a); this->p.frc = 0; };
	xfixed(xshort a) { this->p.num = a; this->p.frc = 0; };
	xfixed(float a);

	inline xfixed& operator=(xfixed& a);
	inline xfixed& operator=(xshort& a);
	
	inline xfixed operator-() const;
	
	inline xfixed operator<<(xlong a) const;
	inline xfixed operator>>(xlong a) const;
	
	inline xfixed& operator++();
	inline xfixed& operator--();
	
	inline xfixed& operator+=(xfixed& a);
	inline xfixed& operator-=(xfixed& a);
	inline xfixed& operator*=(xfixed& a);
	inline xfixed& operator/=(xfixed& a);
	
	inline xfixed& operator+=(xshort& a);
	inline xfixed& operator-=(xshort& a);
	inline xfixed& operator*=(xshort& a);
	inline xfixed& operator/=(xshort& a);
	
	inline xfixed operator+(xfixed& a) const;
	inline xfixed operator-(xfixed& a) const;
	inline xfixed operator*(xfixed& a) const;
	inline xfixed operator/(xfixed& a) const;
	
	inline xfixed operator+(xshort& a) const;
	inline xfixed operator-(xshort& a) const;
	inline xfixed operator*(xshort& a) const;
	inline xfixed operator/(xshort& a) const;
	
	friend xfixed operator+(xshort& a,xfixed& b);
	friend xfixed operator-(xshort& a,xfixed& b);
	friend xfixed operator*(xshort& a,xfixed& b);
	friend xfixed operator/(xshort& a,xfixed& b);
	
	inline bool operator==(xfixed& a) const { return !(this->i ^ a.i); };
	inline bool operator!=(xfixed& a) const { return (this->i ^ a.i); };
	inline bool operator<=(xfixed& a) const { return (this->i<=a.i); };
	inline bool operator>=(xfixed& a) const { return (this->i>=a.i); };
	inline bool operator<(xfixed& a) const { return (this->i<a.i); };
	inline bool operator>(xfixed& a) const { return (this->i>a.i); };
	
	inline bool operator==(xshort& a) const { return !(this->i ^ (xlong(a)<<16) ); };
	inline bool operator!=(xshort& a) const { return (this->i ^ (xlong(a)<<16) ); };
	inline bool operator<=(xshort& a) const { return (this->i<=xlong(a)<<16); };
	inline bool operator>=(xshort& a) const { return (this->i>=xlong(a)<<16); };
	inline bool operator<(xshort& a) const { return (this->i<xlong(a)<<16); };
	inline bool operator>(xshort& a) const { return (this->i>xlong(a)<<16); };
	
	friend bool operator==(xshort& a,xfixed& b) { return !((xlong(a)<<16) ^ b.i); };
	friend bool operator!=(xshort& a,xfixed& b) { return ((xlong(a)<<16) ^ b.i); };
	friend bool operator<=(xshort& a,xfixed& b) { return ((xlong(a)<<16)<=b.i); };
	friend bool operator>=(xshort& a,xfixed& b) { return ((xlong(a)<<16)>=b.i); };
	friend bool operator<(xshort& a,xfixed& b) { return ((xlong(a)<<16)<b.i); };
	friend bool operator>(xshort& a,xfixed& b) { return ((xlong(a)<<16)>b.i); };
	
	operator xshort() const;
	operator float() const;
};
///*

///implementation
//float assigning constructor
xfixed::xfixed(float a) //! noncritical
{
	this->p.num = xshort(a);
	this->p.frc = xshort( (a - float(xshort(a))) * 65536.0);
}
//*

//assign xfixed
xfixed& xfixed::operator=(xfixed& a) //! noncritical
{
	this->i = a.i;
	return *this;
}
//*

//assign xshort
xfixed& xfixed::operator=(xshort& a) //! noncritical
{
	this->p.num = a;
	this->p.frc = 0;
	return *this;
}
//*

//negate xfixed
xfixed xfixed::operator-() const //! critical
{
	xfixed r = *this;
	r.p.num = -this->p.num;
	return r;
}
//*

//increment xfixed
xfixed& xfixed::operator++() //! critical
{
	this->p.num++;
	return *this;
}
//*

//decrement xfixed
xfixed& xfixed::operator--() //! critical
{
	this->p.num--;
	return *this;
}
//*

//assignment addition xfixed
xfixed& xfixed::operator+=(xfixed& a) //! critical
{
	this->i += a.i;
	return *this;
}
//*

//assignment addition xshort
xfixed& xfixed::operator+=(xshort& a) //! critical
{
	this->p.num += a;
	return *this;
}
//*

//assignment subtraction xfixed
xfixed& xfixed::operator-=(xfixed& a) //! critical
{
	this->i -= a.i;
	return *this;
}
//*

//assignment subtraction xshort
xfixed& xfixed::operator-=(xshort& a) //! critical
{
	this->p.num -= a;
	return *this;
}
//*

//assignment multiplication xfixed
xfixed& xfixed::operator*=(xfixed& a) //! critical
{
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a.i) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment multiplication xshort
xfixed& xfixed::operator*=(xshort& a) //! critical
{
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment division xfixed
xfixed& xfixed::operator/=(xfixed& a) //! critical
{
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//assignment division xshort
xfixed& xfixed::operator/=(xshort& a) //! critical
{
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//add with xfixed
xfixed xfixed::operator+(xfixed& a) const //! critical
{
	xfixed r = *this;
	r.i += a.i;
	return r;
}
//*

//add with xshort
xfixed xfixed::operator+(xshort& a) const //! critical
{
	xfixed r = *this;
	r.p.num += a;
	return r;
}
//*

//subtract with xfixed
xfixed xfixed::operator-(xfixed& a) const //! critical
{
	xfixed r = *this;
	r.i -= a.i;
	return r;
}
//*

//subtract with xshort
xfixed xfixed::operator-(xshort& a) const //! critical
{
	xfixed r = *this;
	r.p.num -= a;
	return r;
}
//*

//multiplicate with xfixed
xfixed xfixed::operator*(xfixed& a) const //! critical
{
	xfixed r = *this;
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a.i) ); //swap eax,edx in shrd?
	return r;
}
//*

//multiplicate with short
xfixed xfixed::operator*(xshort& a) const //! critical
{
	xfixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//divide by xfixed
xfixed xfixed::operator/(xfixed& a) const //! critical
{
	xfixed r = *this;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//divide by xshort
xfixed xfixed::operator/(xshort& a) const //! critical
{
	xfixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//shift left
xfixed xfixed::operator<<(xlong a) const //! critical
{
	xfixed temp((this->i)<<a);
	return temp;
}
//*

//shift right
xfixed xfixed::operator>>(xlong a) const //! critical
{
	xfixed temp((this->i)>>a);
	return temp;
}
//*

//cast to xshort
xfixed::operator xshort() const //! noncritical
{
	xshort o = this->p.frc >> 15;
	xshort r = this->p.num + o;
	return r; 
}
//*

//cast to float
xfixed::operator float() const //! noncritical
{
	float r = (float(this->p.num)) + (float(this->p.frc)/65536);
	return r; 
}
//*

//friends:

//add with xshort from left
inline xfixed operator+(xshort& a,xfixed& b) //! critical
{
	xfixed r = b;
	r.p.num += a;
	return r;
}
//*

//subtract with xshort from left
inline xfixed operator-(xshort& a,xfixed& b)  //! critical
{
	xfixed r;
	r.i = 0;
	r.p.num = a;
	r.i -= b.i;
	return r;
}
//*

//multiplicate with xshort from left
inline xfixed operator*(xshort& a,xfixed& b)  //! critical
{
	xfixed r = b;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//xshort divided by xfixed
inline xfixed operator/(xshort& a,xfixed& b)  //! critical
{
	xfixed r;
	r.i = 0;
	r.p.num = a;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(b) );
	return r;
}
//*
///*

#endif
