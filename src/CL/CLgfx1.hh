//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGFX1
#define HH_CLGFX1
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLfifo.hh"
#include "CLmath.hh"

/* class name:	CLgfx1
 * 
 * description:	standard 2d graphics routines.
 * 
 * author:	atcl
 * 
 * notes:	add clipping and complete implementing.
 * 
 * version: 0.1
 */

inline bool isoff(xlong x,xlong y) { return (x<0 || x>=XRES || y<0 || y>=YRES); }

inline bool isoff(xlong x1,xlong y1,xlong x2,xlong y2) { return ( (x1<0&&x2<0) || (x1>=XRES&&x2>=XRES) || (y1<0&&y2<0) || (y1>=YRES&&y2>=YRES) ); }

inline void clip(xlong& x,xlong& y)
{
	if(x<0) x=0;
	if(x>=XRES) x=XRES-1;
	if(y<0) y=0;
	if(y>=YRES) y=YRES-1;
}

struct CLpoint
{
	xlong x;
	xlong y;
	
	CLpoint(xlong px,xlong py) { x=px; y=py; }
};

class CLgfx1 : public virtual CLcl, public CLsingle<CLgfx1>
{
	friend class CLsingle<CLgfx1>;
	
	private:
		void drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		void drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		CLgfx1() { };
		~CLgfx1() { };
	public:
		uxlong readpixel(xlong x,xlong y) const;
		void drawpixel(xlong x,xlong y,uxlong c) const;
		void drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const;
		void copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const;
		void drawbigpixel(xlong x,xlong y,uxlong c) const;
		void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const;
		void drawhorline(xlong x1,xlong y1,xlong x2,uxlong c) const;
		void drawverline(xlong x1,xlong y1,xlong y2,uxlong c) const;
		void drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const;
		void drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const;
		void drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong r,uxlong c) const;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const;
		void drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const;
		void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const;
		void drawcircle(xlong xc,xlong yc,xlong r,uxlong c) const;
		void drawanticircle(xlong xc,xlong yc,xlong r,uxlong c) const;
		void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const;
		void fill(xlong x,xlong y,uxlong oc,uxlong nc) const;
		void fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const;
		void drawsprite(xlong x,xlong y,sprite* s) const;
		void drawsprite(xlong x,xlong y,sprites2* t,xlong a) const;
		void drawspriteanimated(xlong x,xlong y,sprites* s,xlong i) const;
		void putsprite(xlong x,xlong y,sprite* s,xlong m,float e=0) const;
		void drawscreen(sprite* s) const;
		void drawtile(xlong x,xlong y,sprites *s,xlong ti) const;
};

void CLgfx1::drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const
{
	//precalculate linear address components (especially multiplications)
	xlong b1 = (yc*XRES)+xc;
 	xlong a1 = y*XRES;
 	xlong a2 = x*XRES;
	//*
	
	//draw the eight pixels for each (1/8) section of the circle
 	cldoublebuffer[b1+a1+x] = c;
 	cldoublebuffer[b1-a1+x] = c;
 	cldoublebuffer[b1+a1-x] = c;
 	cldoublebuffer[b1-a1-x] = c;
 	cldoublebuffer[b1+a2+y] = c;
 	cldoublebuffer[b1-a2+y] = c;
 	cldoublebuffer[b1+a2-y] = c;
 	cldoublebuffer[b1-a2-y] = c;
	//*
}

void CLgfx1::drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const
{
	//precalculate linear address components (especially multiplications) 
	xlong a = (yc*XRES)+xc;
	xlong b = (y*XRES);
	//*

	//draw the four pixels for each (1/4) section of the ellipse
	cldoublebuffer[a+x+b] = c;
	cldoublebuffer[a-x+b] = c;
	cldoublebuffer[a-x-b] = c;
	cldoublebuffer[a+x-b] = c;
	//*
}

