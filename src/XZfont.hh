///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Font Library ( XZfont.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZstring.hh"
#include "XZmath.hh"
#include "XZformat.hh"
#include "XZscreen.hh"
///</include>

///<define>
namespace font
{
	namespace
	{
		#include "../dat/xzfont.xpm"
		tile f = format::xpm(font);
	}

	            xint draw(xint x,xint y,char a,yint c,yint b);
	            void draw(xint x,xint y,const char* a,yint c,yint b);
	       pure xint width(char x);
	       pure xint width(const char* x);
	inline pure xint height(const char* x);
	inline pure xint height();
	inline pure char map(char a);
}
///</define>

///<code>
xint font::draw(xint x,xint y,char a,yint c,yint b) 
{
	a = map(a);

	const bool bt = (b!=TRANS);
	const bool ct = (c!=TRANS);

	const xint xd = -XRES + x + f.height;
	const xint yd = -YRES + y + f.height;

	const xint h  = f.height - math::set(yd,yd>0);
	const xint w  = f.height - math::set(xd,xd>0);

	const xint sx = f.width - w;
	const xint dx = XRES - w;

	xint r = 0;

	for(xint i=0,o=y*XRES+x,s=f.height*a;i<h;++i)
	{
		for(xint j=0;j<w;++j,++o)
		{
			const yint e = f.data[s++];
			screen::frame[o] = math::set(c,math::set(b,screen::frame[o],bt&&(e==WHITE)),ct&&(e==RED));
			r += (i==0)&&(e!=BLACK);
		}

		o += dx;
		s += sx;
	}
	return r;
}

void font::draw(xint x,xint y,const char* a,yint c,yint b)
{
	const xint z = x;
	char curr = a[0];
	for(xint i=0;curr!=0;++i)
	{
		if(curr=='\n') { y += f.height; x = z; } else { x += draw(x,y,curr,c,b); }
		curr = a[i+1];
	}
}

xint font::width(char x)
{
	x = map(x);
	const xint h = f.height;
	xint r = 0;

	for(xint j=0,s=h*x;j<h;++j)
	{
		r += (f.data[s++]!=BLACK);
	}
	return r;
}

xint font::width(const char* x)
{
	char** b = string::split(x,'\n');
	const xint c = string::count(x,'\n')+1;
	xint r = 0;

	for(xint i=0;i<c;++i)
	{
		const xint l = string::length(b[i]);
		xint s = 0;
		for(xint j=0;j<l;++j)
		{
			s += width(b[i][j]);
		}
		r = math::max(r,s);
	}
	delete b;
	return r;
}

xint font::height(const char* x)
{
	return (string::count(x,'\n')+1)*f.height;
}

xint font::height()
{
	return f.height;
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

