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
 * author:	atcl
 * 
 * notes:	implement xizero intro
 * 
 * version: 0.1
 */
///*

///definitions
class CLintro : public CLbase<CLintro,1>
{
	friend class CLbase<CLintro,1>;
	
	private:
		static CLgfx*    clgfx;
		static CLwindow* clwindow;
		static CLsystem* clsystem;
		static CLscreen* clscreen;
	protected:
		CLintro() { };
		~CLintro() { };
		static CLfile* source;
		static void atcrosslevel();
		static void xizero();
		static void torus();
	public:
		void intro(xlong t,CLfile* f) const;
};

CLgfx *   CLintro::clgfx    = CLgfx::instance();
CLwindow* CLintro::clwindow = CLwindow::instance();
CLsystem* CLintro::clsystem = CLsystem::instance();
CLscreen* CLintro::clscreen = CLscreen::instance();

CLfile* CLintro::source = 0;
///*

///implementation
void CLintro::atcrosslevel() //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(source);
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
	xlong w = clgfx->getfontstringwidth(title,4);
	xlong x = (XRES - w)/2;
	//*
	
	//run animation
	bool skip = 0;
	while(true)
	{
		if(clwindow->getinkey()==SPACE) { skip = 1; break; }
		if(atcl_intro->run()==0) { break; }
		clwindow->draw();
	}
	
	if(skip==0)
	{
		clgfx->drawfontstring(x,100,title,4,0x00800000);
		clwindow->draw();
		clwindow->sleep(4000);
	}
	//*
}

void CLintro::xizero() //! noncritical
{
	//load atcrosslevel model
	CLar* introar = new CLar(source);
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
	while(true)
	{
		//clear buffers
		clscreen->cldoublebuffer.clear(0);
		clscreen->clzbuffer.clear(ZRES);
		//*
		
		//update animation
		r  = ixi->update() + izero->update();
		//*
		
		//display object
		oxi->display(ixi->getposition(),SHAPE + AMBIENT + ZLIGHT);
		ozero->display(izero->getposition(),SHAPE + AMBIENT + ZLIGHT);
		//*
		
		clwindow->draw();
		if(r==0) break;
	}
	//*
}

void CLintro::torus()
{
	//build torus
	
	//*
	
	//setup animation
	
	//*
	
	//play animation
	
	//*
}

void CLintro::intro(xlong t,CLfile* f) const
{
	source = f;
	
	switch(t)
	{
		case 0: clwindow->setdisplay(atcrosslevel); break;
		case 1: clwindow->setdisplay(xizero); break;
		case 2: clwindow->setdisplay(torus); break;
		default: return; break;
	}
}
///*

#endif
