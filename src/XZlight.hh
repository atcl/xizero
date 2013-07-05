///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZlight.hh
// Light Source Class Library 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
#include "XZgfx.hh"
#include "XZmath.hh"
///</include>

///<define>
class light
{
	private:
		xint rad;
		yint col;
		tile mask;

		xint lambert(xint x,xint y) const;
		void init(bool i=0);
	public:
		light(xint r,yint c) : rad(r), col(c), mask({0,0,0}) { init(); }
		~light() { delete mask.data; }
		inline void draw(xint x,xint y) const { gfx::sprite(mask,x-rad,y-rad); }
		inline void color(yint c) { col = c; init(); }
		inline void radius(xint r) { rad = r; init(1); }
		static fvector refract(const fvector& x,const fvector& y,fixed ri);
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

void light::init(bool i)
{
	if(i!=0)
	{
		const xint dim = (rad<<1)+1;
		mask.width = mask.height = dim;
		delete mask.data;
		mask.data = new xint[dim*dim];
	}

	for(xint j=0,t=0;j<mask.width;++j)
	{
		for(xint k=0;k<mask.width;++k)
		{
			mask.data[t++] = lambert(k-rad,j-rad);
		}
	}
}

fvector light::refract(const fvector& x,const fvector& y,fixed ri)
{
	fixed t = x.dot(y);
	fixed r = FXONE - fx::mul(ri,ri) * (FXONE - fx::mul(t,t));

	fvector z(0,0,0,0);
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

