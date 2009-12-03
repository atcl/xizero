///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMAINMENU
#define HH_CLMAINMENU
///*

///includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLconsts.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
///*

///header
/* class name:	CLmainmenu
 * 
 * description:	This class creates a typical game main menu
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLmainmenu : public virtual CLcl
{
	private:
		sprite* background;
		CLbutton** button;
		xlong buttoncount;
		CLlabel* header;
		CLlabel* footer;
	public:
		CLmainmenu(const xchar* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc);
		CLmainmenu(CLfile* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc);
		CLmainmenu(sprite* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc);
		~CLmainmenu();
		void draw() const;
		void setback(sprite* s);
};
///*

///implementation
//~ CLmainmenu::CLmainmenu(const xchar* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc)
//~ { CLmainmenu(clsystem->getfile(bg),bc,bt,a,hl,fl,fc); }
//~ 
//~ CLmainmenu::CLmainmenu(CLfile* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc)
//~ { CLmainmenu(clformat->loadtga(bg),bc,bt,a,hl,fl,fc); }

CLmainmenu::CLmainmenu(sprite* bg,xlong bc,const xchar* bt[],void (*a[])(),const xchar* hl,const xchar* fl,uxlong fc) //! noncritical
{
	//set attributes
	background = bg;
	buttoncount = bc;
	//*
	
	xlong temp = 0;
	
	//determine max button caption length
	xlong mbl = clutils->chararraylength(bt[0]);
	xlong mbli = 0;
	for(uxlong i=1; i<bc; i++)
	{
		temp = clutils->chararraylength(bt[i]);
		if(temp>mbl) { mbli = i; mbl = temp; }
	}
	//*
	
	//determine button positions
	temp = clgfx2->getfontstringwidth(bt[mbli],0);
	xlong bw = temp + 4;
	if(bw<XRES>>2) bw = XRES>>2;
	xlong bx = (XRES>>1) - (bw>>1);
	xlong* by = new xlong[bc];
	xlong bh = ((YRES - (YRES>>2))/bc);
	for(uxlong j=0; j<bc; j++)
	{
		by[j] = (YRES>>2) + j*bh;
	}
	bh -= 8;
	//*
	
	//determine header position
	temp = clgfx2->getfontstringwidth(hl,0) + 4;
	xlong hx = (XRES>>1) - (temp>>1);
	xlong hw = temp;
	xlong hy = 20;
	//*
	
	//determine footer position
	temp = clgfx2->getfontstringwidth(fl,0) + 4;
	xlong fx = XRES - temp - 16;
	xlong fw = temp;
	xlong fh = clgfx2->getfontstringheight(fl,0) + 4;
	xlong fy = YRES - fh; 
	//*
	
	button = new CLbutton*[bc]; 
	for(uxlong k=0; k<bc; k++)
	{
		button[k] = new CLbutton(bx,by[k],bw,bh,0x00FFFFFF,0x00808080,0x00808080,a[k],bt[k],0);
	}
	
	header = new CLlabel(hx,hy,hw,-1,0x00FF0000,0xFF000000,0xFF000000,hl,0);
	footer = new CLlabel(fx,fy,fw,fh,0x00FFFFFF,0xFF000000,0xFF000000,fl,0);
}
		
CLmainmenu::~CLmainmenu()
{
	//todo
}

void CLmainmenu::draw() const //! critical
{
	clgfx1->drawscreen(background);
	header->draw();
	for(uxlong i=0; i<buttoncount; i++) { button[i]->draw(); }
	footer->draw();
}

void CLmainmenu::setback(sprite* s) { background = s; } //! noncritical
///*

#endif