uxlong CLgfx1::readpixel(xlong x,xlong y) const
{
	if(isoff(x,y)) return -1;
	return (cldoublebuffer[(y*XRES)+x]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c) const
{
	if(isoff(x,y)) return;
	cldoublebuffer[(y*XRES)+x] = c;
}

void CLgfx1::drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const
{
	if(isoff(x,y)) return;
	b[(y*XRES)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const
{
	if(isoff(x1,x2)||isoff(x2,y2)) return;
	cldoublebuffer[(y1*XRES)+x1] = cldoublebuffer[(y2*XRES)+x2];
}

void CLgfx1::drawbigpixel(xlong x,xlong y,uxlong c) const
{
	if(isoff(x,y)) return;
	cldoublebuffer[(y*XRES)+x] = c;
	cldoublebuffer[(y*XRES)+x+1] = c;
	cldoublebuffer[((y+1)*XRES)+x] = c;
	cldoublebuffer[((y+1)*XRES)+(x+1)] = c;
}

void CLgfx1::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const
{

}

void CLgfx1::drawhorline(xlong x1,xlong y1,xlong x2,uxlong c) const
{
	if(isoff(x1,y1,x2,y1)) return;
	clip(x1,y1);
	clip(x2,y1);
	
	xlong a = x1;
	xlong b = x2;
	if(a>b) a ^= b ^= a ^= b;
	xlong offsetbase = (y1*XRES);

	for(uxlong i=a; i<=b; i++) { cldoublebuffer[offsetbase+i] = c; }
}

void CLgfx1::drawverline(xlong x1,xlong y1,xlong y2,uxlong c) const
{
	if(isoff(x1,y1,x1,y2)) return;
	clip(x1,y1);
	clip(x1,y2);
	
	xlong a = y1;
	xlong b = y2;
	if(a>b) a ^= b ^= a ^= b;
	xlong offsetbase = (a*XRES)+x1;

	for(uxlong i=a; i<=b; i++)
	{
		cldoublebuffer[offsetbase] = c;
		offsetbase+=XRES;
	}
}

void CLgfx1::drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const
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
	xlong ys = XRES;
	xlong len;
	xlong off = y1*XRES+x1;
	//*

	if(dx<0) { dx = -dx; xs = -xs; }

	if(dy<0) { dy = -dy; ys = -ys; }
 
	if(dy > dx) { dx ^= dy ^= dx ^= dy; xs ^= ys ^= xs ^= ys; }

	len = dx+1;
	e = dy;

	//draw loop
	for(uxlong i=0; i<len; i++)
	{
		cldoublebuffer[off] = c;
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

void CLgfx1::drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const
{

}

void CLgfx1::drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong r,uxlong c) const
{
	//find circle center
	xlong xc = 0;
	xlong yc = 0;
	
	//*
	
	//find start and end angles
	
	
	//*
	
	//draw the arc
	
	
	//*
}

void CLgfx1::drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const
{
	//draw outline of rectangle
	drawhorline(x1,y1,x2,c);
	drawhorline(x1,y2,x2,c);
	drawverline(x1,y1,y2,c);
	drawverline(x2,y1,y2,c);
	//*
}

void CLgfx1::drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) const
{
	//make sure x1<x2 and y1<y2
	if(x2<x1) x1 ^= x2 ^= x1 ^= x2;
	if(y2<y1) y1 ^= y2 ^= y1 ^= y2;
	//*
	
	//decide wether to use horizontal or vertical lines
	if( (x2-x1)>(y2-y1) ) for(xlong i=y1; i<=y2; i++) { drawhorline(x1,i,x2,c); }
	else for(xlong i=x1; i<=x2; i++) { drawverline(i,y1,y2,c); }
	//*
}

void CLgfx1::drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const
{
	//draw outline of four-sided polygon
	drawanyline(x1,y1,x2,y2,c);
	drawanyline(x2,y2,x3,y3,c);
	drawanyline(x3,y3,x4,y4,c);
	drawanyline(x4,y4,x1,y1,c);
	//*
}

void CLgfx1::drawcircle(xlong xc,xlong yc,xlong r,uxlong c) const
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

void CLgfx1::drawanticircle(xlong xc,xlong yc,xlong r,uxlong c) const
{
	drawcircle(xc,yc,r,c);
	drawcircle(xc,yc,r+1,c); //adjust color
	drawcircle(xc,yc,r-1,c); //adjust color
}

void CLgfx1::drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const
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

void CLgfx1::fill(xlong x,xlong y,uxlong oc,uxlong nc) const
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
		if( temp->x>=0 && temp->x<XRES && temp->y>=0 && temp->y<YRES && readpixel(temp->x,temp->y)==oc )
		{
			//fill the pixel with the new color
			cldoublebuffer[((temp->y)*XRES)+(temp->x)] = nc;
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

void CLgfx1::fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const
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
		if( temp->x>=0 && temp->x<XRES && temp->y>=0 && temp->y<YRES && readpixel(temp->x,temp->y)!=fc )
		{
			//fill the pixel with the new color
			cldoublebuffer[((temp->y)*XRES)+(temp->x)] = nc;
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

void CLgfx1::drawsprite(xlong x,xlong y,sprite* s) const
{
	//xlong ssize = s->size;
	xlong swidth = s->width;
	xlong sheight = s->height;
	//xlong hordiff = xres - s->width;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	//*

	//clipping against screen borders
	if(isoff(xs,ys,xe,ye)) return;
	clip(xs,ys);
	clip(xe,ye);
	//*

	//set up variables
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * XRES) + xs;
	xlong linearc = 0;
	xlong lindiff = swidth - ewidth;
	//*

	//drawloop
	for(uxlong i=0; i<eheight ;i++)
	{
		for(uxlong j=0; j<ewidth ;j++)
		{
			if( (s->data[linearc] & 0xFF000000) != 0xFF000000) cldoublebuffer[xoffset+j] = s->data[linearc];
			linearc++;
		}
		linearc += lindiff;
		xoffset += XRES;
	}
	//*	
}

void CLgfx1::drawsprite(xlong x,xlong y,sprites2* t,xlong a) const
{
	//xlong ssize = s->size;
	xlong swidth = t->tilewidth;
	xlong sheight = t->tileheight;
	//xlong hordiff = xres - s->width;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	//*

	//clipping against screen borders
	if(isoff(xs,ys,xe,ye)) return;
	clip(xs,ys);
	clip(xe,ye);
	//*

	//set up variables
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * XRES) + xs;
	xlong linearc = 0;
	xlong lindiff = swidth - ewidth;
	//*

	//drawloop
	for(uxlong i=0; i<eheight ;i++)
	{
		for(uxlong j=0; j<ewidth ;j++)
		{
			//if( (t->tiledata[a][linearc] & 0xFF000000) != 0xFF000000) cldoublebuffer[xoffset+j] = t->tiledata[a][linearc];
			cldoublebuffer[xoffset+j] = t->tiledata[a][linearc]; 
			linearc++;
		}
		linearc += lindiff;
		xoffset += XRES;
	}
	//*	
}

void CLgfx1::drawspriteanimated(xlong x,xlong y,sprites* s,xlong i) const
{

}

void CLgfx1::putsprite(xlong x,xlong y,sprite* s,xlong m,float e) const
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
		
		case 10:  //rotated about e
		
		case 11: //and
		
		case 12: //or
		
		case 13: //xor
		
		case 14: //nand
		
		case 15: //nor
		
		case 16: //not
		
		case 17: //byte add
		
		case 18: //byte sub
		
		case 19: ;//byte mul
	}
}

