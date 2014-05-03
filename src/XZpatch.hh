///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Terrain Patch Class ( XZpatch.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZfixed.hh"
#include "XZvector.hh"
#include "XZscreen.hh"
///</include>

///<define>
class patch
{
	private:
		const vector normal[2];
		const bool   split;		// 0: ul->lr, 1: ll->ur

		yint shade(bool n,const vector& l,yint c) const;
	public:
		patch(const vector& m,const vector&n,bool s);
		void display(const vector& p,const vector& l,yint c) const;
};
///</define>

///<code>
yint patch::shade(bool n,const vector& l,yint c) const
{
	const fixed t = math::lim(FXTNT,fx::dot(normal[n],l),FXONE);

	rgba argb{ c };
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]),t ) ) );
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]),t ) ) );
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]),t ) ) );
	argb.b[3] = 0;

	return argb.d;
}

patch::patch(const vector& m,const vector& n,bool s) :
	normal{m,n},
	split(s)
{ }

void patch::display(const vector& p,const vector& l,yint c) const
{
	yint s[2] = { shade(0,l,c),shade(1,l,c) };
	yint o = p.y*XRES+p.x;
	
	const yint dx = XRES - p.e;
	
	switch(split)
	{
		case 0:
			for(xint i=0;i<p.e;++i)
			{
				for(xint j=0;j<p.e;++j)
				{
					screen::frame[o] = s[j>i];
					++o;
				}
				o += dx;
			}
			break;
			
		case 1:
			for(xint i=0;i<p.e;++i)
			{
				for(xint j=0;j<p.e;++j)
				{
 					screen::frame[o] = s[(p.e-j)>i];
					++o;
				}
				o += dx;
			}
			break;
	}
}
///</code>
