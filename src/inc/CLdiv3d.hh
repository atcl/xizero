//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDIV3D
#define HH_CLDIV3D
#pragma message "Compiling " __FILE__ " ! TODO: draw3dpixel"

#include "CLtypes.hh"
#include "CLbuffer.hh"


//prototypes:
void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db);
void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa);
void drawfloor(xlong z, xlong w=xres);
//


void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db)
{
	if(x>0 && x<xres && y>0 && y<yres && z>0 && z<zres)
	{
		xlong nx = (xlong( ( 80 * x) / z) ) + xres>>1); //95 wenn xclipping läuft
		xlong ny = (xlong( (-95 * y) / z) ) + yres>>1);

		(*db)[(ny*xres)+nx] = color;
	}
}

void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa)
{

}

void drawfloor(xlong z, xlong w)
{
	//determine shade

	//draw screen
	xlong x1 = 0;
	xlong y1 = 0;
	xlong x2 = (xlong( ( 80 * (xres-1)) / z) ) + xres>>1);
	xlong y2 = (xlong( (-95 * (yres-1)) / z) ) + yres>>1);
	//draw filled rectangle
} 

#endif
