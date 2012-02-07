// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZgfx.hh
// Graphic Primitives Library 

///guard
#ifndef HH_XZGFX
#define HH_XZGFX
//#pragma message "Compiling " __FILE__ "..." " TODO: Guards"
///*

///includes
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZmath.hh"
#include "XZscreen.hh"
///*

///declarations
#ifndef TILE
#define TILE
struct tile
{
	sint  width;
	sint  height;
	sint* data;
};
#endif
///*

///definitions
namespace gfx
{
	/*OK*/ inline void circpix(sint x,sint y,sint l,sint t,uint c);			//draw circle pixel
	/*OK*/ inline void pix(sint x,sint y,uint c) { screen::back[(y*XRES+x)] = c; }	//draw pixel
	/*OK*/ void line(sint x,sint y,sint a,sint b,uint c,bool k=0); 			//draw line
	/*OK*/ void rect(sint x,sint y,sint a,sint b,uint c,uint d,bool f=0,bool g=0); 	//draw rectangle
	/*OK*/ void circ(sint x,sint y,sint r,uint c);					//draw circle
	/*OK*/ void sprite(const tile& t,sint x,sint y,bool a=0);			//draw sprite
	/*OK*/ void fsprog(sint p,uint c=RED);						//draw full screen progress bar
	/*OK*/ tile* save();								//save current screen
}
///*

///implementation
void gfx::circpix(sint x,sint y,sint l,sint t,uint c)
{
	const sint o = (y*XRES)+x;
	const sint p = t*XRES;
	const sint q = l*XRES;
	screen::back[o+p+l] = c;
	screen::back[o-p+l] = c;
	screen::back[o+p-l] = c;
	screen::back[o-p-l] = c;
	screen::back[o+q+t] = c;
	screen::back[o-q+t] = c;
	screen::back[o+q-t] = c;
	screen::back[o-q-t] = c;
}

void gfx::line(sint x,sint y,sint a,sint b,uint c,bool k)
{
	guard( (x<0) || (y<0) || (x>XRES) || (y>YRES) || (a<0) || (b<0) || (a>XRES) || (b>YRES) );
	const sint s = math::set(2,sint(y==b)-sint(x==a),k==1);

	sint o = 0;
	sint p = 0;
	sint q = 0;
	sint dx = 0;
	sint dy = 0;
	sint xs = 1;
	sint ys = XRES;
	packed u = { c };
	packed v = { c };

	switch(s)
	{
		case -1: //vertical line
			p = math::min(y,b);
			q = math::max(y,b);
			o = p*XRES+x;
			for(sint i=p;i<=q;++i)
			{
				screen::back[o] = c;
				o += XRES;
			}
		break;

		case 1: //horizontal line
			p = math::min(x,a);
			q = math::max(x,a);
			o = y*XRES+p;
			for(sint i=p;i<=q;++i)
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
			for(sint i=0;i<=q;++i)
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
			for(sint i=0;i<=q;++i)
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
}

void gfx::rect(sint x,sint y,sint a,sint b,uint c,uint d,bool f,bool g)
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
		for(sint i=y;i<b;++i)
		{
			line(x,i,a,i,d,0);
		}
	}
}

void gfx::circ(sint x,sint y,sint r,uint c)
{
	//guard(x-r<0 || x+r>XRES || y-r<0 || y+r>YRES);
	sint d  = 3 - (r<<1);
	sint cx = 0;
	sint cy = r;

	while(cx<=cy)
	{
		circpix(x,y,cx,cy,c);
		const sint ct = math::set(cy-1,cy,d>0);
		d = math::set(d+(cx<<2)+6,d+((cx-cy)<<2)+10,d<0);
		cy = ct;
		++cx;
	}
}

void gfx::sprite(const tile& t,sint x,sint y,bool a)
{
	const sint xd = -XRES+x+t.width;
	const sint yd = -YRES+y+t.height;
	const sint xmax = math::set(t.width,t.width-xd,xd<=0);
	const sint ymax = math::set(t.height,t.width-yd,yd<=0);
	const sint sx = t.width - xmax;
	const sint d = XRES - xmax;

	for(sint i=0,o=y*XRES+x,s=0;i<ymax;++i,o+=d)
	{
		for(sint j=0;j<xmax;++j,++o,++s)
		{
			const packed c = { t.data[s] };
			screen::back[o] = math::set(c.d,screen::back[o],c.b[0]==0xFF||a==1); 
		}
		s+=sx;
	}
}

void gfx::fsprog(sint p,uint c)
{
	p = math::min(math::max(p,100),0);
	const sint r = (XRES*p)/100;
	const sint q = XRES - r*p;
	for(uint i=0,o=0;i<YRES;++i,o+=q)
	{
		for(sint j=0;j<r;++j,++o)
		{
			screen::back[o] &= c;
		}
	}
}

tile* gfx::save()
{
	const sint l = XRES*YRES;
	sint* d = new sint[l];
	tile* r = new tile({XRES,YRES,d});
	for(sint i=0;i<l;++i) { d[i] = screen::back[i]; }
	return r;
}
///*

#endif

