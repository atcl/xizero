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
#include "XZsystem.hh"
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
	class type
	{
		static tile* f[4];

		friend void init(const char* x,long i);
		friend long draw(long x,long y,char a,long c,long b,long t);
		friend void draw(long x,long y,const char* a,long c,long b,long t);
		friend long width(char x,long t);
		friend long width(const char* x,long t);
		friend long height(const char* x,long t);
		friend long height(long t);
	};

	INLINE void init(const char* x,long i);
	       long draw(long x,long y,char a,long c,long b,long t);
	       void draw(long x,long y,const char* a,long c,long b,long t);
	       long width(char x,long t);
	       long width(const char* x,long t);
	INLINE long height(const char* x,long t);
	INLINE long height(long t);
}
///*

///implementation
tile* font::type::f[4] = { 0,0,0,0 };

void font::init(const char* x,long i)
{
	type::f[i] = format::ras(x);
}

long font::draw(long x,long y,char a,long c,long b,long t) 
{
	const long h = type::f[t]->height;
	const long w = type::f[t]->width-h;
	const long d = XRES - h;
	long s = h*a;
	long o = y*XRES+x;
	long r = 0;

	for(long i=0;i<h;++i,o+=d,s+=w)
	{
		for(long j=0;j<h;++j,++o,++s)
		{
			const long e = type::f[t]->data[s];
			screen::back[o] = math::set(c,math::set(b,screen::back[o],e==WHITE&&b!=TRANS),e==BLUE&&c!=TRANS);
			r+=(i==0)&&(e!=BLACK);
		}
	}
	return r-1;
}

void font::draw(long x,long y,const char* a,long c,long b,long t)
{
	const long l = string::length(a);

	for(long i=0;i<l;++i)
	{
		if(a[i]=='\n')
		{
			y += type::f[t]->height;
		}
		else
		{
			x += draw(x,y,a[i],c,b,t);
		}
	}
}

long font::width(char x,long t)
{
	const long h = type::f[t]->height;
	long s = h*x;
	long r = 0;

	for(long j=0;j<h;++j,++s)
	{
		r+=(type::f[t]->data[s]!=BLACK);
	}
	return r-1;
}

long font::width(const char* x,long t)
{
	char** b = string::split(x,'\n');
	const long c = string::count(x,'\n')+1;
	long r = 0;
	for(long i=0;i<c;++i)
	{
		const long l = string::length(b[i]);
		long s = 0;
		for(long j=0;j<l;++j)
		{
			s += width(b[i][j],t);
		}
		r = math::max(r,s);
	}

	return r;
}

long font::height(const char* x,long t)
{
	return (string::count(x,'\n')+1)*type::f[t]->height;
}

long font::height(long t)
{
	return type::f[t]->height;
}
///*

#endif

