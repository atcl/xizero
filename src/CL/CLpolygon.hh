//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPOLYGON
#define HH_CLPOLYGON
//#pragma message "Compiling " __FILE__ " ! TODO: better handling of local globals, orthographic linear projection using matrix"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLmath.hh"
#include "CLcl.hh"
#include "CLvector.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLpolyinc.hh"


class CLpolygon : public virtual CLcl
{
	private:
		static xlong pointcount;
		static float shadezscale;

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
		template<class clvector>clvector getzplanecoords(const clvector& a,const clvector& b,float pz);
		template<class clvector>clvector getxplanecoords(const clvector& a,const clvector& b,float px);
		template<class clvector>clvector getyplanecoords(const clvector& a,const clvector& b,float py);
		void zclipping();
		void project(xlong px=0,xlong py=0,bool c=0);
		void xyclipping();
		bool visible();
		void shape();
		void flatshade(float pz,bool ambient,bool zlight);
		template<class clvector>void setside(const clvector& b,const clvector& e,screenside *s);
		void rasterize(xlong shadow);
		xlong circleinc(xlong x,xlong pc);
		xlong circledec(xlong x,xlong pc);

	public:
		CLpolygon(const CLlvector& a,const CLlvector& b,const CLlvector& c,const CLlvector& d,uxlong co,uxlong sc);
		~CLpolygon();
		
		void update(CLmatrix* m,bool i);
		void partupdate(CLmatrix* m);
		void display(const CLlvector& p,xshort flags);
		void display(const CLlvector& p,screenside* l,screenside* r,CLfbuffer* b,xlong h);
		template<class clvector>void add(const clvector& a);
		void reset();
		void setcolor(uxlong co);
		void resetcolor();
		CLfvector getnormal();
};

xlong CLpolygon::pointcount = 4;
float CLpolygon::shadezscale = 128/100;

void CLpolygon::polyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{
	if(x1==x2 && y1==y2) return;

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;

	if(dx<0)
	{
		dx = -dx;
		xs = -xs;
	}

	if(dy<0)
	{
		dy = -dy;
		ys = -ys;
	}

	if(dy > dx)
	{
		dx ^= dy ^= dx ^= dy;
		xs ^= ys ^= xs ^= ys;
	}

	len = dx+1;
	e = dy;

	for(uxlong i=0; i<len; i++)
	{
		(*CLdoublebuffer)[off] = c;
		off += xs;
		e += dy;
		if(e >= dx)
		{
			e -= dx;
			off += ys;
		}
	}
}

template<class clvector>
clvector CLpolygon::getzplanecoords(const clvector& a,const clvector& b,float pz) //!change float pz to template arg of clvector!
{
	float m = (pz - b.z) / (a.z - b.z);

	clvector c;
	c.x = (a.x - b.x) * m + b.x;
	c.y = (a.y - b.y) * m + b.y;
	c.z = pz;
	return c;
}

template<class clvector>
clvector CLpolygon::getxplanecoords(const clvector& a,const clvector& b,float px) //!change float pz to template arg of clvector!
{
	float m = (px - b.x) / (a.x - b.x);

	clvector c;
	c.x = xlong(px);
	c.y = xlong((a.y - b.y) * m + b.y);
	c.z = xlong((a.z - b.z) * m + b.z);
	return c;
}

template<class clvector>
clvector CLpolygon::getyplanecoords(const clvector& a,const clvector& b,float py) //!change float pz to template arg of clvector!
{
	float m = (py - b.y) / (a.y - b.y);

	clvector c;
	c.x = xlong((a.x - b.x) * m + b.x);
	c.y = xlong(py);
	c.z = xlong((a.z - b.z) * m + b.z);
	return c;
}

