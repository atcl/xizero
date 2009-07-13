//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGFX1
#define HH_CLGFX1
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"


//add clipping: just break the draw loops if screen is left.

namespace CLgfx1
{
	static xlong last_p;
	static xlong interval_p;
	static xlong last_s;
	static xlong interval_s;
	xlong locmaxx;
	xlong locmaxy;

	void drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c);
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
	void fill(xlong x,xlong y,uxlong oc,uxlong nc);
	void fillframe(xlong x,xlong y,uxlong fc,uxlong c);
	void drawsprite(xlong x,xlong y,sprite* s);
	void drawspritescaled(xlong x,xlong y,sprite* s,xlong z);
	void drawspriterotated(xlong x,xlong y,sprite* s,xlong d);
	void drawspriterotated90(xlong x,xlong y,sprite* s,xlong c);
	void drawspritemirrored(xlong x,xlong y,sprite* s,xlong b);
	void drawspriteanimated(xlong x,xlong y,sprites* s,xlong i);
	void putsprite(xlong x,xlong y,sprite* s,xlong m);
	void drawscreen(sprite* s);
	void drawtile(xlong x,xlong y,sprites *s,xlong ti);
}


void CLgfx1::drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c)
{
	xlong b1 = (yc*xres)+xc;
	xlong b2 = (xc*xres)+yc;
 	xlong a1 = y*xres;
 	xlong a2 = x*xres;
 	(*CLdoublebuffer)[b1+a1+x] = c;
 	(*CLdoublebuffer)[b1-a1+x] = c;
 	(*CLdoublebuffer)[b1+a1-x] = c;
 	(*CLdoublebuffer)[b1-a1-x] = c;
 	(*CLdoublebuffer)[b2+a2+y] = c;
 	(*CLdoublebuffer)[b2-a2+y] = c;
 	(*CLdoublebuffer)[b2+a2-y] = c;
 	(*CLdoublebuffer)[b2-a2-y] = c;
}

uxlong CLgfx1::readpixel(xlong x,xlong y)
{
	return ((*CLdoublebuffer)[(y*xres)+x]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c)
{
	(*CLdoublebuffer)[(y*xres)+x] = c;
}

void CLgfx1::drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c)
{
	b[(y*xres)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2)
{
	(*CLdoublebuffer)[(y1*xres)+x1] = (*CLdoublebuffer)[(y2*xres)+x2];
}

void CLgfx1::drawbigpixel(xlong x,xlong y,uxlong c)
{
	(*CLdoublebuffer)[(y*xres)+x] = c;
	(*CLdoublebuffer)[(y*xres)+x+1] = c;
	(*CLdoublebuffer)[((y+1)*xres)+x] = c;
	(*CLdoublebuffer)[((y+1)*xres)+(x+1)] = c;
}

void CLgfx1::putpixel(xlong x,xlong y,uxlong c,xlong m)
{
	switch(m)
	{
		case 1: //AND
			(*CLdoublebuffer)[(y*xres)+x] = (*CLdoublebuffer)[(y*xres)+x] && c;
		break;
		case 2: //OR:
			(*CLdoublebuffer)[(y*xres)+x] = (*CLdoublebuffer)[(y*xres)+x] || c;
		break;
		case 3: //XOR:
			(*CLdoublebuffer)[(y*xres)+x] = (*CLdoublebuffer)[(y*xres)+x] ^ c;
		break;
		default:
			(*CLdoublebuffer)[(y*xres)+x] = c;
	}

}

void CLgfx1::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i)
{

}

void CLgfx1::drawhorline(xlong x1,xlong y1,xlong x2,uxlong c)
{
	xlong offsetbase = (y1*xres);

	xlong a = x1;
	xlong b = x2;

	if(a>b) a ^= b ^= a ^= b;

	for(int i=a; i<=b; i++)
	{
		
		(*CLdoublebuffer)[offsetbase+i] = c;
	}
}

void CLgfx1::drawverline(xlong x1,xlong y1,xlong y2,uxlong c)
{
	xlong a = y1;
	xlong b = y2;

	if(a>b) a ^= b ^= a ^= b;

	xlong offsetbase = (a*xres)+x1;

	for(int i=a; i<=b; i++)
	{
		(*CLdoublebuffer)[offsetbase] = c;
		offsetbase+=xres;
	}
}

void CLgfx1::drawdialine(xlong x1,xlong y1,xlong xy,uxlong c)
{
	xlong offsetbase = (y1*xres)+x1;

	for(int i=0; i<xy; i++)
	{
		(*CLdoublebuffer)[offsetbase] = c;
		offsetbase+=xres+1;
	}
}

void CLgfx1::drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	if(x1==x2 && y1==y2) return;

	if(x1<0) x1=0;
	if(y1<0) y1=0;
	if(x2<0) x2=0;
	if(y2<0) y2=0;
	if(x1>xres-1) x1=xres-1;
	if(y1>yres-1) y1=yres-1;
	if(x2>xres-1) x2=xres-1;
	if(y2>yres-1) y2=yres-1;

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	xlong temp = 0;

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
		(*CLdoublebuffer)[off] = c;
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
	if(x1==x2 && y1==y2) return;

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	xlong temp = 0;

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
		(*CLdoublebuffer)[off] = c;

		(*CLdoublebuffer)[off-1]      = c; //adjust colors 
		(*CLdoublebuffer)[off+1]      = c;
		(*CLdoublebuffer)[off-xres-1] = c;
		(*CLdoublebuffer)[off-xres]   = c;
		(*CLdoublebuffer)[off-xres+1] = c;
		(*CLdoublebuffer)[off+xres-1] = c;
		(*CLdoublebuffer)[off+xres]   = c;
		(*CLdoublebuffer)[off+xres+1] = c; //*
		off += xs;
		e += dy;
		if(e >= dx)
		{
			e -= dx;
			off += ys;
		}
	}
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

