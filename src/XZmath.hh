// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZmath.hh
// Math Library 

///guard
#ifndef HH_XZMATH
#define HH_XZMATH
///*

///declaration
#define SEED 22695477
///*

///definition
namespace math
{
	/*OK*/ long uid() { static short c=0; return ('XZ'<<16)+(c++); }
	/*OK*/ INLINE long clr(long x) { return x^=x; }
	/*OK*/ INLINE long sgn(long x) { return (x>>31)|(long)(((unsigned long)-x)>>31); }
	/*OK*/ INLINE long abs(long x) { return ((long(x>=0)-1) ^ x) | (x<0); }
	/*OK*/ INLINE long fac(long x) { long r=1; for(ulong i=2;i<=x;++i) { r*=i; } return r; } //max x=12
	       INLINE long crc(long x) { return 0b10000100110000010001110110110111^x; }
	/*OK*/ INLINE long avg(long x,long y) { return (x&y)+((x^y)>>1); }
	/*OK*/ INLINE long min(long x,long y) { return y^((x^y) & -(x<y)); }
	/*OK*/ INLINE long max(long x,long y) { return x^((x^y) & -(x<y)); }
	/*OK*/ INLINE long neg(long x,bool y) { return ~(x^(y-1))+y; } 
	/*OK*/ INLINE long set(long x,bool y) { return ((long(!y))-1) & x; }
	/*OK*/ INLINE long set(long x,long y,bool z) { return (((long(!z))-1) & x) | ((long(z)-1) & y); }
	INLINE long lim(long x,long a,long y) { return max(x,min(a,y)); } //!bit version
	/*OK*/        long sqr(unsigned long x) { long r=0; long t=0; for(ulong i=15;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; } //HOTFN 
	/*OK*/        long rnd(unsigned long x) { static long seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; }
	       INLINE long ndn(long x) { packed y = { x }; y.b[0]^=y.b[3]^=y.b[0]^=y.b[3]; y.b[1]^=y.b[2]^=y.b[1]^=y.b[2]; return y.d; }
}
///*

#endif

