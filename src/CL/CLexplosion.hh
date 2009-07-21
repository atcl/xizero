//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


//combine "translate along normals" + "dyadic product"
//use dyadic product to crumple object
//then translate along normals for explosion
class CLexplosion : public virtual CLcl
{
	protected:
		CLobject* object;
		CLmatrix* cllinear;
		CLlvector a;
		CLlvector b;

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
	cllinear = new CLmatrix(1);
	object = o;
	step = 0;
	
	a(-2,1,0);
	b(2,-1,0);
	
	cllinear->dyadic(a,b);
}

CLexplosion::~CLexplosion() { }

void CLexplosion::draw()
{
	o->update(cllinear);
}

void CLexplosion::next()
{
	
}

#endif

