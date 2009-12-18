///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLINTRO
#define HH_CLINTRO
///*

///includes
#include "CLtypes.hh"
#include "CLglobal.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLformat.hh"
#include "CLgfx2.hh"
///*

///header
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
///*

///definitions
class CLintro : public virtual CLcl, public CLsingle<CLintro>
{
	friend class CLsingle<CLintro>;
	
	private:
		CLintro() { };
		~CLintro() { };
	public:
		void atcrosslevel(CLfile* sf) const;
		void xizero(CLfile* sf) const;
};
///*

///implementation
void CLintro::atcrosslevel(CLfile* sf) const //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(sf);
	CLfile *cf = introar->findbyname(u8"atcl.y3d");
	CLobject* atcl_obj = new CLobject(cf,0);
	//*
	
	//load animation file
	CLfile* aniraw = introar->findbyname(u8"atcl.ani");
	//*
	
	//create animation
	CLanim* atcl_intro = new CLanim(atcl_obj,aniraw,0);
	//*
	
	//prepare name string
	const xchar* title = "atCROSSLEVEL";
	xlong w = clgfx2->getfontstringwidth(title,4);
	xlong x = (XRES - w)/2;
	//*
	
	//run animation
	bool skip = 0;
	while(clwindow->run())
	{
		if(clwindow->getinkey()==SPACE) { skip = 1; break; }
		if(atcl_intro->run()==0) { break; }
	}
	
	if(skip==0)
	{
		clgfx2->drawfontstring(x,100,title,4,0x00800000);
		clwindow->run();
		clsystem->wait(4000);
	}
	//*
}

void CLintro::xizero(CLfile* sf) const //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(sf);
	CLfile* fxi = introar->findbyname(u8"xi.y3d");
	CLobject* oxi = new CLobject(fxi,0);
	CLfile* fzero = introar->findbyname(u8"zero.y3d");
	CLobject* ozero = new CLobject(fzero,0);
	//*
	
	//load animation file
	CLfile* axi = introar->findbyname(u8"xi.ani");
	CLfile* azero = introar->findbyname(u8"zero.ani");
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
///*

#endif
