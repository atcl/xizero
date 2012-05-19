// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
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
#include "XZresource.hh"
#include "XZsystem.hh"
///*

///definitions
namespace font
{
	namespace
	{
		tile* f = format::xpm(resource::font);
	}

	            sint draw(sint x,sint y,char a,uint c,uint b);
	            void draw(sint x,sint y,const char* a,uint c,uint b);
	       pure sint width(char x);
	       pure sint width(const char* x);
	inline pure sint height(const char* x);
	inline pure sint height();
	inline pure char map(char a);
}
///*

///implementation
sint font::draw(sint x,sint y,char a,uint c,uint b) 
{
	a = map(a);
	const sint h = f->height;
	const sint w = f->width-h;
	const sint d = XRES - h;
	sint s = h*a;
	sint o = y*XRES+x;
	sint r = 0;

	//min max check for width and height! (will resolve memcheck issues)

	const bool bt = b!=TRANS;
	const bool ct = c!=TRANS;

	for(sint i=0;i<h;++i,o+=d,s+=w)
	{
		for(sint j=0;j<h;++j,++o,++s)
		{
			const uint e = f->data[s]; //memcheck: Use of uninitialised value of size 4
			screen::back[o] = math::set(c,math::set(b,screen::back[o],bt&&(e==WHITE)),ct&&(e==RED)); //memcheck: Use of uninitialised value of size 4 
			r+=(i==0)&&(e!=BLACK);
		}
	}
	return r;
}

void font::draw(sint x,sint y,const char* a,uint c,uint b)
{
	const sint z = x;
	sint i = 0;
	while(a[i]!=0)
	{
		switch(a[i])
		{
			case '\n': y += f->height; x = z; break;
			default: x += draw(x,y,a[i],c,b);
		}
		++i;
	}
}

sint font::width(char x)
{
	x = map(x);
	const sint h = f->height;
	sint s = h*x;
	sint r = 0;

	for(sint j=0;j<h;++j,++s)
	{
		r += (f->data[s]!=BLACK);
	}
	return r;
}

sint font::width(const char* x)
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
			s += width(b[i][j]);
		}
		r = math::max(r,s);
	}
	delete b;
	return r;
}

sint font::height(const char* x)
{
	return (string::count(x,'\n')+1)*f->height;
}

sint font::height()
{
	return f->height;
}

char font::map(char a)
{
	switch(a)
	{
		case 'A'...'Z': return a-'A';
		case 'a'...'z': return a-'a'+26;
		case '0'...'9': return a-'0'+52;
		case '.': return 62;
		default: return 63;
	}
}
///*

#endif

