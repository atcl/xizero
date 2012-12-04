///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZpolygon.hh
// Polygon Library 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: Rasterizer"
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
#define ZMAX 400
#define PRJX 120
#define PRJY 150

#define AMBIENT 25
#define ZLIGHT  FXTNT
#define NOLIGHT 10

#define R_A 0x00000001 //ambient
#define R_Z 0x00000010 //zlight
#define R_N 0x00000100 //nolight
#define R_S 0x00001000 //shape
#define R_B 0x00010000 //blinn shadows
#define R_F 0x00100000 //flat
#define R_C 0x01000000 //single color
#define R_D 0x10000000 //depth test
///</declare>

///<define>
class polygon
{
	private:
		static lvector lpoint[3];	//Render Vertices
		fvector cpoint[3];		//Polygon Vertices
		fvector cnormal;		//Polygon Normal
		const uint color;		//Polygon Color

		/*OK*/ inline void shape() const;
		              uint flat(sint pz,sint f) const;
		              void raster(bool s,uint c) const /*hot*/;
	public:
		/*OK*/      polygon(const lvector& x,const lvector& y,const lvector& z,uint c);
		/*OK*/ void update(const fmatrix& m,bool i=1);
		/*OK*/ void display(const lvector& p,sint f,uint c=0);
		/*OK*/ void pull(fixed a);
		/*OK*/ static lvector project(const lvector& p,const fvector& v);

		static sint  counter;		//Polygon Counter
		static const fvector light;	//Light Vector
		static const fmatrix shadow;	//Shadow Matrix
};
///</define>

///<code>
      lvector polygon::lpoint[] = { lvector(), lvector(), lvector() };
      sint    polygon::counter  = 0;
const fvector polygon::light    = fvector(FXONE,FXONE,FXONE,FXONE+FXONE+FXONE);
const fmatrix polygon::shadow   = []() ->fmatrix { fmatrix m; m.shadow(fvector(0,FXTNT,FXONE),fvector(0,4*FXTNT,FXONE+FXTNT)); return m; }(); 

lvector polygon::project(const lvector& p,const fvector& v)
{
	const fixed z = v.z + fx::l2f(p.z);
	return lvector(p.x+fx::f2l(fx::mul(FX(PRJX),fx::div(v.x,z))),p.y-fx::f2l(fx::mul(FX(PRJY),fx::div(v.y,z))),z);
}

uint polygon::flat(sint pz,sint f) const
{
	const fixed t = math::lim(FXTNT,math::abs(fx::div(cnormal.dot(light),fx::mul(cnormal.e,light.e))),FXONE);
	const byte anz = math::set(AMBIENT,f&R_A) + math::set(NOLIGHT,f&R_N) + math::set(fx::r2l(fx::mul(ZLIGHT,fx::l2f(pz))),f&R_Z); 

	packed argb = { (uint)math::set(ORANGE,color,f&R_C) };
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]),t ) + anz) );
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]),t ) + anz) );
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]),t ) + anz) );
	argb.b[3] = 0;
	return argb.d;
}

void polygon::shape() const 
{
	gfx::line(lpoint[0].x,lpoint[0].y,lpoint[1].x,lpoint[1].y,color);
	gfx::line(lpoint[1].x,lpoint[1].y,lpoint[2].x,lpoint[2].y,color);
	gfx::line(lpoint[2].x,lpoint[2].y,lpoint[0].x,lpoint[0].y,color);
}

