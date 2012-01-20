// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZstring.hh
// C String Library 

///guard
#ifndef HH_XZSTRING
#define HH_XZSTRING
///*

///include
#include "XZfixed.hh"
#include "XZmath.hh"
///*

///definitions
namespace string
{
	/*OK*/ long   length(const char* x);
	/*OK*/ char*  copy(const char* x,long y=0,long z=0x7FFFFFFF);
	/*OK*/ char*  concat(const char* x,const char* y);
	/*OK*/ long   count(const char* x,char='\n');
	/*OK*/ long   scan(const char* x,char y,char z='\n');
	/*OK*/ long   find(const char* x,const char* y);
	/*OK*/ long   conl(const char* x);
	/*OK*/ long   conh(const char* x);
	/*OK*/ char*  cons(long x);
	/*OK*/ char*  conf(fixed x);
	/*OK*/ char*  repl(const char* x,char y,char z);
	/*OK*/ void   repl(char* x,char y,char z);
	/*OK*/ char** split(const char* x,char y);
	/*OK*/ char*  trim(const char* x,char y=' ');
	/*OK*/ char   toup(char x);
}
///*

///implementation
long string::length(const char* x)
{
	long r=0;
	while(x[r]!=0) { r++; }
	return r;
}

char* string::copy(const char* x,long y,long z)
{
	const long l = math::min(length(x),z);
	char* r = new char[l+1];
	for(long i=0;i<l;++i,++y)
	{
		r[i] = x[y];
	}
	r[l] = 0;
	return r;
}

char* string::concat(const char* x,const char* y)
{
	const long a = length(x);
	const long b = length(y);
	char* r = new char[a+b+1];

	long i = 0;
	long j = 0;
	for(;i<a;++i)
	{
		r[i] = x[i];
	}
	for(;j<b;++i,++j)
	{
		r[i] = y[j];
	}
	r[++i] = 0;
	return r;
}

long string::count(const char* x,char y)
{
	long r = 0;
	long i = 0;
	while(x[i]!=0)
	{
		r += (x[i]==y);
		++i;
	}
	return r;
}

long string::scan(const char* x,char y,char z)
{
	long r = 0;
	while( (x[r]!=0) && (x[r]!=y) &&  (x[r]!=z) )
	{
		++r;
	}
	return math::neg(r,x[r]!=y||x[r]!=z);
}

long string::find(const char* x,const char* y)
{
	long i = 0;
	long j = 0;
	long r = -1;
	while( (x[i]!=0) && (y[j]!=0) )
	{
		r  = math::set(i,r<0&&x[i]==y[0]) | math::set(r,r>=0&&x[i]==y[j]) | math::set(-1,x[i]!=y[j]);
		j  = math::set(j+1,r>=0);
		++i;
	}
	return r;
}

long string::conl(const char* x)
{
	long r = 0;
	long i = length(x)-1;
	for(long d=1;i>=0&&x[i]>='0'&&x[i]<='9';--i,d*=10)
	{
		r += (x[i]-'0')*d;
	}
	return math::neg(r,x[i]=='-'&&i>=0);
}

long string::conh(const char* x)
{
	long r = 0;
	long i = length(x)-1;
	for(long d=1;(i<8),(i>=0);--i,d<<=4)
	{
		r += math::set((x[i]-'0')*d,x[i]>='0'&&x[i]<='9') | math::set((x[i]-'a'+10)*d,x[i]>='a'&&x[i]<='f') | math::set((x[i]-'A'+10)*d,x[i]>='A'&&x[i]<='F'); 
	}
	return r;
}

char* string::cons(long x) //! simplify and x=0
{
	long i  = long(x<0);
	x       = math::abs(x);
	char* r = new char[12];
	r[0]    = math::set('-',i);
	long j  = 0;
	long k  = 0;
	for(long d=1000000000;d>0;x%=d,d/=10,i+=k)
	{
		j    = x/d;
		k   |= j!=0;
		r[i] = j+'0';
	}
	r[i] = 0;
	return r;
}

char* string::conf(fixed x) //! simplify
{
	long i  = long(x<0);
	long h  = i;
	x       = math::abs(x);
	char* r = new char[13];
	r[0]    = math::set('-',i);
	long j  = 0;
	long k  = 0;
	long l  = x>>FX;
	for(long d=10000;d>0;l%=d,d/=10,i+=k)
	{
		j    = l/d;
		k   |= j!=0;
		r[i] = j+'0';
	}
	r[i] = '0';
	i += (i==0) || (i==1&&h==1);
	r[i++] = '.';

	l = x & 0x0000FFFF;
	for(long d=10;d<10000000;d*=10,++i)
	{
		j = ((l*d)>>16)%10;
		r[i] = math::set(j,j>0) +'0';
	}

	r[i] = 0;
	return r;
}

char* string::repl(const char* x,char y,char z)
{
	const long l = length(x);
	char* r = new char[l+1];
	r[l] = 0;
	for(long i=0;i<l;++i)
	{
		r[i] = math::set(z,x[i],x[i]==y);
	}
	return r;
}

void string::repl(char* x,char y,char z)
{
	const long l = length(x);
	for(long i=0;i<l;++i)
	{
		x[i] = math::set(z,x[i],x[i]==y);
	}
}

char** string::split(const char* x,char y)
{
	const long c = count(x,y)+1;
	char* s = repl(x,y,0);
	char** r = new char*[c];
	r[0] = &s[0];
	long i = 0;
	long j = 1;
	while(x[i]!=0)
	{
		if(s[i]==0) { r[j] = &s[i+1]; }
		j += s[i++]==0;
	}
	return r;
}

char* string::trim(const char* x,char y)
{
	const long l = length(x);
	const long c = count(x,y);
	char* r = new char[l-c+1];
	long i = 0;
	long j = 0;
	bool k = 0;
	while(x[i]!=0)
	{
		k = x[i]!=y;
		r[j] = math::set(x[i],r[j],k);
		j+=k;
		i++;
	}
	r[j] = 0;
	return r;
}

char string::toup(char x)
{
	return math::set('A',math::set(x-32,x,x>=97),x<65); 
}
///*

#endif

