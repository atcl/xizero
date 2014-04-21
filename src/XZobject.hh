///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Object Class ( XZobject.hh )
#pragma once 
///</header>

///<include>
#include "XZbasic.hh"
#include "XZformat.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZpolygon.hh"
#include "XZsystem.hh"
///</include>

///<define> 
class object
{
	private:
		polygon** poly;
		vector*   dock;		// 0:ammo1; 1:ammo2; 2:exhaust; 3:connector;
		fixed     bound;
		fixed     cbound;	//TODO
		xint      polys;
		xint      docks;
		yint      scolor;	// shadow color

		object&   operator=(const object& o);
	public:

		object(const char* o);
		object(vector* a,vector* b,vector* c,vector* d,xint x,xint e);
		object(const object& o);
		~object();
		vector* docktype(xint i,xint j) const;
		void update(const matrix& m=object::linear,bool j=1);
		void display(const vector& p,xint f) const;
		void pull(fixed x); //translate along normals
		// void rebound();
		inline fixed bounding() const { return cbound; }			// remove if possible
		inline bool collision(const vector& x,const vector& a);

		static matrix linear;
};
///</define>

///<code>
matrix object::linear = matrix();

object::object(const char* o) : poly(0),dock(0),bound(FXMON<<10),polys(0),docks(0),scolor(0)
{
	char** t = format::csv(o);
	xint i = 0;

	system::err(string::find(t[i++],"<y3dtxt>")==-1,"ERROR: y3d format wrong (head)");

	polys = string::str2int(t[i++]); 
	polys += 2*string::str2int(t[i++]); 
	docks = string::str2int(t[i++]);
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new vector[docks]; }

	vector bbox[4];

	system::err(string::find(t[i++],"objt")==-1,"ERROR: y3d format wrong (objt)");

	const xint subs = string::str2int(t[i++]);
	/*char* oid = t[i++];*/ i++;
	scolor = string::hex2int(t[i++]);

	xint pc = 0;
	xint dc = 0;
	for(xint j=0;j<subs;++j)
	{
		system::err(string::find(t[i++],"sobj")==-1,"ERROR: y3d format wrong (sobj)");

		const xint p = string::str2int(t[i++]);
		/*char* sid = t[i++];*/ i++;
		const xint d = string::str2int(t[i++]);

		system::err(string::find(t[i++],"posi")==-1,"ERROR: y3d format wrong (posi)");

		vector pos{string::str2int(t[i]),string::str2int(t[i+1]),string::str2int(t[i+2])}; i+=3;
		vector x[4];
		for(xint k=0;k<p;++k)
		{
			system::err(string::find(t[i++],"poly")==-1,"ERROR: y3d format wrong (poly)");

			const xint verts = string::str2int(t[i++]);
			/*char* pid = t[i++];*/ i++;
			const yint tcolor = string::hex2int(t[i++]);

			for(xint l=0;l<3+(verts==4);++l,i+=3)
			{
				system::err(string::find(t[i++],"vert")==-1,"ERROR: y3d format wrong (vert)");

				x[l] = vector{pos.x+string::str2int(t[i]),pos.y+string::str2int(t[i+1]),pos.z+string::str2int(t[i+2]),0};
			}

			poly[pc++] = new polygon(x[0],x[1],x[2],tcolor);

			if(x[0].x<bbox[0].x) { bbox[0]=x[0]; }
			if(x[1].x<bbox[0].x) { bbox[0]=x[1]; }
			if(x[2].x<bbox[0].x) { bbox[0]=x[2]; }
			if(x[0].y<bbox[1].y) { bbox[1]=x[0]; }
			if(x[1].y<bbox[1].y) { bbox[1]=x[1]; }
			if(x[2].y<bbox[1].y) { bbox[1]=x[2]; }
			if(x[0].x>bbox[2].x) { bbox[2]=x[0]; }
			if(x[1].x>bbox[2].x) { bbox[2]=x[1]; }
			if(x[2].x>bbox[2].x) { bbox[2]=x[2]; }
			if(x[0].y>bbox[3].y) { bbox[3]=x[0]; }
			if(x[1].y>bbox[3].y) { bbox[3]=x[1]; }
			if(x[2].y>bbox[3].y) { bbox[3]=x[2]; }

			if(verts==4)
			{
				if(x[3].x<bbox[0].x) { bbox[0]=x[3]; }
				if(x[3].y<bbox[1].y) { bbox[1]=x[3]; }
				if(x[3].x>bbox[2].x) { bbox[2]=x[3]; }
				if(x[3].y>bbox[3].y) { bbox[3]=x[3]; }
			
				poly[pc++] = new polygon(x[2],x[3],x[0],tcolor);
			}
		}

		for(xint k=0;k<d;++k,i+=3,++dc)
		{
			system::err(string::find(t[i++],"dock")==-1,"ERROR: y3d format wrong (dock)");

			const xint type = string::str2int(t[i++]);
			dock[dc] = vector{fx::l2f(string::str2int(t[i])),fx::l2f(string::str2int(t[i+1])),fx::l2f(string::str2int(t[i+2])),type};
		}
	}

	//compute bounding circle
	bbox[0].z = bbox[1].z = bbox[2].z = bbox[3].z = 0;
	const vector m = fx::mul(bbox[0] + bbox[1] + bbox[2] + bbox[3],FXQRT);

	bound = fx::len(bbox[0]-m);
	bound = math::min(bound,fx::len(bbox[1]-m));
	bound = math::min(bound,fx::len(bbox[2]-m));
	bound = math::min(bound,fx::len(bbox[3]-m));

	bound = math::min(bound,fx::len(fx::mul(bbox[0]+bbox[1]-bbox[0],FXHLF)-m));
	bound = math::min(bound,fx::len(fx::mul(bbox[1]+bbox[2]-bbox[1],FXHLF)-m));
	bound = math::min(bound,fx::len(fx::mul(bbox[2]+bbox[3]-bbox[2],FXHLF)-m));
	bound = math::min(bound,fx::len(fx::mul(bbox[3]+bbox[0]-bbox[3],FXHLF)-m));
	//*

	delete t;
}

