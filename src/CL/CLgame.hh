///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGAME
#define HH_CLGAME
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLstruct.hh"
#include "CLmisc3d.hh"
///*

///header
/* class name:	CLgame
 * 
 * description: collision,boundary and terrain detection (and transformation).
 * 
 * author:	atcl
 * 
 * notes:	fix terrain dependent transformation.
 * 
 * version: 0.1
 */
///*

///definitions
class CLgame : public CLbase<CLgame,1>
{
	friend class CLbase<CLgame,1>;
	
	private:
		static CLmath* clmath;
	protected:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;
		CLfvector slopes[2];
		float intersections[6][2];
		CLgame();
		~CLgame() { };
	public:
		void setboundaries(xlong bx1=0,xlong by1=0,xlong bx2=XRES,xlong by2=YRES);
		template<class clvector>bool boundary(const clvector& p,xlong mark=0);
		template<class clvector>xlong boundary(const clvector& p,const CLbox& bb,bool c=0);
		template<class clvector>xlong collision(clvector& p,CLbox& bb,clvector& q,bool n=1);
		template<class clvector>xlong collision2d(clvector& p,CLbox& bb,clvector& q,bool n=1);
		template<class clvector>xlong collision(clvector& p1,CLbox& bb1,clvector& p2,CLbox& bb2,bool n=1);
		template<class clvector>CLfvector terraincollision(const clvector& p,const CLbox* bb,const CLobject** t,bool fwbw);
};

CLmath* CLgame::clmath = CLmath::instance();
///*

///implementation
CLgame::CLgame() { boundaryx1 = boundaryy1 = 0; boundaryx2 = XRES; boundaryy2 = YRES; } //! noncritical

void CLgame::setboundaries(xlong bx1,xlong by1,xlong bx2,xlong by2) //! noncritical
{
	//set screen boundaries
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;
	//*
}

template<class clvector>
bool CLgame::boundary(const clvector& p,xlong mark) //! critical
{
	return ( (p.x<boundaryx1) && (p.x<boundaryx2) && ( (p.y-mark)>boundaryy1) && ( (p.y-mark)<boundaryy2) );
}

