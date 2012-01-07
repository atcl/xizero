// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZfont.hh
// Font Library 

///guard
#ifndef HH_XZFONT
#define HH_XZFONT
///*

///includes
#include "XZbasic.hh"
#include "XZscreen.hh"
#include "XZstring.hh"
#include "XZmath.hh"
#include "XZformat.hh"
///*

///declarations
#define SYSFONT 0
#define BIGFONT 1
#define SEGFONT 2
#define SYMFONT 3
///*

///definitions
namespace font
{
	tile* f[4];
	INLINE void init(const char* x,long i);
	       long draw(long x,long y,char a,long c,long b,long t);
	       void draw(long x,long y,const char* a,long c,long b,long t);
	       long width(const char* x,long t);
	INLINE long height(const char* x,long t);
	INLINE long height(long t);
}
///*

///implementation
void font::init(const char* x,long i)
{
	f[i] = format::ras(x);
}

long font::draw(long x,long y,char a,long c,long b,long t) 
{
	const long h = f[t]->height;
	const long w = f[t]->width-h;
	const long d = XRES - h;
	long s = a*h;
	long o = y*XRES+x;
	long r = 0;

	for(ulong i=0;i<h;++i,o+=d,s+=w)
	{
		for(ulong j=0;j<h;++j,++o,++s)
		{
			const long e = f[t]->data[s];
			screen::back[o] = math::set(c,math::set(b,screen::back[o],e==WHITE&&b!=TRANS),e==BLUE&&b!=TRANS);
			r+=(i==0)&&(e!=BLACK);
		}
	}
	return r-1;
}

void font::draw(long x,long y,const char* a,long c,long b,long t)
{
	const long l = string::length(a);

	for(ulong i=0;i<l;++i)
	{
		if(a[i]=='\n')
		{
			y += f[t]->height;
		}
		else
		{
			x += draw(x,y,a[i],c,b,t);
		}
	}
}

long font::width(const char* x,long t)
{
	char** b = string::split(x,'\n');
	const long c = string::count(x,'\n');
	long r = string::length(b[0]);
	for(ulong i=1;i<c;++i)
	{
		r = math::max(string::length(b[i]),string::length(b[i-1]));
	}
	return r*f[t]->height;
}

long font::height(const char* x,long t)
{
	return (string::count(x,'\n')+1)*f[t]->height;
}

long font::height(long t)
{
	return f[t]->height;
}
///*

#endif

