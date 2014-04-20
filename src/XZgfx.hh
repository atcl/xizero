///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Graphic Primitives Library ( XZgfx.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZtile.hh"
#include "XZmath.hh"
#include "XZscreen.hh"
///</include>

///<define>
namespace gfx
{
	/*OK*/ void line(xint x,xint y,xint a,xint b,yint c,bool k=0); 			// draw line
	/*OK*/ void rect(xint x,xint y,xint a,xint b,yint c,yint d,bool f=0,bool g=0); 	// draw rectangle
	/*OK*/ void draw(const tile& t,xint x=0,xint y=0,bool a=0);			// draw sprite
	/*OK*/ void fsprog(xint p,yint c=RED);						// draw full screen progress bar
	/*OK*/ tile save();								// save current screen
}
///</define>

///<code>
void gfx::line(xint x,xint y,xint a,xint b,yint c,bool k)
{
	guard( !screen::onscreen(x,y) || !screen::onscreen(a,b) );

	xint dx = a-x;
	xint dy = b-y;

	x = math::lim(0,x,XRES-1);
	y = math::lim(0,y,YRES-1);
	a = math::lim(0,a,XRES-1);
	b = math::lim(0,b,YRES-1);

	xint pp = 0;
	xint qq = 0;

	xint xs = 1;
	xint ys = XRES;
	rgba u = { c };
	rgba v = { c };

	switch( math::set(2,xint(y==b)-xint(x==a),k==1) )
	{
		case -1: // vertical line
		{
			const xint p = math::min(y,b);
			const xint q = math::max(y,b);
			for(xint i=p,o=p*XRES+x;i<=q;++i,o+=XRES)
			{
				screen::frame[o] = c;
			}
		}
		break;

		case 1: // horizontal line
		{
			const xint p = math::min(x,a);
			const xint q = math::max(x,a);
			for(xint i=p,o=y*XRES+p;i<=q;++i)
			{
				screen::frame[o++] = c;
			}
		}
		break;

		default:
		case 0: // general line
		{
			xs = math::neg(xs,dx<0);
			dx = math::neg(dx,dx<0);
			ys = math::neg(ys,dy<0);
			dy = math::neg(dy,dy<0);
			pp = xs;
			xs = math::set(ys,xs,dy>dx);
			ys = math::set(pp,ys,ys==xs);
			pp = dx;
			dx = math::set(dy,dx,dy>dx);
			dy = math::set(pp,dy,dy==dx);
			pp = dy;
			qq = dx;
			for(xint i=0,o=y*XRES+x;i<=qq;++i)
			{
				screen::frame[o] = c;
				pp += dy;
				o += xs + math::set(ys,pp>=dx);
				pp -= math::set(dx,pp>=dx);
			}
		}
		break;

		case 2: // antialiased line
		{
			xs = math::neg(xs,dx<0);
			dx = math::neg(dx,dx<0);
			ys = math::neg(ys,dy<0);
			dy = math::neg(dy,dy<0);
			pp = xs;
			xs = math::set(ys,xs,dy>dx);
			ys = math::set(ys,pp,ys!=xs);
			pp = dx;
			dx = math::set(dy,dx,dy>dx);
			dy = math::set(dy,pp,dy!=dx);
			pp = dy;
			qq = dx;
			for(xint i=0,o=y*XRES+x;i<=qq;++i)
			{
				u.d = screen::frame[o-xs];
				u.b[0] = 0xFF;
				u.b[1] = (u.b[1] + v.b[1])>>1;
				u.b[2] = (u.b[2] + v.b[2])>>1;
				u.b[3] = (u.b[3] + v.b[3])>>1;	
				screen::frame[o-xs] = u.d;
				screen::frame[o] = c;
				u.d = screen::frame[o+xs];
				u.b[0] = 0xFF;
				u.b[1] = (u.b[1] + v.b[1])>>1;
				u.b[2] = (u.b[2] + v.b[2])>>1;
				u.b[3] = (u.b[3] + v.b[3])>>1;	
				screen::frame[o+xs] = u.d;
				pp += dy;
				o += xs + math::set(ys,pp>=dx);
				pp -= math::set(dx,pp>=dx);
			}
		}
		break;
	}
}

void gfx::rect(xint x,xint y,xint a,xint b,yint c,yint d,bool f,bool g)
{
	// draw frame
	line(x,y,a,y,c);
	line(a,y,a,b,c);
	c = math::set(~c|0xFF000000,c,g==1); 
	line(x,y,x,b,c); 
	line(x,b,a,b,c);

	// draw inside
	++x;
	--a;
	for(xint i=y++;i<b&&f==1;++i)
	{
		line(x,i,a,i,d,0);
	}
}

void gfx::draw(const tile& t,xint x,xint y,bool a)
{
	const xint xd = -XRES+x+t.width;
	const xint yd = -YRES+y+t.height;
	const xint xm = t.width-math::set(xd,xd>0);
	const xint ym = t.height-math::set(yd,yd>0);
	const xint sx = t.width - xm;
	const xint d  = XRES - xm;

	for(xint i=0,o=y*XRES+x,s=0;i<ym;++i,o+=d,s+=sx)
	{
		#pragma prefetch t.data
		#pragma prefetch frame 
		for(xint j=0;j<xm;++j,++o)
		{
			const uint c = t.data[s++];
			screen::frame[o] = math::set(c,screen::frame[o],(c!=TRANS)||a);
		}
	}
}

void gfx::fsprog(xint p,yint c)
{
	p = math::lim(0,p,100);
	const xint r = (XRES*p)/100;
	const xint q = XRES - r;
	for(xint i=0,o=0;i<YRES;++i,o+=q)
	{
		for(xint j=0;j<r;++j)
		{
			screen::frame[o++] &= c;
		}
	}
}

tile gfx::save()
{
	tile r({XRES,YRES});
	for(xint i=0;i<XRES*YRES;++i) { r.data[i] = screen::frame[i]; }
	return r;
}
///</code>

