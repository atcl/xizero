//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSPRITES
#define HH_CLSPRITES
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"

/* class name:	CLsprites
 * 
 * description:	Draws compiled sprites
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

class CLsprites : public virtual CLcl, public CLsingle<CLsprites>
{
	friend class CLsingle<CLsprites>;
	
	private:
		CLsprites() { };
		~CLsprites() { };
	public:
		static void drawantimatter(xlong x,xlong y);
		static void drawplasma(xlong x,xlong y);
};

void CLsprites::drawantimatter(xlong x,xlong y)
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) return;
	//*
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw frst row
	cldoublebuffer[l1] = 0x00800080; l1++;
	cldoublebuffer[l1] = 0x00800080; l1++;
	cldoublebuffer[l1] = 0x00800080; l1++;
	cldoublebuffer[l1] = 0x00800080;
	//*
	
	//draw second row
	cldoublebuffer[l2] = 0x00800080; l2++;
	cldoublebuffer[l2] = 0x00AD00AD; l2++;
	cldoublebuffer[l2] = 0x00AD00AD; l2++;
	cldoublebuffer[l2] = 0x00AD00AD; l2++;
	cldoublebuffer[l2] = 0x00AD00AD; l2++;
	cldoublebuffer[l2] = 0x00800080;
	//*
	
	//draw third row
	cldoublebuffer[l3] = 0x00800080; l3++;
	cldoublebuffer[l3] = 0x00AD00AD; l3++;
	cldoublebuffer[l3] = 0x00FF00FF; l3++;
	cldoublebuffer[l3] = 0x00FF00FF; l3++;
	cldoublebuffer[l3] = 0x00AD00AD; l3++;
	cldoublebuffer[l3] = 0x00800080;
	//*
	
	//draw fourth row
	cldoublebuffer[l4] = 0x00800080; l4++;
	cldoublebuffer[l4] = 0x00AD00AD; l4++;
	cldoublebuffer[l4] = 0x00FF00FF; l4++;
	cldoublebuffer[l4] = 0x00FF00FF; l4++;
	cldoublebuffer[l4] = 0x00AD00AD; l4++;
	cldoublebuffer[l4] = 0x00800080;
	//*
	
	//draw fifth row
	cldoublebuffer[l5] = 0x00800080; l5++;
	cldoublebuffer[l5] = 0x00AD00AD; l5++;
	cldoublebuffer[l5] = 0x00AD00AD; l5++;
	cldoublebuffer[l5] = 0x00AD00AD; l5++;
	cldoublebuffer[l5] = 0x00AD00AD; l5++;
	cldoublebuffer[l5] = 0x00800080;
	//*
	
	//draw sixth row
	cldoublebuffer[l6] = 0x00800080; l6++;
	cldoublebuffer[l6] = 0x00800080; l6++;
	cldoublebuffer[l6] = 0x00800080; l6++;
	cldoublebuffer[l6] = 0x00800080;
	//*
}

void CLsprites::drawplasma(xlong x,xlong y)
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) return;
	//*
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw first row
	cldoublebuffer[l1] = 0x00FF0000; l1++;
	cldoublebuffer[l1] = 0x00FF0000; l1++;
	cldoublebuffer[l1] = 0x00FF0000; l1++;
	cldoublebuffer[l1] = 0x00FF0000;
	//*
	
	//draw second row
	cldoublebuffer[l2] = 0x00FF0000; l2++;
	cldoublebuffer[l2] = 0x00800000; l2++;
	cldoublebuffer[l2] = 0x00800000; l2++;
	cldoublebuffer[l2] = 0x00800000; l2++;
	cldoublebuffer[l2] = 0x00800000; l2++;
	cldoublebuffer[l2] = 0x00FF0000;
	//*
	
	//draw third row
	cldoublebuffer[l3] = 0x00FF0000; l3++;
	cldoublebuffer[l3] = 0x00800000; l3++;
	cldoublebuffer[l3] = 0x00800000; l3++;
	cldoublebuffer[l3] = 0x00800000; l3++;
	cldoublebuffer[l3] = 0x00800000; l3++;
	cldoublebuffer[l3] = 0x00FF0000;
	//*
	
	//draw fourth row
	cldoublebuffer[l4] = 0x00FF0000; l4++;
	cldoublebuffer[l4] = 0x00800000; l4++;
	cldoublebuffer[l4] = 0x00800000; l4++;
	cldoublebuffer[l4] = 0x00800000; l4++;
	cldoublebuffer[l4] = 0x00800000; l4++;
	cldoublebuffer[l4] = 0x00FF0000;
	//*
	
	//draw fifth row
	cldoublebuffer[l5] = 0x00FF0000; l5++;
	cldoublebuffer[l5] = 0x00800000; l5++;
	cldoublebuffer[l5] = 0x00800000; l5++;
	cldoublebuffer[l5] = 0x00800000; l5++;
	cldoublebuffer[l5] = 0x00800000; l5++;
	cldoublebuffer[l5] = 0x00FF0000;
	//*
	
	//draw sixth row
	cldoublebuffer[l6] = 0x00FF0000; l6++;
	cldoublebuffer[l6] = 0x00FF0000; l6++;
	cldoublebuffer[l6] = 0x00FF0000; l6++;
	cldoublebuffer[l6] = 0x00FF0000;
	//*
}

#endif

