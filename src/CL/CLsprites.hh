//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSPRITES
#define HH_CLSPRITES
//#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"


namespace CLsprites
{
	void drawantimatter(xlong x,xlong y);
	void drawplasma(xlong x,xlong y);
};

void CLsprites::drawantimatter(xlong x,xlong y)
{
if( (x-2)<0 || (x+3)>=xres || (y-2)<0 || (y+3)>=yres) return;
	
	xlong l1 = ((y-2)*xres)+(x-1);
	xlong l2 = l1 + xres - 1;
	xlong l3 = l2 + xres;
	xlong l4 = l3 + xres;
	xlong l5 = l4 + xres;
	xlong l6 = l5 + xres + 1; 
	
	(*CLdoublebuffer)[l1] = 0x00800080; l1++;
	(*CLdoublebuffer)[l1] = 0x00800080; l1++;
	(*CLdoublebuffer)[l1] = 0x00800080; l1++;
	(*CLdoublebuffer)[l1] = 0x00800080;
	
	(*CLdoublebuffer)[l2] = 0x00800080; l2++;
	(*CLdoublebuffer)[l2] = 0x00AD00AD; l2++;
	(*CLdoublebuffer)[l2] = 0x00AD00AD; l2++;
	(*CLdoublebuffer)[l2] = 0x00AD00AD; l2++;
	(*CLdoublebuffer)[l2] = 0x00AD00AD; l2++;
	(*CLdoublebuffer)[l2] = 0x00800080;
	
	(*CLdoublebuffer)[l3] = 0x00800080; l3++;
	(*CLdoublebuffer)[l3] = 0x00AD00AD; l3++;
	(*CLdoublebuffer)[l3] = 0x00FF00FF; l3++;
	(*CLdoublebuffer)[l3] = 0x00FF00FF; l3++;
	(*CLdoublebuffer)[l3] = 0x00AD00AD; l3++;
	(*CLdoublebuffer)[l3] = 0x00800080;
	
	(*CLdoublebuffer)[l4] = 0x00800080; l4++;
	(*CLdoublebuffer)[l4] = 0x00AD00AD; l4++;
	(*CLdoublebuffer)[l4] = 0x00FF00FF; l4++;
	(*CLdoublebuffer)[l4] = 0x00FF00FF; l4++;
	(*CLdoublebuffer)[l4] = 0x00AD00AD; l4++;
	(*CLdoublebuffer)[l4] = 0x00800080;
	
	(*CLdoublebuffer)[l5] = 0x00800080; l5++;
	(*CLdoublebuffer)[l5] = 0x00AD00AD; l5++;
	(*CLdoublebuffer)[l5] = 0x00AD00AD; l5++;
	(*CLdoublebuffer)[l5] = 0x00AD00AD; l5++;
	(*CLdoublebuffer)[l5] = 0x00AD00AD; l5++;
	(*CLdoublebuffer)[l5] = 0x00800080;
	
	(*CLdoublebuffer)[l6] = 0x00800080; l6++;
	(*CLdoublebuffer)[l6] = 0x00800080; l6++;
	(*CLdoublebuffer)[l6] = 0x00800080; l6++;
	(*CLdoublebuffer)[l6] = 0x00800080;
}

void CLsprites::drawplasma(xlong x,xlong y)
{
	if( (x-2)<0 || (x+3)>=xres || (y-2)<0 || (y+3)>=yres) return;
	
	xlong l1 = ((y-2)*xres)+(x-1);
	xlong l2 = l1 + xres - 1;
	xlong l3 = l2 + xres;
	xlong l4 = l3 + xres;
	xlong l5 = l4 + xres;
	xlong l6 = l5 + xres + 1; 
	
	(*CLdoublebuffer)[l1] = 0x00FF0000; l1++;
	(*CLdoublebuffer)[l1] = 0x00FF0000; l1++;
	(*CLdoublebuffer)[l1] = 0x00FF0000; l1++;
	(*CLdoublebuffer)[l1] = 0x00FF0000;
	
	(*CLdoublebuffer)[l2] = 0x00FF0000; l2++;
	(*CLdoublebuffer)[l2] = 0x00800000; l2++;
	(*CLdoublebuffer)[l2] = 0x00800000; l2++;
	(*CLdoublebuffer)[l2] = 0x00800000; l2++;
	(*CLdoublebuffer)[l2] = 0x00800000; l2++;
	(*CLdoublebuffer)[l2] = 0x00FF0000;
	
	(*CLdoublebuffer)[l3] = 0x00FF0000; l3++;
	(*CLdoublebuffer)[l3] = 0x00800000; l3++;
	(*CLdoublebuffer)[l3] = 0x00800000; l3++;
	(*CLdoublebuffer)[l3] = 0x00800000; l3++;
	(*CLdoublebuffer)[l3] = 0x00800000; l3++;
	(*CLdoublebuffer)[l3] = 0x00FF0000;
	
	(*CLdoublebuffer)[l4] = 0x00FF0000; l4++;
	(*CLdoublebuffer)[l4] = 0x00800000; l4++;
	(*CLdoublebuffer)[l4] = 0x00800000; l4++;
	(*CLdoublebuffer)[l4] = 0x00800000; l4++;
	(*CLdoublebuffer)[l4] = 0x00800000; l4++;
	(*CLdoublebuffer)[l4] = 0x00FF0000;
	
	(*CLdoublebuffer)[l5] = 0x00FF0000; l5++;
	(*CLdoublebuffer)[l5] = 0x00800000; l5++;
	(*CLdoublebuffer)[l5] = 0x00800000; l5++;
	(*CLdoublebuffer)[l5] = 0x00800000; l5++;
	(*CLdoublebuffer)[l5] = 0x00800000; l5++;
	(*CLdoublebuffer)[l5] = 0x00FF0000;
	
	(*CLdoublebuffer)[l6] = 0x00FF0000; l6++;
	(*CLdoublebuffer)[l6] = 0x00FF0000; l6++;
	(*CLdoublebuffer)[l6] = 0x00FF0000; l6++;
	(*CLdoublebuffer)[l6] = 0x00FF0000;
}

#endif

