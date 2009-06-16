//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#warning "Compiling " __FILE__ " ! TODO: complete vector functions"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLdiv3d.hh"


class CLgame : public virtual CLcl
{
	protected:

	private:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;

	public:
		CLgame();
		CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2);
		~CLgame();
		xlong boundary(xlong x1,xlong y1,xlong x2,xlong y2);
		xlong boundary(xlong x,xlong y);
		xlong boundary(vector* v1,vector* v2);
		xlong boundary(vector* v);
		xlong boundary(fvector position,CLbox* bb);
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2);
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b);
		xlong collision(vector* v1,vector* v2,vector* w1,vector* w2);
		xlong collision(vector* v1,vector* v2,vector* w);
		xlong collision(CLbox* bb1,CLbox* bb2);

		xlong impact(fvector p,fvector l,CLbox* bb);
};

CLgame::CLgame()
{
	boundaryx1 = 0;
	boundaryx2 = xres-1;
	boundaryy1 = 0;
	boundaryy2 = yres-1;
}

CLgame::CLgame(xlong bx1,xlong by1,xlong bx2,xlong by2)
{
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

xlong CLgame::boundary(fvector p,CLbox* bb)
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

xlong CLgame::impact(fvector p,fvector l,CLbox* bb)
{
	xlong px1 = p.x+bb->b1.x;
	xlong py1 = p.y+bb->b1.y;
	xlong px2 = p.x+bb->b2.x;
	xlong py2 = p.y+bb->b2.y;
	xlong px3 = p.x+bb->b3.x;
	xlong py3 = p.y+bb->b3.y;
	xlong px4 = p.x+bb->b4.x;
	xlong py4 = p.y+bb->b4.y;

	xlong lx1 = l.x+bb->b1.x;
	xlong ly1 = l.y+bb->b1.y;
	xlong lx2 = l.x+bb->b2.x;
	xlong ly2 = l.y+bb->b2.y;
	xlong lx3 = l.x+bb->b3.x;
	xlong ly3 = l.y+bb->b3.y;
	xlong lx4 = l.x+bb->b4.x;
	xlong ly4 = l.y+bb->b4.y;



}

#endif

