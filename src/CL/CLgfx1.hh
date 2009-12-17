///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGFX1
#define HH_CLGFX1
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLfifo.hh"
#include "CLmath.hh"
#include "CLutils.hh"
///*

///header
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
///*

///definitions
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
		inline void drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		inline void drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		CLgfx1() { };
		~CLgfx1() { };
	public:
		uxlong readpixel(xlong x,xlong y) const;
		void drawpixel(xlong x,xlong y,uxlong c,bool b=0) const;
		void drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const;
		void copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const;
		void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const;
		void drawline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool aa=0) const;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool f=0) const;
		void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const;
		void drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,uxlong c) const;
		void drawcircle(xlong xc,xlong yc,xlong r,uxlong c,bool a=0) const;
		void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const;
		void fill(xlong x,xlong y,uxlong oc,uxlong nc) const;
		void fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const;
		void drawsprite(xlong x,xlong y,sprite* s) const;
		void drawtile(xlong x,xlong y,tileset* t,xlong a) const;
		void drawspriteanimated(xlong x,xlong y,tileset* s,xlong i) const;
		void putsprite(xlong x,xlong y,sprite* s,sprite* t,xlong m) const;
		void drawscreen(sprite* s) const;
};
///*

///implementation
void CLgfx1::drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const //! critical
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

void CLgfx1::drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const //! critical
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

