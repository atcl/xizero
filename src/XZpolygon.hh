// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZpolygon.hh
// Polygon Library 

///guard
#ifndef HH_XZPOLYGON
#define HH_XZPOLYGON
///*

///include
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZbuffer.hh"
#include "XZmath.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZscreen.hh"
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
#define ZLIGHT 100
#define NOLIGHT 10

#define R_A 0x00000001 //ambient
#define R_Z 0x00000010 //zlight
#define R_N 0x00000100 //nolight
#define R_S 0x00001000 //shape
#define R_B 0x00010000 //blinn shadows
#define R_F 0x00100000 //flat
///*

///definition
class polygon
{
	private:
		const long color;
		const long scolor;
		long shade;
		fvector cnormal;
		fvector onormal;
		fvector cpoint[3];
		fvector opoint[3];
		static lvector lpoint[3];

		/*OK*/ INLINE bool  isvisible() const { return cnormal.z<FXMON; }
		/*OK*/ INLINE void  shape() const;
		long  flat(long pz,long f) const;
		/*OK*/ void  project(const lvector& p);
		void  raster(bool s) const /*HOTFN FCALL*/; //based on "Daily Code Gem"
	public:
		/*OK*/ polygon(const lvector& x,const lvector& y,const lvector& z,long c,long s);
		/*OK*/ ~polygon() { ; };
		/*OK*/ void update(const fmatrix& m,bool i=0);
		void display(const lvector& p,long f);
		/*OK*/ INLINE void reset();
		/*OK*/ INLINE void set();
		/*OK*/ INLINE void add(const fvector& a);
		/*OK*/ INLINE fvector normal() const { return cnormal; }

		static fvector plane;
		static fvector light;
};
///*

///implementation
lvector polygon::lpoint[] = { lvector(), lvector(), lvector() };
fvector polygon::plane    = fvector(FXONE,FXONE,0);
fvector polygon::light    = fvector(0,FXONE,FXONE,fvector(0,FXONE,FXONE).length());

void polygon::project(const lvector& p)
{
	const fixed pz = fx::l2f(p.z);

	lpoint[0].z = cpoint[0].z + pz;
	lpoint[0].x = fx::f2l(fx::mul( PRJX<<FX,fx::div(cpoint[0].x,lpoint[0].z))) + p.x;
	lpoint[0].y = fx::f2l(fx::mul(-PRJY<<FX,fx::div(cpoint[0].y,lpoint[0].z))) + p.y;

	lpoint[1].z = cpoint[1].z + pz;
	lpoint[1].x = fx::f2l(fx::mul( PRJX<<FX,fx::div(cpoint[1].x,lpoint[1].z))) + p.x;
	lpoint[1].y = fx::f2l(fx::mul(-PRJY<<FX,fx::div(cpoint[1].y,lpoint[1].z))) + p.y;

	lpoint[2].z = cpoint[2].z + pz;
	lpoint[2].x = fx::f2l(fx::mul( PRJX<<FX,fx::div(cpoint[2].x,lpoint[2].z))) + p.x;
	lpoint[2].y = fx::f2l(fx::mul(-PRJY<<FX,fx::div(cpoint[2].y,lpoint[2].z))) + p.y;
}

long polygon::flat(long pz,long f) const
{
	fixed t = fx::mul(cnormal.e,light.e);
	t = fx::div(cnormal.dot(light),t);
	t = math::abs(t);
	t = math::min(FXONE,t);
	t = math::max(FXTNT,t);

	const char ambient = math::set(AMBIENT,f&R_A);
	const char nolight = math::set(NOLIGHT,f&R_N);
	const char zshade  = math::set(ZLIGHT*pz,f&R_Z);

	packed argb = { color };
	argb.b[0] = (uchar)(fx::r2l( fx::mul( fx::l2f(argb.b[0]) ,t) + ambient + nolight + zshade));
	argb.b[1] = (uchar)(fx::r2l( fx::mul( fx::l2f(argb.b[1]) ,t) + ambient + nolight + zshade));
	argb.b[2] = (uchar)(fx::r2l( fx::mul( fx::l2f(argb.b[2]) ,t) + ambient + nolight + zshade));
	argb.b[3] = 0;

	return argb.d;
}

void polygon::shape() const 
{
	gfx::line(lpoint[0].x,lpoint[0].y,lpoint[1].x,lpoint[1].y,color);
	gfx::line(lpoint[1].x,lpoint[1].y,lpoint[2].x,lpoint[2].y,color);
	gfx::line(lpoint[2].x,lpoint[2].y,lpoint[0].x,lpoint[0].y,color);
}

