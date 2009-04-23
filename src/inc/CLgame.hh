//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGAME
#define HH_CLGAME
#pragma message "Compiling " __FILE__ " ! TODO: ..."

class CLgame
{
	private:
		static xlong version;
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

		xlong getversion();
};

xlong CLgame::version = 0x00010000;

CLgame::CLgame(xlong bx1,xlong bx2,xlong by1,xlong by2)
{
	boundaryx1 = bx1;
	boundaryx2 = bx2;
	boundaryy1 = by1;
	boundaryy2 = by2;

}

CLgame::~CLgame()
{

}

xlong CLgame::boundarydetection(xlong x1,xlong y1,xlong x2,xlong y2)
{

}

xlong CLgame::boundarydetection(xlong x,xlong y)
{

}

xlong CLgame::collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a1,xlong b1,xlong a2,xlong b2)
{

}

xlong CLgame::collisiondetection(xlong x1,xlong y1,xlong x2,xlong y2,xlong a,xlong b)
{

}

xlong CLgame::getversion()
{
	return version;
}

#endif