object::object(vector* a,vector* b,vector* c,vector* d,xint x,xint e)
 : poly(0),
   dock(0),
   bound(0),
   polys(x<<1),
   docks(0),
   scolor(0)
{
	for(xint i=0;i<x;++i)
	{
		polys -= (a[i].z==0 && b[i].z==0 && c[i].z==0);
		polys -= (c[i].z==0 && d[i].z==0 && a[i].z==0);
		polys -= (b[i].z==0 && c[i].z==0 && d[i].z==0);
		polys -= (d[i].z==0 && a[i].z==0 && b[i].z==0);
	}

	poly = new polygon*[polys];

	for(xint i=0,j=0;i<x;++i)
	{
		const xint az = a[i].z;
		const xint bz = b[i].z;
		const xint cz = c[i].z;
		const xint dz = d[i].z;

		const xint y = -(((bz>az) && (bz>cz) && (bz>dz)) || ((dz>az) && (dz>bz) && (dz>cz)) || ((az>bz) && (az>dz) && (cz>bz) && (cz>dz)) || ((dz<az) && (dz<bz) && (dz<cz)) || ((bz<az) && (bz<cz) && (bz<dz)))
			     +  (((az>bz) && (az>cz) && (az>dz)) || ((cz>az) && (cz>bz) && (cz>dz)) || ((bz>az) && (bz>cz) && (dz>az) && (dz>cz)) || ((az<bz) && (az<cz) && (az<dz)) || ((cz<az) && (cz<bz) && (cz<dz))); 

		switch(y)
		{
			case -1:
			case 0:
				if(az!=0 || bz!=0 || cz!=0) { poly[j++] = new polygon(a[i],b[i],c[i],e); }
				if(cz!=0 || dz!=0 || az!=0) { poly[j++] = new polygon(c[i],d[i],a[i],e); }
			break;

			case 1:
				if(bz!=0 || cz!=0 || dz!=0) { poly[j++] = new polygon(b[i],c[i],d[i],e); }
				if(dz!=0 || az!=0 || bz!=0) { poly[j++] = new polygon(d[i],a[i],b[i],e); }
			break;
		}
	}
}

object::object(const object& o) : poly(new polygon*[o.polys]),dock(new vector[o.docks]),bound(o.bound),polys(o.polys),docks(o.docks),scolor(o.scolor)
{
	//copy polygons
	for(xint i=0;i<polys;++i)
	{
		poly[i] = new polygon(*o.poly[i]);
	}

	//copy docking points
	for(xint i=0;i<docks;++i)
	{
		dock[i] = o.dock[i];
	}
}

object::~object()
{
	delete[] dock;
	for(xint i=0;i<polys;++i) { delete poly[i]; }
	delete[] poly;
}

vector* object::docktype(xint i,xint j) const
{
	vector* r[2] = {0,0};
	for(xint k=0,l=-1;k<docks && (l!=j);++k)
	{
		l += (dock[k].e==i);
		r[l==j] = &dock[k];
	}
	return r[1];
}

void object::update(const matrix& m,bool j)
{
	//update polygon
	for(xint i=0;i<polys;++i)
	{
		poly[i]->update(m,j);
	}

	//update docking points
	for(xint i=0;i<docks;++i)
	{
		dock[i] = m*dock[i];
	}

	//update bounding circle
	cbound = bound; //TODO: cbound trafo to match projection
}

void object::display(const vector& p,xint f) const
{
	for(xint i=0;i<polys;++i)
	{
		poly[i]->display(p,f,scolor);
	}
}

void object::pull(fixed x)
{
	for(xint i=0;i<polys;++i)
	{
		poly[i]->pull(x);
	}
}

bool object::collision(const vector& x,const vector& a)
{
	return (a.x>x.x-bound)&&(a.x<x.x+bound)&&(a.y>x.y-bound)&&(a.y<x.y+bound); 	//box works
	//const vector t(a-x);						//component-wise?
	//return t.dot(t)<r*r;						//circle doesn't
}
///</code>

