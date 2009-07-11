//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDIV3D
#define HH_CLDIV3D
#pragma message "Compiling " __FILE__ " ! TODO: draw3dpixel"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLbuffer.hh"
#include "CLvector.hh"
#include "CLglobal.hh"

namespace CLmisc3d
{
	void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db);
	void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa);
	void drawfloor(xlong z, xlong w,uxlong c,CLbuffer<float>* zb,CLbuffer<xlong>* db);
	template<class clvector>CLlvector project(const clvector& v);
	void drawzbuffer(CLbuffer<float>* zb,CLbuffer<xlong>* db,xlong srcdis=0);
	void drawlight();
}


void CLmisc3d::draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db)
{
	if(x>0 && x<xres && y>0 && y<yres && z>0 && z<zres)
	{
		xlong nx = xlong( ( 80 * x) / z) + xres>>1; //95 wenn xclipping läuft
		xlong ny = xlong( (-95 * y) / z) + yres>>1;

		(*db)[(ny*xres)+nx] = color;
	}
}

void CLmisc3d::draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa)
{

}

void CLmisc3d::drawfloor(xlong z, xlong w,uxlong c,CLbuffer<float>* zb,CLbuffer<xlong>* db)
{
	//shade floor
	doubleword argb = { 0 };
	uxlong s;

	float t = CLmath::absolute((clplane * cllight) / ( !clplane * !cllight ));

	//if(t > 1) t = 1;
	//if(t < 0.2) s = nolight;

	argb.dd = c;
	argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t);
	argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t);
	argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t);
	s = argb.dd;
	//
	
	
	//!!!optimize up the for-loops !!! causes 60fps drop!!!!!!!!! 

	//draw screen
	xlong x1 = (xres-w)>>1;
	xlong x2 = xres-((xres-w)>>1);
	
	
	
	//draw filled rectangle and fill zbuffer
	for(int i=(x2-x1)-1; i>=0; i--)
	{
		for(int j=yres-1; j>=0; j--)
		{
			(*db)[(j*xres)+(x1+i)] = s;
			(*zb)[(j*xres)+(x1+i)] = z;
		}
	}
}

template<class clvector>
CLlvector CLmisc3d::project(const clvector& v)
{
	//!!! test project, and may be get rid of one parameter!!!
	CLlvector r;
	
	if(v.z > 0)
	{
		//instead of xlong conversion rather use CLmath::round?
		r.x = xlong( (prjx / v.z) * (v.x - 400)  ) +400;
		r.y = xlong( (prjy / v.z) * (v.y - 300)  ) +300;
		r.z = v.z; // + cleartrans;
	}
	
	return r;
}

void CLmisc3d::drawzbuffer(CLbuffer<float>* zb,CLbuffer<xlong>* db,xlong srcdis)
{
	float z;

	for(int i=0; i<yres-1;i++)
	{
		for(int j=0; j<xres-1; j++)
		{
			z = (*zb)[(i*xres)+j+srcdis] * 4;
			(*db)[(i*xres)+j] = z;
		}
	}
}

void CLmisc3d::drawlight()
{
	
}

#endif
