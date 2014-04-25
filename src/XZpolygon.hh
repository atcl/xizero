///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Polygon Class ( XZpolygon.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZmath.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZgfx.hh"
#include "XZscreen.hh"
///</include>

///<declare>
#define PRJX 100
#define PRJY 100

#define AMBIENT 24
#define ZLIGHT  FXTNT
#define NOLIGHT 10

#define R_A 0x00000001 // ambient
#define R_Z 0x00000010 // z light
#define R_N 0x00000100 // no light
#define R_S 0x00001000 // shape
#define R_B 0x00010000 // blinn shadows
#define R_F 0x00100000 // flat
#define R_C 0x01000000 // unused
#define R_D 0x10000000 // depth test
///</declare>

///<define>

class polygon
{
	private:
		static vector point[3];		// discretized vertices

		vector vertex[3];		// polygon vertices
		vector normal;			// polygon normal
		const yint color;		// polygon color

		yint flat(xint pz,xint f) const;
		void raster(uint c,bool s=0) const; //hot;
		void shape(yint c) const;
	public:
		polygon(const vector& x,const vector& y,const vector& z,yint c);

		void update(const matrix& m);
		void display(const vector& p,xint f,yint c=0) const;

		inline const vector& n() const { return normal; }

		static vector project(const vector& p,const vector& v);

		static yint  counter;		// polygon counter
		static const vector light;	// light vector
		static const matrix blinn;	// blinn shadow matrix
};
///</define>

///<code>
      vector polygon::point[] = { vector{0,0,0,0},vector{0,0,0,0},vector{0,0,0,0} };
      yint   polygon::counter = 0;
const vector polygon::light   = vector{ FXONE, FXONE, FXONE, FXONE+FXONE+FXONE };
const matrix polygon::blinn   = []()->matrix { matrix m; m.shadow(vector{0,FXTNT,FXONE,FXONE},vector{0,4*FXTNT,FXONE+FXTNT,FXONE}); return m; }(); 

void polygon::shape(yint c) const 
{
	gfx::line(point[0].x,point[0].y,point[1].x,point[1].y,c);
	gfx::line(point[1].x,point[1].y,point[2].x,point[2].y,c);
	gfx::line(point[2].x,point[2].y,point[0].x,point[0].y,c);
}

vector polygon::project(const vector& p,const vector& v)
{
	const fixed z = fx::l2f(p.z) + v.z;

	return vector{p.x + fx::f2l(fx::div(fx::mul(v.x,FX(PRJX)),FX(ZRES) - z)),
                      p.y - fx::f2l(fx::div(fx::mul(v.y,FX(PRJY)),FX(ZRES) - z)), z, 0}; //r2l?
}

yint polygon::flat(xint pz,xint f) const
{
	const fixed t = math::lim(FXTNT,math::abs(fx::div(fx::dot(normal,light),fx::mul(normal.e,light.e))),FXONE);
	const byte anz = math::set(AMBIENT,f&R_A) + math::set(NOLIGHT,f&R_N) + math::set(fx::r2l(fx::mul(ZLIGHT,fx::l2f(pz))),f&R_Z); 

	rgba argb{ (uint)math::set(ORANGE,color,f&R_C) };
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]),t ) + anz) );
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]),t ) + anz) );
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]),t ) + anz) );
	argb.b[3] = 0;
	return argb.d;
}

