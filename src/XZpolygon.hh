// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZpolygon.hh
// Polygon Library 

///guard
#ifndef HH_XZPOLYGON
#define HH_XZPOLYGON
//#pragma message "Compiling " __FILE__ "..." " TODO: Shading, Rasterizer"
///*

///include
#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZmath.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZskms.hh"
#include "XZgfx.hh"
///*

///declaration
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
///*

///definition
class polygon
{
	private:
		const uint color;		//Polygon Color
		fvector cnormal;		//Polygon Normal
		fvector cpoint[3];		//Polygon Vertices
		static lvector lpoint[3];	//Render Vertices

		/*OK*/ inline void shape() const;
		              uint flat(sint pz,sint f,uint c) const;
		              void raster(bool s,uint c) const; //based on "Daily Code Gem - Advanced Rasterization"
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
///*

///implementation
      lvector polygon::lpoint[] = { lvector(), lvector(), lvector() };
      sint    polygon::counter  = 0;
const fvector polygon::light    = fvector(FXONE,FXONE,FXONE,FXONE+FXONE+FXONE);
const fmatrix polygon::shadow   = []() ->fmatrix { fmatrix m; m.shadow(fvector(0,FXTNT,FXONE),fvector(0,4*FXTNT,FXONE+FXTNT)); return m; }(); 

lvector polygon::project(const lvector& p,const fvector& v)
{
	lvector r;
	r.z = v.z + fx::l2f(p.z);
	r.x = p.x + fx::f2l(fx::mul(FX(PRJX),fx::div(v.x,r.z)));
	r.y = p.y - fx::f2l(fx::mul(FX(PRJY),fx::div(v.y,r.z)));
	return r;
}

uint polygon::flat(sint pz,sint f,uint c) const
{
	const fixed t = math::lim(FXTNT,math::abs(fx::div(cnormal.dot(light),fx::mul(cnormal.e,light.e))),FXONE);

	const char ambient = math::set(AMBIENT,f&R_A);
	const char nolight = math::set(NOLIGHT,f&R_N);
	const char zshade  = math::set(fx::r2l(fx::mul(ZLIGHT,fx::l2f(pz))),f&R_Z);

	packed argb = { c };
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]) ,t) + ambient + nolight + zshade));
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]) ,t) + ambient + nolight + zshade));
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]) ,t) + ambient + nolight + zshade));
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
	const sint maxx = math::min(XMAX,1+lpoint[maxi].x); //prevent gap
	const sint miny = math::max(YMIN,lpoint[miyi].y);
	const sint maxy = math::min(YMAX,1+lpoint[mayi].y); //prevent gap
	//*

	guard( (maxx==minx) || (maxy==miny) );

	const sint dx01 = lpoint[0].x - lpoint[1].x;
	const sint dx12 = lpoint[1].x - lpoint[2].x;
	const sint dx20 = lpoint[2].x - lpoint[0].x;

	const sint dy01 = lpoint[0].y - lpoint[1].y;
	const sint dy12 = lpoint[1].y - lpoint[2].y;
	const sint dy20 = lpoint[2].y - lpoint[0].y;

	sint cy0 = dy01*(lpoint[0].x - minx) + dx01*(miny - lpoint[0].y) - ((dy01<0) || (dy01==0 && dx01>0));
	sint cy1 = dy12*(lpoint[1].x - minx) + dx12*(miny - lpoint[1].y) - ((dy12<0) || (dy12==0 && dx12>0));
	sint cy2 = dy20*(lpoint[2].x - minx) + dx20*(miny - lpoint[2].y) - ((dy20<0) || (dy20==0 && dx20>0)); 

	const sint str = XRES - (maxx-minx);

	const fixed zx = fx::div(lpoint[maxi].z-lpoint[mixi].z,fx::l2f(maxx-minx));
	const fixed zy = fx::div(lpoint[mayi].z-lpoint[miyi].z,fx::l2f(maxy-miny));
	      fixed ty = lpoint[mixi].z - fx::mul(zy,fx::l2f(math::abs(lpoint[mixi].y-lpoint[miyi].y)));

	for(sint y=miny,off=miny*XRES+minx;y<maxy;++y,off+=str,ty+=zy)
	{
		sint cx0 = cy0;
		sint cx1 = cy1;
		sint cx2 = cy2;

		fixed tx = ty;

		for(sint x=minx;x<maxx;++x,++off,tx+=zx) 
		{
			switch( ( ( (cx0<0) && (cx1<0) && (cx2<0) ) << s ) >> ( (!s)&&(tx>screen::depth[off]) ) ) //error here affecting level display
			{
				case 1: screen::depth[off] = tx;
				case 2: screen::back[off]  = c;  //(c+screen::back[off])>>1;
			}

			cx0 -= dy01;
			cx1 -= dy12;
			cx2 -= dy20;
		}

		cy0 += dx01;
		cy1 += dx12;
		cy2 += dx20;
	}
}

polygon::polygon(const lvector& x,const lvector& y,const lvector& z,uint c) : color(c)
{
	cpoint[0] = x;
	cpoint[1] = y;
	cpoint[2] = z;
	cnormal   = (cpoint[2]-cpoint[0]).cross(cpoint[1]-cpoint[0]) * FXHUN;
	cnormal.e = cnormal.length();
}

void polygon::update(const fmatrix& m,bool i)
{
	cpoint[i] = m.transform(cpoint[i]);
	cpoint[0] = m.transform(cpoint[0]);
	cpoint[2] = m.transform(cpoint[2]);
	cnormal   = (cpoint[2]-cpoint[0]).cross(cpoint[1]-cpoint[0]) * FXHUN;
	cnormal.e = cnormal.length();
}

void polygon::display(const lvector& p,sint f,uint c)
{
	guard(cnormal.z>FXMON);
	++counter;

	if( f&R_B )
	{
		lpoint[0] = project(p,shadow.transform(cpoint[0]));
		lpoint[1] = project(p,shadow.transform(cpoint[1]));
		lpoint[2] = project(p,shadow.transform(cpoint[2]));
	}
	else
	{
		lpoint[0] = project(p,cpoint[0]);
		lpoint[1] = project(p,cpoint[1]);
		lpoint[2] = project(p,cpoint[2]);
		if((f&R_S)!=0) { shape(); return; }
		if((f&R_F)!=0) { c = flat(p.z,f,math::set(ORANGE,color,f&R_C)); } 
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
///*

#endif