template<class clvector>
xlong CLgame::boundary(const clvector& p,const CLbox& bb,bool c) //! critical
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
xlong CLgame::collision(clvector& p,CLbox& bb,clvector& q,bool n) //! critical
{
	//calc levelposition of bounding box
	CLlvector a1( (p.x + bb.c[0].x), (p.y - bb.c[0].y), (p.z + bb.c[0].z) );
	CLlvector a2( (p.x + bb.c[1].x), (p.y - bb.c[1].y), (p.z + bb.c[1].z) );
	CLlvector a3( (p.x + bb.c[2].x), (p.y - bb.c[2].y), (p.z + bb.c[2].z) );
	CLlvector a4( (p.x + bb.c[3].x), (p.y - bb.c[3].y), (p.z + bb.c[3].z) );
	//*
	
	//n decides wether or not the intersection of p+bb (useful if p+bb needs to be checked against a lot of q's
	if(n)
	{
		//calc slopes
		slopes[0] = -a1+a2;
		slopes[1] = -a1+a4;
		//*
		
		//calc intersections of bb1 at p1
		intersections[0][0] = a1.x + ( (a1.y - a1.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[0][1] = a4.x + ( (a4.y - a4.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
		intersections[1][0] = a1.x + ( (a1.y - a1.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[1][1] = a2.x + ( (a2.y - a2.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
		intersections[2][0] = a1.y + ( (a1.x - a1.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[2][1] = a4.y + ( (a4.x - a4.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
		intersections[3][0] = a1.y + ( (a1.x - a1.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[3][1] = a2.y + ( (a2.x - a2.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
		intersections[4][0] = a1.z + ( (a1.x - a1.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[4][1] = a4.z + ( (a4.x - a4.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
		intersections[5][0] = a1.z + ( (a1.x - a1.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		intersections[6][1] = a2.z + ( (a2.x - a2.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		//*
	}
	//*
	
	//calc intersections at q
	xlong intersections2[6];
	intersections2[0] = q.x + ( (q.y - q.z) / (slopes[0].z-slopes[0].y) ) * slopes[0].x;
	intersections2[1] = q.x + ( (q.y - q.z) / (slopes[1].z-slopes[1].y) ) * slopes[1].x;
	intersections2[2] = q.y + ( (q.x - q.z) / (slopes[0].z-slopes[0].x) ) * slopes[0].y;
	intersections2[3] = q.y + ( (q.x - q.z) / (slopes[1].z-slopes[1].x) ) * slopes[1].y;
	intersections2[4] = q.z + ( (q.x - q.y) / (slopes[0].y-slopes[0].x) ) * slopes[0].z;
	intersections2[5] = q.z + ( (q.x - q.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
	//*
	
	//check if intersections overlap
	xlong r = -4;
	//~ if(intersections2[0]<intersections[0][0] && intersections2[0]<intersections[0][1]) r++;
	//~ if(intersections2[1]<intersections[1][0] && intersections2[0]<intersections[1][1]) r++;
	//~ if(intersections2[2]<intersections[2][0] && intersections2[0]<intersections[2][1]) r++;
	//~ if(intersections2[3]<intersections[3][0] && intersections2[0]<intersections[3][1]) r++;
	//~ if(intersections2[4]<intersections[4][0] && intersections2[0]<intersections[4][1]) r++;
	//~ if(intersections2[5]<intersections[5][0] && intersections2[0]<intersections[5][1]) r++;
	
	if(intersections2[0]>clmath->min(intersections[0][0],intersections[0][1]) && intersections2[0]<clmath->max(intersections[0][0],intersections[0][1])) r++;
	if(intersections2[1]>clmath->min(intersections[1][0],intersections[1][1]) && intersections2[0]<clmath->max(intersections[1][0],intersections[1][1])) r++;
	if(intersections2[2]>clmath->min(intersections[2][0],intersections[2][1]) && intersections2[0]<clmath->max(intersections[2][0],intersections[2][1])) r++;
	if(intersections2[3]>clmath->min(intersections[3][0],intersections[3][1]) && intersections2[0]<clmath->max(intersections[3][0],intersections[3][1])) r++;
	if(intersections2[4]>clmath->min(intersections[4][0],intersections[4][1]) && intersections2[0]<clmath->max(intersections[4][0],intersections[4][1])) r++;
	if(intersections2[5]>clmath->min(intersections[5][0],intersections[5][1]) && intersections2[0]<clmath->max(intersections[5][0],intersections[5][1])) r++;
	//*

	return r;
}

template<class clvector>
xlong CLgame::collision2d(clvector& p,CLbox& bb,clvector& q,bool n) //! critical
{
	//calc levelposition of bounding box
	CLlvector a1( (p.x + bb.c[0].x), (p.y - bb.c[0].y), (p.z + bb.c[0].z) );
	CLlvector a2( (p.x + bb.c[1].x), (p.y - bb.c[1].y), (p.z + bb.c[1].z) );
	CLlvector a3( (p.x + bb.c[2].x), (p.y - bb.c[2].y), (p.z + bb.c[2].z) );
	CLlvector a4( (p.x + bb.c[3].x), (p.y - bb.c[3].y), (p.z + bb.c[3].z) );
	//*

	//float intersections[6][2];
	//~ if(n)
	//~ {
		//calc slopes
		slopes[0] = -a1+a2;
		slopes[1] = -a1+a4;
		//*
		
		//calc intersections of bb1 at p1
		intersections[0][0] = a1.x + ( -a1.y / slopes[0].y ) * slopes[0].x;
		intersections[0][1] = a4.x + ( -a4.y / slopes[0].y ) * slopes[0].x;
		intersections[1][0] = a1.x + ( -a1.y / slopes[1].y ) * slopes[1].x;
		intersections[1][1] = a2.x + ( -a2.y / slopes[1].y ) * slopes[1].x;
		intersections[2][0] = a1.y + ( -a1.x / slopes[0].x ) * slopes[0].y;
		intersections[2][1] = a4.y + ( -a4.x / slopes[0].x ) * slopes[0].y;
		intersections[3][0] = a1.y + ( -a1.x / slopes[1].x ) * slopes[1].y;
		intersections[3][1] = a2.y + ( -a2.x / slopes[1].x ) * slopes[1].y;
		//*
	//~ }
	
	//calc intersections at q
	float intersections2[4];
	intersections2[0] = q.x + ( -q.y / slopes[0].y ) * slopes[0].x;
	intersections2[1] = q.x + ( -q.y / slopes[1].y ) * slopes[1].x;
	intersections2[2] = q.y + ( -q.x / slopes[0].x ) * slopes[0].y;
	intersections2[3] = q.y + ( -q.x / slopes[1].x ) * slopes[1].y;
	//*
	
	//check if intersections overlap
	xlong r = -4;
	if(intersections2[0]>=clmath->min(intersections[0][0],intersections[0][1]) && intersections2[0]<=clmath->max(intersections[0][0],intersections[0][1])) r++; //player ammo does not work here
	if(intersections2[1]>=clmath->min(intersections[1][0],intersections[1][1]) && intersections2[1]<=clmath->max(intersections[1][0],intersections[1][1])) r++;
	if(intersections2[2]>=clmath->min(intersections[2][0],intersections[2][1]) && intersections2[2]<=clmath->max(intersections[2][0],intersections[2][1])) r++;
	if(intersections2[3]>=clmath->min(intersections[3][0],intersections[3][1]) && intersections2[3]<=clmath->max(intersections[3][0],intersections[3][1])) r++; //player ammo does not work here
	//*
//~ say(r);
	return r;
}

template<class clvector>
xlong CLgame::collision(clvector& p1,CLbox& bb1,clvector& p2,CLbox& bb2,bool n) //! critical
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

	//n decides wether or not the intersection of p+bb (useful if p+bb1 needs to be checked against a lot of q+bb2's
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
		intersections[5][1] = a3.z + ( (a3.x - a3.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
		//*
	}
	//*

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
	intersections2[5][1] = b3.z + ( (b3.x - b3.y) / (slopes[1].y-slopes[1].x) ) * slopes[1].z;
	//*
	
	//check if intersection intervals overlap
	xlong r = 1;
	if(intersections2[0][0]<intersections[0][0] && intersections2[0][0]<intersections[0][1] && intersections2[0][1]<intersections[0][0] && intersections2[0][1]<intersections[0][1]) r--;
	if(intersections2[1][0]<intersections[1][0] && intersections2[0][0]<intersections[1][1] && intersections2[1][1]<intersections[1][0] && intersections2[1][1]<intersections[1][1]) r--;
	if(intersections2[2][0]<intersections[2][0] && intersections2[0][0]<intersections[2][1] && intersections2[2][1]<intersections[2][0] && intersections2[2][1]<intersections[2][1]) r--;
	if(intersections2[3][0]<intersections[3][0] && intersections2[0][0]<intersections[3][1] && intersections2[3][1]<intersections[3][0] && intersections2[3][1]<intersections[3][1]) r--;
	if(intersections2[4][0]<intersections[4][0] && intersections2[0][0]<intersections[4][1] && intersections2[4][1]<intersections[4][0] && intersections2[4][1]<intersections[4][1]) r--;
	if(intersections2[5][0]<intersections[5][0] && intersections2[0][0]<intersections[5][1] && intersections2[5][1]<intersections[5][0] && intersections2[5][1]<intersections[5][1]) r--;
	//*

	return 1;
}

template<class clvector>
CLfvector CLgame::terraincollision(const clvector& p,const CLbox* bb,const CLobject** t,bool fwbw) //! critical
{
	//assuming p1,p2 is front, p3,p4 is back
	
	//calc levelposition of current bounding box
	CLlvector p1( (p.x + bb->c[0].x), (p.y - bb->c[0].y), (p.z + bb->c[0].z) );
	CLlvector p2( (p.x + bb->c[1].x), (p.y - bb->c[1].y), (p.z + bb->c[1].z) );
	CLlvector p3( (p.x + bb->c[2].x), (p.y - bb->c[2].y), (p.z + bb->c[2].z) );
	CLlvector p4( (p.x + bb->c[3].x), (p.y - bb->c[3].y), (p.z + bb->c[3].z) );
	//*
	
	//determine levelposition in terms of object rows and get normal of terrain polygon at current level position
	xlong r1 = p1.y / 20;
	xlong r2 = p2.y / 20;
	xlong r3 = p3.y / 20;
	xlong r4 = p4.y / 20;
	
	CLfvector* s1 = new CLfvector(p1.x,10,p1.z);
	CLfvector* s2 = new CLfvector(p2.x,10,p2.z);
	CLfvector* s3 = new CLfvector(p3.x,10,p3.z);
	CLfvector* s4 = new CLfvector(p4.x,10,p4.z);
	
	s1 = t[r1]->getnormalat(s1);
	s2 = t[r2]->getnormalat(s2);
	s3 = t[r3]->getnormalat(s3);
	s4 = t[r4]->getnormalat(s4);
	//*
	
	CLlvector r(0,0,0,0); //x-angle,y-angle,z-angle,collision bool
	
	//calculate terrain angles
	
	//*
	
	//test for collision
	
	//* 
	
	return r;
}
///*

#endif

