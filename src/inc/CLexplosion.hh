//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


//combi aus "translate along normals" + "dyadic product"
class CLexplosion : public virtual CLcl
{
	protected:
		CLobject* object;

	private:
		float last_time;
		float curr_time;
		xlong interval;
		xlong step;

	public:
		CLexplosion(CLobject* o);
		~CLexplosion();

		void draw();
		void next();
};

CLexplosion::CLexplosion(CLobject* o)
{
	object = o;
	step = 0;
}

CLexplosion::~CLexplosion() { }

void CLexplosion::draw()
{

}

void CLexplosion::next()
{

}

#endif

