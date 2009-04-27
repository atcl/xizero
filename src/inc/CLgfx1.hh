//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGFX1
#define HH_CLGFX1
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLgfx1 : public virtual CLcl
{
	protected: 
		CLbuffer<xlong>* doublebuffer;

	private:
		static xlong last_p;
		static xlong interval_p;
		static xlong last_s;
		static xlong interval_s;

		xlong locmaxx;
		xlong locmaxy;

		void drawcirclepixel(xlong x,xlong y,uxlong c);
		
	public:
		CLgfx1(CLbuffer<xlong>* db);
		~CLgfx1();
		uxlong readpixel(xlong x,xlong y);
		void drawpixel(xlong x,xlong y,uxlong c);
		void drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
		void copypixel(xlong x1,xlong y1,xlong x2,xlong y2);
		void drawbigpixel(xlong x,xlong y,uxlong c);
		void putpixel(xlong x,xlong y,uxlong c,xlong m);
		void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
		void drawhorline(xlong x1,xlong y1,xlong x2,uxlong c);
		void drawverline(xlong x1,xlong y1,xlong y2,uxlong c);
		void drawdialine(xlong x1,xlong y1,xlong xy,uxlong c);
		void drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c);
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c);
		void drawcircle(xlong xc,xlong yc,xlong r,uxlong c);
		void drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
		void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c);
		void fill(xlong x,xlong y,uxlong c);
		void fillframe(xlong x,xlong y,uxlong fc,uxlong c);
		xlong getspritesize(xlong* s);
		xlong getspritewidth(xlong* s);
		xlong getspriteheight(xlong* s);
		void drawsprite(xlong x,xlong y,xlong* s);
		void drawspritescaled(xlong x,xlong y,xlong* s,xlong z);
		void drawspriterotated(xlong x,xlong y,xlong* s,xlong d);
		void drawspriterotated90(xlong x,xlong y,xlong* s,xlong c);
		void drawspritemirrored(xlong x,xlong y,xlong* s,xlong b);
		void drawspriteanimated(xlong x,xlong y,xlong** s,xlong i);
		void putsprite(xlong x,xlong y,xlong* s,xlong m);
		void drawscreen(xlong* s);
		void drawtile(xlong x,xlong y,xlong tx,xlong ty,xlong *s);
};

void CLgfx1::drawcirclepixel(xlong x,xlong y,uxlong c)
{

}

CLgfx1::CLgfx1(CLbuffer<xlong>* db)
{
	doublebuffer = db;
}

CLgfx1::~CLgfx1() { }

uxlong CLgfx1::readpixel(xlong x,xlong y)
{
	return ((*doublebuffer)[(y*xres)+y]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c)
{
	(*doublebuffer)[(y*xres)+x] = c;
}

void CLgfx1::drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c)
{
	b[(y*xres)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2)
{
	(*doublebuffer)[(y1*xres)+x1] = (*doublebuffer)[(y2*xres)+x2];
}

void CLgfx1::drawbigpixel(xlong x,xlong y,uxlong c)
{
	(*doublebuffer)[(y*xres)+x] = c;
	(*doublebuffer)[(y*xres)+x+1] = c;
	(*doublebuffer)[((y+1)*xres)+x] = c;
	(*doublebuffer)[((y+1)*xres)+(x+1)] = c;
}

void CLgfx1::putpixel(xlong x,xlong y,uxlong c,xlong m)
{
	switch(m)
	{
		case 1: //AND
			(*doublebuffer)[(y*xres)+x] = (*doublebuffer)[(y*xres)+x] && c;
		break;
		case 2: //OR:
			(*doublebuffer)[(y*xres)+x] = (*doublebuffer)[(y*xres)+x] || c;
		break;
		case 3: //XOR:
			(*doublebuffer)[(y*xres)+x] = (*doublebuffer)[(y*xres)+x] ^ c;
		break;
		default:
			(*doublebuffer)[(y*xres)+x] = c;
	}

}

void CLgfx1::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i)
{

}

void CLgfx1::drawhorline(xlong x1,xlong y1,xlong x2,uxlong c)
{
	xlong offsetbase = (y1*xres);

	for(int i=x1; i<=x2; i++)
	{
		
		(*doublebuffer)[offsetbase+i] = c;
	}
}

void CLgfx1::drawverline(xlong x1,xlong y1,xlong y2,uxlong c)
{
	xlong offsetbase = (y1*xres)+x1;

	for(int i=y1; i<=y2; i++)
	{
		(*doublebuffer)[offsetbase] = c;
		offsetbase+=xres;
	}
}

void CLgfx1::drawdialine(xlong x1,xlong y1,xlong xy,uxlong c)
{
	xlong offsetbase = (y1*xres);

	for(int i=x1; i<=xy; i++)
	{
		(*doublebuffer)[offsetbase+i] = c;
		offsetbase+=xres+1;
	}
}

void CLgfx1::drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	if(x1==x2 && y1==y2) return;

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	xlong temp;

	if(dx<0)
	{
		dx = -dx;
		xs = -xs;
	}


	if(dy<0)
	{
		dy = -dy;
		ys = -ys;
	}

	if(dy > dx)
	{
		dx ^= dy ^= dx ^= dy;
		xs ^= ys ^= xs ^= ys;
	}

	len = dx+1;
	e = dy;

	for(int i=0; i<len; i++)
	{
		(*doublebuffer)[off] = c;
		off += xs;
		e += dy;
		if(e >= dx)
		{
			e -= dx;
			off += ys;
		}
	}
}

void CLgfx1::drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{

}

void CLgfx1::drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c)
{

}

void CLgfx1::drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	drawhorline(x1,y1,x2,c);
	drawhorline(x1,y2,x2,c);
	drawverline(x1,y1,y2,c);
	drawverline(x2,y1,y2,c);
}

void CLgfx1::drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	for(int i=y1; i<=y2; i++)
	{
		drawhorline(x1,i,x2,c);
	}
}

void CLgfx1::drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c)
{
	drawanyline(x1,y1,x2,y2,c);
	drawanyline(x2,y2,x3,y3,c);
	drawanyline(x3,y3,x4,y4,c);
	drawanyline(x4,y4,x1,y1,c);
}

#endif