void CLgfx1::drawcircle(xlong xc,xlong yc,xlong r,uxlong c)
{
	xlong d = 3 - (r<<1);
	xlong cx = 0;
	xlong cy = r;

	while(cx<=cy)
	{
		drawcirclepixel(xc,yc,cx,cy,c);
		if(d<0) d += (cx*4)+6;
		else { d += ((cx-cy)*4)+10; cy--; }
		cx++; 
	}
}

void CLgfx1::drawanticircle(xlong xc,xlong yc,xlong r,uxlong c)
{
	drawcircle(xc,yc,r,c);
	drawcircle(xc,yc,r+1,c); //adjust color
	drawcircle(xc,yc,r-1,c); //adjust color
}

void CLgfx1::drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c)
{

}

void CLgfx1::fill(xlong x,xlong y,uxlong oc,uxlong nc)
{
	if (readpixel(x,y)!=oc) return;

	drawpixel(x,y,nc);

	if(y<yres) fill(x,y+1,oc,nc);
	if(y>0)    fill(x,y-1,oc,nc);
	if(x<xres) fill(x+1,y,oc,nc);
	if(x>0)    fill(x-1,y,oc,nc);

	return;
}

void CLgfx1::fillframe(xlong x,xlong y,uxlong fc,uxlong c)
{
	if(readpixel(x,y)==fc || readpixel(x,y)==c) return;

	drawpixel(x,y,c);
	
	if(y<yres) fillframe(x,y+1,fc,c);
	if(y>0) fillframe(x,y-1,fc,c);
	if(x<xres) fillframe(x+1,y,fc,c);
	if(x>0) fillframe(x-1,y,fc,c);

	return;
}

void CLgfx1::drawsprite(xlong x,xlong y,sprite* s)
{
	//init
	if(x>xres || y>yres) return;
	xlong ssize = s->size;
	xlong swidth = s->width;
	xlong sheight = s->height;
	xlong hordiff = xres - s->width;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	if(xe<0 || ye<0) return;

	//clipping
	if(xs<0) xs = 0;
	if(xe>xres) xe = xres-1;
	if(ys<0) ys = 0;
	if(ye>yres) ye = yres-1;

	//draw vars
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * xres) + xs;
	xlong linearc = 0;

	//drawloop
	for(int i=0; i<eheight ;i++)
	{
		for(int j=0; j<ewidth ;j++)
		{
			if( (s->data[linearc] && 0xFF000000) != 0xFF)
			{
				(*CLdoublebuffer)[xoffset+j] = s->data[linearc];
			}
			linearc++;
		}
		xoffset += xres;
	}
	
}

void CLgfx1::drawspritescaled(xlong x,xlong y,sprite* s,xlong z)
{

}

void CLgfx1::drawspriterotated(xlong x,xlong y,sprite* s,xlong d)
{

}

void CLgfx1::drawspriterotated90(xlong x,xlong y,sprite* s,xlong c)
{

}

void CLgfx1::drawspritemirrored(xlong x,xlong y,sprite* s,xlong b)
{

}

void CLgfx1::drawspriteanimated(xlong x,xlong y,sprites* s,xlong i)
{

}

void CLgfx1::putsprite(xlong x,xlong y,sprite* s,xlong m)
{

}

void CLgfx1::drawscreen(sprite* s)
{
	if(s->width==xres && s->height==yres)
	{
		for(int i=0; i<s->size; i++)
		{
			(*CLdoublebuffer)[i] = s->data[i];
		}
	}
}

void CLgfx1::drawtile(xlong x,xlong y,sprites *s,xlong ti)
{
	//find tile
	xlong toffset = (ti / s->perrow) * s->tilesize;
	toffset += (ti % s->perrow) * (s->tilewidth-1);

	//init
	if(x>xres || y>yres) return;
	xlong tsize = s->tilesize;
	xlong twidth = s->tilewidth;
	xlong theight = s->tileheight;
	xlong hordiff = xres - s->tilewidth;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + twidth;
	xlong ye = y + theight;
	if(xe<0 || ye<0) return;

	//clipping
	if(xs<0) xs = 0;
	if(xe>xres) xe = xres-1;
	if(ys<0) ys = 0;
	if(ye>yres) ye = yres-1;

	//draw vars
	xlong ewidth = xe - xs + 1;
	xlong eheight = ye - ys + 1;
	xlong eoffset = (y - ys) * twidth;
	xlong xoffset = (ys * xres) + xs + (xs - x); //optimize!
	xlong linearc = toffset;
	xlong shordiff = s->width - s->tilewidth;

	//drawloop
	for(int i=0; i<eheight ;i++)
	{
		for(int j=0; j<ewidth ;j++)
		{
			if(s->data[linearc] && 0xFF000000 != 0xFF)
			{
				(*CLdoublebuffer)[xoffset+j] = s->data[linearc];
			}
			linearc++;
		}
		xoffset += hordiff;
		toffset += shordiff;
	}
}

#endif

