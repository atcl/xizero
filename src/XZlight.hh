// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZlight.hh
// Light Source Class Library 

///guard
#ifndef HH_XZLIGHT
#define HH_XZLIGHT
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
		sint col;
		tile mask;

		sint lambert(sint x,sint y) const;
		void init(bool i=0);
	public:
		light(sint r,sint c) : rad(r), col(c) { init(); }
		~light() { delete mask.data; }
		inline void draw(sint x,sint y) const;
		inline void color(sint c);
		inline void radius(sint r);
};
///*

///implementation
sint light::lambert(sint x,sint y) const
{
	const sint  i = rad*rad;
	const sint  d = (x*x)+(y*y);
	const fixed l = fixed(-i+d)/fixed(d*(1-i));
	
	packed c = { col };
	
	c.b[1] = math::max(0,fx::mul(fixed(c.b[1]),l));
	c.b[2] = math::max(0,fx::mul(fixed(c.b[2]),l));
	c.b[3] = math::max(0,fx::mul(fixed(c.b[3]),l));

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

	for(sint i=0,t=0;i<mask.width;++i)
	{
		for(sint j=0;j<mask.width;++j,++t)
		{
			mask.data[t] = lambert(j-rad,i-rad);
		}
	}
}

void light::draw(sint x,sint y) const
{
	gfx::sprite(mask,x-rad,y-rad);
}

void light::color(sint c)
{
	col = c;
	init();
}

void light::radius(sint r)
{
	rad = r;
	init(1);
}
///*

#endif

