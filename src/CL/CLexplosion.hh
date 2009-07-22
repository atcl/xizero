//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLvector.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLcl.hh"
#include "CLapi.hh"


//combine "translate along normals" + "dyadic product"
//use dyadic product to crumple object
//then translate along normals for explosion
class CLexplosion : public virtual CLcl
{
	protected:
		CLobject* object;
		CLmatrix* cllinear;
		CLfvector a;
		CLfvector b;

	private:
		xlong lastupdate;
		xlong interval;
		xlong step;

	public:
		CLexplosion(CLobject* o);
		~CLexplosion();

		void next();
};

CLexplosion::CLexplosion(CLobject* o)
{
	cllinear = new CLmatrix(1);
	object = o;
	step = 0;
	interval = 30;
	
	a = CLfvector(1.0f,0.0f,0.0f);
	b = CLfvector(1.0f,0.0f,0.0f);
	
	cllinear->dyadic(a,b);
}

CLexplosion::~CLexplosion() { }

void CLexplosion::next()
{
	xlong temp = CLsystem::getmilliseconds();
	if(temp >= lastupdate + interval)
	{
		object->update(cllinear);
		
		//object->translatealongnormals(1.1);
		
		lastupdate = temp;	
	}
}

#endif

