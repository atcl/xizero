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
	
	void circleblend(xlong x,xlong y,xlong r);
	void dissolve();
	void fadetoblack();
};


void CLtransitions::circleblend(xlong x,xlong y,xlong r)
{
	while(win->run())
	{
		//draw circle with radius r around (x,y)
		//fill surface around circle with black
	}
}

void CLtransitions::dissolve()
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong i = 0;
	
	while(win->run() && i<400)
	{
		for(uxlong j=0; j<1000; j++)
		{
			rx = CLutils::getrandom(800);
			ry = CLutils::getrandom(600);
			CLgfx1::drawbigpixel(rx,ry,0x00FFFFFF);
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
