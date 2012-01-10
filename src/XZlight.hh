// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZlight.hh
// Light Source Class Library 

///guard
#ifndef HH_XZLIGHT
#define HH_XZLIGHT
///*

///includes
#include "XZgfx.hh"
#include "XZmath.hh"
///*

///definitions
class light
{
	private:
		long _radius;
		long _color;
		tile _mask;

		long lambert(long x,long y) const;
		void init(bool i=0);
	public:
		light(long r,long c) : _radius(r), _color(c) { init(); }
		~light() { delete _mask.data; }
		INLINE void draw(long x,long y) const;
		INLINE void color(long c);
		INLINE void radius(long r);
};
///*

///implementation
long light::lambert(long x,long y) const
{
	const long i = _radius*_radius;
	const long d = (x*x)+(y*y);
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
		const long dim = (_radius<<1)+1;
		_mask.width = _mask.height = dim;
		delete _mask.data;
		_mask.data = new long[dim*dim];
	}

	for(long i=0,t=0;i<_mask.width;++i)
	{
		for(long j=0;j<_mask.width;++j,++t)
		{
			_mask.data[t] = lambert(j-_radius,i-_radius);
		}
	}
}

void light::draw(long x,long y) const
{
	gfx::sprite(_mask,x-_radius,y-_radius);
}

void light::color(long c)
{
	_color = c;
	init();
}

void light::radius(long r)
{
	_radius = r;
	init(1);
}
///*

#endif

