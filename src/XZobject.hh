// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZobject.hh
// Object Library 

///guard
#pragma once
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
		fixed     bound;
		sint      polys;
		sint      docks;
		uint      scolor;	//Shadow Color
		object& operator=(const object& o);
	public:

		/*OK*/ object(const char* o);
		/*OK*/ object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e);
		/*OK*/ object(const object& o);
		       ~object();
		/*OK*/ fvector* docktype(sint i,sint j) const;
		/*OK*/ void update(const fmatrix& m=object::linear,bool j=1);
		/*OK*/ void display(const lvector& p,sint f) const;
		/*OK*/ void pull(fixed x);
		// void rebound();
		/*OK*/ inline fixed bounding() const { return bound; }

		static fmatrix linear;
};
///*

///implementation
fmatrix object::linear = fmatrix();

object::object(const char* o) : poly(0),dock(0),bound(FXMON<<10),polys(0),docks(0),scolor(0)
{
	char** t = format::csv(o);
	sint i = 0;

	if(string::find(t[i++],"<y3dtxt>")==-1) { system::say("Error: y3d format wrong (head)",1); system::bye(-1); }

	polys = string::str2int(t[i++]); 
	polys += 2*string::str2int(t[i++]); 
	docks = string::str2int(t[i++]);
	poly  = new polygon*[polys];
	if(docks!=0) { dock = new fvector[docks]; }

	lvector bbox[4];

	if(string::find(t[i++],"objt")==-1) { system::say("Error: y3d format wrong (objt)",1); system::bye(-1); }

	const sint subs = string::str2int(t[i++]);
	/*char* oid = t[i++];*/ i++;
	scolor = string::str2hex(t[i++]);

	sint pc = 0;
	sint dc = 0;
	for(sint j=0;j<subs;++j)
	{
		if(string::find(t[i++],"sobj")==-1) { system::say("Error: y3d format wrong (sobj)",1); system::bye(-1); }

		const sint p = string::str2int(t[i++]);
		/*char* sid = t[i++];*/ i++;
		const sint d = string::str2int(t[i++]);

		if(string::find(t[i++],"posi")==-1) { system::say("Error: y3d format wrong (posi)",1); system::bye(-1); }

		lvector pos(string::str2int(t[i]),string::str2int(t[i+1]),string::str2int(t[i+2])); i+=3;
		lvector x[4];
		for(sint k=0;k<p;++k)
		{
			if(string::find(t[i++],"poly")==-1) { system::say("Error: y3d format wrong (poly)",1); system::bye(-1); }

			const sint verts = string::str2int(t[i++]);
			/*char* pid = t[i++];*/ i++;
			const uint tcolor = string::str2hex(t[i++]);

			for(sint l=0;l<3+(verts==4);++l,i+=3)
			{
				if(string::find(t[i++],"vert")==-1) { system::say("Error: y3d format wrong (vert)",1); system::bye(-1); }

				x[l].set(pos.x+string::str2int(t[i]),pos.y+string::str2int(t[i+1]),pos.z+string::str2int(t[i+2]));
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

		for(sint k=0;k<d;++k,i+=3,++dc)
		{
			if(string::find(t[i++],"dock")==-1) { system::say("Error: y3d format wrong (dock)",1); system::bye(-1); }

			const sint type = string::str2int(t[i++]);
			dock[dc].set(fx::l2f(string::str2int(t[i])),fx::l2f(string::str2int(t[i+1])),fx::l2f(string::str2int(t[i+2])),type);
		}
	}

	//compute bounding circle
	bbox[0].z = bbox[1].z = bbox[2].z = bbox[3].z = 0;
	const fixed p = fx::l2f((bbox[1]-bbox[0]).length()+(bbox[3]-bbox[2]).length());
	const fixed q = fx::l2f((bbox[2]-bbox[1]).length()+(bbox[0]-bbox[3]).length());
	const fixed a = fx::l2f(((bbox[3]-bbox[1]).cross(bbox[2]-bbox[0])).length())>>1;
	bound = fx::div(a,math::min(p,q));

	/*lvector j = (bbox[0]+bbox[1]+bbox[2]+bbox[3]);
	j.x>>=2;
	j.y>>=2;
	const uint u = (bbox[0]-j).length();
	const uint v = (bbox[1]-j).length();
	const uint x = (bbox[2]-j).length();
	const uint y = (bbox[3]-j).length();
	const uint uv = u*v;
	const uint xy = x*y;
	const uint ux = u*x;
	const uint vy = v*y;
	const uint uy = u*y;
	const uint vx = v*x;
	const uint uvx = uv*x;
	const uint vxy = vx*y;
	const uint xyu = xy*u;
	const uint yuv = uy*v;
	const uint uvxy = uv*xy;
	const uint m = (uvx>>1) + (vxy>>1) + (xyu>>1) + (yuv>>1);
	bound = ( ((m-uvx)/(uvxy)) * ((m-vxy)/(uv+xy)) * ((m-xyu)/(ux*vy)) * ((m-yuv)/(uy+vx)) );*/
	//bound = fx::sqr(fx::l2f( ((m-uvx)*(m-vxy)*(m-xyu)*(m-yuv))/(uvxy*(uv+xy)*(ux+vy)*(uy+vx)) )<<1);

alerf(bound);
	//*

	delete t;
}

object::object(lvector* a,lvector* b,lvector* c,lvector* d,sint x,sint e) : poly(0),dock(0),bound(0),polys(x<<1),docks(0),scolor(0)
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
	for(int i=0;i<polys;++i) { delete poly[i]; }
	delete[] poly;
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

void object::update(const fmatrix& m,bool j)
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->update(m,j);
	}
	for(sint i=0;i<docks;++i)
	{
		dock[i] = m.transform(dock[i]);
	}
}

void object::display(const lvector& p,sint f) const
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->display(p,f,scolor);
	}
}

void object::pull(fixed x) //translate along normals
{
	for(sint i=0;i<polys;++i)
	{
		poly[i]->pull(x);
	}
}
///*

