// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZobject.hh
// Object Library 

///guard
#ifndef HH_XZOBJECT
#define HH_XZOBJECT
//#pragma message "Compiling " __FILE__ "..." " TODO: Destructor"
///*

///include
#include "XZbasic.hh"
#include "XZformat.hh"
#include "XZvector.hh"
#include "XZmatrix.hh"
#include "XZpolygon.hh"
#include "XZsystem.hh"
///*

///declarations
typedef fvector box[2];
///*

///definition 
class object
{
	private:
		polygon** poly;
		fvector*  dock;		// 0:ammo1; 1:ammo2; 2:exhaust; 3:connector;
		box       bbox;
		sint      polys;
		sint      docks;
	public:
		static lvector project(const lvector& p,const lvector& v);
		/*OK*/ object(const char* o);
		/*OK*/ object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e);
		/*OK*/ object(const object& o);
		       ~object();
		/*OK*/ void   update(const fmatrix& m=object::linear);
		/*OK*/ void   display(const lvector& p,sint f) const;
		/*OK*/ fvector* docktype(sint i,sint j) const;
		/*OK*/ void   pull(sint x);
		/*OK*/ inline box& boundingbox() { return bbox; }

		static fmatrix linear;
};
///*

///implementation
fmatrix object::linear = fmatrix(FXONE);

lvector object::project(const lvector& p,const lvector& v)
{
	lvector r;
	r.z = v.z+p.z;
	r.x = fx::f2l(fx::mul( PRJX<<FX,fx::div(fx::l2f(v.x),fx::l2f(r.z)))) + p.x;
	r.y = fx::f2l(fx::mul(-PRJY<<FX,fx::div(fx::l2f(v.y),fx::l2f(r.z)))) + p.y;
	return r;
}

object::object(const char* o) : poly(0),dock(0),polys(0),docks(0)
{
	char** t = format::csv(o);
	sint i = 0;

	if(string::find(t[i++],"<y3dtxt>")==-1) { system::say("y3d format wrong (head)",1); system::bye(-1); }

	polys = string::str2int(t[i++]); 
	polys += 2*string::str2int(t[i++]); 
	docks = string::str2int(t[i++]);
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new fvector[docks]; }

	if(string::find(t[i++],"objt")==-1) { system::say("y3d format wrong (objt)",1); system::bye(-1); }

	const sint subs = string::str2int(t[i++]);
	/*char* oid = t[i++];*/ i++;
	sint scolor = string::str2hex(t[i++]);

	sint pc = 0;
	sint dc = 0;
	for(uint j=0;j<subs;++j)
	{
		if(string::find(t[i++],"sobj")==-1) { system::say("y3d format wrong (sobj)",1); system::bye(-1); }

		const sint p = string::str2int(t[i++]);
		/*char* sid = t[i++];*/ i++;
		const sint d = string::str2int(t[i++]);

		if(string::find(t[i++],"posi")==-1) { system::say("y3d format wrong (posi)",1); system::bye(-1); }

		lvector pos(string::str2int(t[i]),string::str2int(t[i+1]),string::str2int(t[i+2])); i+=3;
		lvector x[4];
		for(sint k=0;k<p;++k)
		{
			if(string::find(t[i++],"poly")==-1) { system::say("y3d format wrong (poly)",1); system::bye(-1); }

			const sint verts = string::str2int(t[i++]);
			/*char* pid = t[i++];*/ i++;
			const sint tcolor = string::str2hex(t[i++]);

			for(sint l=0;l<3+(verts==4);++l,i+=3)
			{
				if(string::find(t[i++],"vert")==-1) { system::say("y3d format wrong (vert)",1); system::bye(-1); }

				x[l].set(pos.x+string::str2int(t[i]),pos.y+string::str2int(t[i+1]),pos.z+string::str2int(t[i+2]));
			}

			poly[pc++] = new polygon(x[0],x[1],x[2],tcolor,scolor);

			bbox[0].x = fx::l2f(math::min(bbox[0].x,math::min(x[0].x,math::min(x[1].x,x[2].x))));
			bbox[0].y = fx::l2f(math::min(bbox[0].y,math::min(x[0].y,math::min(x[1].y,x[2].y))));
			bbox[0].z = fx::l2f(math::min(bbox[0].z,math::min(x[0].z,math::min(x[1].z,x[2].z))));
			bbox[1].x = fx::l2f(math::max(bbox[1].x,math::max(x[0].x,math::max(x[1].x,x[2].x))));
			bbox[1].y = fx::l2f(math::max(bbox[1].y,math::max(x[0].y,math::max(x[1].y,x[2].y))));
			bbox[1].z = fx::l2f(math::max(bbox[1].z,math::max(x[0].z,math::max(x[1].z,x[2].z))));

			if(verts==4)
			{
				bbox[0].x = fx::l2f(math::min(bbox[0].x,x[3].x));
				bbox[0].y = fx::l2f(math::min(bbox[0].y,x[3].y));
				bbox[0].z = fx::l2f(math::min(bbox[0].z,x[3].z));
				bbox[1].x = fx::l2f(math::max(bbox[1].x,x[3].x));
				bbox[1].y = fx::l2f(math::max(bbox[1].y,x[3].y));
				bbox[1].z = fx::l2f(math::max(bbox[1].z,x[3].z));
			
				poly[pc++] = new polygon(x[2],x[3],x[0],tcolor,scolor);
			}
		}

		for(sint k=0;k<d;++k,i+=3,++dc)
		{
			if(string::find(t[i++],"dock")==-1) { system::say("y3d format wrong (dock)",1); system::bye(-1); }

			const sint type = string::str2int(t[i++]);
			dock[dc].set(fx::l2f(string::str2int(t[i])),fx::l2f(string::str2int(t[i+1])),fx::l2f(string::str2int(t[i+2])),type);
		}
	}

	delete t;
}