uxlong CLgfx1::readpixel(xlong x,xlong y) const //! critical
{
	if(isoff(x,y)) return -1;
	return (cldoublebuffer[(y*XRES)+x]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c,bool b) const //! critical
{
	if(isoff(x,y)) return;
	cldoublebuffer[(y*XRES)+x] = c;
	if(b)
	{
		cldoublebuffer[(y*XRES)+x+1] = c;
		cldoublebuffer[((y+1)*XRES)+x] = c;
		cldoublebuffer[((y+1)*XRES)+(x+1)] = c;
	}
}

void CLgfx1::drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const //! critical
{
	if(isoff(x,y)) return;
	b[(y*XRES)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const //! critical
{
	if(isoff(x1,x2)||isoff(x2,y2)) return;
	cldoublebuffer[(y1*XRES)+x1] = cldoublebuffer[(y2*XRES)+x2];
}

void CLgfx1::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const //! critical
{

}

void CLgfx1::drawline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool aa) const //! critical
{
	//! todo: if(a) then anti-aliased line
	
	if( (x1==x2 && y1==y2) || isoff(x1,y1,x2,y2)) return;
	clip(x1,y1);
	clip(x2,y2);

	xlong s = xlong(x1==x2) - xlong(y1==y2);
	if(aa) s = 2;
	xlong a = 0;
	xlong b = 0;
	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e = 0;
	xlong xs = 1;
	xlong ys = XRES;
	xlong len = 0;
	uxlong offset = 0;
	
	switch(s)
	{
		case 1:
			a = y1;
			b = y2;
			if(a>b) a ^= b ^= a ^= b;
			offset = (a*XRES)+x1;
			for(uxlong i=a; i<=b; i++) { cldoublebuffer[offset] = c; offset+=XRES; }
		break;
		
		case -1:
			a = x1;
			b = x2;
			if(a>b) a ^= b ^= a ^= b;
			offset = (y1*XRES);
			for(uxlong i=a; i<=b; i++) { cldoublebuffer[offset+i] = c; }
		break;
		
		case 0:
			offset = y1*XRES+x1;

			if(dx<0) { dx = -dx; xs = -xs; }
			if(dy<0) { dy = -dy; ys = -ys; }
			if(dy > dx) { dx ^= dy ^= dx ^= dy; xs ^= ys ^= xs ^= ys; }

			len = dx+1;
			e = dy;

			for(uxlong i=0; i<len; i++)
			{
				cldoublebuffer[offset] = c;
				offset += xs;
				e += dy;
				if(e >= dx) { e -= dx; offset += ys; }
			}
		break;
		
		case 2:
			offset = y1*XRES+x1;

			if(dx<0) { dx = -dx; xs = -xs; }
			if(dy<0) { dy = -dy; ys = -ys; }
			if(dy > dx) { dx ^= dy ^= dx ^= dy; xs ^= ys ^= xs ^= ys; }
		
			len = dx+1;
			e = dy;

			//split color components
			doubleword ccolor = { c };
			doubleword tcolor = { c };
			//*

			//aa as in xiaolin wu

			for(uxlong i=0; i<len; i++)
			{
				tcolor.dd = cldoublebuffer[offset-xs];
				tcolor.db[1] = (tcolor.db[1] + ccolor.db[1])>>1;
				tcolor.db[2] = (tcolor.db[2] + ccolor.db[2])>>1;
				tcolor.db[3] = (tcolor.db[3] + ccolor.db[3])>>1;	
				cldoublebuffer[offset-xs] = tcolor.dd;
				cldoublebuffer[offset] = c;
				tcolor.dd = cldoublebuffer[offset+xs];
				tcolor.db[1] = (tcolor.db[1] + ccolor.db[1])>>1;
				tcolor.db[2] = (tcolor.db[2] + ccolor.db[2])>>1;
				tcolor.db[3] = (tcolor.db[3] + ccolor.db[3])>>1;	
				cldoublebuffer[offset+xs] = tcolor.dd;
				offset += xs;
				e += dy;
				if(e >= dx) { e -= dx; offset += ys; }
			}
		break;
	}
}

void CLgfx1::drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool f) const //! critical
{
	if(f)
	{
		//make sure x1<x2 and y1<y2
		if(x2<x1) x1 ^= x2 ^= x1 ^= x2;
		if(y2<y1) y1 ^= y2 ^= y1 ^= y2;
		//*
		
		//decide wether to use horizontal or vertical lines
		if( (x2-x1)>(y2-y1) ) for(xlong i=y1; i<=y2; i++) { drawline(x1,i,x2,i,c); }
		else for(xlong i=x1; i<=x2; i++) { drawline(i,y1,i,y2,c); }
		//*
	}
	else
	{
		//draw outline of rectangle
		drawline(x1,y1,x2,y1,c);
		drawline(x1,y2,x2,y2,c);
		drawline(x1,y1,x1,y2,c);
		drawline(x2,y1,x2,y2,c);
		//*
	}
}

void CLgfx1::drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const //! critical
{
	//draw outline of four-sided polygon
	drawline(x1,y1,x2,y2,c);
	drawline(x2,y2,x3,y3,c);
	drawline(x3,y3,x4,y4,c);
	drawline(x4,y4,x1,y1,c);
	//*
}

void CLgfx1::drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,uxlong c) const //! critical
{
	//angle preparations
	xlong as = clmath->sign(a);
	a = a%180;
	//*
	
	//calc segments
	xlong segs = (x2-x1)*((x2-x1)>=(y2-y1)) + (y2-y1)*((x2-x1)<(y2-y1));
	//*
	
	//calc position of control point
	float abx = ((-x1 + x2) / 2); 
	float aby = ((-y1 + y2) / 2);
	float abx2 = abx * abx; 
	float aby2 = aby * aby;
	float tana = clmath->tan(90-(a/2));
	float hx = clmath->sqrt( (aby2 / abx2) * (tana * tana * (abx2 + aby2) ) );
	float hy = -(abx / aby) * hx;
	xlong x3 = xlong(x1 + abx + as*hx);
	xlong y3 = xlong(y1 + aby + as*hy);
	//*

	//draw quadratic bezier
	float t = 0.0;
	float m = 0.0;
	float n = 0.0;
	float o = 0.0;
	float x = 0.0;
	float y = 0.0;
	for(xlong i=0; i<=segs; i++)
	{
		t = float(i)/float(segs);
		m = (1-t)*(1-t);
		n = 2*(1-t)*t;
		o = t*t; 
		x = xlong(m * x1 + n * x3 + o * x2);
		y = xlong(m * y1 + n * y3 + o * y2);
		cldoublebuffer[(y*XRES)+x] = c;
	}
	//*
}

void CLgfx1::drawcircle(xlong xc,xlong yc,xlong r,uxlong c,bool a) const //! critical
{
	//center and radius clipping
	if(isoff(xc-r,yc-r,xc+r,yc+r)) return;
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
	
	if(a)
	{
		drawcircle(xc,yc,r+1,c); //adjust color
		drawcircle(xc,yc,r-1,c); //adjust color
	}
}

void CLgfx1::drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const //! critical
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

