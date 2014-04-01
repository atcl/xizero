///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZpolygon.hh
// Polygon Library
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZmath.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZgfx.hh"
#include "XZfrmbuf.hh"
///</include>

///<declare>
#define XMIN 1
#define XMAX XRES-1
#define YMIN 1
#define YMAX YRES-1
#define ZMIN 1
#define ZMAX YRES-1

#define PRJX 600
#define PRJY 480

#define AMBIENT 24
#define ZLIGHT  FXTNT
#define NOLIGHT 10

#define R_A 0x00000001 //ambient
#define R_Z 0x00000010 //zlight
#define R_N 0x00000100 //nolight
#define R_S 0x00001000 //shape
#define R_B 0x00010000 //blinn shadows
#define R_F 0x00100000 //flat
#define R_C 0x01000000 //unused
#define R_D 0x10000000 //depth test
///</declare>

///<define>
class polygon
{
	private:
		static tuple point[4];		// discrete vertices
		vector vertex[3];		// polygon vertices
		vector normal;			// polygon normal
		const yint color;		// polygon color

		/*OK*/ inline void shape() const;
		              yint flat(xint pz,xint f) const;
		              void raster(uint c) const hot;
		              void shadow(uint c) const hot;
	public:
		/*OK*/      polygon(const tuple& x,const tuple& y,const tuple& z,yint c);
		/*OK*/ void update(const matrix& m,bool i=1);
		/*OK*/ void display(const tuple& p,xint f,yint c=0);
		/*OK*/ void pull(fixed a);
		/*OK*/ static tuple project(const tuple& p,const vector& v);

		static xint  counter;		// polygon counter
		static const vector light;	// light vector
		static const matrix blinn;	// blinn shadow matrix
};
///</define>

///<code>
      tuple  polygon::point[] = { tuple(), tuple(), tuple(), tuple() };
      xint   polygon::counter = 0;
const vector polygon::light   = vector(FXONE,FXONE,FXONE,FXONE+FXONE+FXONE);
const matrix polygon::blinn   = []()->matrix { matrix m; m.shadow(vector(0,FXTNT,FXONE),vector(0,4*FXTNT,FXONE+FXTNT)); return m; }(); 

tuple polygon::project(const tuple& p,const vector& v)
{
	const fixed z = v.z + fx::l2f(p.z);
	return tuple{p.x + fx::f2l(fx::mul(FX(PRJX),fx::div(v.x,z))),
                     p.y - fx::f2l(fx::mul(FX(PRJY),fx::div(v.y,z))),z};
}

yint polygon::flat(xint pz,xint f) const
{
	const fixed t = math::lim(FXTNT,math::abs(fx::div(normal.dot(light),fx::mul(normal.e,light.e))),FXONE);
	const byte anz = math::set(AMBIENT,f&R_A) + math::set(NOLIGHT,f&R_N) + math::set(fx::r2l(fx::mul(ZLIGHT,fx::l2f(pz))),f&R_Z); 

	rgba argb{ (uint)math::set(ORANGE,color,f&R_C) };
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]),t ) + anz) );
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]),t ) + anz) );
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]),t ) + anz) );
	argb.b[3] = 0;
	return argb.d;
}

void polygon::shape() const 
{
	gfx::line(point[0].x,point[0].y,point[1].x,point[1].y,color);
	gfx::line(point[1].x,point[1].y,point[2].x,point[2].y,color);
	gfx::line(point[2].x,point[2].y,point[0].x,point[0].y,color);
}

