///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZfont.hh
// Font Library 
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZstring.hh"
#include "XZmath.hh"
#include "XZformat.hh"
#include "XZfrmbuf.hh"
///</include>

///<define>
namespace font
{
	namespace
	{
		#include "../dat/xzfont.xpm"
		tile* f = format::xpm(font);
	}

	            sint draw(sint x,sint y,char a,uint c,uint b);
	            void draw(sint x,sint y,const char* a,uint c,uint b);
	       pure sint width(char x);
	       pure sint width(const char* x);
	inline pure sint height(const char* x);
	inline pure sint height();
	inline pure char map(char a);
}
///</define>

///<code>
sint font::draw(sint x,sint y,char a,uint c,uint b) 
{
	a = map(a);
	const bool bt = b!=TRANS;
	const bool ct = c!=TRANS;
	const sint xd = -XRES+x+f->height;
	const sint yd = -YRES+y+f->height;
	const sint h = f->height-math::set(yd,yd>0);
	const sint w = f->height-math::set(xd,xd>0);
	const sint sx = f->width-w;
	const sint d = XRES - w;
	sint r = 0;

	for(sint i=0,o=y*XRES+x,s=f->height*a;i<h;++i,o+=d,s+=sx)
	{
		for(sint j=0;j<w;++j,++o,++s)
		{
			const uint e = f->data[s];
			screen::back[o] = math::set(c,math::set(b,screen::back[o],bt&&(e==WHITE)),ct&&(e==RED));
			r += (i==0)&&(e!=BLACK);
		}
	}
	return r;
}

void font::draw(sint x,sint y,const char* a,uint c,uint b)
{
	const sint z = x;
	char curr = a[0];
	for(sint i=0;curr!=0;++i)
	{
		switch(curr)
		{
			case '\n': y += f->height; x = z; break;
			default: x += draw(x,y,curr,c,b);
		}
		curr = a[i+1];
	}
}

sint font::width(char x)
{
	x = map(x);
	const sint h = f->height;
	sint r = 0;

	for(sint j=0,s=h*x;j<h;++j,++s)
	{
		r += (f->data[s]!=BLACK); //++s to data[s++]
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
///</code>

