///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
///*

///includes
#include "CLtypes.hh"
#include "CLvector.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLbase.hh"
#include "CLwindow.hh"
///*

///header
/* class name:	CLexplosion
 * 
 * description:	This class manages explosions of 3d objects.
 * 
 * author:	atcl
 * 
 * notes:	.
 * 
 * version: 0.1
 */
///*

///definitions
//combine "translate along normals" + "dyadic product"
//use dyadic product to crumple object
//reorder cyclic / or reset
//then translate along normals for explosion
class CLexplosion : public CLbase<CLexplosion,0>
{
	private:
		static CLwindow* clwindow;
	protected:
		CLobject* object;
		CLfvector a;
		CLfvector b;
		xlong starttime;
		xlong lastupdate;
		xlong interval;
		xlong step;
		float transdir;
		bool type;
	public:
		CLexplosion(CLobject* o);
		~CLexplosion() { };
		void first(bool t);
		xlong next();
};

CLwindow* CLexplosion::clwindow = CLwindow::instance();
///*

///implementation
CLexplosion::CLexplosion(CLobject* o) //! noncritical
{
	//set up attributes
	object = o;
	step = 0;
	interval = 30;
	transdir = 1.1f;
	//*
}

void CLexplosion::first(bool t) //! noncritical
{
	//set up explosion matrix
	a = CLfvector(1,0.5,1);
	b = CLfvector(0.5,1,0.5);
	object->getmatrix()->dyadic(a,b);
	//*
	
	type = t;
	if(type) { object->partupdate(); }
	starttime = lastupdate = clwindow->getmilliseconds(); 
}

xlong CLexplosion::next() //! critical
{
	xlong temp = clwindow->getmilliseconds();

	switch(type)
	{
		//normal explosion (just translate along normals)
		case 0:
			//phase 1:
			if(temp < starttime + 800) { object->translatealongnormals( 1 + ( (temp-lastupdate)/1000 ) ); }
			//*
			
			//done:
			else { return 1; }
			//*
		break;
		//*
		
		//other dimension explosion (crumple object, then grow, then shring to zero)
		case 1:
			//phase 1:
			if(temp < starttime + 200) { object->translatealongnormals( 1 + ( (temp-lastupdate)/2000 ) ); }
			//*
			
			//phase 2:
			else if(temp < starttime + 400)
			{
				object->getmatrix()->unit();
				object->getmatrix()->aspectscale( 1 - ( float(temp-lastupdate)/50.0 ) );
				object->update();
			}	
			//*
			
			//done:
			else return 1;
			//*
		break;
		//*
		
		//other boss dimension explosion (crumple object, then grow big, then shring to zero)
		case 2:
			//phase 1:
			if(temp < starttime + 400) object->translatealongnormals( 1 + ( (temp-lastupdate)/2000 ) );
			//*
			
			//phase 2:
			else if(temp < starttime + 400)
			{
				object->getmatrix()->unit();
				object->getmatrix()->aspectscale( 1 - ( float(temp-lastupdate)/50.0 ) );
				object->update();
			}	
			//*
			
			//done:
			else { return 1; }
			//*
		break;
		//*
	}
	
	lastupdate = temp;
	return 0;
}
///*

#endif

