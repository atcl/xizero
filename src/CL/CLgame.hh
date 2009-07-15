//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLdiv3d.hh"


namespace CLgame
{
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;

		void init(xlong bx1=0,xlong by1=0,xlong bx2=xres,xlong by2=yres);
		
		template<class clvector>bool boundary(const clvector& p);
		template<class clvector>bool boundary(const clvector& p,const CLbox& bb);
		
		template<class clvector>xlong collision(const clvector& p,const CLbox* bb,const clvector& q);
		template<class clvector>xlong collision(const clvector& p1,const CLbox& bb1,const clvector& p2,const CLbox& bb2);

		template<class clvector>bool terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,xlong& xa,xlong& ya,xlong& zd);
};


void CLgame::init(xlong bx1,xlong by1,xlong bx2,xlong by2)
{
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;
}

template<class clvector>
bool CLgame::boundary(const clvector& p)
{
	if( (p.x < boundaryx1) || (p.x > boundaryx2) || (p.y < boundaryy1) || (p.y > boundaryy2) ) return 1;
	return 0;
}

template<class clvector>
bool CLgame::boundary(const clvector& p,const CLbox& bb)
{
	if( ( (  bb.t1.x + p.x ) < boundaryx1 ) || ( (  bb.t2.x + p.x ) < boundaryx1 ) || ( (  bb.t3.x + p.x ) < boundaryx1 ) || ( (  bb.t4.x + p.x ) < boundaryx1 ) ) return -1;
	if( ( (  bb.t1.x + p.x ) > boundaryx2 ) || ( (  bb.t2.x + p.x ) > boundaryx2 ) || ( (  bb.t3.x + p.x ) > boundaryx2 ) || ( (  bb.t4.x + p.x ) > boundaryx2 ) ) return 1;
	if( ( ( -bb.t1.y + p.y ) < boundaryy1 ) || ( ( -bb.t2.y + p.y ) < boundaryy1 ) || ( ( -bb.t3.y + p.y ) < boundaryy1 ) || ( ( -bb.t4.y + p.y ) < boundaryy1 ) ) return -2;
	if( ( ( -bb.t1.y + p.y ) > boundaryy2 ) || ( ( -bb.t2.y + p.y ) > boundaryy2 ) || ( ( -bb.t3.y + p.y ) > boundaryy2 ) || ( ( -bb.t4.y + p.y ) > boundaryy2 ) ) return 2;

	return 0;
}

template<class clvector>
xlong CLgame::collision(const clvector& p,const CLbox* bb,const clvector& q)
{
	CLlvector a1( (p.x + bb->b1.x), (p.y - bb->b1.y), (p.z + bb->b1.z) );
	CLlvector a2( (p.x + bb->b2.x), (p.y - bb->b2.y), (p.z + bb->b2.z) );
	CLlvector a3( (p.x + bb->b3.x), (p.y - bb->b3.y), (p.z + bb->b3.z) );
	CLlvector a4( (p.x + bb->b4.x), (p.y - bb->b4.y), (p.z + bb->b4.z) );
	
	//
	
	return 0;
}

template<class clvector>
xlong CLgame::collision(const clvector& p1,const CLbox* bb1,const clvector& p2,const CLbox* bb2)
{
	CLlvector a1( (p1.x + bb1->b1.x), (p1.y - bb1->b1.y), (p1.z + bb1->b1.z) );
	CLlvector a2( (p1.x + bb1->b2.x), (p1.y - bb1->b2.y), (p1.z + bb1->b2.z) );
	CLlvector a3( (p1.x + bb1->b3.x), (p1.y - bb1->b3.y), (p1.z + bb1->b3.z) );
	CLlvector a4( (p1.x + bb1->b4.x), (p1.y - bb1->b4.y), (p1.z + bb1->b4.z) );
	
	CLlvector b1( (p2.x + bb2->b1.x), (p2.y - bb2->b1.y), (p2.z + bb2->b1.z) );
	CLlvector b2( (p2.x + bb2->b2.x), (p2.y - bb2->b2.y), (p2.z + bb2->b2.z) );
	CLlvector b3( (p2.x + bb2->b3.x), (p2.y - bb2->b3.y), (p2.z + bb2->b3.z) );
	CLlvector b4( (p2.x + bb2->b4.x), (p2.y - bb2->b4.y), (p2.z + bb2->b4.z) );

	//

	return 0;
}