void CLpolygon::zclipping()
{
	xlong x = 0;
	xlong y = 0;
	xlong localpointcount = 0;

	//check all vertices if protruding minimum z 
	for(x=pointcount-1, y=0; y<pointcount; x=y, y++)
	{
		//inside
		if(ppoint[x].z >= zmin && ppoint[y].z >= zmin)
		{
			cpoint[localpointcount++] = ppoint[y];
		}
		//*
		
		//entering
		else if(ppoint[x].z >= zmin && ppoint[y].z < zmin)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],zmin);
		}
		//*
		
		//leaving
		else if(ppoint[x].z < zmin && ppoint[y].z >= zmin)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],zmin);
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
		if(cpoint[x].z <= zmax && cpoint[y].z <= zmax)
		{
			ppoint[cpointcount++] = cpoint[y];
		}
		//*
		
		//entering
		else if(cpoint[x].z <= zmax && cpoint[y].z > zmax)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],zmax);
		}
		//*
		
		//leaving
		else if(cpoint[x].z > zmax && cpoint[y].z <= zmax)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],zmax);
			cpoint[cpointcount++] = cpoint[y];
		}
		//*
	}
	//*
}

void CLpolygon::project(xlong px,xlong py,bool c)
{
	//use screen center as attached position if wanted
	if(c)
	{
		px = (xres>>1);
		py = (yres>>1);
	}
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



void CLpolygon::xyclipping()
{
	xlong x = 0;
	xlong y = 0;
	xlong localpointcount = 0;

	//check all vertices if protruding minimum x 
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		//inside
		if(spoint[x].x >= xmin && spoint[y].x >= xmin)
		{
			dpoint[localpointcount++] = spoint[y];
		}
		//*
		
		//entering
		else if(spoint[x].x >= xmin && spoint[y].x < xmin )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],xmin);
		}
		//*
		
		//leaving
		else if(spoint[x].x < xmin && spoint[y].x >= xmin )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],xmin);
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
		if(dpoint[x].x <= xmax && dpoint[y].x <= xmax)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		//*
		
		//entering
		else if(dpoint[x].x <= xmax && dpoint[y].x > xmax)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],xmax);
		}
		//*
		
		//leaving
		else if(dpoint[x].x > xmax && dpoint[y].x <= xmax)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],xmax);
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
		if(spoint[x].y >= ymin && spoint[y].y >= ymin)
		{
			dpoint[localpointcount++] = spoint[y];
		}
		//*
		
		//entering
		else if(spoint[x].y >= ymin && spoint[y].y < ymin )
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],ymin);
		}
		//*
		
		//leaving
		else if(spoint[x].y < ymin && spoint[y].y >= ymin)
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],ymin);
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
		if(dpoint[x].y <= ymax && dpoint[y].y <= ymax)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		//*
		
		//entering
		else if(dpoint[x].y <= ymax && dpoint[y].y > ymax)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],ymax);
		}
		//*
		
		//leaving
		else if(dpoint[x].y > ymax && dpoint[y].y <= ymax)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],ymax);
			spoint[cpointcount++] = dpoint[y];	
		}
		//*
	}
	//*
}

bool CLpolygon::visible()
{
	//calc z component of cross product of two edges (with one comon corner)
	xlong f = xlong(((spoint[cpointcount-1].x - spoint[0].x) * (spoint[1].y - spoint[0].y)) - ((spoint[cpointcount-1].y - spoint[0].y) * (spoint[1].x - spoint[0].x)));
	//*

	//poly is visible if z component of cross product is below zero (below one to rule out rundng erros)
	return( (f < 1) ? 1 : 0 );
	//*
}

void CLpolygon::shape()
{
	//draw wireframe of polygon
	xlong x = 0;
	xlong y = 0;
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		polyline(xlong(spoint[x].x),xlong(spoint[x].y),xlong(spoint[y].x),xlong(spoint[y].y),0x000000FF);
	}
	//*
}

void CLpolygon::flatshade(float pz,bool ambient,bool zlight)
{
	doubleword argb = { 0 };

	float t = (normal * cllight) / ( !normal * !cllight );
	t = CLmath::absolute(t);
	
	if(t > 1) t = 1;

	uxchar ambientlighting = 0;
	if(t<0.1)
	{
		switch(ambient)
		{
			case false:
				shade = nolight;
			return;
			
			case true:
				ambientlighting = 25;
			break;
		}
	}

	uxchar zlevellighting = 0; 
	if(zlight && normal.x == 0 && normal.y == 0) zlevellighting = 128 - (pz * shadezscale); //change == to != for "plain shading" (leave &&)
	argb.dd = color;
	//light color!!!
	argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t) + zlevellighting + ambientlighting;
	argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t) + zlevellighting + ambientlighting;
	argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t) + zlevellighting + ambientlighting;
	shade = argb.dd;
}