void polygon::raster(yint c) const
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
	const xint maxx = math::min(XMAX,point[maxi].x+1); //prevent gap
	const xint miny = math::max(YMIN,point[miyi].y);
	const xint maxy = math::min(YMAX,point[mayi].y+1); //prevent gap


	guard( (maxx==minx) || (maxy==miny) );

	const fixed zx = math::abs(fx::div(point[maxi].z-point[mixi].z,fx::l2f(maxx-minx)));
	const fixed zy = math::abs(fx::div(point[mayi].z-point[miyi].z,fx::l2f(maxy-miny)));

	const xint dx[4]{point[0].x-point[1].x,point[1].x-point[2].x,point[2].x-point[0].x,-zx};
	const xint dy[4]{point[0].y-point[1].y,point[1].y-point[2].y,point[2].y-point[0].y,zy};

	xint cy[4]{dy[0]*(point[0].x - minx) + dx[0]*(miny - point[0].y) - ((dy[0]<0) || (dy[0]==0 && dx[0]>0)),
	           dy[1]*(point[1].x - minx) + dx[1]*(miny - point[1].y) - ((dy[1]<0) || (dy[1]==0 && dx[1]>0)),
	           dy[2]*(point[2].x - minx) + dx[2]*(miny - point[2].y) - ((dy[2]<0) || (dy[2]==0 && dx[2]>0)),
	           point[miyi].z-fx::mul(fx::l2f(point[miyi].x-point[mixi].x),zx)}; 

	const xint str = XRES - (maxx-minx);

	for(xint y=miny,off=miny*XRES+minx;y<maxy;++y)
	{
		xint cx[4]{cy[0],cy[1],cy[2],cy[3]};

		#pragma prefetch back
		for(xint x=minx;x<maxx;++x) 
		{
			// prefetch(&back[off]);
			const bool inside = (cx[0]<0) && (cx[1]<0) && (cx[2]<0);
			const bool above  = cx[3]>=math::abs(screen::depth[off]);
			if(inside && above)
			{
				screen::depth[off] = cx[3]; //math::neg(cx[3],!screen::zs);
				screen::frame[off] = c;
			}

			cx[0] -= dy[0];
			cx[1] -= dy[1];
			cx[2] -= dy[2];
			cx[3] -= dy[3];
			++off;
		}

		cy[0] += dx[0];
		cy[1] += dx[1];
		cy[2] += dx[2];
		cy[3] += dx[3];
		off   += str;
	}
}

void polygon::shadow(yint c) const
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
	const xint maxx = math::min(XMAX,point[maxi].x+1); // prevent gap
	const xint miny = math::max(YMIN,point[miyi].y);
	const xint maxy = math::min(YMAX,point[mayi].y+1); // prevent gap


	guard( (maxx==minx) || (maxy==miny) );

	const xint dx[4]{point[0].x-point[1].x,point[1].x-point[2].x,point[2].x-point[0].x,0};
	const xint dy[4]{point[0].y-point[1].y,point[1].y-point[2].y,point[2].y-point[0].y,0};

	xint cy[4]{dy[0]*(point[0].x - minx) + dx[0]*(miny - point[0].y) - ((dy[0]<0) || (dy[0]==0 && dx[0]>0)),
	           dy[1]*(point[1].x - minx) + dx[1]*(miny - point[1].y) - ((dy[1]<0) || (dy[1]==0 && dx[1]>0)),
	           dy[2]*(point[2].x - minx) + dx[2]*(miny - point[2].y) - ((dy[2]<0) || (dy[2]==0 && dx[2]>0)),
	           0}; 

	const xint str = XRES - (maxx-minx);

	for(xint y=miny,off=miny*XRES+minx;y<maxy;++y)
	{
		xint cx[4]{cy[0],cy[1],cy[2],cy[3]};

		#pragma prefetch back
		for(xint x=minx;x<maxx;++x) 
		{
			//prefetch(&back[off]);
			const bool inside = (cx[0]<0) && (cx[1]<0) && (cx[2]<0);
			if(inside)
			{
				screen::frame[off] = screen::frame[off]&c;
			}

			cx[0] -= dy[0];
			cx[1] -= dy[1];
			cx[2] -= dy[2];
			cx[3] -= dy[3];
			++off;
		}

		cy[0] += dx[0];
		cy[1] += dx[1];
		cy[2] += dx[2];
		cy[3] += dx[3];
		off   += str;
	}
}

polygon::polygon(const tuple& x,const tuple& y,const tuple& z,yint c) : vertex{x,y,z},color(c) 
{
	normal   = (vertex[2]-vertex[0]).cross(vertex[1]-vertex[0]) *= FXCEN;
	normal.e = normal.length();
}

void polygon::update(const matrix& m,bool i)
{
	vertex[i] = m*vertex[i];
	vertex[0] = m*vertex[0];
	vertex[2] = m*vertex[2];
	normal   = (vertex[2]-vertex[0]).cross(vertex[1]-vertex[0]) *= FXCEN;
	normal.e = normal.length();
}

void polygon::display(const tuple& p,xint f,yint c)
{
	guard(normal.z>FXMON); //z
	++counter;	

	if(f&R_B) 
	{
		point[0] = project(p,blinn*vertex[0]);
		point[1] = project(p,blinn*vertex[1]);
		point[2] = project(p,blinn*vertex[2]);
		shadow(c);
	}
	else
	{
		point[0] = project(p,vertex[0]);
		point[1] = project(p,vertex[1]);
		point[2] = project(p,vertex[2]);
		ifl(f&R_F) { c = flat(p.z,f); }
		ifu(f&R_S) { shape(); } else { raster(c); }
	}
}

void polygon::pull(fixed a)
{
	const fixed  l = fx::mul(fx::div(FXONE,normal.length()),a);
	const vector m = normal*l;
	vertex[0] += m;
	vertex[1] += m;
	vertex[2] += m;
}
///</code>

