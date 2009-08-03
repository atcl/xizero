//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
//#pragma message "Compiling " __FILE__ " ! TODO: "

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
		
		CLfvector slopes[2];
		float     intersections[6][2];

		void init(xlong bx1=0,xlong by1=0,xlong bx2=xres,xlong by2=yres);
		
		template<class clvector>bool boundary(const clvector& p);
		template<class clvector>bool boundary(const clvector& p,const CLbox& bb,bool c=0);
		
		template<class clvector>xlong collision(clvector& p,CLbox& bb,clvector& q,bool n=1);
		template<class clvector>xlong collision(clvector& p1,CLbox& bb1,clvector& p2,CLbox& bb2,bool n=1);

		template<class clvector>bool terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,xlong& xa,xlong& ya,xlong& zd);
};


void CLgame::init(xlong bx1,xlong by1,xlong bx2,xlong by2)
{
	//set screen boundaries
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;
	//*
}

template<class clvector>
bool CLgame::boundary(const clvector& p)
{
	//check if p id inside screen boundaries
	if( (p.x < boundaryx1) || (p.x > boundaryx2) || (p.y < boundaryy1) || (p.y > boundaryy2) ) return 1;
	//*
	
	return 0;
}

template<class clvector>
bool CLgame::boundary(const clvector& p,const CLbox& bb,bool c)
{
	switch(c)
	{
		case false:
			//check if bounding box bb at position p is inside screen boundaries
			if( ( (  bb.c[4].x + p.x ) < boundaryx1 ) || ( (  bb.c[5].x + p.x ) < boundaryx1 ) || ( (  bb.c[6].x + p.x ) < boundaryx1 ) || ( (  bb.c[7].x + p.x ) < boundaryx1 ) ) return -1;
			if( ( (  bb.c[4].x + p.x ) > boundaryx2 ) || ( (  bb.c[5].x + p.x ) > boundaryx2 ) || ( (  bb.c[6].x + p.x ) > boundaryx2 ) || ( (  bb.c[7].x + p.x ) > boundaryx2 ) ) return 1;
			if( ( ( -bb.c[4].y + p.y ) < boundaryy1 ) || ( ( -bb.c[5].y + p.y ) < boundaryy1 ) || ( ( -bb.c[6].y + p.y ) < boundaryy1 ) || ( ( -bb.c[7].y + p.y ) < boundaryy1 ) ) return -2;
			if( ( ( -bb.c[4].y + p.y ) > boundaryy2 ) || ( ( -bb.c[5].y + p.y ) > boundaryy2 ) || ( ( -bb.c[6].y + p.y ) > boundaryy2 ) || ( ( -bb.c[7].y + p.y ) > boundaryy2 ) ) return 2;
			//*
		break;
		
		case true:
			//check if bounding box bb at position p is completely inside screen boundaries
			if( ( (  bb.c[4].x + p.x ) < boundaryx1 ) && ( (  bb.c[5].x + p.x ) < boundaryx1 ) && ( (  bb.c[6].x + p.x ) < boundaryx1 ) && ( (  bb.c[7].x + p.x ) < boundaryx1 ) ) return -1;
			if( ( (  bb.c[4].x + p.x ) > boundaryx2 ) && ( (  bb.c[5].x + p.x ) > boundaryx2 ) && ( (  bb.c[6].x + p.x ) > boundaryx2 ) && ( (  bb.c[7].x + p.x ) > boundaryx2 ) ) return 1;
			if( ( ( -bb.c[4].y + p.y ) < boundaryy1 ) && ( ( -bb.c[5].y + p.y ) < boundaryy1 ) && ( ( -bb.c[6].y + p.y ) < boundaryy1 ) && ( ( -bb.c[7].y + p.y ) < boundaryy1 ) ) return -2;
			if( ( ( -bb.c[4].y + p.y ) > boundaryy2 ) && ( ( -bb.c[5].y + p.y ) > boundaryy2 ) && ( ( -bb.c[6].y + p.y ) > boundaryy2 ) && ( ( -bb.c[7].y + p.y ) > boundaryy2 ) ) return 2;
			//*
		break;
	}

	return 0;
}

