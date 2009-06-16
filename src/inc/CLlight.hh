//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIGHT
#define HH_CLLIGHT
#warning "Compiling " __FILE__ " ! TODO: use this class"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLvector.hh"
#include "CLmath.hh"
#include "CLmatrix.hh"


class CLlight : public virtual CLcl
{
	protected:
		CLmath*     clmath;
		CLmatrix*   lm;

	private:
		fvector     light;
		uxlong      color;
		xlong       type;
		
	public:
		CLlight(float lx,float ly,float lz,float li,uxlong c,CLmath* clm);
		~CLlight();

		fvector getlight();
		uxlong getcolor();
		void addintensity(float p);
		void subintensity(float m);
		void mulintensity(float f);
		void setintensity(float i);
		void rotatelight(xlong x,xlong y,xlong z);
		void setlight(float x,float y,float z);
		void setlightx(float x);
		void setlighty(float x);
		void setlightz(float x);
		void draw();
};

CLlight::CLlight(float lx,float ly,float lz,float li,uxlong c,CLmath* clm)
{
	light.x = lx;
	light.y = ly;
	light.z = lz;
	light.l = li;
	color   = c;
	clmath  = clm;
	lm = new CLmatrix(1,clmath);
}

CLlight::~CLlight()
{
	delete lm;
}

fvector CLlight::getlight()
{
	return light;
}

uxlong CLlight::getcolor()
{
	return color;
}

void CLlight::addintensity(float p)
{
	light.l += p;
}

void CLlight::subintensity(float m)
{
	light.l -= m;
}

void CLlight::mulintensity(float f)
{
	light.l *= f;
}

void CLlight::setintensity(float i)
{
	light.l = i;
}

void CLlight::rotatelight(xlong x, xlong y, xlong z)
{
	lm->unit();
	lm->rotate(x,y,z);
	light = lm->transform(light);
}

void CLlight::setlight(float x,float y,float z)
{
	light.x = x;
	light.y = y;
	light.z = z;
}

void CLlight::setlightx(float x)
{
	light.x = x;
}

void CLlight::setlighty(float y)
{
	light.y = y;
}

void CLlight::setlightz(float z)
{
	light.z = z;
}

void CLlight::draw()
{

}

#endif

