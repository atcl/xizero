//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: complete vector functions"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLdiv3d.hh"


class CLgame : public virtual CLcl
{
	protected:
		CLmath* clmath;

	private:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;

	public:
		CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2,CLmath* clm);
		~CLgame();
		xlong boundary(xlong x1,xlong y1,xlong x2,xlong y2);
		xlong boundary(xlong x,xlong y);
		xlong boundary(vector* v1,vector* v2);
		xlong boundary(vector* v);
		xlong boundary(vector position,CLbox* bb);
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2);
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b);
		xlong collision(vector* v1,vector* v2,vector* w1,vector* w2);
		xlong collision(vector* v1,vector* v2,vector* w);
		xlong collision(CLbox* bb1,CLbox* bb2);

		xlong impact(vector p,CLbox* bb,CLbuffer<float>* zbuffer,xlong zdiff);
};

CLgame::CLgame(xlong bx1,xlong by1,xlong bx2,xlong by2,CLmath* clm)
{
	clmath = clm;
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;
}

CLgame::~CLgame() { }

xlong CLgame::boundary(xlong x1,xlong y1,xlong x2,xlong y2)
{
	if(x1>boundaryx1 && x2<boundaryx2 && y1>boundaryy1 && y2<boundaryy2) return 1;
	return 0;
}

xlong CLgame::boundary(xlong x,xlong y)
{
	if(x>boundaryx1 && x<boundaryx2 && y>boundaryy1 && y<boundaryy2) return 1;
	return 0;
}

xlong CLgame::boundary(vector* v1,vector* v2)
{

}

xlong CLgame::boundary(vector* v)
{

}

xlong CLgame::boundary(vector p,CLbox* bb)
{
	if( ( ( bb->t1.x + p.x ) < boundaryx1 ) || ( ( bb->t2.x + p.x ) < boundaryx1 ) || ( ( bb->t3.x + p.x ) < boundaryx1 ) || ( ( bb->t4.x + p.x ) < boundaryx1 ) ) return -1;
	if( ( ( bb->t1.x + p.x ) > boundaryx2 ) || ( ( bb->t2.x + p.x ) > boundaryx2 ) || ( ( bb->t3.x + p.x ) > boundaryx2 ) || ( ( bb->t4.x + p.x ) > boundaryx2 ) ) return 1;
	if( ( ( -bb->t1.y + p.y ) < boundaryy1 ) || ( ( -bb->t2.y + p.y ) < boundaryy1 ) || ( ( -bb->t3.y + p.y ) < boundaryy1 ) || ( ( -bb->t4.y + p.y ) < boundaryy1 ) ) return -2;
	if( ( ( -bb->t1.y + p.y ) > boundaryy2 ) || ( ( -bb->t2.y + p.y ) > boundaryy2 ) || ( ( -bb->t3.y + p.y ) > boundaryy2 ) || ( ( -bb->t4.y + p.y ) > boundaryy2 ) ) return 2;

	return 0;
}

xlong CLgame::collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2)
{
	if( x1>a1 && x1<a2 && y1>b1 && y1<b2 ) return 1;
	if( x2>a1 && x2<a2 && y2>b1 && y2<b2 ) return 1;
	if( a1>x1 && a1<x2 && b1>y1 && b1<y2 ) return 1;
	if( a2>x1 && a2<x2 && b2>y1 && b2<y2 ) return 1;
	return 0;
}

xlong CLgame::collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b)
{
	if( a<x1 && a>x2 && b>y1 && b<y2 ) return 1;
	return 0;
}

xlong CLgame::collision(vector* v1,vector* v2,vector* w1,vector* w2)
{

}

xlong CLgame::collision(vector* v1,vector* v2,vector* w)
{

}

xlong CLgame::collision(CLbox* bb1, CLbox* bb2) //!test!
{/*
	if( ( bb1->a.x > bb2->a.x )
	 && ( bb1->a.x < bb2->b.x )
	 && ( bb1->a.y > bb2->a.y )
	 && ( bb1->a.y < bb2->b.y ) ) return 1;

	if( ( bb1->b.x > bb2->a.x )
	 && ( bb1->b.x < bb2->b.x )
	 && ( bb1->b.y > bb2->a.y )
	 && ( bb1->b.y < bb2->b.y ) ) return 1;

	if( ( bb2->a.x > bb1->a.x )
	 && ( bb2->a.x < bb1->b.x )
	 && ( bb2->a.y > bb1->a.y )
	 && ( bb2->a.y < bb1->b.y ) ) return 1;

	if( ( bb2->b.x > bb1->a.x )
	 && ( bb2->b.x < bb1->b.x )
	 && ( bb2->b.y > bb1->a.y )
	 && ( bb2->b.y < bb1->b.y ) ) return 1;*/

	return 0;
}

xlong CLgame::impact(vector p,CLbox* bb,CLbuffer<float>* zbuffer,xlong zdiff)
{
	vector v1 = { p.x + bb->b1.x, p.y + bb->b1.y, p.z + bb->b1.z, 0 };
	vector v2 = { p.x + bb->b2.x, p.y + bb->b2.y, p.z + bb->b2.z, 0 };
	vector v3 = { p.x + bb->b3.x, p.y + bb->b3.y, p.z + bb->b3.z, 0 };
	vector v4 = { p.x + bb->b4.x, p.y + bb->b4.y, p.z + bb->b4.z, 0 };

	CLpoint p1 = project(v1);
	CLpoint p2 = project(v2);
	CLpoint p3 = project(v3);
	CLpoint p4 = project(v4);

	//test zbuffer at o1-o4

	xlong pxmin = clmath->min(p1.x,p2.x,p3.x,p4.x);
	xlong pymin = clmath->min(p1.y,p2.y,p3.y,p4.y);
	xlong pxmax = clmath->max(p1.x,p2.x,p3.x,p4.x);
	xlong pymax = clmath->max(p1.y,p2.y,p3.y,p4.y);

	xlong o1 = (pymin * xres) + pxmin;
	xlong o2 = (pymin * xres) + pxmax;
	xlong o3 = (pymax * xres) + pxmax;
	xlong o4 = (pymax * xres) + pxmin;

	xlong r = 0;

	//here is checked, by binary setting, if and where a terrain object (already drawn [into z-buffer]) around is hit
	//here no check is performed if the z-difference is above or below zero aka dip or rock. 
	r += xlong(clmath->heaviside( ( (*zbuffer)[o1] - (*zbuffer)[o1-1] ) ));        //test left
	r += xlong(clmath->heaviside( ( (*zbuffer)[o1] - (*zbuffer)[o1-xres] ) )) <<4; //test top

	r += xlong(clmath->heaviside( ( (*zbuffer)[o2] - (*zbuffer)[o2+1] ) )) <<1;    //test right
	r += xlong(clmath->heaviside( ( (*zbuffer)[o2] - (*zbuffer)[o2-xres] ) )) <<5; //test top

	r += xlong(clmath->heaviside( ( (*zbuffer)[o3] - (*zbuffer)[o3+1] ) )) <<2;    //test right
	r += xlong(clmath->heaviside( ( (*zbuffer)[o3] - (*zbuffer)[o3+xres] ) )) <<6; //test bottom

	r += xlong(clmath->heaviside( ( (*zbuffer)[o4] - (*zbuffer)[o4-1] ) )) <<3;    //test left
	r += xlong(clmath->heaviside( ( (*zbuffer)[o4] - (*zbuffer)[o4+xres] ) )) <<7; //test bottom



}

#endif

