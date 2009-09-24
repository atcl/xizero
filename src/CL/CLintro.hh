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

class CLintro : public virtual CLcl, public CLsingle<CLintro> 
{
	friend class CLsingle<CLintro>;
	
	private:
		CLintro() { };
		~CLintro() { };
	public:
		void atcrosslevel() const;
		void xizero() const;
};

void CLintro::atcrosslevel() const
{
	//load atcrosslevel model
	CLfile *cf = clsystem->getfile("../dat/intro/atcl.y3d");
	CLobject* atcl_obj = new CLobject(cf,0);
	//*
	
	//load animation file
	CLfile* aniraw = clsystem->getfile("../dat/intro/atcl.ani");
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

void CLintro::xizero() const
{
	//load atcrosslevel model
	CLfile* fxi = clsystem->getfile("../dat/intro/xi.y3d");
	CLobject* oxi = new CLobject(fxi,0);
	CLfile* fzero = clsystem->getfile("../dat/intro/zero.y3d");
	CLobject* ozero = new CLobject(fzero,0);
	//*
	
	//load animation file
	CLfile* axi = clsystem->getfile("../dat/intro/xi.ani");
	CLfile* azero = clsystem->getfile("../dat/intro/zero.ani");
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
		if(clwindow->getinkey()==ESC) break;
	
		//clear buffers
		cldoublebuffer.clear(0);
		clzbuffer.clear(zres);
		clstencilbuffer.clear(0);
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