template<class clvector>
xlong CLgame::collision(clvector& p,CLbox& bb,clvector& q,bool n)
{
	//calc levelposition of bounding box
	CLlvector a1( (p.x + bb.c[0].x), (p.y - bb.c[0].y), (p.z + bb.c[0].z) );
	CLlvector a2( (p.x + bb.c[1].x), (p.y - bb.c[1].y), (p.z + bb.c[1].z) );
	CLlvector a3( (p.x + bb.c[2].x), (p.y - bb.c[2].y), (p.z + bb.c[2].z) );
	CLlvector a4( (p.x + bb.c[3].x), (p.y - bb.c[3].y), (p.z + bb.c[3].z) );
	//*
	
	if(n)
	{
		//calc slopes
		slopes[0] = -a1+a2;
		slopes[1] = -a1+a3;
		//*
		
		//calc intersections of bb1 at p1
		intersections[0][0] = a1.x + ( (a1.y - a1.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[0][1] = a3.x + ( (a3.y - a3.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[1][0] = a1.x + ( (a1.y - a1.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[1][1] = a3.x + ( (a3.y - a3.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[2][0] = a1.y + ( (a1.x - a1.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[2][1] = a3.y + ( (a3.x - a3.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[3][0] = a1.y + ( (a1.x - a1.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[3][1] = a3.y + ( (a3.x - a3.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[4][0] = a1.z + ( (a1.x - a1.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[4][1] = a3.z + ( (a3.x - a3.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[5][0] = a1.z + ( (a1.x - a1.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		intersections[6][1] = a3.z + ( (a3.x - a3.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		//*
	}
	
	//calc intersections at q
	xlong intersections2[6];
	intersections[0] = q.x + ( (q.y - q.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
	intersections[1] = q.x + ( (q.y - q.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
	intersections[2] = q.y + ( (q.x - q.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
	intersections[3] = q.y + ( (q.x - q.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
	intersections[4] = q.z + ( (q.x - q.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
	intersections[5] = q.z + ( (q.x - q.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
	//*
	
	//check if intersections overlap
	
	//*
	
	return 0;
}

template<class clvector>
xlong CLgame::collision(clvector& p1,CLbox& bb1,clvector& p2,CLbox& bb2,bool n)
{
	//calc levelposition of first bounding box bb1 at p1 
	CLfvector a1( (p1.x + bb1.c[0].x), (p1.y - bb1.c[0].y), (p1.z + bb1.c[0].z) );
	CLfvector a2( (p1.x + bb1.c[1].x), (p1.y - bb1.c[1].y), (p1.z + bb1.c[1].z) );
	CLfvector a3( (p1.x + bb1.c[2].x), (p1.y - bb1.c[2].y), (p1.z + bb1.c[2].z) );
	CLfvector a4( (p1.x + bb1.c[3].x), (p1.y - bb1.c[3].y), (p1.z + bb1.c[3].z) );
	//*
	
	//calc levelposition of second bounding box bb2 at p2
	CLfvector b1( (p2.x + bb2.c[0].x), (p2.y - bb2.c[0].y), (p2.z + bb2.c[0].z) );
	CLfvector b2( (p2.x + bb2.c[1].x), (p2.y - bb2.c[1].y), (p2.z + bb2.c[1].z) );
	CLfvector b3( (p2.x + bb2.c[2].x), (p2.y - bb2.c[2].y), (p2.z + bb2.c[2].z) );
	CLfvector b4( (p2.x + bb2.c[3].x), (p2.y - bb2.c[3].y), (p2.z + bb2.c[3].z) );
	//*

	if(n)
	{
		//calc slopes
		slopes[0] = -a1+a2;
		slopes[1] = -a1+a3;
		//*
		
		//calc intersections of bb1 at p1
		intersections[0][0] = a1.x + ( (a1.y - a1.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[0][1] = a3.x + ( (a3.y - a3.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[1][0] = a1.x + ( (a1.y - a1.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[1][1] = a3.x + ( (a3.y - a3.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[2][0] = a1.y + ( (a1.x - a1.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[2][1] = a3.y + ( (a3.x - a3.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[3][0] = a1.y + ( (a1.x - a1.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[3][1] = a3.y + ( (a3.x - a3.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[4][0] = a1.z + ( (a1.x - a1.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[4][1] = a3.z + ( (a3.x - a3.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[5][0] = a1.z + ( (a1.x - a1.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		intersections[6][1] = a3.z + ( (a3.x - a3.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		//*
	}

	//calc intersections of bb2 at p2
	xlong intersections2[6][2];
	intersections2[0][0] = b1.x + ( (b1.y - b1.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
	intersections2[0][1] = b3.x + ( (b3.y - b3.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
	intersections2[1][0] = b1.x + ( (b1.y - b1.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
	intersections2[1][1] = b3.x + ( (b3.y - b3.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
	intersections2[2][0] = b1.y + ( (b1.x - b1.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
	intersections2[2][1] = b3.y + ( (b3.x - b3.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
	intersections2[3][0] = b1.y + ( (b1.x - b1.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
	intersections2[3][1] = b3.y + ( (b3.x - b3.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
	intersections2[4][0] = b1.z + ( (b1.x - b1.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
	intersections2[4][1] = b3.z + ( (b3.x - b3.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
	intersections2[5][0] = b1.z + ( (b1.x - b1.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
	intersections2[6][1] = b3.z + ( (b3.x - b3.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
	//*
	
	//check if intersection intervals overlap
	
	//*

	return 1;
}

template<class clvector>
bool CLgame::terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,xlong& xa,xlong& ya,xlong& zd)
{
	//calc levelposition of current bounding box
	CLlvector p1( (p.x + bb->c[0].x), (p.y - bb->c[0].y), (p.z + bb->c[0].z) );
	CLlvector p2( (p.x + bb->c[1].x), (p.y - bb->c[1].y), (p.z + bb->c[1].z) );
	CLlvector p3( (p.x + bb->c[2].x), (p.y - bb->c[2].y), (p.z + bb->c[2].z) );
	CLlvector p4( (p.x + bb->c[3].x), (p.y - bb->c[3].y), (p.z + bb->c[3].z) );
	//*
	
	//calc levelposition of last bounding box
	CLlvector l1( (l.x + ob->c[0].x), (l.y - ob->c[0].y), (l.z + ob->c[0].z) );
	CLlvector l2( (l.x + ob->c[1].x), (l.y - ob->c[1].y), (l.z + ob->c[1].z) );
	CLlvector l3( (l.x + ob->c[2].x), (l.y - ob->c[2].y), (l.z + ob->c[2].z) );
	CLlvector l4( (l.x + ob->c[3].x), (l.y - ob->c[3].y), (l.z + ob->c[3].z) );
	//*
	
	//temp! only for landscape trace, see xizero.cc l68,l69
	lv.x = l.x;
	lv.y = l.y;
	tv.x = p.x;
	tv.y = p.y;
	//*

	//calc zlevel of current bounding box corners in levellandscape
	float n1 = (*ll)[ (p1.y * xres) + p1.x ];
	float n2 = (*ll)[ (p2.y * xres) + p2.x ];
	float n3 = (*ll)[ (p3.y * xres) + p3.x ];
	float n4 = (*ll)[ (p4.y * xres) + p4.x ];
	//*
	
	//calc zlevel of last bounding box corners in levellandscape
	float o1 = (*ll)[ (l1.y * xres) + l1.x ];
	float o2 = (*ll)[ (l2.y * xres) + l2.x ];
	float o3 = (*ll)[ (l3.y * xres) + l3.x ];
	float o4 = (*ll)[ (l4.y * xres) + l4.x ];
	//*

	//check if zlevel from current bbox corners differ more than 3
	xlong r = 0; //this is the return value
	if( n1 < o1-3 ) r = 1;
	if( n2 < o2-3 ) r = 1;
	if( n3 < o3-3 ) r = 1;
	if( n4 < o4-3 ) r = 1;
	//*
	
	//calc y angle ya
	float az = ( n2+n3-n1-n4 ); // /2?
	float bz = ( o2+o3-o1-o4 ); // /2?
	CLfvector a( (-p1.x+p2.x),(-p1.y+p2.y),az );
	CLfvector b( (-p1.x+p2.x),(-p1.y+p2.y),bz );	
	ya = -(a % b);
	//*
	
	//calc x angle xa
	float cz = ( n3+n4-n1-n2 ); // /2?
	float dz = ( o3+o4-o1-o2 ); // /2?
	CLfvector c( (-p1.x+p4.x),(-p1.y+p4.y),cz );
	CLfvector d( (-p1.x+p4.x),(-p1.y+p4.y),dz );
	xa = -(c % d);
	//*
	
	//calc z difference in bbox center between current and last
	//if(r==0) //activate when working correctly
	zd = (n1+n2+n3+n4-o1-o2-o3-o4)/4;
	//*

	return r;
}

#endif

