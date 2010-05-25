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
		static CLmath& clmath;
	protected:
		CLfvector position;
		CLfvector direction;
		float     roll;
		CLfvector up;
		CLfvector fw;
		CLfvector rg;
		CLmatrix* linear;
		CLcamera();
		~CLcamera();
		void setup();
	public:
		void setroll(float r);
		void setposition(CLfvector p);
		void setdirection(CLfvector d);
		
};

CLmath& CLcamera::clmath = CLmath::instance();
///*

///implementation
CLcamera::CLcamera() { linear = new CLmatrix(1); } //! noncritical

CLcamera::~CLcamera() { delete linear; } //! noncritical

void CLcamera::setup()
{
	fw = direction - position;
	rg = CLfvector(up * fw);
	rg *= (!rg);
	up = CLfvector(rg * fw);
	linear->set(rg.x,rg.y,rg.z,0,up.x,up.y,up.z,0,fw.x,fw.y,fw.z,0,0,0,0,1);
	linear->translate(position.x,position.y,position.z);
}

void CLcamera::setroll(float r)
{
	up.x = clmath.sin(r);
	up.y = - clmath.cos(r);
	up.z = 0;
	setup();
}

void CLcamera::setposition(CLfvector p)
{
	position = p;
	setup();
}

void CLcamera::setdirection(CLfvector d)
{
	direction = d;
	setup();
}
///*

#endif
