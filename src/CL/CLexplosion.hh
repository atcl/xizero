//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
//#pragma message "Compiling " __FILE__ " ! TODO: all"

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
		float transdir;

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
	transdir = 1.1f;
	
	a = CLfvector(1,0.5,0.3);
	b = CLfvector(-1,0,1);
	
	//cllinear->dyadic(a,b);
	//cllinear->shear(0,0,0.1);
}

CLexplosion::~CLexplosion() { }

void CLexplosion::next()
{
	xlong temp = CLsystem::getmilliseconds();
	if(temp >= lastupdate + interval)
	{
		cllinear->transpone();
		cllinear->shear(0.1,0,0);
		object->update(cllinear);
		
		//object->translatealongnormals(transdir);
		
		lastupdate = temp;
		interval++;
		//~ if(interval>50 && transdir>0) transdir*=-1;
	}
}

#endif

