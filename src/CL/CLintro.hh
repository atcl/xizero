///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLINTRO
#define HH_CLINTRO
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLsystem.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLformat.hh"
#include "CLgfx.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLintro
 * 
 * description:	Holds atcrosslevel and xizero intro animations
 * 
 * author:		atcl
 * 
 * notes:		implement torus animation
 * 
 * version: 	0.2
 */
///*

///definitions
class CLintro : public CLbase<CLintro,1>
{
	friend class CLbase<CLintro,1>;
	friend class CLglobal;
	
	private:
		CLgfx&    clgfx;
		CLwindow& clwindow;
		CLsystem& clsystem;
		CLscreen& clscreen;
	protected:
		CLintro();
		~CLintro() { };
	public:
		sprite* atcrosslevel(CLfile* f,bool s=0) const;
		sprite* xizero(CLfile* f,bool s=0) const;
		sprite* torus(CLfile* f,bool s=0) const;
};
///*

///implementation
CLintro::CLintro() //! noncritical
: clgfx(CLgfx::instance()), clwindow(CLwindow::instance()), clsystem(CLsystem::instance()), clscreen(CLscreen::instance())
{ }

sprite* CLintro::atcrosslevel(CLfile* f,bool s) const //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(f);
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
	xlong w = clgfx.getfontstringwidth(title,4);
	xlong x = (XRES - w)/2;
	//*
	
	//run animation
	bool skip = 0;
	while(clwindow.run())
	{
		if(clwindow.getinkey()==SPACE) { skip = 1; break; }
		if(atcl_intro->run()==0) { break; }
	}
	
	if(skip==0)
	{
		clgfx.drawfontstring(x,100,title,4,0x00800000);
		clwindow.run();
		clwindow.sleep(4000);
	}
	//*
	
	if(s!=0) { clgfx.savescreen(); } else { return 0; }
}

sprite* CLintro::xizero(CLfile* f,bool s) const //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(f);
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
	while(clwindow.run())
	{
		//clear buffers
		clscreen.cldoublebuffer.clear(0);
		clscreen.clzbuffer.clear(ZRES);
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
	
	if(s!=0) { clgfx.savescreen(); } else { return 0; }
}

sprite* CLintro::torus(CLfile* f,bool s) const
{
	//build torus
	
	//*
	
	//setup animation
	
	//*
	
	//play animation
	
	//*
}
///*

#endif
