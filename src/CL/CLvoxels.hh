///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLVOXEL
#define HH_CLVOXEL
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLvector.hh"
///*

///header
/* class name:	CLvoxel
 * 
 * description:	fast voxel class
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
struct CLvoxel : CLfvector
{
	public:
		uxlong c;
		CLvoxel() : CLfvector() { c=0; }
		CLvoxel(float tx,float ty,float tz,float te=0,uxlong tc=0) : CLfvector(tx,ty,tz,te) { c=tc; } 
		~CLvoxel() { }
};

struct ngroup
{
	xlong voxels;
	CLvoxel* voxel;
	CLfvector normal;
};

class CLvobj : public CLbase<CLid,0>
{
	friend class CLbase<CLvobj,0>;
	
	private:
		CLscreen& clscreen;
		CLmath&   clmath;
	protected:
		static CLmatrix* shadow;
		CLmatrix linear;
		CLlvector position;
		xlong ngroups;
		uxlong shadowcolor;
		ngroup* n;
		inline uxlong project(CLlvector p,CLfvector v,bool c) const;
		inline uxlong shade(CLfvector n,uxlong c,bool ambient,bool zlight) const;
	public:
		CLvobj(CLfile* f,CLlvector p);
		~CLvobj() { delete n; }
		void display(CLlvector p,xshort flags);
		void update();
};

CLmatrix* CLvobj::shadow = 0;
///*

///implementation
uxlong CLvobj::project(CLlvector p,CLfvector v,bool c) const ///! noncritical
{
	if(c) { p.x = (XRES>>1); p.y = (YRES>>1); }

	v.x = xlong( ( PRJX * (v.x / v.z) ) + p.x );
	v.y = xlong( (-PRJY * (v.y / v.z) ) + p.y );
	return(v.y*XRES+v.x);
}

uxlong CLvobj::shade(CLfvector n,uxlong c,bool ambient,bool zlight) const ///! critical
{
	doubleword argb = { 0 };
	uxchar amb = 0;
	float t = clmath.absolute((n * clscreen.cllight)/(!n * !clscreen.cllight));
	if(t > 1.0) { t = 1; } //check if possible!?!?
	if(t < 0.1 && ambient==1) { amb = 32; }
	
	argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t) + amb;
	argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t) + amb;
	argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t) + amb;
	argb.db[3] = uxchar((float(uxchar(argb.db[3])))*t) + amb;
	
	return argb.dd;
}

CLvobj::CLvobj(CLfile* f,CLlvector p) ///! noncritical
: clscreen(CLscreen::instance()), clmath(CLmath::instance()), linear(1), position(p)
{
	if(shadow==0) { shadow = new CLmatrix(clscreen.clslight,clscreen.clplane); }
	linear = new CLmatrix(1);
	
	xlong* dataptr = f->data;
	xlong d = 0;
	
	if(dataptr[d] != '<CLV') { err(__FILE__,__func__,u8"wrong y3d format, may be endianess?"); }
	d++;
	
	if(dataptr[d] == 'X>10')
	{
		d++;
		ngroups = dataptr[d]; d++;
		n = new ngroup[ngroups];
		shadowcolor = dataptr[d]; d++;
		
		for(xlong i=0;i<ngroups;i++)
		{
			n[i].voxels = dataptr[d]; d++;
			n[i].voxel = new CLvoxel[n[i].voxels];
			n[i].normal.x = dataptr[d]; d++;
			n[i].normal.y = dataptr[d]; d++;
			n[i].normal.z = dataptr[d]; d++;
			
			for(xlong j=0;j<n[i].voxels;j++)
			{
				n[i].voxel[j].c = dataptr[d]; d++;
				n[i].voxel[j].x = dataptr[d]; d++;
				n[i].voxel[j].y = dataptr[d]; d++;
				n[i].voxel[j].z = dataptr[d]; d++;
			}
		}
	}
}

void CLvobj::display(CLlvector p,xshort flags) ///! critical
{
	CLfvector t;
	uxlong l = 0;
	uxlong s = 0;
	
	ngroup* g = 0;
	CLvoxel* v = 0;
	
	for(xlong i=0;i<ngroups;i++)
	{
		if(clscreen.clview * n[i].normal<0)
		{
			for(xlong j=0;j<n[i].voxels;j++)
			{
				t = n[i].voxel[j] + position;
				s = project(p,n[i].voxel[j],0);
				if(s>0 && s<clscreen.clpixelsize && clscreen.clzbuffer[s] < n[i].voxel[j].z)
				{
					clscreen.cldoublebuffer[s] = shade(n[i].normal,n[i].voxel[j].c,0,0);
					clscreen.clzbuffer[s] = n[i].voxel[j].z;
				}
			}
		}
	}
}

void CLvobj::update() ///! critical
{
	for(xlong i=0;i<ngroups;i++)
	{
		n[i].normal = linear.transform(n[i].normal);
		for(xlong j=0;j<n[i].voxels;j++)
		{
			n[i].voxel[j] = linear.transform(n[i].voxel[j]);
		}
	}
}
///*

#endif
