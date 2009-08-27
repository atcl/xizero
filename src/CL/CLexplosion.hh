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
#include "CLsystem.hh"


//combine "translate along normals" + "dyadic product"
//use dyadic product to crumple object
//reorder cyclic / or reset
//then translate along normals for explosion
class CLexplosion : public virtual CLcl
{
	protected:
		CLobject* object;
		CLmatrix* linear;
		CLfvector a;
		CLfvector b;

	private:
		xlong starttime;
		xlong lastupdate;
		xlong interval;
		xlong step;
		float transdir;
		bool type;

	public:
		CLexplosion(CLobject* o);
		~CLexplosion();
	
		void first(bool t);
		xlong next();
};

CLexplosion::CLexplosion(CLobject* o)
{
	linear = new CLmatrix(1);
	object = o;
	step = 0;
	interval = 30;
	transdir = 1.1f;
	
	a = CLfvector(1,0.5,1);
	b = CLfvector(0.5,1,0.5);
	linear->dyadic(a,b);
}

CLexplosion::~CLexplosion()
{ 
	delete linear;	
}

void CLexplosion::first(bool t)
{
	type = t;
	if(type) object->partupdate(linear);
	starttime = lastupdate = CLsystem::getmilliseconds(); 
}

xlong CLexplosion::next()
{
	xlong temp = CLsystem::getmilliseconds();

	switch(type)
	{
		case 0:
			//phase 1:
			if(temp < starttime + 800)
			{	
				object->translatealongnormals( 1 + ( (temp-lastupdate)/1000 ) );
			}
			//*
			
			//done:
			else return 1;
			//*
		break;
		
		case 1:
			//phase 1:
			if(temp < starttime + 200)
			{	
				object->translatealongnormals( 1 + ( (temp-lastupdate)/2000 ) );
			}
			//*
			
			//phase 2:
			else if(temp < starttime + 400)
			{
				linear->unit();
				linear->aspectscale( 1 - ( float(temp-lastupdate)/50.0 ) );
				object->update(linear);
			}	
			//*
			
			//done:
			else return 1;
			//*
		break;
	}
	
	lastupdate = temp;
	return 0;
}

#endif

