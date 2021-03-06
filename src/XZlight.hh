///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Light Source Class ( XZlight.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZtile.hh"
#include "XZgfx.hh"
#include "XZmath.hh"
///</include>

///<define>
class light
{
	private:
		tile mask;
		const xint rad;
		const yint col;

		xint lambert(xint x,xint y) const;
	public:
		light(xint r,yint c);
		inline void draw(xint x,xint y) const;
		
		static vector refract(const vector& x,const vector& y,fixed ri);
};
///</define>

///<code>
xint light::lambert(xint x,xint y) const
{
	const xint  i = rad*rad;
	const xint  d = (x*x)+(y*y);
	const fixed l = fx::div(fx::l2f(-i+d),fx::l2f(d*(1-i)));
	
	rgba c = { col };
	c.b[1] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[1]),l)));
	c.b[2] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[2]),l)));
	c.b[3] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[3]),l)));
	return c.d;
}

light::light(xint r,yint c) : mask((r*2)+1,(r*2)+1), rad(r), col(c)
{
	for(yint j=0,t=0;j<mask.width;++j)
	{
		for(yint k=0;k<mask.width;++k)
		{
			mask.data[t++] = lambert(k-rad,j-rad);
		}
	}
}

void light::draw(xint x,xint y) const
{
	gfx::draw(mask,x-rad,y-rad);
}

vector light::refract(const vector& x,const vector& y,fixed ri)
{
	fixed t = fx::dot(x,y);
	fixed r = FXONE - fx::mul(ri,ri) * (FXONE - fx::mul(t,t));

	vector z{0,0,0,0};
	if(r>0)
	{
		fixed s = ri * t + fx::sqr(r);
		z.x = fx::mul(ri,x.x) - fx::mul(s,y.x);
		z.y = fx::mul(ri,x.y) - fx::mul(s,y.y);
		z.z = fx::mul(ri,x.z) - fx::mul(s,y.z);
	}

	return z;
}
///</code>

