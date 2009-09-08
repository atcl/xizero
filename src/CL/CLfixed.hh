//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFIXED
#define HH_CLFIXED
//#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"

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

	xfixed();
	xfixed(xlong a);
	xfixed(xshort a);
	xfixed(float a);

	inline xfixed& operator=(xfixed &a);
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
	
	inline bool operator==(xfixed& a) const;
	inline bool operator!=(xfixed& a) const;
	inline bool operator<=(xfixed& a) const;
	inline bool operator>=(xfixed& a) const;
	inline bool operator<(xfixed& a) const;
	inline bool operator>(xfixed& a) const;
	
	inline bool operator==(xshort& a) const;
	inline bool operator!=(xshort& a) const;
	inline bool operator<=(xshort& a) const;
	inline bool operator>=(xshort& a) const;
	inline bool operator<(xshort& a) const;
	inline bool operator>(xshort& a) const;
	
	friend bool operator==(xshort& a,xfixed& b);
	friend bool operator!=(xshort& a,xfixed& b);
	friend bool operator<=(xshort& a,xfixed& b);
	friend bool operator>=(xshort& a,xfixed& b);
	friend bool operator<(xshort& a,xfixed& b);
	friend bool operator>(xshort& a,xfixed& b);
	
	operator xshort() const;
	operator float() const;
};

//unassigning constructor
xfixed::xfixed()
{
	this->i = 0;
}
//*

//xlong assigning constructor
xfixed::xfixed(xlong a)
{
	this->i = 0;
	this->p.num = xshort(a);
}
//*

//xshort assigning construcotr
xfixed::xfixed(xshort a)
{
	this->i = 0;
	this->p.num = a;
}
//*

//float assigning constructor
xfixed::xfixed(float a)
{
	this->p.num = xshort(a);
	this->p.frc = xshort((a - float(this->p.num))*65536);
}
//*

//assign xfixed
xfixed& xfixed::operator=(xfixed &a)
{
	this->i = a.i;
	return *this;
}
//*

//assign xshort
xfixed& xfixed::operator=(xshort &a)
{
	this->p.num = a;
	this->p.frc = 0;
	return *this;
}
//*

//negate xfixed
xfixed xfixed::operator-() const
{
	xfixed r = *this;
	r.p.num = -this->p.num;
	return r;
}
//*

//increment xfixed
xfixed& xfixed::operator++()
{
	this->p.num++;
	return *this;
}
//*

//decrement xfixed
xfixed& xfixed::operator--()
{
	this->p.num--;
	return *this;
}
//*

//assignment addition xfixed
xfixed& xfixed::operator+=(xfixed& a)
{
	this->i += a.i;
	return *this;
}
//*

//assignment addition xshort
xfixed& xfixed::operator+=(xshort& a)
{
	this->p.num += a;
	return *this;
}
//*

//assignment subtraction xfixed
xfixed& xfixed::operator-=(xfixed& a)
{
	this->i -= a.i;
	return *this;
}
//*

//assignment subtraction xshort
xfixed& xfixed::operator-=(xshort& a)
{
	this->p.num -= a;
	return *this;
}
//*

//assignment multiplication xfixed
xfixed& xfixed::operator*=(xfixed& a)
{
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a.i) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment multiplication xshort
xfixed& xfixed::operator*=(xshort& a)
{
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(this->i) : "a"(this->i),"b"(a) ); //swap eax,edx in shrd?

	return *this;
}
//*

//assignment division xfixed
xfixed& xfixed::operator/=(xfixed& a)
{
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//assignment division xshort
xfixed& xfixed::operator/=(xshort& a)
{
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(this->i) : "a"(this->i),"d"(this->i),"b"(a) );

	return *this;
}
//*

//add with xfixed
xfixed xfixed::operator+(xfixed& a) const
{
	xfixed r = *this;
	r.i += a.i;
	return r;
}
//*

//add with xshort
xfixed xfixed::operator+(xshort& a) const
{
	xfixed r = *this;
	r.p.num += a;
	return r;
}
//*

//subtract with xfixed
xfixed xfixed::operator-(xfixed& a) const
{
	xfixed r = *this;
	r.i -= a.i;
	return r;
}
//*

//subtract with xshort
xfixed xfixed::operator-(xshort& a) const
{
	xfixed r = *this;
	r.p.num -= a;
	return r;
}
//*