template<class clvector>
bool CLgame::terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,xlong& xa,xlong& ya,xlong& zd)
{
	CLlvector p1( (p.x + bb->b1.x), (p.y - bb->b1.y), (p.z + bb->b1.z) );
	CLlvector p2( (p.x + bb->b2.x), (p.y - bb->b2.y), (p.z + bb->b2.z) );
	CLlvector p3( (p.x + bb->b3.x), (p.y - bb->b3.y), (p.z + bb->b3.z) );
	CLlvector p4( (p.x + bb->b4.x), (p.y - bb->b4.y), (p.z + bb->b4.z) );
	
	CLlvector l1( (l.x + ob->b1.x), (l.y - ob->b1.y), (l.z + ob->b1.z) );
	CLlvector l2( (l.x + ob->b2.x), (l.y - ob->b2.y), (l.z + ob->b2.z) );
	CLlvector l3( (l.x + ob->b3.x), (l.y - ob->b3.y), (l.z + ob->b3.z) );
	CLlvector l4( (l.x + ob->b4.x), (l.y - ob->b4.y), (l.z + ob->b4.z) );
	
	//temp! only for landscape trace
	lv.x = l.x;
	lv.y = l.y;
	tv.x = p.x;
	tv.y = p.y;
	//*

	xlong r = 0;
	
	//p1<l1
	if( ((*ll)[ (p1.y * xres) + p1.x ]) < ((*ll)[ (l1.y * xres) + l1.x ])-3 ) r = 1;
	
	//p2<l2
	if( ((*ll)[ (p2.y * xres) + p2.x ]) < ((*ll)[ (l2.y * xres) + l2.x ])-3 ) r = 1;
	
	//p3<l3
	if( ((*ll)[ (p3.y * xres) + p3.x ]) < ((*ll)[ (l3.y * xres) + l3.x ])-3 ) r = 1;
	
	//p4<l4
	if( ((*ll)[ (p4.y * xres) + p4.x ]) < ((*ll)[ (l4.y * xres) + l4.x ])-3 ) r = 1;
	
	//~ //p1>l1
	//~ if( ((*ll)[ (p1.y * xres) + p1.x ]) > ((*ll)[ (l1.y * xres) + l1.x ])-3 ) r = -1;
	//~ 
	//~ //p2>l2
	//~ if( ((*ll)[ (p2.y * xres) + p2.x ]) > ((*ll)[ (l2.y * xres) + l2.x ])-3 ) r = -2;
	//~ 
	//~ //p3>l3
	//~ if( ((*ll)[ (p3.y * xres) + p3.x ]) > ((*ll)[ (l3.y * xres) + l3.x ])-3 ) r = -4;
	//~ 
	//~ //p4>l4
	//~ if( ((*ll)[ (p4.y * xres) + p4.x ]) > ((*ll)[ (l4.y * xres) + l4.x ])-3 ) r = -8;

	//calc xa,ya,zd
	
	xlong az = -( ( ((*ll)[ (p1.y * xres) + p1.x ]) + ((*ll)[ (p4.y * xres) + p4.x ]) ) / 2 )
	           +( ( ((*ll)[ (p2.y * xres) + p2.x ]) + ((*ll)[ (p3.y * xres) + p3.x ]) ) / 2 )
			   
	xlong bz = -( ( ((*ll)[ (l1.y * xres) + l1.x ]) + ((*ll)[ (l4.y * xres) + l4.x ]) ) / 2 )
	           +( ( ((*ll)[ (l2.y * xres) + l2.x ]) + ((*ll)[ (l3.y * xres) + l3.x ]) ) / 2 )
	
	CLvector a( (-p1.x+p2.x),(-p1.y+p2.y),az );
	
	CLvector b( (-l1.x+l2.x),(-l1.y+l2.y),bz );
	
	ya = a % b;
	
	xlong cz = -( ( ((*ll)[ (p1.y * xres) + p1.x ]) + ((*ll)[ (p2.y * xres) + p2.x ]) ) / 2 )
	           +( ( ((*ll)[ (p4.y * xres) + p4.x ]) + ((*ll)[ (p3.y * xres) + p3.x ]) ) / 2 )
			   
	xlong dz = -( ( ((*ll)[ (l1.y * xres) + l1.x ]) + ((*ll)[ (l2.y * xres) + l2.x ]) ) / 2 )
	           +( ( ((*ll)[ (l4.y * xres) + l4.x ]) + ((*ll)[ (l3.y * xres) + l3.x ]) ) / 2 )
	
	CLvector c( (-p1.x+p4.x),(-p1.y+p4.y),cz );
	
	CLvector d( (-l1.x+l4.x),(-l1.y+l4.y),dz );
	
	xa = c % d;
	
	zd = ( ((*ll)[ (p1.y * xres) + p1.x ]) 
	   +   ((*ll)[ (p2.y * xres) + p2.x ])
	   +   ((*ll)[ (p3.y * xres) + p3.x ])
	   +   ((*ll)[ (p4.y * xres) + p4.x ]) )
	   / 4;

	//*

	return r;
}

#endif