template<class clvector>
void CLpolygon::setside(const clvector& b, const clvector& e, screenside *s)
{
	xlong length = xlong(e.y - b.y);
	if(length <= 0) return;

	float b_off = (b.y * xres) + b.x;
	float e_off = (e.y * xres) + e.x;
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

xlong CLpolygon::circleinc(xlong x,xlong pc)
{
	return ( (x+1) >= pc ) ? 0 : x+1;
}

xlong CLpolygon::circledec(xlong x,xlong pc)
{
	return ( (x-1) < 0 ) ? pc-1 : x-1;
}

void CLpolygon::rasterize(xlong shadow)
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
	for(y=circledec(y,cpointcount); x!=bot; x=y, y=circledec(y,cpointcount))
	{
		setside(spoint[x],spoint[y],leftside);
	}
	//*
	
	//set right side rasterizing start position(s)
	x=y=top;
	for(y=circleinc(y,cpointcount); x!=bot; x=y, y=circleinc(y,cpointcount))
	{
		setside(spoint[x],spoint[y],rightside);
	}
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
					if(actz < (*CLzbuffer)[offset] || (actz==(*CLzbuffer)[offset] &&normal.z<0) )
					{
						(*CLdoublebuffer)[offset] = shade;
						(*CLzbuffer)[offset] = actz;
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
					(*CLstencilbuffer)[offset] = scolor;
					
					offset++;
					length--;
				}
			break;
			//*
			
			//z rasterizing
			case 2:
				while(length > 0)
				{
					if(actz < (*CLzbuffer)[offset])
					{
						(*CLzbuffer)[offset] = actz;
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

CLpolygon::CLpolygon(const CLlvector& a,const CLlvector& b,const CLlvector& c,const CLlvector& d,uxlong co,uxlong sc)
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

CLpolygon::~CLpolygon() { }

void CLpolygon::display(const CLlvector& p,xshort flags)
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
	if(cpointcount == 0) return;

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
				flatshade(p.z,flags&AMBIENT,flags&ZLIGHT);
				rasterize(0);
			}
			else			//shadow
			{
				rasterize(1);
			}
		}
	}

}

void CLpolygon::display(const CLlvector& p,screenside* l,screenside* r,CLfbuffer* b,xlong h)
{
	screenside* backup_left = leftside;
	screenside* backup_right = rightside;
	CLfbuffer* backup_zbuffer = CLzbuffer;
	xlong backup_ymax = ymax;

	leftside = l;
	rightside = r;
	CLzbuffer = b;
	ymax = h-1;
	
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
			CLsystem::exit(1,0,__func__,"Invalid z value: ",ppoint[x].z);
		}
	}
	
	xyclipping();

	if(visible() && cpointcount!=0)
	{
		rasterize(2);
	}

	//

	leftside = backup_left;
	rightside = backup_right;
	CLzbuffer = backup_zbuffer;
	ymax = backup_ymax;
}

void CLpolygon::update(CLmatrix* m,bool i=0)
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

void CLpolygon::partupdate(CLmatrix* m)
{
			points[0] = m->transform(points[0]);
			//~ points[1] = m->transform(points[1]);
			points[2] = m->transform(points[2]);
			//~ points[3] = m->transform(points[3]);
			//~ normal = CLfvector((points[1]-points[0]) * (points[3]-points[0]));	
}

template<class clvector>
void CLpolygon::add(const clvector& a)
{
	points[0] += a;
	points[1] += a;
	points[2] += a;
	points[3] += a;
}

void CLpolygon::reset()
{
	points[0] = pointr[0];
	points[1] = pointr[1];
	points[2] = pointr[2];
	points[3] = pointr[3];
	normal    = rnormal;
}

void CLpolygon::setcolor(uxlong co)
{
	color = co;
}

void CLpolygon::resetcolor()
{
	color = rcolor;
}

CLfvector CLpolygon::getnormal()
{
	return normal;
}

#endif

