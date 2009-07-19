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
#include "CLgfx1.hh"


namespace CLmisc3d
{
	template<class clvector>CLlvector project(const clvector& v);
	template<class clvector>void draw3dpixel(clvector& p,uxlong c);
	template<class clvector>void draw3dline(clvector& p,clvector& q,uxlong c,bool aa);
	template<class clvector>void drawlight(clvector& p,xlong i,uxlong c);
	void drawzbuffer(CLfbuffer* zb=0,xlong srcdis=0);
	void drawfloor(xlong z, xlong w,uxlong c);
};


template<class clvector>
CLlvector CLmisc3d::project(const clvector& v)
{
	CLlvector r;
	
	if(v.z > 0)
	{
		r.x = xlong( (prjx / v.z) * (v.x - 400)  ) +400;
		r.y = xlong( (prjy / v.z) * (v.y - 300)  ) +300;
		r.z = v.z; // + cleartrans;
	}
	
	return r;
}

template<class clvector>
void CLmisc3d::draw3dpixel(clvector p,uxlong c)
{
	if(p.x>0 && p.x<xres && p.y>0 && p.y<yres && p.z>0 && p.z<zres)
	{
		xlong nx = xlong( ( 80 * p.x) / p.z) + (xres>>1);
		xlong ny = xlong( (-95 * p.y) / p.z) + (yres>>1);

		(*CLdoublebuffer)[(ny*xres)+nx] = c;
	}
}

template<class clvector>
void CLmisc3d::draw3dline(clvector p,clvector q,uxlong c,bool aa)
{
	if(p.z>0 && p.z<zres && q.z >0 && q.z<zres)
	{
		xlong nx = xlong( ( 80 * p.x) / p.z) + (xres>>1);
		xlong ny = xlong( (-95 * p.y) / p.z) + (yres>>1);
		
		xlong ox = xlong( ( 80 * q.x) / q.z) + (xres>>1);
		xlong oy = xlong( (-95 * q.y) / q.z) + (yres>>1);

		if(aa) CLgfx1::drawantiline(nx,ny,ox,oy,c);
		else   CLgfx1::drawanyline(nx,ny,ox,oy,c);
	}
}

template<class clvector>
void CLmisc3d::drawlight(clvector& p,xlong i,uxlong c)
{
	
}

void CLmisc3d::drawzbuffer(CLfbuffer* zb,xlong srcdis)
{
	float z;
	
	if(zb==0) zb = CLzbuffer;

	for(int i=0; i<yres-1;i++)
	{
		for(int j=0; j<xres-1; j++)
		{
			z = (*zb)[(i*xres)+j+srcdis] * 4;
			(*CLdoublebuffer)[(i*xres)+j] = z;
		}
	}
}

void CLmisc3d::drawfloor(xlong z, xlong w,uxlong c)
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
			(*CLdoublebuffer)[(j*xres)+(x1+i)] = s;
			(*CLzbuffer)[(j*xres)+(x1+i)] = z;
		}
	}
}


#endif
