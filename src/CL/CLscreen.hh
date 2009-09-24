//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSCREEN
#define HH_CLSCREEN
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLutils.hh"
#include "CLgfx1.hh"
#include "CLwindow.hh"

/* class name:	CLtranstions
 * 
 * description:	some screen transitions
 * 
 * author:	atcl
 * 
 * notes:	finish implementing.
 * 
 * version: 0.1
 */

class CLtransitions : public virtual CLcl, public CLsingle<CLtransitions>
{
	friend class CLsingle<CLtransitions>;
	
	private:
		xchar fade_const;
		xlong diss_const;
		xlong circ_const;
		CLtransitions();
		~CLtransitions();
	public:
		void circleblend(xlong x,xlong y,xlong r,xlong t);
		void dissolve();
		void fadetoblack();
};

CLtransitions::CLtransitions() { }

CLtransitions::~CLtransitions() { }

void CLtransitions::circleblend(xlong x,xlong y,xlong r,xlong t)
{
	bool secondtime = 0;
	
	while(clwindow->run())
	{
		if(secondtime) { clsystem->wait(t); break; }
		clgfx1->drawcircle(x,y,r,0x00FFFFFF);
		clgfx1->fill(5,30,0,0x00FFFFFF);
		secondtime = 1;
	}
}

void CLtransitions::dissolve()
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong c = 0;
	uxlong i = 0;
	
	while(clwindow->run() && i<200)
	{
		for(uxlong j=0; j<1000; j++)
		{
			rx = clutils->getrandom(800);
			ry = clutils->getrandom(600);
			c = clutils->getrandom(-1);
			clgfx1->drawbigpixel(rx,ry,c);
		}
		i++;
		clsystem->wait(10);
	}
}

void CLtransitions::fadetoblack()
{
	uxchar comp[4] = { 0,0,0,0 };
	uxlong i = 0;
	
	while(clwindow->run() && i<256)
	{		
		for(uxlong j=0; j<SCRS; j++)
		{
			clutils->long2char(cldoublebuffer[j],comp[0],comp[1],comp[2],comp[3]);
			if(comp[0] > 0) comp[0]--;
			if(comp[1] > 0) comp[1]--;
			if(comp[2] > 0) comp[2]--;
			if(comp[3] > 0) comp[3]--;
			cldoublebuffer[j] = clutils->chars2long(comp[0],comp[1],comp[2],comp[3]);
		}
		i++;
		clsystem->wait(5);
	}
}

#endif
