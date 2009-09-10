//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGFX1
#define HH_CLGFX1
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLfifo.hh"
#include "CLmath.hh"


//add clipping: just break the draw loops if screen is left.

inline bool isoff(xlong x,xlong y)
{
	return (x<0 || x>=xres || y<0 || y>=yres);
}

inline void clip(xlong& x,xlong& y)
{
	if(x<0) x=0;
	if(x>=xres) x=xres-1;
	if(y<0) y=0;
	if(y>=yres) y=yres-1;
}

namespace CLgfx1
{
	//~ xlong last_p;
	//~ xlong interval_p;
	//~ xlong last_s;
	//~ xlong interval_s;

	void drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c);
	void drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c);
	uxlong readpixel(xlong x,xlong y);
	void drawpixel(xlong x,xlong y,uxlong c);
	void drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
	void copypixel(xlong x1,xlong y1,xlong x2,xlong y2);
	void drawbigpixel(xlong x,xlong y,uxlong c);
	void putpixel(xlong x,xlong y,uxlong c,xlong m);
	void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
	void drawhorline(xlong x1,xlong y1,xlong x2,uxlong c);
	void drawverline(xlong x1,xlong y1,xlong y2,uxlong c);
	void drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
	void drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
	void drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong r,uxlong c);
	void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
	void drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
	void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c);
	void drawcircle(xlong xc,xlong yc,xlong r,uxlong c);
	void drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
	void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c);
	void fill(xlong x,xlong y,uxlong oc,uxlong nc);
	void fillframe(xlong x,xlong y,uxlong fc,uxlong nc);
	void drawsprite(xlong x,xlong y,sprite* s);
	void drawspriteanimated(xlong x,xlong y,sprites* s,xlong i);
	void putsprite(xlong x,xlong y,sprite* s,xlong m,float e=0);
	void drawscreen(sprite* s);
	void drawtile(xlong x,xlong y,sprites *s,xlong ti);
};


void CLgfx1::drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c)
{
	//precalculate linear address components (especially multiplications)
	xlong b1 = (yc*xres)+xc;
 	xlong a1 = y*xres;
 	xlong a2 = x*xres;
	//*
	
	//draw the eight pixels for each (1/8) section of the circle
 	(*CLdoublebuffer)[b1+a1+x] = c;
 	(*CLdoublebuffer)[b1-a1+x] = c;
 	(*CLdoublebuffer)[b1+a1-x] = c;
 	(*CLdoublebuffer)[b1-a1-x] = c;
 	(*CLdoublebuffer)[b1+a2+y] = c;
 	(*CLdoublebuffer)[b1-a2+y] = c;
 	(*CLdoublebuffer)[b1+a2-y] = c;
 	(*CLdoublebuffer)[b1-a2-y] = c;
	//*
}

void CLgfx1::drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c)
{
	//precalculate linear address components (especially multiplications) 
	xlong a = (yc*xres)+xc;
	xlong b = (y*xres);
	//*

	//draw the four pixels for each (1/4) section of the ellipse
	(*CLdoublebuffer)[a+x+b] = c;
	(*CLdoublebuffer)[a-x+b] = c;
	(*CLdoublebuffer)[a-x-b] = c;
	(*CLdoublebuffer)[a+x-b] = c;
	//*
}

uxlong CLgfx1::readpixel(xlong x,xlong y)
{
	if(isoff(x,y)) return 0;
	return ((*CLdoublebuffer)[(y*xres)+x]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c)
{
	clip(x,y);
	(*CLdoublebuffer)[(y*xres)+x] = c;
}

void CLgfx1::drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c)
{
	clip(x,y);
	b[(y*xres)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2)
{
	if(isoff(x1,x2)||isoff(x2,y2)) return;
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
	clip(x,y);
	
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
	clip(x1,y1);
	clip(x2,y1);
	
	xlong offsetbase = (y1*xres);

	xlong a = x1;
	xlong b = x2;

	if(a>b) a ^= b ^= a ^= b;

	for(uxlong i=a; i<=b; i++)
	{
		
		(*CLdoublebuffer)[offsetbase+i] = c;
	}
}

void CLgfx1::drawverline(xlong x1,xlong y1,xlong y2,uxlong c)
{
	clip(x1,y1);
	clip(x1,y2);
	
	xlong a = y1;
	xlong b = y2;

	if(a>b) a ^= b ^= a ^= b;

	xlong offsetbase = (a*xres)+x1;

	for(uxlong i=a; i<=b; i++)
	{
		(*CLdoublebuffer)[offsetbase] = c;
		offsetbase+=xres;
	}
}

void CLgfx1::drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	//check if necessary to draw at all
	if(x1==x2 && y1==y2) return;
	//*

	//clip line against screen borders
	clip(x1,y1);
	clip(x2,y2);
	//*

	//set up variables
	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e = 0;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	//*

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

	//draw loop
	for(uxlong i=0; i<len; i++)
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
	//*
}

