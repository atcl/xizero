//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLDIV3D
#define HH_CLDIV3D
#pragma message "Compiling " __FILE__ " ! TODO: draw3dpixel"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLbuffer.hh"
#include "CLvector.hh"

namespace CLmisc3d
{
	void draw3dpixel(xlong x,xlong y,xlong z,uxlong color,CLbuffer<xlong>* db);
	void draw3dline(xlong x1,xlong y1,xlong z1,xlong x2,xlong y2,xlong z2,uxlong color,CLbuffer<xlong>* db,bool aa);
	void drawfloor(xlong z, xlong w,uxlong c,CLbuffer<float>* zb,CLbuffer<xlong>* db);
	template<class clvector>clvector project(const clvector& v,const clvector& p);
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
	//!!!determine shade of floor!!!

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

template<class clvector>
clvector CLmisc3d::project(clvector v,clvector p)
{
	//!!! test project, and may be get rid of one parameter!!!
	clvector r;
	
	if(v.z > 0)
	{
		//instead of xlong conversion rather use CLmath::round?
		r.x = float(xlong( ( prjx * v.x) / (v.z) ) + p.x );
		r.y = float(xlong( (-prjy * v.y) / (v.z) ) + p.y );
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
			z = (*zb)[(i*xres)+j+srcdis] *4;
			(*db)[(i*xres)+j] = z;
		}
	}
}

void CLmisc3d::drawlight()
{
	
}

#endif