void CLgfx1::drawscreen(sprite* s) const
{
	//check if sprite has correct dimensions
	if(s->width==XRES && s->height==YRES)
	{
		//draw loop
		for(uxlong i=0; i<s->size; i++) { if( (s->data[i] & 0xFF000000) != 0xFF) cldoublebuffer[i] = s->data[i]; }
		//*
	}
	//*
}

void CLgfx1::drawtile(xlong x,xlong y,sprites *s,xlong ti) const
{
	//find tile
	xlong pr = (s->width / s->tilewidth);
	xlong off = ((ti % pr) * (s->width*s->tileheight)) + ((ti / pr) * s->tilewidth);
	//*

	xlong hordiff = s->width-s->tilewidth;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + s->tilewidth;
	xlong ye = y + s->tileheight;

	//clipping against screen borders
	if(isoff(xs,ys,xe,ye)) return;
	clip(xs,ys);
	clip(xe,ye);
	//*

	//set up variables
	xlong ewidth = xe - xs;
	xlong eheight = ye - ys;
	xlong xoffset = (ys * XRES) + xs;
	xlong linearc = off;
	
	hordiff += (s->tilewidth - ewidth); //if clipping on xmax. test! 
	//*

	//drawloop
	for(uxlong i=0; i<eheight ;i++)
	{
		for(uxlong j=0; j<ewidth ;j++)
		{
			if( (s->data[linearc] & 0xFF000000) != 0xFF000000) cldoublebuffer[xoffset+j] = s->data[linearc];
			linearc++;
		}
		xoffset += XRES;
		linearc += hordiff;
	}
	//*
}

#endif

