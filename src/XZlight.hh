// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZlight.hh
// Light Source Class Library 

///guard
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZgfx.hh"
#include "XZmath.hh"
///*

///definitions
class light
{
	private:
		sint rad;
		uint col;
		tile mask;

		sint lambert(sint x,sint y) const;
		void init(bool i=0);
	public:
		light(sint r,uint c) : rad(r), col(c), mask({0,0,0}) { init(); }
		~light() { delete mask.data; }
		inline void draw(sint x,sint y) const { gfx::sprite(mask,x-rad,y-rad); }
		inline void color(uint c) { col = c; init(); }
		inline void radius(sint r) { rad = r; init(1); }
};
///*

///implementation
sint light::lambert(sint x,sint y) const
{
	const sint  i = rad*rad;
	const sint  d = (x*x)+(y*y);
	const fixed l = fx::div(fx::l2f(-i+d),fx::l2f(d*(1-i)));
	
	packed c = { col };
	c.b[1] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[1]),l)));
	c.b[2] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[2]),l)));
	c.b[3] = fx::r2l(math::max(0,fx::mul(fx::l2f(c.b[3]),l)));
	return c.d;
}

void light::init(bool i)
{
	if(i!=0)
	{
		const sint dim = (rad<<1)+1;
		mask.width = mask.height = dim;
		delete mask.data;
		mask.data = new sint[dim*dim];
	}

	for(sint j=0,t=0;j<mask.width;++j)
	{
		for(sint k=0;k<mask.width;++k,++t)
		{
			mask.data[t] = lambert(k-rad,j-rad);
		}
	}
}
///*