void polygon::raster(yint c,bool s) const
{
	// determine projected minima and maxima
	const xint mix01 = point[1].x<point[0].x;
	const xint max01 = !mix01;
	const xint miy01 = point[1].y<point[0].y;
	const xint may01 = !miy01;

	const xint mixi = math::set(2,mix01,point[2].x<point[mix01].x);
	const xint maxi = math::set(2,max01,point[2].x>point[max01].x);
	const xint miyi = math::set(2,miy01,point[2].y<point[miy01].y);
	const xint mayi = math::set(2,may01,point[2].y>point[may01].y);

	const xint minx = math::max(XMIN,point[mixi].x);
	const xint maxx = math::min(XMAX,point[maxi].x); //+1 prevent gap
	const xint miny = math::max(YMIN,point[miyi].y);
	const xint maxy = math::min(YMAX,point[mayi].y); //+1 prevent gap

	guard( (maxx==minx) || (maxy==miny) );

	const fixed zx = fx::div(point[maxi].z-point[mixi].z,fx::l2f(point[maxi].x-point[mixi].x));
	const fixed zy = fx::div(point[mayi].z-point[miyi].z,fx::l2f(point[mayi].y-point[miyi].y));

	const vector dx{ point[0].x-point[1].x, point[1].x-point[2].x, point[2].x-point[0].x,  zy };
	const vector dy{ point[0].y-point[1].y, point[1].y-point[2].y, point[2].y-point[0].y, -zx };

	vector cy{ dy.x*(point[0].x - minx) + dx.x*(miny - point[0].y) + ((dy.x<0) || (dy.x==0 && dx.x>0)),
	           dy.y*(point[1].x - minx) + dx.y*(miny - point[1].y) + ((dy.y<0) || (dy.y==0 && dx.y>0)),
	           dy.z*(point[2].x - minx) + dx.z*(miny - point[2].y) + ((dy.z<0) || (dy.z==0 && dx.z>0)),
	           point[miyi].z + fx::mul(fx::l2f(point[mixi].x-point[miyi].x),zx) }; 

	const xint str = XRES - (maxx-minx);

	switch(s)
	{
		case false: // default
			for(xint y=miny,off=miny*XRES+minx;y<maxy;++y)
			{
				vector cx(cy);

				//#pragma prefetch frame
				for(xint x=minx;x<maxx;++x) 
				{
					const bool inside = (cx.x>0) && (cx.y>0) && (cx.z>0);
					const bool above  = cx.e > screen::depth[off]; //math::neg(screen::depth[off],screen::zs);
					if(inside && above)
					{
						screen::depth[off] = cx.e; //math::neg(cx.e,screen::zs);
						screen::frame[off] = c;
					}

					cx -= dy;
					++off;
				}

				cy += dx;
				off += str;
			}
			break;

		case true: // shadow
			for(xint y=miny,off=miny*XRES+minx;y<maxy;++y)
			{
				vector cx(cy);

				//#pragma prefetch frame[off]
				for(xint x=minx;x<maxx;++x) 
				{
					const bool inside = (cx.x>0) && (cx.y>0) && (cx.z>0);
					if(inside)
					{
						screen::frame[off] = screen::frame[off]&c; // &=
					}

					cx -= dy;
					++off;
				}

				cy += dx;
				off += str;
			}
			break;
	}

}

polygon::polygon(const vector& x,const vector& y,const vector& z,yint c) : vertex{x,y,z},normal{0,0,0,0},color(c) 
{
	vertex[0].e = FXONE;
	vertex[1].e = FXONE;
	vertex[2].e = FXONE;

	normal   = fx::cross(vertex[2]-vertex[0],vertex[1]-vertex[0]);
	normal   = fx::mul(normal,FXCEN);
	normal.e = fx::len(normal);
}

void polygon::update(const matrix& m)
{
	vertex[0] = m*vertex[0];
	vertex[1] = m*vertex[1];
	vertex[2] = m*vertex[2];

	normal    = fx::cross(vertex[2]-vertex[0],vertex[1]-vertex[0]);
	normal    = fx::mul(normal,FXCEN);
	normal.e  = fx::len(normal);
}

void polygon::display(const vector& p,xint f,yint c) const
{
	guard(normal.z>FXHLF);
	++counter;	

	if(f&R_B) 
	{
		point[0] = project(p,blinn*vertex[0]);
		point[1] = project(p,blinn*vertex[1]);
		point[2] = project(p,blinn*vertex[2]);
		raster(c,1);
	}
	else
	{
		point[0] = project(p,vertex[0]);
		point[1] = project(p,vertex[1]);
		point[2] = project(p,vertex[2]);
		ifl(f&R_F) { c = flat(p.z,f); }
		ifu(f&R_S) { shape(c); } else { raster(c); }
	}
}
///</code>