object::object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e) : poly(0),dock(0),polys(x<<1),docks(0)
{
	for(sint i=0;i<x;++i)
	{
		polys -= (a[i].z==0 && b[i].z==0 && c[i].z==0);
		polys -= (c[i].z==0 && d[i].z==0 && a[i].z==0);
		polys -= (b[i].z==0 && c[i].z==0 && d[i].z==0);
		polys -= (d[i].z==0 && a[i].z==0 && b[i].z==0);
	}

	poly = new polygon*[polys];

	for(sint i=0,j=0;i<x;++i)
	{
		const sint az = a[i].z;
		const sint bz = b[i].z;
		const sint cz = c[i].z;
		const sint dz = d[i].z;

		const sint y = -(((bz>az) && (bz>cz) && (bz>dz)) || ((dz>az) && (dz>bz) && (dz>cz)) || ((az>bz) && (az>dz) && (cz>bz) && (cz>dz)) || ((dz<az) && (dz<bz) && (dz<cz)) || ((bz<az) && (bz<cz) && (bz<dz)))
			     +  (((az>bz) && (az>cz) && (az>dz)) || ((cz>az) && (cz>bz) && (cz>dz)) || ((bz>az) && (bz>cz) && (dz>az) && (dz>cz)) || ((az<bz) && (az<cz) && (az<dz)) || ((cz<az) && (cz<bz) && (cz<dz))); 

		switch(y)
		{
			case -1:
			case 0:
				if(az!=0 || bz!=0 || cz!=0) { poly[j++] = new polygon(a[i],b[i],c[i],e,0); }
				if(cz!=0 || dz!=0 || az!=0) { poly[j++] = new polygon(c[i],d[i],a[i],e,0); }
			break;

			case 1:
				if(bz!=0 || cz!=0 || dz!=0) { poly[j++] = new polygon(b[i],c[i],d[i],e,0); }
				if(dz!=0 || az!=0 || bz!=0) { poly[j++] = new polygon(d[i],a[i],b[i],e,0); }
			break;
		}
	}
}

object::object(const object& o) : poly(0),dock(0),polys(o.polys),docks(o.docks)
{
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new fvector[docks]; }
	
	for(sint i=0;i<polys;++i)
	{
		poly[i] = new polygon(*o.poly[i]);
	}
	for(sint i=0;i<docks;++i)
	{
		dock[i] = o.dock[i];
	}
	bbox[0] = o.bbox[0];
	bbox[1] = o.bbox[1];
}

object::~object()
{
	//delete[] dock;
	//for(int i=0;i<polys;++i) { delete poly[i]; }
	//delete[] poly;
}

void object::update(const fmatrix& m)
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->update(m);
	}
	for(sint i=0;i<docks;++i)
	{
		dock[i] = m.transform(dock[i]);
	}
	bbox[0] = m.transform(bbox[0]);
	bbox[1] = m.transform(bbox[1]);
}

void object::display(const lvector& p,sint f) const
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->display(p,f);
	}
}

fvector* object::docktype(sint i,sint j) const
{
	for(sint k=0,l=-1;k<docks;++k)
	{
		l += (dock[k].e==i);
		if(l==j) { return &dock[k]; }
	}
	return 0;
}

void object::pull(fixed x) //translate along normals
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->pull(x);
	}
}
///*

#endif

