///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLgfx
#define HH_CLgfx
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLfifo.hh"
#include "CLmath.hh"
#include "CLutils.hh"
#include "CLformat.hh"
#include "CLstring.hh"
#include "CLar.hh"
///*

///header
/* class name:	CLgfx
 * 
 * description:	standard 2d graphics routines.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///declarations
//font types
#define TELEFONT 0;
#define MONOFONT 1;
#define LINEFONT 2;
#define TERMFONT 3;
#define SEGMFONT 4;
#define TALLFONT 5;
#define SYMBFONT 6;
//*

typedef sprite* CLfont;
///*

///definitions
struct CLpoint
{
	xlong x;
	xlong y;
	
	CLpoint(xlong px,xlong py) { x=px; y=py; }
};

class CLgfx : public CLbase<CLgfx,1>
{
	friend class CLbase<CLgfx,1>;
	friend class CLglobal;
	
	private:
		static CLmath&   clmath;
		static CLscreen& clscreen;
		static CLformat& clformat;
		static CLstring& clstring;
		static CLsystem& clsystem;
	protected:
		static CLfont** fonts;
		inline void drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		inline void drawclipcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		inline void drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const;
		CLgfx() { };
		~CLgfx() { };
	public:
		uxlong readpixel(xlong x,xlong y) const;
		void drawpixel(xlong x,xlong y,uxlong c,bool b=0) const;
		void drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const;
		void copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const;
		void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const;
		void drawline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool aa=0) const;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool f=0) const;
		void drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f) const;
		void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const;
		void drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,uxlong c) const;
		void drawcircle(xlong xc,xlong yc,xlong r,uxlong c,bool a=0) const;
		void drawclipcircle(xlong xc,xlong yc,xlong r,uxlong c) const;
		void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const;
		void fill(xlong x,xlong y,uxlong oc,uxlong nc) const;
		void fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const;
		void drawsprite(xlong x,xlong y,sprite* s) const;
		void drawspriterotated(xlong x,xlong y,sprite* s,xlong w) const;
		void drawspriteanimated(xlong x,xlong y,sprite** s,xlong i) const;
		void putsprite(xlong x,xlong y,sprite* s,sprite* t,xlong m) const;
		void loadfonts(CLfile* f);
		xlong drawfontchar(xlong x,xlong y,const uxlong a,uxlong f,uxlong fc,uxlong bc=0) const;
		void drawfontstring(xlong x,xlong y,const xchar* a,uxlong f,uxlong fc,uxlong bc=0,xlong s=0) const;
		xlong getfontstringwidth(const char* a,uxlong f) const;
		xlong getfontstringheight(const char* a,uxlong f) const;
		sprite* savescreen() const;
		void drawscreen(sprite* s) const;
		uxlong getEGAcolor(xchar c) const;
		uxlong blendcolors(uxlong c1,uxlong c2,xlong m) const;
		uxlong* getgradient(uxlong s,uxlong e,xlong i) const;	
};

CLmath&   CLgfx::clmath   = CLmath::instance();
CLscreen& CLgfx::clscreen = CLscreen::instance();
CLformat& CLgfx::clformat = CLformat::instance();
CLstring& CLgfx::clstring = CLstring::instance();
CLsystem& CLgfx::clsystem = CLsystem::instance();

CLfont** CLgfx::fonts = 0;
///*

///implementation
void CLgfx::drawcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const //! critical
{
	//precalculate linear address components (especially multiplications)
	xlong b1 = (yc*XRES)+xc;
 	xlong a1 = y*XRES;
 	xlong a2 = x*XRES;
	//*
	
	//draw the eight pixels for each (1/8) section of the circle
 	clscreen.cldoublebuffer[b1+a1+x] = c;
 	clscreen.cldoublebuffer[b1-a1+x] = c;
 	clscreen.cldoublebuffer[b1+a1-x] = c;
 	clscreen.cldoublebuffer[b1-a1-x] = c;
 	clscreen.cldoublebuffer[b1+a2+y] = c;
 	clscreen.cldoublebuffer[b1-a2+y] = c;
 	clscreen.cldoublebuffer[b1+a2-y] = c;
 	clscreen.cldoublebuffer[b1-a2-y] = c;
	//*
}

void CLgfx::drawclipcirclepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const //! critical
{
	//precalculate linear address components (especially multiplications)
	xlong b1 = (yc*XRES)+xc;
 	xlong a1 = y*XRES;
 	xlong a2 = x*XRES;
	//*
	
	//draw the eight pixels for each (1/8) section of the circle
 	if( (b1+a1<XRES*YRES) && (b1+a1>0) && (xc+x<XRES) && (xc+x>0) ) { clscreen.cldoublebuffer[b1+a1+x] = c; }
 	if( (b1-a1<XRES*YRES) && (b1-a1>0) && (xc+x<XRES) && (xc+x>0) ) { clscreen.cldoublebuffer[b1-a1+x] = c; }
 	if( (b1+a1<XRES*YRES) && (b1+a1>0) && (xc-x<XRES) && (xc-x>0) ) { clscreen.cldoublebuffer[b1+a1-x] = c; }
 	if( (b1-a1<XRES*YRES) && (b1-a1>0) && (xc-x<XRES) && (xc-x>0) ) { clscreen.cldoublebuffer[b1-a1-x] = c; }
 	if( (b1+a2<XRES*YRES) && (b1+a1>0) && (xc+y<XRES) && (xc+y>0) ) { clscreen.cldoublebuffer[b1+a2+y] = c; }
 	if( (b1-a2<XRES*YRES) && (b1-a2>0) && (xc+y<XRES) && (xc+y>0) ) { clscreen.cldoublebuffer[b1-a2+y] = c; }
 	if( (b1+a2<XRES*YRES) && (b1+a2>0) && (xc-y<XRES) && (xc-y>0) ) { clscreen.cldoublebuffer[b1+a2-y] = c; }
 	if( (b1-a2<XRES*YRES) && (b1-a2>0) && (xc-y<XRES) && (xc-y>0) ) { clscreen.cldoublebuffer[b1-a2-y] = c; }
	//*
}

void CLgfx::drawellipsepixel(xlong xc,xlong yc,xlong x,xlong y,uxlong c) const //! critical
{
	//precalculate linear address components (especially multiplications) 
	xlong a = (yc*XRES)+xc;
	xlong b = (y*XRES);
	//*

	//draw the four pixels for each (1/4) section of the ellipse
	clscreen.cldoublebuffer[a+x+b] = c;
	clscreen.cldoublebuffer[a-x+b] = c;
	clscreen.cldoublebuffer[a-x-b] = c;
	clscreen.cldoublebuffer[a+x-b] = c;
	//*
}

uxlong CLgfx::readpixel(xlong x,xlong y) const //! critical
{
	if(isoff(x,y)) { return -1; }
	return (clscreen.cldoublebuffer[(y*XRES)+x]);
}

void CLgfx::drawpixel(xlong x,xlong y,uxlong c,bool b) const //! critical
{
	if(isoff(x,y)) { return; }
	clscreen.cldoublebuffer[(y*XRES)+x] = c;
	if(b)
	{
		clscreen.cldoublebuffer[(y*XRES)+x+1] = c;
		clscreen.cldoublebuffer[((y+1)*XRES)+x] = c;
		clscreen.cldoublebuffer[((y+1)*XRES)+(x+1)] = c;
	}
}

void CLgfx::drawpixeldirect(uxlong* b,xlong x,xlong y,uxlong c) const //! critical
{
	if(isoff(x,y)) { return; }
	b[(y*XRES)+x] = c;
}

void CLgfx::copypixel(xlong x1,xlong y1,xlong x2,xlong y2) const //! critical
{
	if(isoff(x1,x2)||isoff(x2,y2)) { return; }
	clscreen.cldoublebuffer[(y1*XRES)+x1] = clscreen.cldoublebuffer[(y2*XRES)+x2];
}

void CLgfx::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i) const //! critical
{
	if(isoff(x,y)) { return; }
	//else if(clsystem.getmilliseconds()%i<0) { clscreen.cldoublebuffer[(y*XRES)+x] = c1; }
	//else { clscreen.cldoublebuffer[(y*XRES)+x] = c2; }
}

void CLgfx::drawline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool aa) const //! critical
{
	if( (x1==x2 && y1==y2) || isoff(x1,y1,x2,y2)) { return; }
	clip(x1,y1);
	clip(x2,y2);

	xlong s = xlong(x1==x2) - xlong(y1==y2);
	if(aa) { s = 2; }
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
			if(a>b) { swap(&a,&b); }
			offset = (a*XRES)+x1;
			for(xlong i=a; i<=b; i++) { clscreen.cldoublebuffer[offset] = c; offset+=XRES; }
		break;
		
		case -1:
			a = x1;
			b = x2;
			if(a>b) { swap(&a,&b); }
			offset = (y1*XRES);
			for(xlong i=a; i<=b; i++) { clscreen.cldoublebuffer[offset+i] = c; }
		break;
		
		case 0:
			offset = y1*XRES+x1;

			if(dx<0) { dx = -dx; xs = -xs; }
			if(dy<0) { dy = -dy; ys = -ys; }
			if(dy > dx) { swap(&dx,&dy); swap(&xs,&ys); }

			len = dx+1;
			e = dy;

			for(xlong i=0; i<len; i++)
			{
				clscreen.cldoublebuffer[offset] = c;
				offset += xs;
				e += dy;
				if(e >= dx) { e -= dx; offset += ys; }
			}
		break;
		
		case 2:
			offset = y1*XRES+x1;

			if(dx<0) { dx = -dx; xs = -xs; }
			if(dy<0) { dy = -dy; ys = -ys; }
			if(dy > dx) { swap(&dx,&dy); swap(&xs,&ys); }
		
			len = dx+1;
			e = dy;

			//split color components
			doubleword ccolor = { c };
			doubleword tcolor = { c };
			//*

			//aa as in xiaolin wu
			for(xlong i=0; i<len; i++)
			{
				tcolor.dd = clscreen.cldoublebuffer[offset-xs];
				tcolor.db[1] = (tcolor.db[1] + ccolor.db[1])>>1;
				tcolor.db[2] = (tcolor.db[2] + ccolor.db[2])>>1;
				tcolor.db[3] = (tcolor.db[3] + ccolor.db[3])>>1;	
				clscreen.cldoublebuffer[offset-xs] = tcolor.dd;
				clscreen.cldoublebuffer[offset] = c;
				tcolor.dd = clscreen.cldoublebuffer[offset+xs];
				tcolor.db[1] = (tcolor.db[1] + ccolor.db[1])>>1;
				tcolor.db[2] = (tcolor.db[2] + ccolor.db[2])>>1;
				tcolor.db[3] = (tcolor.db[3] + ccolor.db[3])>>1;	
				clscreen.cldoublebuffer[offset+xs] = tcolor.dd;
				offset += xs;
				e += dy;
				if(e >= dx) { e -= dx; offset += ys; }
			}
		break;
	}
}

void CLgfx::drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,bool f) const //! critical
{
	if(f)
	{
		//make sure x1<x2 and y1<y2
		if(x2<x1) { swap(&x1,&x2); }
		if(y2<y1) { swap(&y1,&y2); }
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

void CLgfx::drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f) const //! critical
{
	//draw to top level of zbuffer!
	xlong offset1 = (y1*XRES)+x1;
	xlong offset2 = offset1;
	xlong doffset = ((y2-y1)*XRES);
	xlong diff    = x2-x1;
	uxlong c3     = c2;
	if(f) { c3 = 0x00FFFFFF - c2; }

	for(xlong i=y1; i<=y2; i++)
	{
		clscreen.cldoublebuffer[offset1] = c3;
		offset1++;
		for(xlong j=x1+1; j<x2; j++)
		{
			clscreen.cldoublebuffer[offset1] = c1;
			offset1++;
		}
		clscreen.cldoublebuffer[offset1] = c2;
		offset1 += XRES - diff;
	}

	for(xlong k=x1; k<x2; k++)
	{
		clscreen.cldoublebuffer[offset2] = c3;
		clscreen.cldoublebuffer[offset2+doffset] = c2;
		offset2++;
	}
}

void CLgfx::drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c) const //! critical
{
	//draw outline of four-sided polygon
	drawline(x1,y1,x2,y2,c);
	drawline(x2,y2,x3,y3,c);
	drawline(x3,y3,x4,y4,c);
	drawline(x4,y4,x1,y1,c);
	//*
}

void CLgfx::drawarc(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,uxlong c) const //! critical
{
	//angle preparations
	xlong as = clmath.sign(a);
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
	float tana = clmath.tan(90-(a/2));
	float hx = clmath.sqrt( (aby2 / abx2) * (tana * tana * (abx2 + aby2) ) );
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
		clscreen.cldoublebuffer[(y*XRES)+x] = c;
	}
	//*
}

void CLgfx::drawcircle(xlong xc,xlong yc,xlong r,uxlong c,bool a) const //! critical
{
	//center and radius clipping
	if(isoff(xc-r,yc-r,xc+r,yc+r)) { return; }
	//*
	
	xlong d = 3 - (r<<1);
	xlong cx = 0;
	xlong cy = r;

	while(cx<=cy)
	{
		drawcirclepixel(xc,yc,cx,cy,c);
		if(d<0) { d += (cx<<2)+6; }
		else { d += ((cx-cy)<<2)+10; cy--; }
		cx++; 
	}
	
	if(a)
	{
		drawcircle(xc,yc,r+1,c); //adjust color
		drawcircle(xc,yc,r-1,c); //adjust color
	}
}

void CLgfx::drawclipcircle(xlong xc,xlong yc,xlong r,uxlong c) const //! critical
{
	//center and radius clipping
	if(isoff(xc-r,yc-r,xc+r,yc+r)) { return; }
	//*
	
	xlong d = 3 - (r<<1);
	xlong cx = 0;
	xlong cy = r;

	while(cx<=cy)
	{
		drawclipcirclepixel(xc,yc,cx,cy,c);
		if(d<0) { d += (cx<<2)+6; }
		else { d += ((cx-cy)<<2)+10; cy--; }
		cx++; 
	}
}

void CLgfx::drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c) const //! critical
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

void CLgfx::fill(xlong x,xlong y,uxlong oc,uxlong nc) const //! critical
{
	if(isoff(x,y)) { return; }
	
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
			clscreen.cldoublebuffer[((temp->y)*XRES)+(temp->x)] = nc;
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

void CLgfx::fillframe(xlong x,xlong y,uxlong fc,uxlong nc) const //! critical
{
	if(isoff(x,y)) { return; }
	
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
			clscreen.cldoublebuffer[((temp->y)*XRES)+(temp->x)] = nc;
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

void CLgfx::drawsprite(xlong x,xlong y,sprite* s) const //! critical
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
	if(isoff(xs,ys,xe,ye)) { return; }
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
			if(istrans != 0xFF000000) { clscreen.cldoublebuffer[doffset+j] = svalue; }
			//clscreen.cldoublebuffer[doffset+j] = ((xlong(istrans!=0xFF000000)--) & svalue) + ((xlong(istrans==0xFF000000)--) & clscreen.cldoublebuffer[doffset+j])
			soffset++;
		}
		soffset += cdiff;
		doffset += XRES;
	}
	//*	
}

void CLgfx::drawspriterotated(xlong x,xlong y,sprite* s,xlong w) const //! critical
{
	float a = -clmath.tan(w/2);
	float b = clmath.sin(w);
	float c = a;
	
	float t00 = 1+(a*b);
	float t01 = a+c*(1+(a*b));
	float t10 = b;
	float t11 = (b*c)+1;
	
	xlong f = 0;
	xlong g = 0;
	
	for(uxlong u=0; u<s->height; u++)
	{
		for(uxlong v=0; v<s->width; v++)
		{
			f = (v*t00)+(u*t01);
			g = (v*t10)+(u*t11);
			clscreen.cldoublebuffer[(x+g)*XRES+(f+y)] = s->data[u*s->width+v];
		}
	}
}

void CLgfx::drawspriteanimated(xlong x,xlong y,sprite** s,xlong i) const //! critical
{

}

void CLgfx::putsprite(xlong x,xlong y,sprite* s,sprite* t,xlong m) const //! critical
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
	//uxlong dvalue = 0;
	//*

	//set combinee
	xlong tdiff = 0;
	uxlong* tbuffer = 0;
	if(t==0)
	{
		tdiff = XRES;
		tbuffer = clscreen.cldoublebuffer.getbuffer();
	}
	else
	{
		tdiff = t->width;
		tbuffer = t->data;
	}
	//*
	
	//draw loop
	for(xlong i=0; i<cheight; i++)
	{
		for(xlong j=0; j<cwidth; j++)
		{
			svalue = s->data[soffset];
			switch(m)
			{
				case 0:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //normal
				case 1:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //mirrored horizontal
				case 2:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //mirrored vertical
				case 3:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 90 degrees left
				case 4:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 180 dgrees left
				case 5:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 270 dgrees left
				case 6:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 90 degrees right
				case 7:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 180 degrees right
				case 8:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //rotate 270 degrees right
				case 9:  if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //and
				case 10: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //or
				case 11: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //xor
				case 12: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //nand
				case 13: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //nor
				case 14: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //not
				case 15: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //byte add
				case 16: if( (svalue & 0xFF000000) != 0xFF000000) clscreen.cldoublebuffer[doffset+j] = svalue; break; //byte sub
			}
			soffset++;
		}
		soffset += cdiff;
		doffset += XRES;
	}
	//*
}

void CLgfx::loadfonts(CLfile* f) //! critical
{
	fonts = new CLfont*[8];
	CLar* fontsa = new CLar(f);
	fonts[0] = clformat.loadtileset(fontsa->findbyname(u8"CLmonotype.fnt"),16,16);
	fonts[1] = clformat.loadtileset(fontsa->findbyname(u8"CLteletype.fnt"),16,16);
	fonts[2] = clformat.loadtileset(fontsa->findbyname(u8"CLlinetype.fnt"),16,16);
	fonts[3] = clformat.loadtileset(fontsa->findbyname(u8"CLtermtype.fnt"),16,16);
	fonts[4] = clformat.loadtileset(fontsa->findbyname(u8"CLsegmtype.fnt"),32,60);
	fonts[5] = clformat.loadtileset(fontsa->findbyname(u8"CLtalltype.fnt"),32,32);
	fonts[6] = clformat.loadtileset(fontsa->findbyname(u8"CLsymbtype.fnt"),16,16);
}

xlong CLgfx::drawfontchar(xlong x,xlong y,const uxlong a,uxlong f,uxlong fc,uxlong bc) const //! critical
{
	//select font
	if( (fonts==0) || (a>255) ) { return -1; }
	if(f>6) { f = 0; }
	CLfont* t = fonts[f];
	//*
	
	//xlong ssize = s->size;
	xlong swidth = t[a]->width;
	xlong sheight = t[a]->height;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	//*

	//clipping against screen borders
	if(isoff(xs,ys,xe,ye)) { return -1; }
	clip(xs,ys);
	clip(xe,ye);
	//*

	//set up variables
	xlong xoffset = (ys * XRES) + xs;
	xlong linearc = 0;
	xlong rx = x;
	uxlong srcval = 0;
	//*

	//drawloop
	for(xlong i=0; i<sheight; i++)
	{
		for(xlong j=0; j<swidth; j++)
		{
			srcval = t[a]->data[linearc];
			if(srcval == 0x00FF0000) clscreen.cldoublebuffer[xoffset+j] = fc;
			else if(bc!=0 && srcval == 0x00FFFFFF) clscreen.cldoublebuffer[xoffset+j] = bc;
			if(i==0 && srcval != 0x00000000) rx++;
			linearc++;
		}
		xoffset += XRES;
	}
	//*
	
	return rx;
}

void CLgfx::drawfontstring(xlong x,xlong y,const xchar* a,uxlong f,uxlong fc,uxlong bc,xlong s) const //! critical
{
	if(fonts==0) { return; }
	if(f>6) { f = 0; }
	if(s==0) { s = clstring.length(a); }
	xlong t = x;
	//xlong dx = fonts[f][0]->width;
	xlong dy = fonts[f][0]->height;
	
	for(xlong i=0; i<s; i++)
	{
		if(a[i]=='\n') { t = x; y += dy; }
		else
		{
			t = drawfontchar(t,y,a[i],f,fc,bc);
			if(t==-1) { return; }
		}
	}
}

xlong CLgfx::getfontstringwidth(const char* a,uxlong f) const //! critical
{
	//select font
	if(fonts==0) { return 0; }
	if(f>6) { f = 0; }
	CLfont* t = fonts[f];
	//*
	
	xlong l = clstring.length(a);
	xlong r = 0;
	//uxlong srcoff = 0;
	
	for(xlong i=0; i<l; i++)
	{ for(uxlong j=0; j<t[i]->width; j++) { if(t[i]->data[j] != 0x00000000) { r++; } } }
	
	return r;
}

xlong CLgfx::getfontstringheight(const char* a,uxlong f) const //! critical
{
	//select font
	if(fonts==0) { return 0; }
	if(f>6) { f = 0; }
	CLfont* t = fonts[f];
	//*
	
	xlong l = clstring.length(a);
	xlong r = t[0]->height;
	
	for(xlong i=0; i<l; i++) { if(a[i]=='\n') r += t[i]->height; }
	
	return r;
}

sprite* CLgfx::savescreen() const //! critical
{
	sprite* r = new sprite;
	r->width = XRES;
	r->height = YRES;
	r->size = (XRES*YRES);
	r->data = new uxlong[(r->size)];
	
	for(uxlong i=0; i<r->size; i++) { r->data[i] = clscreen.cldoublebuffer[i]; }
	
	return r;
}

void CLgfx::drawscreen(sprite* s) const //! critical
{
	//check if sprite has correct dimensions
	if(s->width==XRES && s->height==YRES)
	{
		//draw loop
		for(uxlong i=0; i<s->size; i++) { if( (s->data[i] & 0xFF000000) != 0xFF) { clscreen.cldoublebuffer[i] = s->data[i]; } }
		//*
	}
	//*
}

uxlong CLgfx::getEGAcolor(xchar c) const //! critical
{
	//return the 16 EGA colors
	switch(c)
	{
		case 0:  return 0x00000000; break; //black
		case 1:  return 0x000000FF; break; //blue
		case 2:  return 0x0000FF00; break; //green
		case 3:  return 0x0000FFFF; break; //cyan
		case 4:  return 0x00FF0000; break; //red
		case 5:  return 0x00FF00FF; break; //magenta
		case 6:  return 0x00FFFF00; break; //yellow
		case 7:  return 0x00808080; break; //dark white?
		case 8:  return 0x00404040; break; //gray
		case 9:  return 0x008080FF; break; //light blue
		case 10: return 0x0080FF80; break; //light green
		case 11: return 0x0080FFFF; break; //light cyan
		case 12: return 0x00FF8080; break; //light red
		case 13: return 0x00FF80FF; break; //light magenta
		case 14: return 0x00FFFF80; break; //light yellow
		case 15: return 0x00FFFFFF; break; //white
		default: return 0;
	}
	//*
}

uxlong* CLgfx::getgradient(uxlong s,uxlong e,xlong i) const //! critical
{
	doubleword a = { s };
	doubleword b = { e };
	
	float at = (float(b.db[0]) - float(a.db[0])) / float(i); 
	float rt = (float(b.db[1]) - float(a.db[1])) / float(i); 
	float gt = (float(b.db[2]) - float(a.db[2])) / float(i); 
	float bt = (float(b.db[3]) - float(a.db[3])) / float(i); 
	
	uxlong* r = new uxlong[i];
	
	for(xlong j=0; j<i; j++)
	{
		a.db[0] = uxchar(float(a.db[0]) + at);
		a.db[1] = uxchar(float(a.db[1]) + rt);
		a.db[2] = uxchar(float(a.db[2]) + gt);
		a.db[3] = uxchar(float(a.db[3]) + bt);
		r[i] = a.dd;
	}
	
	return r;
}

uxlong CLgfx::blendcolors(uxlong c1,uxlong c2,xlong m) const //! critical
{
	uxlong r = 0;
	
	switch(m)
	{
		case 0:  r = c1; break; //normal
		case 1:  r = c1 & c2; break; //and
		case 2:  r = c1 | c2; break; //or	
		case 3:  r = c1 ^ c2; break; //xor
		case 4:  r = ~(c1 & c2); break; //nand
		case 5:  r = ~(c1 | c2); break; //nor
		case 6:  r = ~c1; break; //not
		case 7:  r = byteadd(c1,c2); break; //add
		case 8:  r = bytesub(c1,c2); break; //sub
		default: r = c1; break;
	}
	
	return r;
}
///*

#endif

