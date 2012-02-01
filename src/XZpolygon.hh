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
		const sint color;		//Polygon Color
		const sint scolor;		//Shadow Color
		sint shade;			//Current Color Shade
		fvector cnormal;		//Polygon Normal
		fvector onormal;		//Backup Normal
		fvector cpoint[3];		//Polygon Vertices
		fvector opoint[3];		//Backup Vertices
		static lvector lpoint[3];	//Render Vertices

		/*OK*/ inline bool isvisible() const { return cnormal.z<FXMON; }
		/*OK*/ inline void shape() const;
		/*OK*/        void project(const lvector& p) const;
		              void flat(sint pz,sint f);
		              void raster(bool s) const /*HOTFN FCALL*/; //based on "Daily Code Gem - Advanced Rasterization"
	public:
		/*OK*/ polygon(const lvector& x,const lvector& y,const lvector& z,sint c,sint s);
		/*OK*/        void update(const fmatrix& m,bool i=0);
		/*OK*/        void display(const lvector& p,sint f);
		/*OK*/ inline void reset();
		/*OK*/ inline void set();
		/*OK*/ inline void add(const fvector& a);
		/*OK*/ inline fvector normal() const { return cnormal; }

		static sint  counter;		//Polygon Counter
		static const fvector light;	//Light Vector
		static const fmatrix shadow;	//Shadow Matrix
};
///*

///implementation
lvector       polygon::lpoint[] = { lvector(), lvector(), lvector() };
sint          polygon::counter  = 0;
const fvector polygon::light    = fvector(FXONE,FXONE,FXONE,FXONE+FXONE);
const fmatrix polygon::shadow   = []() ->fmatrix { fmatrix m; m.shadow(fvector(0,FXTNT,FXONE),fvector(0,4*FXTNT,FXONE+FXTNT)); return m; }(); 

void polygon::project(const lvector& p) const
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

void polygon::flat(sint pz,sint f)
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
	argb.b[0] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[0]) ,t) + ambient + nolight + zshade));
	argb.b[1] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[1]) ,t) + ambient + nolight + zshade));
	argb.b[2] = (byte)(fx::r2l( fx::mul( fx::l2f(argb.b[2]) ,t) + ambient + nolight + zshade));
	argb.b[3] = 0;
	shade = argb.d;
}

void polygon::shape() const 
{
	gfx::line(lpoint[0].x,lpoint[0].y,lpoint[1].x,lpoint[1].y,color);
	gfx::line(lpoint[1].x,lpoint[1].y,lpoint[2].x,lpoint[2].y,color);
	gfx::line(lpoint[2].x,lpoint[2].y,lpoint[0].x,lpoint[0].y,color);
}

void polygon::raster(bool s) const
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

	guard(maxx==minx || maxy==miny);

	const sint dx01 = lpoint[0].x - lpoint[1].x;
	const sint dx12 = lpoint[1].x - lpoint[2].x;
	const sint dx20 = lpoint[2].x - lpoint[0].x;

	const sint dy01 = lpoint[0].y - lpoint[1].y;
	const sint dy12 = lpoint[1].y - lpoint[2].y;
	const sint dy20 = lpoint[2].y - lpoint[0].y;

	sint cy0 = (dy01*lpoint[0].x) - (dx01*lpoint[0].y) + (dx01*miny) - (dy01*minx) - ((dy01<0) || (dy01==0 && dx01>0));
	sint cy1 = (dy12*lpoint[1].x) - (dx12*lpoint[1].y) + (dx12*miny) - (dy12*minx) - ((dy12<0) || (dy12==0 && dx12>0));
	sint cy2 = (dy20*lpoint[2].x) - (dx20*lpoint[2].y) + (dx20*miny) - (dy20*minx) - ((dy20<0) || (dy20==0 && dx20>0)); 

	const sint st = XRES - (maxx-minx);
	sint off = miny * XRES + minx;

	if(s==0)
	{
		const fixed zx = fx::div(lpoint[maxi].z-lpoint[mixi].z,fx::l2f(maxx-minx));
		const fixed zy = fx::div(lpoint[mayi].z-lpoint[miyi].z,fx::l2f(maxy-miny));
		fixed ty = lpoint[mixi].z - fx::mul(zy,lpoint[mixi].z-lpoint[miyi].z); 

		for(sint y=miny;y<maxy;++y,off+=st,ty+=zy)
		{
			sint cx0 = cy0;
			sint cx1 = cy1;
			sint cx2 = cy2;

			fixed tx = ty;

			for(sint x=minx;x<maxx;++x,++off,tx+=zx) 
			{
				const fixed sz = screen::depth[off];
		
				if( cx0<0 && cx1<0 && cx2<0 && tx<sz )
				{
					screen::depth[off] = tx;
					screen::back[off]  = shade;
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
	else
	{
		for(sint y=miny;y<maxy;++y,off+=st)
		{
			sint cx0 = cy0;
			sint cx1 = cy1;
			sint cx2 = cy2;

			for(sint x=minx;x<maxx;++x,++off) 
			{
				if( cx0<0 && cx1<0 && cx2<0 )
				{
					screen::back[off]  = (shade+screen::back[off])>>1; 
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
}

polygon::polygon(const lvector& x,const lvector& y,const lvector& z,sint c,sint s) : color(c), scolor(s), shade(c)
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

void polygon::display(const lvector& p,sint f)
{
	guard(isvisible()==0);
	counter++;

	if( f&R_B )
	{
		const fvector s[3] = cpoint;
		cpoint[0] = shadow.transform(cpoint[0]);
		cpoint[1] = shadow.transform(cpoint[1]);
		cpoint[2] = shadow.transform(cpoint[2]);
		project(p);
		cpoint[0] = s[0];
		cpoint[1] = s[1];
		cpoint[2] = s[2];
		shade = scolor;
	}
	else
	{
		project(p);
		if((f&R_S)!=0) { shape(); return; }
		shade = color; 
		if((f&R_F)!=0) { flat(p.z,f); } 
	}

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

