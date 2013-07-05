///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZobject.hh
// Object Library 
///</header>

///<include>
#pragma once
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
		fvector*  dock;		// 0:ammo1; 1:ammo2; 2:exhaust; 3:connector;
		fixed     bound;
		fixed     cbound;	//TODO
		xint      polys;
		xint      docks;
		yint      scolor;	//Shadow Color
		object& operator=(const object& o);
	public:

		/*OK*/ object(const char* o);
		/*OK*/ object(lvector* a,lvector* b,lvector* c,lvector* d,xint x,xint e);
		/*OK*/ object(const object& o);
		/*OK*/ ~object();
		/*OK*/ fvector* docktype(xint i,xint j) const;
		/*OK*/ void update(const fmatrix& m=object::linear,bool j=1);
		/*OK*/ void display(const lvector& p,xint f) const;
		/*OK*/ void pull(fixed x); //translate along normals
		// void rebound();
		/*OK*/ inline fixed bounding() const { return cbound; }			//remove if possible
		/*OK*/ inline bool collision(const fvector& x,const fvector& a);

		static fmatrix linear;
};
///</define>

///<code>
fmatrix object::linear = fmatrix();

object::object(const char* o) : poly(0),dock(0),bound(FXMON<<10),polys(0),docks(0),scolor(0)
{
	char** t = format::csv(o);
	xint i = 0;

	system::err(string::find(t[i++],"<y3dtxt>")==-1,"ERROR: y3d format wrong (head)");

	polys = string::str2int(t[i++]); 
	polys += 2*string::str2int(t[i++]); 
	docks = string::str2int(t[i++]);
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new fvector[docks]; }

	lvector bbox[4];

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

		lvector pos(string::str2int(t[i]),string::str2int(t[i+1]),string::str2int(t[i+2])); i+=3;
		lvector x[4];
		for(xint k=0;k<p;++k)
		{
			system::err(string::find(t[i++],"poly")==-1,"ERROR: y3d format wrong (poly)");

			const xint verts = string::str2int(t[i++]);
			/*char* pid = t[i++];*/ i++;
			const yint tcolor = string::hex2int(t[i++]);

			for(xint l=0;l<3+(verts==4);++l,i+=3)
			{
				system::err(string::find(t[i++],"vert")==-1,"ERROR: y3d format wrong (vert)");

				x[l].set(pos.x+string::str2int(t[i]),pos.y+string::str2int(t[i+1]),pos.z+string::str2int(t[i+2]));
			}

			poly[pc++] = new polygon(x[0],x[1],x[2],tcolor);
			//poly[pc++] = new polygon(x[0],x[2],x[1],tcolor); //z

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
				//poly[pc++] = new polygon(x[2],x[0],x[3],tcolor); //z
			}
		}

		for(xint k=0;k<d;++k,i+=3,++dc)
		{
			system::err(string::find(t[i++],"dock")==-1,"ERROR: y3d format wrong (dock)");

			const xint type = string::str2int(t[i++]);
			dock[dc].set(fx::l2f(string::str2int(t[i])),fx::l2f(string::str2int(t[i+1])),fx::l2f(string::str2int(t[i+2])),type);
		}
	}

	//compute bounding circle
	bbox[0].z = bbox[1].z = bbox[2].z = bbox[3].z = 0;
	const fvector m = fvector(bbox[0] + bbox[1] + bbox[2] + bbox[3])*FXQRT;

	bound = ((fvector(bbox[0]))-m).length();
	bound = math::min(bound,((fvector(bbox[1]))-m).length());
	bound = math::min(bound,((fvector(bbox[2]))-m).length());
	bound = math::min(bound,((fvector(bbox[3]))-m).length());

	bound = math::min(bound,((fvector(bbox[0]+(bbox[1]-bbox[0])*FXHLF))-m).length());
	bound = math::min(bound,((fvector(bbox[1]+(bbox[2]-bbox[1])*FXHLF))-m).length());
	bound = math::min(bound,((fvector(bbox[2]+(bbox[3]-bbox[2])*FXHLF))-m).length());
	bound = math::min(bound,((fvector(bbox[3]+(bbox[0]-bbox[3])*FXHLF))-m).length());
	//*

	delete t;
}

object::object(lvector* a,lvector* b,lvector* c,lvector* d,xint x,xint e) : poly(0),dock(0),bound(0),polys(x<<1),docks(0),scolor(0)
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
				//if(az!=0 || bz!=0 || cz!=0) { poly[j++] = new polygon(a[i],c[i],b[i],e); } //z
				//if(cz!=0 || dz!=0 || az!=0) { poly[j++] = new polygon(c[i],a[i],d[i],e); } //z
			break;

			case 1:
				if(bz!=0 || cz!=0 || dz!=0) { poly[j++] = new polygon(b[i],c[i],d[i],e); }
				if(dz!=0 || az!=0 || bz!=0) { poly[j++] = new polygon(d[i],a[i],b[i],e); }
				//if(bz!=0 || cz!=0 || dz!=0) { poly[j++] = new polygon(b[i],d[i],c[i],e); } //z
				//if(dz!=0 || az!=0 || bz!=0) { poly[j++] = new polygon(d[i],b[i],a[i],e); } //z
			break;
		}
	}
}

object::object(const object& o) : poly(new polygon*[o.polys]),dock(new fvector[o.docks]),bound(o.bound),polys(o.polys),docks(o.docks),scolor(o.scolor)
{
	for(xint i=0;i<polys;++i)
	{
		poly[i] = new polygon(*o.poly[i]);
	}
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

fvector* object::docktype(xint i,xint j) const
{
	fvector* r[2] = {0,0};
	for(xint k=0,l=-1;k<docks && (l!=j);++k)
	{
		l += (dock[k].e==i);
		r[l==j] = &dock[k];
	}
	return r[1];
}

void object::update(const fmatrix& m,bool j)
{
	for(xint i=0;i<polys;++i)
	{
		poly[i]->update(m,j);
	}
	for(xint i=0;i<docks;++i)
	{
		dock[i] = m*dock[i];
	}
	cbound = bound; //TODO: cbound trafo to match projection
}

void object::display(const lvector& p,xint f) const
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

bool object::collision(const fvector& x,const fvector& a)
{
	return (a.x>x.x-bound)&&(a.x<x.x+bound)&&(a.y>x.y-bound)&&(a.y<x.y+bound); 	//box works
	//const fvector t(a-x);						//component-wise?
	//return t.dot(t)<r*r;						//circle doesn't
}
///</code>

