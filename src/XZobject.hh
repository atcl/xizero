// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
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

///definition 
class object
{
	private:
		polygon** poly;
		fvector*  dock;		// 0:ammo1; 1:ammo2; 2:exhaust; 3:connector;
		fvector   bound;
		sint      polys;
		sint      docks;
		uint      scolor;	//Shadow Color
	public:

		/*OK*/ object(const char* o);
		/*OK*/ object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e);
		/*OK*/ object(const object& o);
		       ~object();
		/*OK*/ void   update(const fmatrix& m=object::linear);
		/*OK*/ void   display(const lvector& p,sint f) const;
		/*OK*/ fvector* docktype(sint i,sint j) const;
		/*OK*/ void   pull(fixed x);
		/*OK*/ inline fvector bounding() { return bound; }

		static fmatrix linear;
};
///*

///implementation
fmatrix object::linear = fmatrix();

object::object(const char* o) : poly(0),dock(0),bound(0,0,0,0),polys(0),docks(0)
{
	char** t = format::csv(o);
	sint i = 0;

	if(string::find(t[i++],"<y3dtxt>")==-1) { system::say("y3d format wrong (head)",1); system::bye(-1); }

	polys = string::str2int(t[i++]); 
	polys += 2*string::str2int(t[i++]); 
	docks = string::str2int(t[i++]);
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new fvector[docks]; }

	fvector bbox[2];

	if(string::find(t[i++],"objt")==-1) { system::say("y3d format wrong (objt)",1); system::bye(-1); }

	const sint subs = string::str2int(t[i++]);
	/*char* oid = t[i++];*/ i++;
	scolor = string::str2hex(t[i++]);

	sint pc = 0;
	sint dc = 0;
	for(sint j=0;j<subs;++j)
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
			const uint tcolor = string::str2hex(t[i++]);

			for(sint l=0;l<3+(verts==4);++l,i+=3)
			{
				if(string::find(t[i++],"vert")==-1) { system::say("y3d format wrong (vert)",1); system::bye(-1); }

				x[l].set(pos.x+string::str2int(t[i]),pos.y+string::str2int(t[i+1]),pos.z+string::str2int(t[i+2]));
			}

			poly[pc++] = new polygon(x[0],x[1],x[2],tcolor);

			bbox[0].x = math::min(bbox[0].x,fx::l2f(math::min(x[0].x,math::min(x[1].x,x[2].x))));
			bbox[0].y = math::min(bbox[0].y,fx::l2f(math::min(x[0].y,math::min(x[1].y,x[2].y))));
			bbox[0].z = math::min(bbox[0].z,fx::l2f(math::min(x[0].z,math::min(x[1].z,x[2].z))));
			bbox[1].x = math::max(bbox[1].x,fx::l2f(math::max(x[0].x,math::max(x[1].x,x[2].x))));
			bbox[1].y = math::max(bbox[1].y,fx::l2f(math::max(x[0].y,math::max(x[1].y,x[2].y))));
			bbox[1].z = math::max(bbox[1].z,fx::l2f(math::max(x[0].z,math::max(x[1].z,x[2].z))));

			if(verts==4)
			{
				bbox[0].x = math::min(bbox[0].x,fx::l2f(x[3].x));
				bbox[0].y = math::min(bbox[0].y,fx::l2f(x[3].y));
				bbox[0].z = math::min(bbox[0].z,fx::l2f(x[3].z));
				bbox[1].x = math::max(bbox[1].x,fx::l2f(x[3].x));
				bbox[1].y = math::max(bbox[1].y,fx::l2f(x[3].y));
				bbox[1].z = math::max(bbox[1].z,fx::l2f(x[3].z));
			
				poly[pc++] = new polygon(x[2],x[3],x[0],tcolor);
			}
		}

		for(sint k=0;k<d;++k,i+=3,++dc)
		{
			if(string::find(t[i++],"dock")==-1) { system::say("y3d format wrong (dock)",1); system::bye(-1); }

			const sint type = string::str2int(t[i++]);
			dock[dc].set(fx::l2f(string::str2int(t[i])),fx::l2f(string::str2int(t[i+1])),fx::l2f(string::str2int(t[i+2])),type);
		}
	}

	const fixed mx = math::min(bbox[0].x,bbox[1].x);
	const fixed my = math::min(bbox[0].y,bbox[1].y);
	bound.x = fx::sqr(fx::mul(mx,mx)+fx::mul(my,my));

	delete t;
}

object::object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e) : poly(0),dock(0),bound(0,0,0,0),polys(x<<1),docks(0),scolor(0)
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

object::object(const object& o) : poly(0),dock(0),bound(o.bound),polys(o.polys),docks(o.docks),scolor(o.scolor)
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

}

object::~object()
{
	delete[] dock;
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
	//transform bounding ellipsoid here later
}

void object::display(const lvector& p,sint f) const
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->display(p,f,scolor);
	}
}

fvector* object::docktype(sint i,sint j) const
{
	for(sint k=0,l=-1;k<docks;++k)
	{
		l += (dock[k].e==i);
		if(l==j) { return &dock[k]; } //to break condition
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

