// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZobject.hh
// Object Library 

///guard
#ifndef HH_XZOBJECT
#define HH_XZOBJECT
///*

///include
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
		fvector*  odock;
		box       bbox;
		box       obox;
		long      polys;
		long      docks;
	public:
		static lvector project(const lvector& p,const lvector& v);
		/*OK*/ object(const char* o);
		/*OK*/ object(lvector* a,lvector* b,lvector* c,lvector* d,long x,long e);
		/*OK*/ object(const object& o);
		       ~object();
		/*OK*/ void   update(const fmatrix& m=object::linear);
		/*OK*/ INLINE void display(const lvector& p,long f) const;
		/*OK*/ void   reset();
		/*OK*/ void   set();
		/*OK*/ fvector* docktype(long i,long j) const;
		/*OK*/ INLINE box& boundingbox() { return bbox; }
		/*OK*/ void   pull(long x);

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

object::object(const char* o)
{
	char** t = format::csv(o);
	long i = 0;

	if(string::find(t[i++],"<y3dtxt>")==-1) { system::say("y3d format wrong (head)",1); system::bye(-1); }

	polys = string::conl(t[i++]); 
	polys += 2*string::conl(t[i++]); 
	docks = string::conl(t[i++]);
	poly  = new polygon*[polys];
	dock  = new fvector[docks];
	odock = new fvector[docks];

	if(string::find(t[i++],"objt")==-1) { system::say("y3d format wrong (objt)",1); system::bye(-1); }

	const long subs = string::conl(t[i++]);
	/*char* oid = t[i++];*/ i++;
	long scolor = string::conh(t[i++]);

	long pc = 0;
	long dc = 0;
	for(long j=0;j<subs;++j)
	{
		if(string::find(t[i++],"sobj")==-1) { system::say("y3d format wrong (sobj)",1); system::bye(-1); }

		const long p = string::conl(t[i++]);
		/*char* sid = t[i++];*/ i++;
		const long d = string::conl(t[i++]);

		if(string::find(t[i++],"posi")==-1) { system::say("y3d format wrong (posi)",1); system::bye(-1); }

		lvector pos(string::conl(t[i]),string::conl(t[i+1]),string::conl(t[i+2])); i+=3;
		lvector x[4];
		for(long k=0;k<p;++k)
		{
			if(string::find(t[i++],"poly")==-1) { system::say("y3d format wrong (poly)",1); system::bye(-1); }

			const long verts = string::conl(t[i++]);
			/*char* pid = t[i++];*/ i++;
			const long tcolor = string::conh(t[i++]);

			for(long l=0;l<3+(verts==4);++l,i+=3)
			{
				if(string::find(t[i++],"vert")==-1) { system::say("y3d format wrong (vert)",1); system::bye(-1); }

				x[l].set(pos.x+string::conl(t[i]),pos.y+string::conl(t[i+1]),pos.z+string::conl(t[i+2]));
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

		for(long k=0;k<d;++k,i+=3,++dc)
		{
			if(string::find(t[i++],"dock")==-1) { system::say("y3d format wrong (dock)",1); system::bye(-1); }

			const long type = string::conl(t[i++]);
			dock[dc].set(fx::l2f(string::conl(t[i])),fx::l2f(string::conl(t[i+1])),fx::l2f(string::conl(t[i+2])),type);
			odock[dc] = dock[dc];
		}
	}

	obox[0] = bbox[0];
	obox[1] = bbox[1];

	delete t;
}

object::object(lvector* a,lvector* b,lvector* c,lvector* d,long x,long e) : poly(0),dock(0),odock(0),polys(x<<1),docks(0)
{
	for(long i=0;i<x;++i)
	{
		polys -= (a[i].z==0 && b[i].z==0 && c[i].z==0);
		polys -= (c[i].z==0 && d[i].z==0 && a[i].z==0);
		polys -= (b[i].z==0 && c[i].z==0 && d[i].z==0);
		polys -= (d[i].z==0 && a[i].z==0 && b[i].z==0);
	}

	poly = new polygon*[polys];

	for(long i=0,j=0;i<x;++i)
	{
		const long az = a[i].z;
		const long bz = b[i].z;
		const long cz = c[i].z;
		const long dz = d[i].z;

		const long y = -(((bz>az) && (bz>cz) && (bz>dz)) || ((dz>az) && (dz>bz) && (dz>cz)) || ((az>bz) && (az>dz) && (cz>bz) && (cz>dz)) || ((dz<az) && (dz<bz) && (dz<cz)) || ((bz<az) && (bz<cz) && (bz<dz)))
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

object::object(const object& o) : poly(0),dock(0),odock(0),polys(o.polys),docks(o.docks)
{
	poly  = new polygon*[polys];
	dock  = new fvector[docks];
	odock = new fvector[docks];
	
	for(long i=0;i<polys;++i)
	{
		poly[i] = new polygon(*o.poly[i]);
	}
	for(long i=0;i<docks;++i)
	{
		dock[i] = o.dock[i];
		odock[i] = o.odock[i];
	}
	bbox[0] = o.bbox[0];
	bbox[1] = o.bbox[1];
	obox[0] = o.obox[0];
	obox[1] = o.obox[1];
}

object::~object()
{
	//delete[] dock;
	//for(int i=0;i<polys;++i) { delete poly[i]; }
	//delete[] poly;
}

void object::update(const fmatrix& m)
{
	for(long i=0;i<polys;++i)
	{
		poly[i]->update(m);
	}
	for(long i=0;i<docks;++i)
	{
		dock[i] = m.transform(dock[i]);
	}
	bbox[0] = m.transform(bbox[0]);
	bbox[1] = m.transform(bbox[1]);
}

void object::display(const lvector& p,long f) const
{
	for(long i=0;i<polys;++i)
	{
		poly[i]->display(p,f);
	}
}

void object::reset()
{
	for(long i=0;i<polys;++i)
	{
		(*poly[i]).reset();
	}
	for(long i=0;i<docks;++i)
	{
		dock[i] = odock[i];
	}
	bbox[0] = obox[0];
	bbox[1] = obox[1];
}

void object::set()
{
	for(long i=0;i<polys;++i)
	{
		(*poly[i]).set();
	}
	for(long i=0;i<docks;++i)
	{
		odock[i] = dock[i];
	}
	obox[0] = bbox[0];
	obox[1] = bbox[1];
}

fvector* object::docktype(long i,long j) const
{
	for(long k=0,l=-1;k<docks;++k)
	{
		l += (dock[k].e==i);
		if(l==j) { return &dock[k]; }
	}
	return 0;
}

void object::pull(fixed x) //translate along normals
{
	for(long i=0;i<polys;++i)
	{
		fvector t = poly[i]->normal();
		const fixed l = t.length();
		t.x = fx::mul(fx::div(t.x,l),x);
		t.y = fx::mul(fx::div(t.y,l),x);
		t.z = fx::mul(fx::div(t.z,l),x);
		poly[i]->add(t);
	}
}
///*

#endif

