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

	public:
		CLexplosion(CLobject* o);
		~CLexplosion();

		void start();
};

CLexplosion::CLexplosion(CLobject* o)
{
	object = o;
}

CLexplosion::~CLexplosion() { }

void CLexplosion::start()
{

}

#endif
