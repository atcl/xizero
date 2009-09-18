//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMISC3D
#define HH_CLMISC3D
#pragma message "Compiling " __FILE__ " ! TODO: draw3dpixel"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLbuffer.hh"
#include "CLvector.hh"
#include "CLglobal.hh"

class CLmisc3d : public virtual CLcl, public CLsingle<CLmisc3d>
{
	friend class CLsingle<CLmisc3d>;
	
	private:
		CLmisc3d();
		~CLmisc3d();
	public:
		template<class clvector>void project(clvector& v,const clvector& p);
		template<class clvector>void project2(clvector& o);
		template<class clvector>void draw3dpixel(clvector& p,uxlong c);
		template<class clvector>void draw3dline(clvector& p,clvector& q,uxlong c,bool aa);
		template<class clvector>void drawlight(clvector& p,xlong i,uxlong c);
		void drawzbuffer(CLfbuffer* zb=0,xlong srcdis=0);
};

CLmisc3d::CLmisc3d() { }

CLmisc3d::~CLmisc3d() { }

template<class clvector>
void CLmisc3d::project(clvector& v,const clvector& p)
{
	if(v.z > 0)
	{
		v.x = xlong( ( prjx * (v.x / v.z ) ) + p.x );
		v.y = xlong( (-prjy * (v.y / v.z ) ) + p.y );
		v.z = v.z;
	}
	else
	{
		//CLsystem::exit(1,0,__func__,"Invalid z value: ",v.z);
	}
}

template<class clvector>
void CLmisc3d::project2(clvector& o)
{
	CLfvector temp;
	
	temp.x = o.x - (xres>>1);
	temp.y = -o.y - (yres>>1);
	temp.z = o.z;
	
	if(o.z > 0)
	{
		o.x = xlong( ( prjx * (temp.x / temp.z ) ) + (xres>>1) ); //test: (prjx * (xres>>1) / o.z)
		o.y = xlong( ( prjy * (temp.y / temp.z ) ) + (yres>>1) ); //test: (prjy * (yres>>1) / o.z)
		o.z = o.z;
	}
	else
	{
		//CLsystem::exit(1,0,__func__,"Invalid z value: ",v.z);
	}
}

template<class clvector>
void CLmisc3d::draw3dpixel(clvector& p,uxlong c)
{
	//if on screen project and draw a pixel 
	if(p.x>0 && p.x<xres && p.y>0 && p.y<yres && p.z>0 && p.z<zres)
	{
		xlong nx = xlong( ( 80 * p.x) / p.z) + (xres>>1);
		xlong ny = xlong( (-95 * p.y) / p.z) + (yres>>1);

		(*CLdoublebuffer)[(ny*xres)+nx] = c;
	}
	//*
}

template<class clvector>
void CLmisc3d::draw3dline(clvector& p,clvector& q,uxlong c,bool aa)
{
	//if on screen project and draw a line  
	if(p.z>0 && p.z<zres && q.z >0 && q.z<zres)
	{
		xlong nx = xlong( ( 80 * p.x) / p.z) + (xres>>1);
		xlong ny = xlong( (-95 * p.y) / p.z) + (yres>>1);
		xlong ox = xlong( ( 80 * q.x) / q.z) + (xres>>1);
		xlong oy = xlong( (-95 * q.y) / q.z) + (yres>>1);
		if(aa) CLgfx1::drawantiline(nx,ny,ox,oy,c);
		else   CLgfx1::drawanyline(nx,ny,ox,oy,c);
	}
	//*
}

template<class clvector>
void CLmisc3d::drawlight(clvector& p,xlong i,uxlong c)
{
	
}

void CLmisc3d::drawzbuffer(CLfbuffer* zb,xlong srcdis)
{
	xlong z = 0;
	
	if(zb==0) zb = CLzbuffer;

	xlong ii = 0;
	xlong tt = 0;

	for(uxlong i=0; i<yres-1;i++)
	{
		for(uxlong j=0; j<xres-1; j++)
		{
			tt = ii + j;
			z = xlong((*zb)[tt+srcdis])<<2 ;
			(*CLdoublebuffer)[tt] = z;
		}
		
		ii +=xres;
	}
}

#endif