void CLgfx1::fill(xlong x,xlong y,uxlong oc,uxlong nc) const //! critical
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
}

void CLgfx1::fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const //! critical
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
}

void CLgfx1::drawsprite(xlong x,xlong y,sprite* s) const //! critical
{
	//set up variables
	xlong swidth = s->width;
	xlong sheight = s->height;
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
	xlong cwidth = xe - xs;
	xlong cheight = ye - ys;
	xlong cdiff = swidth - cwidth;
	xlong doffset = (ys * XRES) + xs;
	xlong soffset = 0;
	uxlong svalue = 0;
	xlong istrans = 0;
	//*

	//drawloop
	for(xlong i=0; i<cheight; i++)
	{
		for(xlong j=0; j<cwidth; j++)
		{
			svalue = s->data[soffset];
			istrans = svalue & 0xFF000000;
			if( istrans != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
			//cldoublebuffer[doffset+j] = ((xlong(istrans!=0xFF000000)--) & svalue) + ((xlong(istrans==0xFF000000)--) & cldoublebuffer[doffset+j])
			soffset++;
		}
		soffset += cdiff;
		doffset += XRES;
	}
	//*	
}

void CLgfx1::drawtile(xlong x,xlong y,tileset* t,xlong a) const //! critical
{
	//set up variables
	xlong swidth = t->tilewidth;
	xlong sheight = t->tileheight;
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
	xlong cwidth = xe - xs;
	xlong cheight = ye - ys;
	xlong cdiff = swidth - cwidth;
	xlong doffset = (ys * XRES) + xs;
	xlong soffset = 0;
	uxlong svalue = 0;
	//*

	//drawloop
	for(uxlong i=0; i<sheight ;i++)
	{
		for(uxlong j=0; j<swidth ;j++)
		{
			svalue = t->tiledata[a][soffset];
			if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
			soffset++;
		}
		doffset += XRES;
		soffset += cdiff;
	}
	//*	
}

void CLgfx1::drawspriteanimated(xlong x,xlong y,tileset* s,xlong i) const //! critical
{

}

void CLgfx1::putsprite(xlong x,xlong y,sprite* s,sprite* t,xlong m) const //! critical
{
	//set up variables
	xlong swidth = s->width;
	xlong sheight = s->height;
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
	xlong cwidth = xe - xs;
	xlong cheight = ye - ys;
	xlong cdiff = swidth - cwidth;
	xlong doffset = (ys * XRES) + xs;
	xlong soffset = 0;
	uxlong svalue = 0;
	uxlong dvalue = 0;
	//*

	//set combinee
	xlong tdiff = 0;
	uxlong* tbuffer = 0;
	if(t=0)
	{
		tdiff = XRES;
		tbuffer = cldoublebuffer.getbuffer();
	}
	else
	{
		tdiff = t->width;
		tbuffer = t->data;
	}
	//*
	
	//! todo: implement cases
	//draw loop
	switch(m)
	{
		case 0: //normal
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 1: //mirrored horizontal
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 2: //mirrored vertical
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 3: //rotated 90° left
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 4: //rotated 180° left
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 5: //rotated 270° left
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 6: //rotated 90° right
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 7: //rotated 180° right
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 8: //rotated 270° right
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 11: //and
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = dvalue & svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 12: //or
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = dvalue | svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 13: //xor
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = dvalue ^ svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 14: //nand
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = ~(dvalue | svalue);
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 15: //nor
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = ~(dvalue | svalue);
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 16: //not
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					if( (svalue & 0xFF000000) != 0xFF000000) cldoublebuffer[doffset+j] = ~svalue;
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 17: //byte add
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = byteadd(dvalue,svalue);
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 18: //byte sub
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = bytesub(dvalue,svalue);
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
		
		case 19: //byte mul
			for(xlong i=0; i<cheight; i++)
			{
				for(xlong j=0; j<cwidth; j++)
				{
					svalue = s->data[soffset];
					dvalue = t->data[soffset];
					cldoublebuffer[doffset+j] = svalue; //bytemul(dvalue,svalue)
					soffset++;
				}
				soffset += cdiff;
				doffset += XRES;
			}
		break;
	}
	//*
}

void CLgfx1::drawscreen(sprite* s) const //! critical
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
///*

#endif

