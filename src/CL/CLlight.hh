//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIGHT
#define HH_CLLIGHT
#pragma message "Compiling " __FILE__ " ! TODO: use this class"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLvector.hh"
#include "CLmath.hh"
#include "CLmatrix.hh"

//obsolete!!!! 
class CLlight : public virtual CLcl
{
	protected:
		CLmatrix*   lm;

	private:
		CLfvector light;
		float     intensity;
		uxlong    color;
		xlong     type;
		
	public:
		CLlight(CLfvector l,float li,uxlong c);
		~CLlight();

		CLfvector getlight();
		uxlong getcolor();
		void addintensity(float p);
		void mulintensity(float f);
		void setintensity(float i);
		void rotatelight(xlong x,xlong y,xlong z);
		void setlight(float x,float y,float z);
		void setlightx(float x);
		void setlighty(float x);
		void setlightz(float x);
		void draw();
};

CLlight::CLlight(CLfvector l,float li,uxlong c)
{
	light = l;
	intensity = li;
	color   = c;
	lm = new CLmatrix(1);
}

CLlight::~CLlight()
{
	delete lm;
}

CLfvector CLlight::getlight()
{
	return light;
}

uxlong CLlight::getcolor()
{
	return color;
}

void CLlight::addintensity(float p)
{
	intensity += p;
}

void CLlight::mulintensity(float f)
{
	intensity *= f;
}

void CLlight::setintensity(float i)
{
// 	intensity = i;
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
