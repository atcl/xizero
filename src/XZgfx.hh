// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZgfx.hh
// Graphic Primitives Library 

///guard
#ifndef HH_XZGFX
#define HH_XZGFX
///*

///includes
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZmath.hh"
#include "XZscreen.hh"
///*

///declarations
struct tile
{
	long  width;
	long  height;
	long* data;
};
///*

///definitions
namespace gfx
{
	/*OK*/ INLINE void circpix(long x,long y,long l,long t,long c);
	/*OK*/ INLINE long pix(long x,long y) { return screen::back[(y*XRES+x)]; }
	/*OK*/ INLINE void pix(long x,long y,long c) { screen::back[(y*XRES+x)] = c; }
	/*OK*/ void line(long x,long y,long a,long b,long c,bool k=0); 
	/*OK*/ void rect(long x,long y,long a,long b,long c,long d,bool f=0,bool g=0); 
	/*OK*/ void circ(long x,long y,long r,long c);
	void sprite(const tile& t,long x,long y);
	void fsprog(long p,long c=RED);
}
///*

///implementation
void gfx::circpix(long x,long y,long l,long t,long c)
{
	const long o = (y*XRES)+x;
	const long p = t*XRES;
	const long q = l*XRES;
	screen::back[o+p+l] = c;
	screen::back[o-p+l] = c;
	screen::back[o+p-l] = c;
	screen::back[o-p-l] = c;
	screen::back[o+q+t] = c;
	screen::back[o-q+t] = c;
	screen::back[o+q-t] = c;
	screen::back[o-q-t] = c;
}

void gfx::line(long x,long y,long a,long b,long c,bool k)
{
	guard(x<0 || y<0 || x>XRES || y>YRES || a<0 || b<0 || a>XRES || b>YRES);
	const long s = math::set(2,long(y==b)-long(x==a),k==1);

	long o = 0;
	long p = 0;
	long q = 0;
	long dx = 0;
	long dy = 0;
	long xs = 1;
	long ys = XRES;
	packed u = { c };
	packed v = { c };

	switch(s)
	{
		case -1: //vertical line
			p = math::min(y,b);
			q = math::max(y,b);
			o = p*XRES+x;
			for(ulong i=p;i<=q;++i)
			{
				screen::back[o] = c;
				o += XRES;
			}
		break;

		case 1: //horizontal line
			p = math::min(x,a);
			q = math::max(x,a);
			o = y*XRES+p;
			for(ulong i=p;i<=q;++i)
			{
				screen::back[o++] = c;
			}
		break;

		default:
		case 0: //general line
			o = y*XRES+x;
			dx = a-x;
			dy = b-y;
			xs = math::neg(xs,dx<0);
			dx = math::neg(dx,dx<0);
			ys = math::neg(ys,dy<0);
			dy = math::neg(dy,dy<0);
			p = xs;
			xs = math::set(ys,xs,dy>dx);
			ys = math::set(p,ys,ys==xs);
			p = dx;
			dx = math::set(dy,dx,dy>dx);
			dy = math::set(p,dy,dy==dx);
			p = dy;
			q = dx;
			for(ulong i=0;i<=q;++i)
			{
				screen::back[o] = c;
				p += dy;
				o += xs + math::set(ys,p>=dx);
				p -= math::set(dx,p>=dx);
			} 
		break;

		case 2: //antialiased line
			o = y*XRES+x;
			dx = a-x;
			dy = b-y;
			xs = math::neg(xs,dx<0);
			dx = math::neg(dx,dx<0);
			ys = math::neg(ys,dy<0);
			dy = math::neg(dy,dy<0);
			p = xs;
			xs = math::set(ys,xs,dy>dx);
			ys = math::set(ys,p,ys!=xs);
			p = dx;
			dx = math::set(dy,dx,dy>dx);
			dy = math::set(dy,p,dy!=dx);
			p = dy;
			q = dx;
			for(ulong i=0;i<=q;++i)
			{
				u.d = screen::back[o-xs];
				u.b[0] = 0xFF;
				u.b[1] = (u.b[1] + v.b[1])>>1;
				u.b[2] = (u.b[2] + v.b[2])>>1;
				u.b[3] = (u.b[3] + v.b[3])>>1;	
				screen::back[o-xs] = u.d;
				screen::back[o] = c;
				u.d = screen::back[o+xs];
				u.b[0] = 0xFF;
				u.b[1] = (u.b[1] + v.b[1])>>1;
				u.b[2] = (u.b[2] + v.b[2])>>1;
				u.b[3] = (u.b[3] + v.b[3])>>1;	
				screen::back[o+xs] = u.d;
				p += dy;
				o += xs + math::set(ys,p>=dx);
				p -= math::set(dx,p>=dx);
			} 
		break;
	}

	return;
}

void gfx::rect(long x,long y,long a,long b,long c,long d,bool f,bool g)
{
	line(x,y,a,y,c);
	line(a,y,a,b,c);
	c = math::set(~c|0xFF000000,c,g==1); 
	line(x,y,x,b,c); 
	line(x,b,a,b,c);

	if(f==1)
	{
		++x;
		++y;
		--a;
		for(ulong i=y;i<b;++i)
		{
			line(x,i,a,i,d,0);
		}
	}

	return;
}

void gfx::circ(long x,long y,long r,long c)
{
	//guard
	long d  = 3 - (r<<1);
	long cx = 0;
	long cy = r;
	long ct = 0;

	while(cx<=cy)
	{
		circpix(x,y,cx,cy,c);
		ct = math::set(cy-1,cy,d>0);
		d = math::set(d+(cx<<2)+6,d+((cx-cy)<<2)+10,d<0);
		cy = ct;
		++cx;
	}

	return;
}

void gfx::sprite(const tile& t,long x,long y)
{
	const long d = XRES - t.width;

	for(ulong i=0,o=y*XRES+x,s=0;i<t.height;++i,o+=d)
	{
		for(ulong j=0;j<t.width;++j,++o,++s)
		{
			const packed c = { t.data[s] };
			screen::back[o] = c.d; //math::set(c.d,screen::back[o],c.b[0]==0xFF); 
		}
	}

	return;
}

void gfx::fsprog(long p,long c)
{
	p = math::min(math::max(p,100),0);
	const long r = (XRES*p)/100;
	const long q = -r*p + XRES;
	for(ulong i=0,o=0;i<YRES;++i,o+=q)
	{
		for(ulong j=0;j<r;++j,++o)
		{
			screen::back[o] &= c;
		}
	}

	return;
}
///*

#endif