void CLgfx1::drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	//check if necessary to draw at all
	if(x1==x2 && y1==y2) return;
	//*
	
	//clip line against screen borders
	clip(x1,y1);
	clip(x2,y2);
	//*

	//set up variables
	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e = 0;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	//*

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

	//draw loop
	for(uxlong i=0; i<len; i++)
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
	//*
}

void CLgfx1::drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong r,uxlong c)
{
	//find circle center
	xlong xc = 0;
	xlong yc = 0;
	
	//*
	
	//find circle angles
	
	
	//*
	
	//draw the arc
	
	
	//*
}

void CLgfx1::drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	//draw outline of rectangle
	drawhorline(x1,y1,x2,c);
	drawhorline(x1,y2,x2,c);
	drawverline(x1,y1,y2,c);
	drawverline(x2,y1,y2,c);
	//*
}

void CLgfx1::drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	//make sure x1<x2 and y1<y2
	if(x2<x1) x1 ^= x2 ^= x1 ^= x2;
	if(y2<y1) y1 ^= y2 ^= y1 ^= y2;
	//*
	
	//decide wether to use horizontal or vertical lines
	if( (x2-x1)>(y2-y1) )
	{
		//longer horizontal runs, so use horizontal lines
		for(xlong i=y1; i<=y2; i++)
		{
			drawhorline(x1,i,x2,c);
		}
		//*
	}
	else
	{
		//longer vertical lines, so use vertical lines
		for(xlong i=x1; i<=x2; i++)
		{
			drawverline(i,y1,y2,c);
		}
		//*
	}
	//*
}

void CLgfx1::drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c)
{
	//draw outline of four-sided polygon
	drawanyline(x1,y1,x2,y2,c);
	drawanyline(x2,y2,x3,y3,c);
	drawanyline(x3,y3,x4,y4,c);
	drawanyline(x4,y4,x1,y1,c);
	//*
}

void CLgfx1::drawcircle(xlong xc,xlong yc,xlong r,uxlong c)
{
	//center and radius clipping
	// ...
	//*
	
	xlong d = 3 - (r<<1);
	xlong cx = 0;
	xlong cy = r;

	while(cx<=cy)
	{
		drawcirclepixel(xc,yc,cx,cy,c);
		if(d<0) d += (cx<<2)+6;
		else { d += ((cx-cy)<<2)+10; cy--; }
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
	xlong x = r1;
	xlong y = 0;
	xlong xd = r2 * r2 * (1 - (r1<<1));
	xlong yd = r1 * r1;
	xlong as = (r1 * r1)<<1;
	xlong bs = (r2 * r2)<<1;
	xlong ee = 0;
	xlong sx = bs * r1;
	xlong sy = 0;
	
	while(sx>sy)
	{
		drawellipsepixel(xc,yc,x,y,c);
		y++;
		sy += as;
		ee += yd;
		yd += as;
		if( ((ee<<1) + xd) > 0 )
		{
			x--;
			sx -= bs;
			ee += xd;
			xd += bs;
		}
	}
	
	x = 0;
	y = r2;
	xd = r2 * r2;
	yd = r1 * r1 * (1-(r2<<1));
	ee = 0;
	sx = 0;
	sy = as * r2;
	while(sx <= sy)
	{
		drawellipsepixel(xc,yc,x,y,c);
		x++;
		sx += bs;
		ee += xd;
		xd += bs;
		if( (ee<<1) + yd > 0 )
		{
			y--;
			sy -= as;
			ee += yd;
			yd += as;
		}
	}
}

void CLgfx1::fill(xlong x,xlong y,uxlong oc,uxlong nc)
{
	//set up fifo
	CLfifo<CLpoint> fillfifo;
	//*
	
	//initial fifo fill with four surroundng pixel
	CLpoint* u0 = new CLpoint(x,y-1);
	CLpoint* d0 = new CLpoint(x,y+1);
	CLpoint* l0 = new CLpoint(x-1,y);
	CLpoint* r0 = new CLpoint(x+2,y);
	fillfifo.in(u0);
	fillfifo.in(d0);
	fillfifo.in(l0);
	fillfifo.in(r0);
	//*
	
	CLpoint* temp = 0;
	
	//do until all reacable pixels have been recolored
	while(fillfifo.getlength() != 0)
	{
		temp = fillfifo.out();
		
		//test if on screen and color is the to-fill-color
		if( temp->x>=0 && temp->x<xres && temp->y>=0 && temp->y<yres && readpixel(temp->x,temp->y)==oc )
		{
			//fill the pixel with the new color
			(*CLdoublebuffer)[((temp->y)*xres)+(temp->x)] = nc;
			//*
			
			//enque the four surrounding pixel
			u0 = new CLpoint((temp->x),(temp->y)-1);
			d0 = new CLpoint((temp->x),(temp->y)+1);
			l0 = new CLpoint((temp->x)-1,(temp->y));
			r0 = new CLpoint((temp->x)+1,(temp->y));
			fillfifo.in(u0);
			fillfifo.in(d0);
			fillfifo.in(l0);
			fillfifo.in(r0);
			//*
		}
		//*
		
		//kill the point
		delete temp;
		//*
	}
	//*

	return;
}

void CLgfx1::fillframe(xlong x,xlong y,uxlong fc,uxlong nc)
{
	//set up fifo
	CLfifo<CLpoint> fillfifo;
	//*
	
	//initial fifo fill with four surroundng pixel
	CLpoint* u0 = new CLpoint(x,y-1);
	CLpoint* d0 = new CLpoint(x,y+1);
	CLpoint* l0 = new CLpoint(x-1,y);
	CLpoint* r0 = new CLpoint(x+2,y);
	fillfifo.in(u0);
	fillfifo.in(d0);
	fillfifo.in(l0);
	fillfifo.in(r0);
	//*
	
	CLpoint* temp = 0;
	
	//do until all reacable pixels have been recolored
	while(fillfifo.getlength() != 0)
	{
		temp = fillfifo.out();
		
		//test if on screen and color is the to-fill-color
		if( temp->x>=0 && temp->x<xres && temp->y>=0 && temp->y<yres && readpixel(temp->x,temp->y)!=fc )
		{
			//fill the pixel with the new color
			(*CLdoublebuffer)[((temp->y)*xres)+(temp->x)] = nc;
			//*
			
			//enque the four surrounding pixel
			u0 = new CLpoint((temp->x),(temp->y)-1);
			d0 = new CLpoint((temp->x),(temp->y)+1);
			l0 = new CLpoint((temp->x)-1,(temp->y));
			r0 = new CLpoint((temp->x)+1,(temp->y));
			fillfifo.in(u0);
			fillfifo.in(d0);
			fillfifo.in(l0);
			fillfifo.in(r0);
			//*
		}
		//*
		
		//kill the point
		delete temp;
		//*
	}
	//*

	return;
}

void CLgfx1::drawsprite(xlong x,xlong y,sprite* s)
{
	//init
	if(x>xres || y>yres) return;
	//xlong ssize = s->size;
	xlong swidth = s->width;
	xlong sheight = s->height;
	//xlong hordiff = xres - s->width;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	if(xe<0 || ye<0) return;
	//*

	//clipping against screen borders
	if(xs<0) xs = 0;
	if(xe>xres) xe = xres-1;
	if(ys<0) ys = 0;
	if(ye>yres) ye = yres-1;
	//*

	//set up variables
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * xres) + xs;
	xlong linearc = 0;
	//*

	//drawloop
	for(uxlong i=0; i<eheight ;i++)
	{
		for(uxlong j=0; j<ewidth ;j++)
		{
			if( (s->data[linearc] & 0xFF000000) != 0xFF)
			{
				(*CLdoublebuffer)[xoffset+j] = s->data[linearc];
			}
			linearc++;
		}
		xoffset += xres;
	}
	//*	
}

