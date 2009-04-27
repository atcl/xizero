//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLcl.hh"


class CLgame : public virtual CLcl
{
	private:
		xlong boundaryx1;
		xlong boundaryx2;
		xlong boundaryy1;
		xlong boundaryy2;


	public:
		CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2);
		~CLgame();
		xlong boundarydetection(xlong x1,xlong y1,xlong x2,xlong y2);
		xlong boundarydetection(xlong x,xlong y);
		xlong collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2);
		xlong collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b);
};

CLgame::CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2)
{
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;

}

CLgame::~CLgame() { }

xlong CLgame::boundarydetection(xlong x1,xlong y1,xlong x2,xlong y2)
{
	if(x1>boundaryx1 && x2<boundaryx2 && y1>boundaryy1 && y2<boundaryy2) return 0;
	return 1;
}

xlong CLgame::boundarydetection(xlong x,xlong y)
{
	if(x>boundaryx1 && x<boundaryx2 && y>boundaryy1 && y<boundaryy2) return 0;
	return 1;
}

xlong CLgame::collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2)
{
	if(x1>a1 && x1<a2 && y1>b1 && y1<b2) return 1;
	if(x2>a1 && x2<a2 && y2>b1 && y2<b2) return 1;
	if(a1>x1 && a1<x2 && b1>y1 && b1<y2) return 1;
	if(a2>x1 && a2<x2 && b2>y1 && b2<y2) return 1;
	return 0;
}

xlong CLgame::collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b)
{
	if(a<x1 && a>x2 && b>y1 && b<y2) return 1;
	return 0;
}

#endif

