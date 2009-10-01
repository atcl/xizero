//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#pragma message Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLglobal.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
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

class CLintro : public virtual CLcl
{
	friend class CLsingle<CLintro>;
	
	private:
		CLar* introar;
	public:
		CLintro(CLfile* sf) { introar = new CLar(sf); };
		~CLintro() { };
		void atcrosslevel() const;
		void xizero() const;
};

void CLintro::atcrosslevel() const
{
	//load atcrosslevel model
	CLfile *cf = introar->findbyname("atcl.y3d");
	CLobject* atcl_obj = new CLobject(cf,0,0);
	//*
	
	//load animation file
	CLfile* aniraw = introar->findbyname("atcl.ani");
	//*
	
	//create animation
	CLanim* atcl_intro = new CLanim(atcl_obj,aniraw,0);
	//*
	
	//run animation
	while(clwindow->run())
	{
		if(clwindow->getinkey()==SPACE) break;
		if(atcl_intro->run()==0) break;
	}
	//*
}

void CLintro::xizero() const
{
	//load atcrosslevel model
	CLfile* fxi = introar->findbyname("xi.y3d");
	CLobject* oxi = new CLobject(fxi,0,0);
	CLfile* fzero = introar->findbyname("zero.y3d");
	CLobject* ozero = new CLobject(fzero,0,0);
	//*
	
	//load animation file
	CLfile* axi = introar->findbyname("xi.ani");
	CLfile* azero = introar->findbyname("zero.ani");
	//*
	
	//create animation
	CLanim* ixi = new CLanim(oxi,axi,0);
	CLanim* izero = new CLanim(ozero,azero,0);
	//*
	
	//run animation
	xlong r = 0;
	CLlvector position;
	position = 0;
	while(clwindow->run())
	{
		//clear buffers
		cldoublebuffer.clear(0);
		clzbuffer.clear(ZRES);
		//*
		
		//update animation
		r  = ixi->update() + izero->update();
		//*
		
		//display object
		oxi->display(ixi->getposition(),SHAPE + AMBIENT + ZLIGHT);
		ozero->display(izero->getposition(),SHAPE + AMBIENT + ZLIGHT);
		//*
		
		if(r==0) break;
	}
	//*
}

#endif
