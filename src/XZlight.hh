// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
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
		sint _radius;
		sint _color;
		tile _mask;

		sint lambert(sint x,sint y) const;
		void init(bool i=0);
	public:
		light(sint r,sint c) : _radius(r), _color(c) { init(); }
		~light() { delete _mask.data; }
		inline void draw(sint x,sint y) const;
		inline void color(sint c);
		inline void radius(sint r);
};
///*

///implementation
sint light::lambert(sint x,sint y) const
{
	const sint i = _radius*_radius;
	const sint d = (x*x)+(y*y);
	const fixed l = fixed(-i+d)/fixed(d*(1-i));
	
	packed c = { _color };
	
	c.b[1] = math::max(0,fixed(c.b[1])*l);
	c.b[2] = math::max(0,fixed(c.b[2])*l);
	c.b[3] = math::max(0,fixed(c.b[3])*l);

	return c.d;
}

void light::init(bool i)
{
	if(i!=0)
	{
		const sint dim = (_radius<<1)+1;
		_mask.width = _mask.height = dim;
		delete _mask.data;
		_mask.data = new sint[dim*dim];
	}

	for(sint i=0,t=0;i<_mask.width;++i)
	{
		for(sint j=0;j<_mask.width;++j,++t)
		{
			_mask.data[t] = lambert(j-_radius,i-_radius);
		}
	}
}

void light::draw(sint x,sint y) const
{
	gfx::sprite(_mask,x-_radius,y-_radius);
}

void light::color(sint c)
{
	_color = c;
	init();
}

void light::radius(sint r)
{
	_radius = r;
	init(1);
}
///*

#endif

