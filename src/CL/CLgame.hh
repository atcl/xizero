//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLmisc3d.hh"

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

class CLgame : public virtual CLcl, public CLsingle<CLgame>
{
	friend class CLsingle<CLgame>;
	
	private:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;
		CLfvector slopes[2];
		float intersections[6][2];
		CLgame();
		~CLgame() { };
	public:
		void init(xlong bx1=0,xlong by1=0,xlong bx2=XRES,xlong by2=YRES);
		template<class clvector>bool boundary(const clvector& p,xlong mark=0);
		template<class clvector>bool boundary(const clvector& p,const CLbox& bb,bool c=0);
		template<class clvector>xlong collision(clvector& p,CLbox& bb,clvector& q,bool n=1);
		template<class clvector>xlong collision2d(clvector& p,CLbox& bb,clvector& q,bool n=1);
		template<class clvector>xlong collision(clvector& p1,CLbox& bb1,clvector& p2,CLbox& bb2,bool n=1);
		template<class clvector>bool terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,float& xa,float& ya,float& zd);
};

CLgame::CLgame() { boundaryx1 = boundaryy1 = 0; boundaryx2 = XRES; boundaryy2 = YRES; }

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
bool CLgame::boundary(const clvector& p,xlong mark)
{
	//check if p id inside screen boundaries
	if( (p.x > boundaryx1) && (p.x < boundaryx2) && ( (p.y-mark) > boundaryy1) && ( (p.y-mark) < boundaryy2) ) return 0;
	//*
	
	return 1;
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
xlong CLgame::collision2d(clvector& p,CLbox& bb,clvector& q,bool n)
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
bool CLgame::terrain(CLfbuffer* ll,const CLbox* bb,const CLbox* ob,const clvector& p,const clvector& l,float& xa,float& ya,float& zd)
{
	//! this is still a big constrution site! Especially the not working calculation of the x and y angle !
	
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
	
	//calc zlevel of current bounding box corners in levellandscape
	float n1 = (*ll)[ (p1.y * XRES) + p1.x ];
	float n2 = (*ll)[ (p2.y * XRES) + p2.x ];
	float n3 = (*ll)[ (p3.y * XRES) + p3.x ];
	float n4 = (*ll)[ (p4.y * XRES) + p4.x ];
	//*
	
	//calc zlevel of last bounding box corners in levellandscape
	float o1 = (*ll)[ (l1.y * XRES) + l1.x ];
	float o2 = (*ll)[ (l2.y * XRES) + l2.x ];
	float o3 = (*ll)[ (l3.y * XRES) + l3.x ];
	float o4 = (*ll)[ (l4.y * XRES) + l4.x ];
	//*

	//check if zlevel from current bbox corners differ more than 3
	xlong r = 0; //this is the return value
	if( n1 < o1-3 ) r = 1;
	if( n2 < o2-3 ) r = 1;
	if( n3 < o3-3 ) r = 1;
	if( n4 < o4-3 ) r = 1;
	//*
	
	//calc z difference in bbox center between current and last
	//if(r==0) //activate when working correctly
	zd = (n1+n2+n3+n4)/4;
	//*
	
	CLfvector unitx = CLfvector(1,0,0);
	CLfvector unity = CLfvector(0,1,0);
	
	//calc new ground touching box with normal
	CLfvector na = CLfvector((-p1.x+p4.x),(-p1.y+p4.y),(n4+n3-n2-n1)/2);
	CLfvector nb = CLfvector((-p1.x+p2.x),(-p1.y+p2.y),(n1+n4-n2-n3)/2);
	CLfvector nn = (na*nb);
	//*
	
	//calc old ground touching box with normal
	CLfvector oa = CLfvector((-l1.x+l4.x),(-l1.y+l4.y),(l4.z+l3.z-l2.z-l1.z)/2);
	CLfvector oc = CLfvector((-l1.x+l2.x),(-l1.y+l2.y),(l1.z+l4.z-l2.z-l3.z)/2);
	CLfvector oo = (oa*oc);
	//*
	
	//calc x and y angle
	xa = 0; //-(nn%unitx) + (oo%unitx);
	ya = 0; //(nn%unity) - (oo%unity);
	//*
	
	//adjust angles depending on z values
	//~ if(nn.z < oo.z) xa = -xa; //replace by xa*=CLmath::sign(nx.z-lx.z)?
	//~ if(nn.z < oo.z) ya = -ya; //replace by ya*=CLmath::sign(nx.z-lx.z)?
	//*

	return r;
}

#endif