void polygon::raster(bool s) const
{
	const long minx = math::max(XMIN,math::min(lpoint[0].x,math::min(lpoint[1].x,lpoint[2].x)));
	const long maxx = math::min(XMAX,math::max(lpoint[0].x,math::max(lpoint[1].x,lpoint[2].x)))+1; //no gap
	const long miny = math::max(YMIN,math::min(lpoint[0].y,math::min(lpoint[1].y,lpoint[2].y)));
	const long maxy = math::min(YMAX,math::max(lpoint[0].y,math::max(lpoint[1].y,lpoint[2].y)));

	guard(maxx==minx || maxy==miny);

	const long dx01 = lpoint[0].x - lpoint[1].x;
	const long dx12 = lpoint[1].x - lpoint[2].x;
	const long dx20 = lpoint[2].x - lpoint[0].x;

	const long dy01 = lpoint[0].y - lpoint[1].y;
	const long dy12 = lpoint[1].y - lpoint[2].y;
	const long dy20 = lpoint[2].y - lpoint[0].y;

	const long mixi = math::set(1,math::set(2,minx==lpoint[2].x),minx==lpoint[1].x); //inc into max finding
	const long maxi = math::set(1,math::set(2,maxx==lpoint[2].x),maxx==lpoint[1].x);
	const long miyi = math::set(1,math::set(2,miny==lpoint[2].y),miny==lpoint[1].y);
	const long mayi = math::set(1,math::set(2,maxy==lpoint[2].y),maxy==lpoint[1].y);

	const fixed zx = fx::div(lpoint[maxi].z-lpoint[mixi].z,fx::l2f(maxx-minx));
	const fixed zy = fx::div(lpoint[mayi].z-lpoint[miyi].z,fx::l2f(maxy-miny));

	const long c0 = (dy01 * lpoint[0].x) - (dx01 * lpoint[0].y) - ((dy01<0) || (dy01==0 && dx01>0));
	const long c1 = (dy12 * lpoint[1].x) - (dx12 * lpoint[1].y) - ((dy12<0) || (dy12==0 && dx12>0));
	const long c2 = (dy20 * lpoint[2].x) - (dx20 * lpoint[2].y) - ((dy20<0) || (dy20==0 && dx20>0));

	long cy0 = c0 + dx01 * miny - dy01 * minx;
	long cy1 = c1 + dx12 * miny - dy12 * minx;
	long cy2 = c2 + dx20 * miny - dy20 * minx;

	long cx0 = 0;
	long cx1 = 0;
	long cx2 = 0; 

	const long st = XRES - (maxx-minx);
	long off = miny * XRES + minx;

	fixed tx = 0;
	fixed ty = lpoint[mixi].z - fx::mul(zy,lpoint[mixi].z-lpoint[miyi].z); 

	for(long y=miny;y<maxy;++y,off+=st,ty+=zy)
	{
		cx0 = cy0;
		cx1 = cy1;
		cx2 = cy2;

		tx = ty;

		for(long x=minx;x<maxx;++x,++off,tx+=zx) 
		{
			const fixed sz = screen::depth[off];
		
			switch( math::neg(long(cx0<0 && cx1<0 && cx2<0 && ( (tx<sz) ||s==1)),s==1) )
			{
				case 1:  screen::depth[off] = tx;
				case -1: screen::back[off]  = shade;
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

polygon::polygon(const lvector& x,const lvector& y,const lvector& z,long c,long s) : color(c), scolor(s), shade(c)
{
	opoint[0] = cpoint[0] = x;
	opoint[1] = cpoint[1] = y;
	opoint[2] = cpoint[2] = z;
	onormal   = cnormal   = (cpoint[2]-cpoint[0]).cross(cpoint[1]-cpoint[0]) * FXHUN;
	onormal.e = cnormal.e = cnormal.length();
}

void polygon::update(const fmatrix& m,bool i)
{
	/*if(i==0)*/ { cpoint[1] = m.transform(cpoint[1]); }
	cpoint[0] = m.transform(cpoint[0]);
	cpoint[2] = m.transform(cpoint[2]);
	cnormal   = (cpoint[2]-cpoint[0]).cross(cpoint[1]-cpoint[0]) * FXHUN;
	cnormal.e = cnormal.length();
}

void polygon::display(const lvector& p,long f)
{
	guard(isvisible()==0);
	project(p);

	//switch() ?

	if((f&R_S)!=0) { shape(); return; }
	shade = color; 
	if((f&R_F)!=0) { shade = flat(p.z,f); } 
	shade = math::set(scolor,shade,(f&R_B)!=0); 
	raster( f&R_B );
}

void polygon::add(const fvector& a)
{
	cpoint[0] += a;
	cpoint[1] += a;
	cpoint[2] += a;
}

void polygon::set()
{
	opoint[0] = cpoint[0];
	opoint[1] = cpoint[1];
	opoint[2] = cpoint[2];
	onormal   = cnormal;
}

void polygon::reset()
{
	cpoint[0] = opoint[0];
	cpoint[1] = opoint[1];
	cpoint[2] = opoint[2];
	cnormal   = onormal;
}
///*

#endif

