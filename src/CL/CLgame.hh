//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLdiv3d.hh"


class CLgame : public virtual CLcl
{
	private:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;

	public:
		CLgame();
		CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2);
		~CLgame();
		
		xlong boundary(xlong x1,xlong y1,xlong x2,xlong y2) const;
		xlong boundary(xlong x,xlong y) const;
		
		template<class clvector>xlong boundary(const clvector& v1,const clvector& v2) const;
		template<class clvector>xlong boundary(const clvector& v) const;
		template<class clvector>xlong boundary(const clvector& position,const CLbox& bb) const;
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2) const;
		xlong collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b) const;
		template<class clvector>xlong collision(const clvector& v1,const clvector& v2,const clvector& w1,const clvector& w2) const;
		template<class clvector>xlong collision(const clvector& v1,const clvector& v2,const clvector& w) const;
		xlong collision(const CLbox& bb1,const CLbox& bb2) const;

		template<class clvector>xlong impact(CLfbuffer* ll,const CLbox* bb,clvector& p,clvector& l) const;
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

xlong CLgame::boundary(xlong x1,xlong y1,xlong x2,xlong y2) const
{
	if(x1>boundaryx1 && x2<boundaryx2 && y1>boundaryy1 && y2<boundaryy2) return 1;
	return 0;
}

xlong CLgame::boundary(xlong x,xlong y) const
{
	if(x>boundaryx1 && x<boundaryx2 && y>boundaryy1 && y<boundaryy2) return 1;
	return 0;
}

template<class clvector>
xlong CLgame::boundary(const clvector& v1,const clvector& v2) const
{

}

template<class clvector>
xlong CLgame::boundary(const clvector& v) const
{

}

template<class clvector>
xlong CLgame::boundary(const clvector& p,const CLbox& bb) const
{
	if( ( (  bb.t1.x + p.x ) < boundaryx1 ) || ( (  bb.t2.x + p.x ) < boundaryx1 ) || ( (  bb.t3.x + p.x ) < boundaryx1 ) || ( (  bb.t4.x + p.x ) < boundaryx1 ) ) return -1;
	if( ( (  bb.t1.x + p.x ) > boundaryx2 ) || ( (  bb.t2.x + p.x ) > boundaryx2 ) || ( (  bb.t3.x + p.x ) > boundaryx2 ) || ( (  bb.t4.x + p.x ) > boundaryx2 ) ) return 1;
	if( ( ( -bb.t1.y + p.y ) < boundaryy1 ) || ( ( -bb.t2.y + p.y ) < boundaryy1 ) || ( ( -bb.t3.y + p.y ) < boundaryy1 ) || ( ( -bb.t4.y + p.y ) < boundaryy1 ) ) return -2;
	if( ( ( -bb.t1.y + p.y ) > boundaryy2 ) || ( ( -bb.t2.y + p.y ) > boundaryy2 ) || ( ( -bb.t3.y + p.y ) > boundaryy2 ) || ( ( -bb.t4.y + p.y ) > boundaryy2 ) ) return 2;

	return 0;
}

xlong CLgame::collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2) const
{
	if( x1>a1 && x1<a2 && y1>b1 && y1<b2 ) return 1;
	if( x2>a1 && x2<a2 && y2>b1 && y2<b2 ) return 1;
	if( a1>x1 && a1<x2 && b1>y1 && b1<y2 ) return 1;
	if( a2>x1 && a2<x2 && b2>y1 && b2<y2 ) return 1;
	return 0;
}

xlong CLgame::collision(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b) const
{
	if( a<x1 && a>x2 && b>y1 && b<y2 ) return 1;
	return 0;
}

template<class clvector>
xlong CLgame::collision(const clvector& v1,const clvector& v2,const clvector& w1,const clvector& w2) const
{

}

template<class clvector>
xlong CLgame::collision(const clvector& v1,const clvector& v2,const clvector& w) const
{

}

xlong CLgame::collision(const CLbox& bb1, const CLbox& bb2) const //!test!
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

template<class clvector>
xlong CLgame::impact(CLfbuffer* ll,const CLbox* bb,clvector& p,clvector& l) const
{
	clvector p1( (p.x + bb->b1.x), (p.y - bb->b1.y), (p.z + bb->b1.z) );
	clvector p2( (p.x + bb->b2.x), (p.y - bb->b2.y), (p.z + bb->b2.z) );
	clvector p3( (p.x + bb->b3.x), (p.y - bb->b3.y), (p.z + bb->b3.z) );
	clvector p4( (p.x + bb->b4.x), (p.y - bb->b4.y), (p.z + bb->b4.z) );
	
	clvector l1( (l.x + bb->b1.x), (l.y - bb->b1.y), (l.z + bb->b1.z) );
	clvector l2( (l.x + bb->b2.x), (l.y - bb->b2.y), (l.z + bb->b2.z) );
	clvector l3( (l.x + bb->b3.x), (l.y - bb->b3.y), (l.z + bb->b3.z) );
	clvector l4( (l.x + bb->b4.x), (l.y - bb->b4.y), (l.z + bb->b4.z) );

//working when only driving parallel to axes, as soon as other rotations, not working

	tv.x = p.x;
	tv.y = p.y;
	lv.x = l.x;
	lv.y = l.y;
	
	//(*ll)[ (xlong(p.y)*xres) + xlong(p.x) ] = 0x000FFFFFF;
	//(*ll)[ (xlong(l.y)*xres) + xlong(l.x) ] = 0x000FFFF00; 
	
	//p1<l1
	if( (*ll)[ (xlong(p.y) * xres) + xlong(p.x) ] < (*ll)[ (xlong(l.y) * xres) + xlong(l.x) ] )
	{
		CLsystem::print((*ll)[ (xlong(p.y) * xres) + xlong(p.x) ],0);
		CLsystem::print(" vs ",0);
		CLsystem::print((*ll)[ (xlong(l.y) * xres) + xlong(l.x) ]);
		return 1;
	}
	//~ 
	//~ //p2<l2
	//~ if( ((*ll)[ (p2.y * xres) + p2.x ]) < ((*ll)[ (l2.y * xres) + l2.x ]) ) return 2;
	//~ 
	//~ //p3<l3
	//~ if( ((*ll)[ (p3.y * xres) + p3.x ]) < ((*ll)[ (l3.y * xres) + l3.x ]) ) return 4;
	//~ 
	//~ //p4<l4
	//~ if( ((*ll)[ (p4.y * xres) + p4.x ]) < ((*ll)[ (l4.y * xres) + l4.x ]) ) return 8;
	
	return 0;
}

#endif

