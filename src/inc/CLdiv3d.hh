//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDIV3D
#define HH_CLDIV3D
#warning "Compiling " __FILE__ " ! TODO: draw3dpixel"

#include "CLtypes.hh"
#include "CLbuffer.hh"

//prototypes:
void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db);
void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa);
void drawfloor(xlong z, xlong w,uxlong c,CLbuffer<float>* zb,CLbuffer<xlong>* db);
template<class clvector>CLpoint project(clvector p);
void drawzbuffer(CLbuffer<float>* zb,CLbuffer<xlong>* db);
//


void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db)
{
	if(x>0 && x<xres && y>0 && y<yres && z>0 && z<zres)
	{
		xlong nx = xlong( ( 80 * x) / z) + xres>>1; //95 wenn xclipping läuft
		xlong ny = xlong( (-95 * y) / z) + yres>>1;

		(*db)[(ny*xres)+nx] = color;
	}
}

void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa)
{

}

void drawfloor(xlong z, xlong w,uxlong c,CLbuffer<float>* zb,CLbuffer<xlong>* db)
{
	//determine shade

	//draw screen
	xlong x1 = (xres-w)>>1;
	xlong x2 = xres-((xres-w)>>1);
	
	//draw filled rectangle and fill zbuffer
	for(int i=0; i<(x2-x1); i++)
	{
		for(int j=0; j<yres; j++)
		{
			(*db)[(j*xres)+(x1+i)] = c;
			(*zb)[(j*xres)+(x1+i)] = z;
		}
	}
} 

void drawzbuffer(CLbuffer<float>* zb,CLbuffer<xlong>* db)
{
	float z;

	for(int i=0; i<yres-1;i++)
	{
		for(int j=0; j<xres-1; j++)
		{
			z = (*zb)[(i*xres)+j] * 4;
			(*db)[(i*xres)+j] = xshort(z)<<8;
		}
	}
}

#endif
