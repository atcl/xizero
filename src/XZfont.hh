// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZfont.hh
// Font Library 

///guard
#ifndef HH_XZFONT
#define HH_XZFONT
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
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
	namespace
	{
		tile* f[4] = { 0,0,0,0 };
	}

	inline void init(const char* x,sint i);
	       sint draw(sint x,sint y,char a,sint c,sint b,sint t);
	       void draw(sint x,sint y,const char* a,sint c,sint b,sint t);
	       sint width(char x,sint t);
	       sint width(const char* x,sint t);
	inline sint height(const char* x,sint t);
	inline sint height(sint t);
}
///*

///implementation
void font::init(const char* x,sint i)
{
	f[i] = format::ras(x);
}

sint font::draw(sint x,sint y,char a,sint c,sint b,sint t) 
{
	const sint h = f[t]->height;
	const sint w = f[t]->width-h;
	const sint d = XRES - h;
	sint s = h*a;
	sint o = y*XRES+x;
	sint r = 0;

	for(sint i=0;i<h;++i,o+=d,s+=w)
	{
		for(sint j=0;j<h;++j,++o,++s)
		{
			const sint e = f[t]->data[s];
			screen::back[o] = math::set(c,math::set(b,screen::back[o],e==WHITE&&b!=TRANS),e==BLUE&&c!=TRANS);
			r+=(i==0)&&(e!=BLACK);
		}
	}
	return r-1;
}

void font::draw(sint x,sint y,const char* a,sint c,sint b,sint t)
{
	const sint z = x;
	sint i = 0;
	while(a[i]!=0)
	{
		if(a[i]=='\n')
		{
			y += f[t]->height;
			x = z-f[t]->height;
		}
		else
		{
			x += draw(x,y,a[i],c,b,t);
		}
		++i;
	}
}

sint font::width(char x,sint t)
{
	const sint h = f[t]->height;
	sint s = h*x;
	sint r = 0;

	for(sint j=0;j<h;++j,++s)
	{
		r+=(f[t]->data[s]!=BLACK);
	}
	return r-1;
}

sint font::width(const char* x,sint t)
{
	char** b = string::split(x,'\n');
	const sint c = string::count(x,'\n')+1;
	sint r = 0;
	for(sint i=0;i<c;++i)
	{
		const sint l = string::length(b[i]);
		sint s = 0;
		for(sint j=0;j<l;++j)
		{
			s += width(b[i][j],t);
		}
		r = math::max(r,s);
	}

	return r;
}

sint font::height(const char* x,sint t)
{
	return (string::count(x,'\n')+1)*f[t]->height;
}

sint font::height(sint t)
{
	return f[t]->height;
}
///*

#endif