void polygon::raster(bool s,uint c) const
{
	//determine projected minima and maxima
	const sint mix01 = lpoint[1].x<lpoint[0].x;
	const sint max01 = !mix01;
	const sint miy01 = lpoint[1].y<lpoint[0].y;
	const sint may01 = !miy01;

	const sint mixi = math::set(2,mix01,lpoint[2].x<lpoint[mix01].x);
	const sint maxi = math::set(2,max01,lpoint[2].x>lpoint[max01].x);
	const sint miyi = math::set(2,miy01,lpoint[2].y<lpoint[miy01].y);
	const sint mayi = math::set(2,may01,lpoint[2].y>lpoint[may01].y);

	const sint minx = math::max(XMIN,lpoint[mixi].x);
	const sint maxx = math::min(XMAX,lpoint[maxi].x+1); //prevent gap
	const sint miny = math::max(YMIN,lpoint[miyi].y);
	const sint maxy = math::min(YMAX,lpoint[mayi].y+1); //prevent gap
	//*

	guard( (maxx==minx) || (maxy==miny) );

	const sint dx[3]{lpoint[0].x-lpoint[1].x,lpoint[1].x-lpoint[2].x,lpoint[2].x-lpoint[0].x};
	const sint dy[3]{lpoint[0].y-lpoint[1].y,lpoint[1].y-lpoint[2].y,lpoint[2].y-lpoint[0].y};

	sint cy[3]{dy[0]*(lpoint[0].x - minx) + dx[0]*(miny - lpoint[0].y) - ((dy[0]<0) || (dy[0]==0 && dx[0]>0)),
	           dy[1]*(lpoint[1].x - minx) + dx[1]*(miny - lpoint[1].y) - ((dy[1]<0) || (dy[1]==0 && dx[1]>0)),
	           dy[2]*(lpoint[2].x - minx) + dx[2]*(miny - lpoint[2].y) - ((dy[2]<0) || (dy[2]==0 && dx[2]>0))}; 

	const sint str = XRES - (maxx-minx);

	const fixed zx = math::abs(fx::div(lpoint[maxi].z-lpoint[mixi].z,fx::l2f(maxx-minx)));
	const fixed zy = math::abs(fx::div(lpoint[mayi].z-lpoint[miyi].z,fx::l2f(maxy-miny)));
	      fixed ty = lpoint[miyi].z-fx::mul(fx::l2f(lpoint[miyi].x-lpoint[mixi].x),zx);

	for(sint y=miny,off=miny*XRES+minx;y<maxy;++y)
	{
		sint cx[3]{cy[0],cy[1],cy[2]};

		fixed tx = ty;

		#pragma prefetch back
		for(sint x=minx;x<maxx;++x) 
		{
			//__builtin_prefetch(back[off],1,0123);
			switch( ( ( (cx[0]<0) && (cx[1]<0) && (cx[2]<0) ) << s ) >> ( (!s) && (tx>screen::depth[off]) ) )
			{
				case 1: screen::depth[off] = tx;
				case 2: screen::back[off]  = c;  //(c+screen::back[off])>>1;
			}

			cx[0] -= dy[0];
			cx[1] -= dy[1];
			cx[2] -= dy[2];
			++off;
			tx += zx;
		}

		cy[0] += dx[0];
		cy[1] += dx[1];
		cy[2] += dx[2];
		off +=str;
		ty += zy;
	}
}

polygon::polygon(const lvector& x,const lvector& y,const lvector& z,uint c) : cpoint{x,y,z},cnormal(fvector(((z-x).cross(y-x)))*FXCEN),color(c) 
{
	cnormal.e = cnormal.length();
}

void polygon::update(const fmatrix& m,bool i)
{
	cpoint[i] = m*cpoint[i];
	cpoint[0] = m*cpoint[0];
	cpoint[2] = m*cpoint[2];
	cnormal   = (cpoint[2]-cpoint[0]).cross(cpoint[1]-cpoint[0]) *= FXCEN;
	cnormal.e = cnormal.length();
}

void polygon::display(const lvector& p,sint f,uint c)
{
	guard(cnormal.z>FXMON);
	++counter;

	if( f&R_B )
	{
		lpoint[0] = project(p,shadow*cpoint[0]);
		lpoint[1] = project(p,shadow*cpoint[1]);
		lpoint[2] = project(p,shadow*cpoint[2]);
	}
	else
	{
		lpoint[0] = project(p,cpoint[0]);
		lpoint[1] = project(p,cpoint[1]);
		lpoint[2] = project(p,cpoint[2]);
		if((f&R_S)!=0) { shape(); return; }
		if((f&R_F)!=0) { c = flat(p.z,f); } 
	}

	raster( f&R_B,c );
}

void polygon::pull(fixed a)
{
	const fixed l = fx::mul(fx::div(FXONE,cnormal.length()),a);
	const fvector m = (cnormal*l);
	cpoint[0] += m;
	cpoint[1] += m;
	cpoint[2] += m;
}
///</code>

