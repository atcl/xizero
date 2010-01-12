///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLPOLYGON
#define HH_CLPOLYGON
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLmath.hh"
#include "CLbase.hh"
#include "CLvector.hh"
#include "CLbuffer.hh"
#include "CLglobals.hh"
#include "CLutils.hh"
///*

///header
/* class name:	CLpolygon
 * 
 * description:	Handles four sided polygons.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

CLfvector ppoint[8];
CLfvector cpoint[8];
CLfvector dpoint[8];
CLfvector spoint[8];

///definitions
struct screenside
{
	xlong offset;
	float z;
};

class CLpolygon : CLbase<CLpolygon,0>
{
	private:
		static CLmath*   clmath;
		static CLscreen* clscreen;
	protected:
		static xlong pointcount;
		static float shadezscale;
		static CLfvector* vpoint;
		static CLfvector* wpoint;
		static screenside* leftside;
		static screenside* rightside;

		uxlong color;
		uxlong rcolor;
		uxlong scolor;
		uxlong shade;
		xlong cpointcount;
		CLfvector pointr[4];
		CLfvector points[4];
		CLfvector pointt[4];
		CLfvector normal;
		CLfvector rnormal;

		void polyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		template<class clvector> inline clvector getzplanecoords(const clvector& a,const clvector& b,float pz) const;
		template<class clvector> inline clvector getxplanecoords(const clvector& a,const clvector& b,float px) const;
		template<class clvector> inline clvector getyplanecoords(const clvector& a,const clvector& b,float py) const;
		void zclipping();
		void project(xlong px=0,xlong py=0,bool c=0);
		void xyclipping();
		bool visible();
		void shape();
		void flatshade(float pz,bool ambient,bool zlight);
		template<class clvector>void setside(const clvector& b,const clvector& e,screenside *s);
		void rasterize(xlong shadow,CLfbuffer* t=0); //too slow!!!
		inline xlong circleinc(xlong x,xlong pc) const;
		inline xlong circledec(xlong x,xlong pc) const;
	public:
		CLpolygon(const CLlvector& a,const CLlvector& b,const CLlvector& c,const CLlvector& d,uxlong co,uxlong sc);
		CLpolygon(const CLpolygon& c);
		~CLpolygon() { };
		void update(CLmatrix* m,bool i);
		void partupdate(CLmatrix* m);
		void display(const CLlvector& p,xshort flags);
		void display(const CLlvector& p,screenside* l,screenside* r,CLfbuffer* b,xlong h);
		template<class clvector>void add(const clvector& a);
		void reset();
		void setcolor(uxlong co) { color = co; };
		void resetcolor();
		CLfvector getnormal() const { return normal; };
		bool isinside(CLfvector* p) const;
};

CLmath*   CLpolygon::clmath   = CLmath::instance();
CLscreen* CLpolygon::clscreen = CLscreen::instance();
xlong CLpolygon::pointcount = 4;
float CLpolygon::shadezscale = 128/100;
CLfvector* CLpolygon::vpointpoint = new CLfvector[8];
CLfvector* CLpolygon::wpointpoint = new CLfvector[8];
screenside* CLpolygon::leftside = new screenside[YRES];
screenside* CLpolygon::rightside = new screenside[YRES];
///*

///implementation
void CLpolygon::polyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c) //! critical
{
	if(x1==x2 && y1==y2) { return; }

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = XRES;
	xlong length;
	xlong offset = y1*XRES+x1;

	if(dx<0) { dx = -dx; xs = -xs; }
	if(dy<0) { dy = -dy; ys = -ys; }
	if(dy>dx) { swap(&dx,&dy); swap(&xs,&ys); }

	length = dx+1;
	e = dy;

	for(xlong i=0; i<length; i++)
	{
		clscreen->cldoublebuffer[offset] = c;
		offset += xs;
		e += dy;
		if(e >= dx)
		{
			e -= dx;
			offset += ys;
		}
	}
}

template<class clvector>
clvector CLpolygon::getzplanecoords(const clvector& a,const clvector& b,float pz) const //! noncritical
{
	float m = (pz - b.z) / (a.z - b.z);
	clvector c( ((a.x - b.x) * m + b.x),((a.y - b.y) * m + b.y),pz );
	return c;
}

template<class clvector>
clvector CLpolygon::getxplanecoords(const clvector& a,const clvector& b,float px) const //! noncritical
{
	float m = (px - b.x) / (a.x - b.x);
	clvector c( xlong(px),xlong((a.y - b.y) * m + b.y),xlong((a.z - b.z) * m + b.z) );
	return c;
}

template<class clvector>
clvector CLpolygon::getyplanecoords(const clvector& a,const clvector& b,float py) const //! noncritical
{
	float m = (py - b.y) / (a.y - b.y);
	clvector c( xlong((a.x - b.x) * m + b.x),xlong(py),xlong((a.z - b.z) * m + b.z) );
	return c;
}

void CLpolygon::zclipping() //! noncritical
{
	xlong x = 0;
	xlong y = 0;
	xlong localpointcount = 0;

	//check all vertices if protruding minimum z 
	for(x=pointcount-1, y=0; y<pointcount; x=y, y++)
	{
		//inside
		if(ppoint[x].z >= ZMIN && ppoint[y].z >= ZMIN)
		{
			cpoint[localpointcount++] = ppoint[y];
		}
		//*
		
		//entering
		else if(ppoint[x].z >= ZMIN && ppoint[y].z < ZMIN)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],ZMIN);
		}
		//*
		
		//leaving
		else if(ppoint[x].z < ZMIN && ppoint[y].z >= ZMIN)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],ZMIN);
			cpoint[localpointcount++] = ppoint[y];
		}
		//*
	}
	//*

	cpointcount = 0;
	
	//check all vertices if protruding maximum z 
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		//inside
		if(cpoint[x].z <= ZMAX && cpoint[y].z <= ZMAX)
		{
			ppoint[cpointcount++] = cpoint[y];
		}
		//*
		
		//entering
		else if(cpoint[x].z <= ZMAX && cpoint[y].z > ZMAX)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],ZMAX);
		}
		//*
		
		//leaving
		else if(cpoint[x].z > ZMAX && cpoint[y].z <= ZMAX)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],ZMAX);
			cpoint[cpointcount++] = cpoint[y];
		}
		//*
	}
	//*
}

void CLpolygon::project(xlong px,xlong py,bool c) //! critical
{
	//use screen center as attached position if wanted
	if(c) { px = (XRES>>1); py = (YRES>>1); }
	//*

	//project each vertex if vertex's z is greater zero
	for(xlong x=0; x<cpointcount; x++)
	{
		if(ppoint[x].z > 0)
		{
			spoint[x].x = xlong( ( prjx * (ppoint[x].x / ppoint[x].z) ) + px );
			spoint[x].y = xlong( (-prjy * (ppoint[x].y / ppoint[x].z) ) + py );
			spoint[x].z = ppoint[x].z;
		}
		else
		{
			//CLsystem::exit(1,0,__func__,"Invalid z value: ",ppoint[x].z);
		}
	}
	//*
}



void CLpolygon::xyclipping() //! noncritical
{
	xlong x = 0;
	xlong y = 0;
	xlong localpointcount = 0;

	//check all vertices if protruding minimum x 
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		//inside
		if(spoint[x].x >= XMIN && spoint[y].x >= XMIN)
		{
			dpoint[localpointcount++] = spoint[y];
		}
		//*
		
		//entering
		else if(spoint[x].x >= XMIN && spoint[y].x < XMIN )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],XMIN);
		}
		//*
		
		//leaving
		else if(spoint[x].x < XMIN && spoint[y].x >= XMIN )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],XMIN);
			dpoint[localpointcount++] = spoint[y];	
		}
		//*
	}
	//*
  
	cpointcount = 0;
	
	//check all vertices if protruding maximum x 
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		//inside
		if(dpoint[x].x <= XMAX && dpoint[y].x <= XMAX)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		//*
		
		//entering
		else if(dpoint[x].x <= XMAX && dpoint[y].x > XMAX)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],XMAX);
		}
		//*
		
		//leaving
		else if(dpoint[x].x > XMAX && dpoint[y].x <= XMAX)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],XMAX);
			spoint[cpointcount++] = dpoint[y];	
		}
		//*
	}
	//*

	localpointcount = 0;
	
	//check all vertices if protruding minimum y
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		//inside
		if(spoint[x].y >= YMIN && spoint[y].y >= YMIN)
		{
			dpoint[localpointcount++] = spoint[y];
		}
		//*
		
		//entering
		else if(spoint[x].y >= YMIN && spoint[y].y < YMIN )
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],YMIN);
		}
		//*
		
		//leaving
		else if(spoint[x].y < YMIN && spoint[y].y >= YMIN)
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],YMIN);
			dpoint[localpointcount++] = spoint[y];	
		}
		//*
	}
	//*

	cpointcount = 0;
	
	//check all vertices if protruding maximum y
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		//inside
		if(dpoint[x].y <= YMAX && dpoint[y].y <= YMAX)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		//*
		
		//entering
		else if(dpoint[x].y <= YMAX && dpoint[y].y > YMAX)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],YMAX);
		}
		//*
		
		//leaving
		else if(dpoint[x].y > YMAX && dpoint[y].y <= YMAX)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],YMAX);
			spoint[cpointcount++] = dpoint[y];	
		}
		//*
	}
	//*
}

bool CLpolygon::visible() //! noncritical
{
	//calc z component of cross product of two edges (with one comon corner)
	xlong f = xlong(((spoint[cpointcount-1].x - spoint[0].x) * (spoint[1].y - spoint[0].y)) - ((spoint[cpointcount-1].y - spoint[0].y) * (spoint[1].x - spoint[0].x)));
	//*

	//poly is visible if z component of cross product is below zero (below one to rule out rundng erros)
	return( (f < 1) ? 1 : 0 );
	//*
}

void CLpolygon::shape() //! noncritical
{
	//draw wireframe of polygon
	xlong x = 0;
	xlong y = 0;
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		polyline(xlong(spoint[x].x),xlong(spoint[x].y),xlong(spoint[y].x),xlong(spoint[y].y),0x00FF0000);
	}
	//*
}

void CLpolygon::flatshade(float pz,bool ambient,bool zlight) //! critical
{
	doubleword argb = { 0 };

	float t = (normal * cllight) / ( !normal * !cllight );
	t = clmath->absolute(t);
	
	if(t > 1) { t = 1; }

	uxchar ambientlighting = 0;
	if(t<0.1)
	{
		if(ambient) { ambientlighting = 25; }
		else { shade = nolight; return; }
	}

	uxchar zlevellighting = 0; 
	if(zlight && normal.x == 0 && normal.y == 0) { zlevellighting = 100 - (pz * shadezscale); }//change == to != for "plain shading" (leave &&)
	argb.dd = color;
	//light color!!!
	argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t) + zlevellighting + ambientlighting;
	argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t) + zlevellighting + ambientlighting;
	argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t) + zlevellighting + ambientlighting;
	argb.db[3] = uxchar((float(uxchar(argb.db[3])))*t) + zlevellighting + ambientlighting;
	shade = argb.dd;
}

template<class clvector>
void CLpolygon::setside(const clvector& b, const clvector& e, screenside *s) //! critical
{
	xlong length = xlong(e.y - b.y);
	if(length<=0) { return; }

	float b_off = (b.y * XRES) + b.x;
	float e_off = (e.y * XRES) + e.x;
	float off_s = ( (e_off - b_off) / length );
	float z_s   = ( (  e.z - b.z  ) / length );
	float act_z = b.z;
	xlong act_y = xlong(b.y);

	for( ;act_y <= e.y; act_y++, b_off += off_s, act_z += z_s)
	{
		s[act_y].offset = xlong(b_off);
		s[act_y].z = act_z;
	}
}

xlong CLpolygon::circleinc(xlong x,xlong pc) const { return ( (x+1) >= pc ) ? 0 : x+1; } //! critical

xlong CLpolygon::circledec(xlong x,xlong pc) const { return ( (x-1) < 0 ) ? pc-1 : x-1; } //! critical

void CLpolygon::rasterize(xlong shadow,CLfbuffer* t) //! critical
{
	xlong x = 0;
	xlong y = 0;
	xlong top = 0;
	xlong bot = 0;

	//find top and bottom vertex
	for(x=1; x<cpointcount; x++)
	{
		if(xlong(spoint[top].y) > xlong(spoint[x].y)) top = x;
		if(xlong(spoint[bot].y) < xlong(spoint[x].y)) bot = x;
	}
	//*

	//set left side rasterizing start postion(s)
	x=y=top;
	for(y=circledec(y,cpointcount); x!=bot; x=y, y=circledec(y,cpointcount)) { setside(spoint[x],spoint[y],leftside); }
	//*
	
	//set right side rasterizing start position(s)
	x=y=top;
	for(y=circleinc(y,cpointcount); x!=bot; x=y, y=circleinc(y,cpointcount)) { setside(spoint[x],spoint[y],rightside); }
	//*

	xlong m = xlong(spoint[bot].y + spoint[top].y) >> 1;
	if(leftside[m].offset > rightside[m].offset)
	{
		//swap left and right per xor trick?

		screenside *t = leftside;
		leftside = rightside;
		rightside = t;
	}

	xlong length = 0;
	xlong offset = 0;
	float actz = 0;
	float zstep = 0;

	for(y=xlong(spoint[top].y); y<=xlong(spoint[bot].y); y++)
	{
		length = rightside[y].offset - leftside[y].offset;
		if(length != 0) zstep = ((rightside[y].z - leftside[y].z) / length);
		else zstep = 0;
		actz = leftside[y].z;
		offset = leftside[y].offset;

		switch(shadow)
		{
			//normal rasterizing
			case 0:
				while(length > 0)
				{
					if(actz < clscreen->clzbuffer[offset] || (actz==clscreen->clzbuffer[offset] &&normal.z<0) )
					{
						clscreen->cldoublebuffer[offset] = shade;
						clscreen->clzbuffer[offset] = actz;
					}
					
					offset++;
					actz += zstep;
					length--;
				}
			break;
			//*

			//shadow rasterizing
			case 1:
				while(length > 0)
				{
					clscreen->clstencilbuffer[offset] = scolor;
					
					offset++;
					length--;
				}
			break;
			//*
			
			//z rasterizing
			case 2:
				while(length > 0)
				{
					if(actz < (*t)[offset])
					{
						(*t)[offset] = actz;
					}
					
					offset++;
					actz += zstep;
					length--;
				}
			break;
			//*
		}
	}
}

CLpolygon::CLpolygon(const CLlvector& a,const CLlvector& b,const CLlvector& c,const CLlvector& d,uxlong co,uxlong sc) //! noncritical
{	
	//set colors and pointcount
	rcolor = color = co;
	scolor = sc;
	cpointcount = 4;
	//*

	//set poly vertices
	points[0] = pointr[0] = pointt[0] = a;
	points[1] = pointr[1] = pointt[1] = b;
	points[2] = pointr[2] = pointt[2] = c;
	points[3] = pointr[3] = pointt[3] = d;
	//*

	//calc normals
	normal = rnormal = CLfvector((points[1]-points[0]) * (points[2]-points[0]));
	//*
}

CLpolygon::CLpolygon(const CLpolygon& c)
{
	color  = c.color;
	rcolor = c.rcolor;
	scolor = c.scolor;
	cpointcount = c.cpointcount;
	
	pointr[0] = c.pointr[0];
	pointr[1] = c.pointr[1];
	pointr[2] = c.pointr[2];
	pointr[3] = c.pointr[3];
	points[0] = c.points[0];
	points[1] = c.points[1];
	points[2] = c.points[2];
	points[3] = c.points[3];
	pointt[0] = c.pointt[0];
	pointt[1] = c.pointt[1];
	pointt[2] = c.pointt[2];
	pointt[3] = c.pointt[3];
	
	normal = c.normal;
	rnormal = c.rnormal;
}

void CLpolygon::display(const CLlvector& p,xshort flags) //! critical
{
	if(flags&SHADOW)
	{
		ppoint[0] = pointt[0];
		ppoint[1] = pointt[1];
		ppoint[2] = pointt[2];
		ppoint[3] = pointt[3];
	}
	else if(flags&ANTICY)
	{
		ppoint[0] = points[0];
		ppoint[2] = points[1];
		ppoint[1] = points[2];
		ppoint[3] = points[3];
	}
	else
	{
		ppoint[0] = points[0];
		ppoint[1] = points[1];
		ppoint[2] = points[2];
		ppoint[3] = points[3];
	}

	ppoint[0].z += float(p.z);
	ppoint[1].z += float(p.z);
	ppoint[2].z += float(p.z);
	ppoint[3].z += float(p.z);
	zclipping();
	project(p.x,p.y,flags&CENTER);
	xyclipping();
	if(cpointcount == 0) { return; }

	if( !((flags&FLAT) || (flags&SHADOW)) ) //wireframe
	{
		shape();
	}
	else
	{
		if(visible())
		{
			if(flags&DEBUG)		//plain color
			{
				shade=color;
				rasterize(0);
			}
			else if( !(flags&SHADOW) )	//default
			{
				flatshade(ppoint[0].z,flags&AMBIENT,flags&ZLIGHT);
				rasterize(0);
			}
			else			//shadow
			{
				rasterize(1);
			}
		}
	}

}

void CLpolygon::display(const CLlvector& p,screenside* l,screenside* r,CLfbuffer* b,xlong h) //! critical
{
	screenside* backup_left = leftside;
	screenside* backup_right = rightside;

	leftside = l;
	rightside = r;
	
	//

	ppoint[0] = points[0];
	ppoint[1] = points[1];
	ppoint[2] = points[2];
	ppoint[3] = points[3];

	ppoint[0].z += float(p.z);
	ppoint[1].z += float(p.z);
	ppoint[2].z += float(p.z);
	ppoint[3].z += float(p.z);
	zclipping();
	
	for(xlong x=0; x<cpointcount; x++)
	{
		if(ppoint[x].z > 0)
		{
			spoint[x].x = float(xlong(  ppoint[x].x ) + p.x );
			spoint[x].y = float(xlong( -ppoint[x].y ) + p.y );
			spoint[x].z = ppoint[x].z; // + cleartrans;
		}
		else
		{
			tty(__func__);
			tty("Invalid z value: ");
			say(ppoint[x].z);
		}
	}
	
	xyclipping();

	if(visible() && cpointcount!=0) { rasterize(2,b); }
	//

	leftside = backup_left;
	rightside = backup_right;
}

void CLpolygon::update(CLmatrix* m,bool i=0) //! noncritical
{
	switch(i)
	{
		case 0:
			//transform temp vertices
			points[0] = m->transform(points[0]);
			points[1] = m->transform(points[1]);
			points[2] = m->transform(points[2]);
			points[3] = m->transform(points[3]);
			normal = CLfvector((points[1]-points[0]) * (points[3]-points[0]));
			//*
		break;
			
		case 1:
			//transform temp vertices for shadow casting
			pointt[0] = m->transform(points[0]);
			pointt[1] = m->transform(points[1]);
			pointt[2] = m->transform(points[2]);
			pointt[3] = m->transform(points[3]);
			//*
		break;
	}
}

void CLpolygon::partupdate(CLmatrix* m) //! noncritical
{
			points[0] = m->transform(points[0]);
			//~ points[1] = m->transform(points[1]);
			points[2] = m->transform(points[2]);
			//~ points[3] = m->transform(points[3]);
			//~ normal = CLfvector((points[1]-points[0]) * (points[3]-points[0]));	
}

template<class clvector>
void CLpolygon::add(const clvector& a) //! noncritical
{
	points[0] += a;
	points[1] += a;
	points[2] += a;
	points[3] += a;
}

void CLpolygon::reset() //! noncritical
{
	points[0] = pointr[0];
	points[1] = pointr[1];
	points[2] = pointr[2];
	points[3] = pointr[3];
	normal    = rnormal;
}

void CLpolygon::resetcolor() { color = rcolor; } //! noncritical

bool CLpolygon::isinside(CLfvector* p) const //! critical
{
	if( p->x > points[0].x && p->x < points[1].x && p->x > points[3].x && p->x < points[2].x &&
	    p->y > points[0].y && p->y > points[1].y && p->y < points[3].x && p->y < points[2].y) { return 1; }
	else { return 0; }
}
///*

#endif