//multiplicate with xfixed
xfixed xfixed::operator*(xfixed& a) const
{
	xfixed r = *this;
	__asm__ __volatile__ ( "imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a.i) ); //swap eax,edx in shrd?
	return r;
}
//*

//multiplicate with short
xfixed xfixed::operator*(xshort& a) const
{
	xfixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//divide by xfixed
xfixed xfixed::operator/(xfixed& a) const
{
	xfixed r = *this;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//divide by xshort
xfixed xfixed::operator/(xshort& a) const
{
	xfixed r = *this;
	__asm__ __volatile__ ( "shll $16,%%ebx; sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(a) );
	return r;
}
//*

//equality with xfixed
bool xfixed::operator==(xfixed& a) const
{
	return !(this->i ^ a.i);
}
//*

//equality with xshort
bool xfixed::operator==(xshort& a) const
{
	return !(this->i ^ (xlong(a)<<16) );
}
//*

//inequality with xfixed
bool xfixed::operator!=(xfixed& a) const
{
	return (this->i ^ a.i);
}
//*

//inequality with xshort
bool xfixed::operator!=(xshort& a) const
{
	return (this->i ^ (xlong(a)<<16) );
}
//*

//less or equal with xfixed
bool xfixed::operator<=(xfixed& a) const
{
	return (this->i<=a.i);
}
//*

//less or equal with xshort
bool xfixed::operator<=(xshort& a) const
{
	return (this->i<=xlong(a)<<16);
}
//*

//greater or equal with xfixed
bool xfixed::operator>=(xfixed& a) const
{
	return (this->i>=a.i);
}
//*

//greater or equal with xshort
bool xfixed::operator>=(xshort& a) const
{
	return (this->i>=xlong(a)<<16);
}
//*

//less with xfixed
bool xfixed::operator<(xfixed& a) const
{
	return (this->i<a.i);
}
//*

//less with xshort
bool xfixed::operator<(xshort& a) const
{
	return (this->i<xlong(a)<<16);
}
//*

//greater with xfixed
bool xfixed::operator>(xfixed& a) const
{
	return (this->i>a.i);
}
//*

//greater with xshort
bool xfixed::operator>(xshort& a) const
{
	return (this->i>xlong(a)<<16);
}
//*

//shift left
xfixed xfixed::operator<<(xlong a) const
{
	xfixed temp((this->i)<<a);
	return temp;
}
//*

//shift right
xfixed xfixed::operator>>(xlong a) const
{
	xfixed temp((this->i)>>a);
	return temp;
}
//*

//cast to xshort
xfixed::operator xshort() const
{
	xshort o = this->p.frc >> 15;
	xshort r = (this->i >> 16) + o;
	return r; 
}
//*

//cast to float
xfixed::operator float() const
{
	float r = float(this->p.num) + (float(this->p.frc)/65536);
	return r; 
}
//*

//friends:

//add with xshort from left
inline xfixed operator+(xshort& a,xfixed& b)
{
	xfixed r = b;
	r.p.num += a;
	return r;
}
//*

//subtract with xshort from left
inline xfixed operator-(xshort& a,xfixed& b)
{
	xfixed r;
	r.i = 0;
	r.p.num = a;
	r.i -= b.i;
	return r;
}
//*

//multiplicate with xshort from left
inline xfixed operator*(xshort& a,xfixed& b)
{
	xfixed r = b;
	__asm__ __volatile__ ( "shll $16,%%ebx; imull %%ebx; shrdl $16,%%eax,%%edx;" : "=a"(r.i) : "a"(r.i),"b"(a) ); //swap eax,edx in shrd?
	return r;
}
//*

//xshort divided by xfixed
inline xfixed operator/(xshort& a,xfixed& b)
{
	xfixed r;
	r.i = 0;
	r.p.num = a;
	__asm__ __volatile__ ( "sarl $16,%%edx; shll $16,%%eax; idiv %%ebx;" : "=a"(r.i) : "a"(r.i),"d"(r.i),"b"(b) );
	return r;
}
//*

//equality with xshort from left
inline bool operator==(xshort& a,xfixed& b)
{
	return !((xlong(a)<<16) ^ b.i);	
}
//*

//inequality with xshort from left
inline bool operator!=(xshort& a,xfixed& b)
{
	return ((xlong(a)<<16) ^ b.i);
}
//*

//less or equal with xshort from left
inline bool operator<=(xshort& a,xfixed& b)
{
	return ((xlong(a)<<16)<=b.i);
}
//*

//greater or equal with xshort from left
inline bool operator>=(xshort& a,xfixed& b)
{
	return ((xlong(a)<<16)>=b.i);
}
//*

//less with xshort from left
inline bool operator<(xshort& a,xfixed& b)
{
	return ((xlong(a)<<16)<b.i);
}
//*

//greater with xshort from left
inline bool operator>(xshort& a,xfixed& b)
{
	return ((xlong(a)<<16)>b.i);
}
//*

#endif
