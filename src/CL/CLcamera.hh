///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLCAMERA
#define HH_CLCAMERA
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLbase.hh"
#include "CLbuffer.hh"
#include "CLvector.hh"
#include "CLmath.hh"
///*

///header
/* class name:	CLcamera
 * 
 * description:	UNV camera object
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLcamera : public CLbase<CLcamera,1>
{
	friend class CLbase<CLcamera,1>;
	
	private:
		CLmath& clmath;
	protected:
		CLfvector position;
		CLfvector direction;
		float     roll;
		CLfvector zo;
		CLfvector up;
		CLfvector fw;
		CLfvector rg;
		CLmatrix* linear;
		CLcamera();
		~CLcamera()  { delete linear; };
		void setup();
	public:
		void setroll(float r);
		void setposition(CLfvector p);
		void setdirection(CLfvector d);
		
};
///*

///implementation
CLcamera::CLcamera() //! noncritical
: clmath(CLmath::instance())
{ linear = new CLmatrix(1); } 

void CLcamera::setup()
{
	zo = CLfvector(0,0,0,1);
	fw = direction - position;
	rg = CLfvector(up * fw);
	rg *= (!rg);
	up = CLfvector(rg * fw);
	linear->set(rg,up,fw,zo);
	linear->translate(position.x,position.y,position.z);
}

void CLcamera::setroll(float r) //! noncritical
{
	up.x = clmath.sin(r);
	up.y = - clmath.cos(r);
	up.z = 0;
	setup();
}

void CLcamera::setposition(CLfvector p) //! noncritical
{
	position = p;
	setup();
}

void CLcamera::setdirection(CLfvector d) //! noncritical
{
	direction = d;
	setup();
}
///*

#endif
