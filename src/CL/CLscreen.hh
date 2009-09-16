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

namespace CLtransitions
{
	xchar fade_const;
	xlong diss_const;
	xlong circ_const;
	
	void circleblend(xlong x,xlong y,xlong r,xlong t);
	void dissolve();
	void fadetoblack();
};


void CLtransitions::circleblend(xlong x,xlong y,xlong r,xlong t)
{
	bool secondtime = 0;
	
	while(win->run())
	{
		if(secondtime) { CLsystem::wait(t); break; }
		CLgfx1::drawcircle(x,y,r,0x00FFFFFF);
		CLgfx1::fill(5,30,0,0x00FFFFFF);
		secondtime = 1;
	}
}

void CLtransitions::dissolve()
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong c = 0;
	uxlong i = 0;
	
	while(win->run() && i<200)
	{
		for(uxlong j=0; j<1000; j++)
		{
			rx = CLutils::getrandom(800);
			ry = CLutils::getrandom(600);
			c = CLutils::getrandom(-1);
			CLgfx1::drawbigpixel(rx,ry,c); //alternating colors?
		}
		i++;
		CLsystem::wait(10);
	}
}

void CLtransitions::fadetoblack()
{
	uxchar comp[4] = { 0,0,0,0 };
	uxlong i = 0;
	
	while(win->run() && i<256)
	{		
		for(uxlong j=0; j<scrs; j++)
		{
			CLutils::long2char((*CLdoublebuffer)[j],comp[0],comp[1],comp[2],comp[3]);
			if(comp[0] > 0) comp[0]--;
			if(comp[1] > 0) comp[1]--;
			if(comp[2] > 0) comp[2]--;
			if(comp[3] > 0) comp[3]--;
			(*CLdoublebuffer)[j] = CLutils::chars2long(comp[0],comp[1],comp[2],comp[3]);
		}
		i++;
		CLsystem::wait(5);
	}
}

#endif