void CLgfx1::drawspriteanimated(xlong x,xlong y,sprites* s,xlong i)
{

}

void CLgfx1::putsprite(xlong x,xlong y,sprite* s,xlong m,float e)
{
	
	switch(m)
	{
		case 0: //normal
		
		case 1: //mirrored horizontal
		
		case 2: //mirrored vertical
		
		case 3: //rotated 90° left
		
		case 4: //rotated 180° left
		
		case 5: //rotated 270° left
		
		case 6: //rotated 90° right
		
		case 7: //rotated 180° right
		
		case 8: //rotated 270° right
		
		case 9: //scaled about e
		
		case 10: ; //rotated about e
	}
}

void CLgfx1::drawscreen(sprite* s)
{
	//check if sprite has correct dimensions
	if(s->width==xres && s->height==yres)
	{
		//draw loop
		for(uxlong i=0; i<s->size; i++)
		{
			(*CLdoublebuffer)[i] = s->data[i];
		}
		//*
	}
	//*
}

void CLgfx1::drawtile(xlong x,xlong y,sprites *s,xlong ti)
{
	//init
	if(x>xres || y>yres) return;
	//*
	
	//find tile
	xlong pr = (s->width / s->tilewidth);
	xlong off = ((ti % pr) * (s->width*s->tileheight)) + ((ti / pr) * s->tilewidth);
	//*

	xlong hordiff = s->width-s->tilewidth;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + s->tilewidth;
	xlong ye = y + s->tileheight;
	if(xe<0 || ye<0) return;

	//clipping against screen borders
	if(xs<0) xs = 0;
	if(xe>xres) xe = xres-1;
	if(ys<0) ys = 0;
	if(ye>yres) ye = yres-1;
	//*

	//set up variables
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * xres) + xs;
	xlong linearc = off;
	//*

	//drawloop
	for(uxlong i=0; i<eheight ;i++)
	{
		for(uxlong j=0; j<ewidth ;j++)
		{
			if( (s->data[linearc] & 0xFF000000) != 0xFF000000)
			{
				(*CLdoublebuffer)[xoffset+j] = s->data[linearc];
			}
			linearc++;
		}
		xoffset += xres;
		linearc += hordiff;
	}
	//*
}

#endif

