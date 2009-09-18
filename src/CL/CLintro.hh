//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#pragma message Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLglobal.hh"
#include "CLsystem.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLformat.hh"

/* class name:	CLintro
 * 
 * description:	Holds atcrosslevel and xizero intro animations
 * 
 * author:	atcl
 * 
 * notes:	implement xizero intro
 * 
 * version: 0.1
 */

class CLintro : public virtual CLcl, public CLsingle<CLintro> 
{
	friend class CLsingle<CLintro>;
	
	private:
		CLmatrix* linear;
		CLintro() { };
		~CLintro() { };
	public:
		void atcrosslevel();
		void xizero();
};

void CLintro::atcrosslevel()
{
	//load atcrosslevel model
	CLfile *cf = clsystem->getfile("../dat/other/atcl.y3d");
	CLobject* atcl_obj = new CLobject(cf,0);
	//*
	
	//load animation file
	CLfile* aniraw = clsystem->getfile("../dat/other/atcl.ani");
	//*
	
	//create animation
	CLanim* atcl_intro = new CLanim(atcl_obj,aniraw,0);
	//*
	
	//run animation
	while(clwindow->run())
	{
		if(clwindow->getinkey()==ESC) break;
		if(atcl_intro->run()==0) break;
	}
	//*
}

void CLintro::xizero()
{

}

#endif
