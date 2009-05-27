//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPOLYGON
#define HH_CLPOLYGON
#pragma message "Compiling " __FILE__ " ! TODO: better handling of local globals"

#include <iostream>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLpolyinc.hh"
#include "CLapi.hh"
#include "CLmath.hh"
#include "CLlight.hh"
#include "CLstruct.hh"
#include "CLvector.hh"


class CLpolygon : public virtual CLcl
{
	protected:
		CLmath* clmath;
		CLlight* cllight;

		CLbuffer<xlong>* doublebuffer;
		CLbuffer<float>* zbuffer;
		CLbuffer<xlong>* sbuffer;

	private:
		static xlong pointcount;
		uxlong color;
		uxlong shade;
		uxlong scolor;
		xlong cpointcount;
		fvertex pointr[4];
		fvertex points[4];
		fvertex pointt[4];
		vector normal;
		vector rnormal;
		xlong active;
		
		void polyline(uxlong x1,uxlong y1,uxlong x2,uxlong y2,uxlong c);	//todo: swap
		fvertex getzplanecoords(fvertex a,fvertex b,float pz);
		fvertex getxplanecoords(fvertex a,fvertex b,float px);
		fvertex getyplanecoords(fvertex a,fvertex b,float py);
		void zclipping();
		void project(xlong px,xlong py,xlong projconstx,xlong projconsty,bool center);
		void xyclipping();
		bool visible();
		void shape();
		void flatshade(bool ambient);
		void setside(fvertex b,fvertex e,screenside *s);
		void rasterize(bool shadow);	//todo: swap
		xlong circleinc(xlong x,xlong pc);
		xlong circledec(xlong x,xlong pc);
	
	public:
		CLpolygon(CLbuffer<xlong>* db,CLbuffer<float>* zb,CLbuffer<xlong>* sb,xlong ax,xlong ay,xlong az,xlong bx,xlong by,xlong bz,xlong cx,xlong cy,xlong cz,xlong dx,xlong dy,xlong dz,uxlong co,uxlong sc,CLmath* clm,CLlight* cll);
		~CLpolygon();
		void update(CLmatrix* m,bool i);
		void display(xlong px,xlong py,xlong pz,bool center,bool flat,bool ambient,bool shadow,bool pixelshader,bool debug);
		vector getnormal();
		void add(xlong x,xlong y,xlong z);
		void reset();
		void setcolor(xlong c);
		uxlong getcolor();
		void setactive(xlong b);
		xlong getactive();
};

xlong CLpolygon::pointcount = 4;

void CLpolygon::polyline(uxlong x1,uxlong y1,uxlong x2,uxlong y2,uxlong c)
{
	if(x1==x2 && y1==y2) return;

	xlong dx = x2 - x1;
	xlong dy = y2 - y1;
	xlong e;
	xlong xs = 1;
	xlong ys = xres;
	xlong len;
	xlong off = y1*xres+x1;
	xlong temp;

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

// 		temp = dy;
// 		dy = dx;
// 		dx = temp;
// 
// 		temp = ys;
// 		ys = xs;
// 		xs = temp;
	}

	len = dx+1;
	e = dy;

	for(int i=0; i<len; i++)
	{
		(*doublebuffer)[off] = c;
		off += xs;
		e += dy;
		if(e >= dx)
		{
			e -= dx;
			off += ys;
		}
	}
}

fvertex CLpolygon::getzplanecoords(fvertex a, fvertex b, float pz)
{
	float m = (pz - b.z) / (a.z - b.z);

	fvertex c;
	c.x = (a.x - b.x) * m + b.x;
	c.y = (a.y - b.y) * m + b.y;
	c.z = pz;

	return c;
}

fvertex CLpolygon::getxplanecoords(fvertex a, fvertex b, float px)
{
	float m = (px - b.x) / (a.x - b.x);

	fvertex c;
	c.x = px;
	c.y = (a.y - b.y) * m + b.y;
	c.z = (a.z - b.z) * m + b.z;
	
	return c;
}

fvertex CLpolygon::getyplanecoords(fvertex a, fvertex b, float py)
{
	float m = (py - b.y) / (a.y - b.y);

	fvertex c;
	c.x = (a.x - b.x) * m + b.x;
	c.y = py;
	c.z = (a.z - b.z) * m + b.z;
	
	return c;
}

void CLpolygon::zclipping()
{
	xlong x;
	xlong y;
	xlong localpointcount = 0;

	for(x=pointcount-1, y=0; y<pointcount; x=y, y++)
	{
		if(ppoint[x].z >= zmin && ppoint[y].z >= zmin)
		{
			cpoint[localpointcount++] = ppoint[y];
		}
		else if(ppoint[x].z >= zmin && ppoint[y].z < zmin)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],zmin);
		}
		else if(ppoint[x].z < zmin && ppoint[y].z >= zmin)
		{
			cpoint[localpointcount++] = getzplanecoords(ppoint[x],ppoint[y],zmin);
			cpoint[localpointcount++] = ppoint[y];
		}
	}

	cpointcount = 0;
	
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		if(cpoint[x].z <= zmax && cpoint[y].z <= zmax)
		{
			ppoint[cpointcount++] = cpoint[y];
		}
		else if(cpoint[x].z <= zmax && cpoint[y].z > zmax)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],zmax);
		}
		else if(cpoint[x].z > zmax && cpoint[y].z <= zmax)
		{
			cpoint[cpointcount++] = getzplanecoords(cpoint[x],cpoint[y],zmax);
			cpoint[cpointcount++] = cpoint[y];
		}
	}
}

void CLpolygon::project(xlong px,xlong py,xlong projconstx,xlong projconsty,bool center)
{
	if(center==true)
	{
		px = xres >> 1;
		py = yres >> 1;
	}

	for(xlong x=0; x<cpointcount; x++)
	{
		if(ppoint[x].z > 0)
		{
			spoint[x].x = float(xlong( ( 80 * ppoint[x].x) / (ppoint[x].z) ) + px); //95 wenn xclipping läuft
			spoint[x].y = float(xlong( (-95 * ppoint[x].y) / (ppoint[x].z) ) + py);
			spoint[x].z = ppoint[x].z + cleartrans;
		}
		else
		{
			CLexit_(1,__func__,"Invalid z value: ",ppoint[x].z);
		}
	}
}

void CLpolygon::xyclipping()
{
	xlong x;
	xlong y;
	xlong localpointcount = 0;

	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		if(spoint[x].x >= xmin && spoint[y].x >= xmin)
		{
			dpoint[localpointcount++] = spoint[y];
		}  
		else if(spoint[x].x >= xmin && spoint[y].x < xmin )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],xmin);
		}  
		else if(spoint[x].x < xmin && spoint[y].x >= xmin )
		{
			dpoint[localpointcount++] = getxplanecoords(spoint[x],spoint[y],xmin);
			dpoint[localpointcount++] = spoint[y];	
		}
	}
  
	cpointcount = 0;
	
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		if(dpoint[x].x <= xmax && dpoint[y].x <= xmax)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		else if(dpoint[x].x <= xmax && dpoint[y].x > xmax)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],xmax);
		}
		else if(dpoint[x].x > xmax && dpoint[y].x <= xmax)
		{
			spoint[cpointcount++] = getxplanecoords(dpoint[x],dpoint[y],xmax);
			spoint[cpointcount++] = dpoint[y];	
		}
	}

	localpointcount = 0;
	
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		if(spoint[x].y >= ymin && spoint[y].y >= ymin)
		{
			dpoint[localpointcount++] = spoint[y];
		}
		else if(spoint[x].y >= ymin && spoint[y].y < ymin )
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],ymin);
		}
		else if(spoint[x].y < ymin && spoint[y].y >= ymin)
		{
			dpoint[localpointcount++] = getyplanecoords(spoint[x],spoint[y],ymin);
			dpoint[localpointcount++] = spoint[y];	
		}
	}

	cpointcount = 0;
	
	for(x=localpointcount-1, y=0; y<localpointcount; x=y, y++)
	{
		if(dpoint[x].y <= ymax && dpoint[y].y <= ymax)
		{
			spoint[cpointcount++] = dpoint[y];
		}
		else if(dpoint[x].y <= ymax && dpoint[y].y > ymax)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],ymax);
		}
		else if(dpoint[x].y > ymax && dpoint[y].y <= ymax)
		{
			spoint[cpointcount++] = getyplanecoords(dpoint[x],dpoint[y],ymax);
			spoint[cpointcount++] = dpoint[y];	
		}
	}
}

bool CLpolygon::visible()
{
		xlong f = xlong(((spoint[cpointcount-1].x - spoint[0].x) * (spoint[1].y - spoint[0].y)) - ((spoint[cpointcount-1].y - spoint[0].y) * (spoint[1].x - spoint[0].x)));
		
		 return( f < 1L ? 1 : 0 );
}

void CLpolygon::shape()
{
	xlong x;
	xlong y;
	for(x=cpointcount-1, y=0; y<cpointcount; x=y, y++)
	{
		polyline(xlong(spoint[x].x),xlong(spoint[x].y),xlong(spoint[y].x),xlong(spoint[y].y),0x000000FF);
	}
	
}

void CLpolygon::flatshade(bool ambient)
{
	uxlong d;
	doubleword argb;
	fvector tlight = cllight->getlight();

	//float t = clmath->dotproduct(normal,tlight) / (normal.l * tlight.l);
	float t = ((normal.x*tlight.x) + (normal.y*tlight.y) + (normal.z*tlight.z)) / (normal.l*tlight.l);
	if(t < 0) t *= -1;
	if(t > 1) t = 1;
	if(t < 0.2 && ambient==false)
	{
		shade = nolight;
		return;
		
	}

	argb.dd = color;
	//light color!!!
	argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t);
	argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t);
	argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t);
	shade = argb.dd;

	if(ambient==true)
	{
		if(t<0.2)
		{
		argb.db[0] += 25;
		argb.db[1] += 25;
		argb.db[2] += 25;
		shade  = argb.dd;
		}
	}
}

void CLpolygon::setside(fvertex b, fvertex e, screenside *s)
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
	if( (x+1) >= pc)
	{
		return 0;
	}
	else
	{
		return x+1;
	}
}

xlong CLpolygon::circledec(xlong x,xlong pc)
{
	if( (x-1) < 0)
	{
		return pc-1;
	}
	else
	{
		return x-1;
	}
}

void CLpolygon::rasterize(bool shadow)
{
	xlong x;
	xlong y;
	xlong top = 0;
	xlong bot = 0;

	for(x=1; x<cpointcount; x++)
	{
		if(xlong(spoint[top].y) > xlong(spoint[x].y)) top = x;
		if(xlong(spoint[bot].y) < xlong(spoint[x].y)) bot = x;
	}

	x=y=top;
	for(y=circledec(y,cpointcount); x!=bot; x=y, y=circledec(y,cpointcount))
	{
		setside(spoint[x],spoint[y],leftside);
	}
	
	x=y=top;
	for(y=circleinc(y,cpointcount); x!=bot; x=y, y=circleinc(y,cpointcount))
	{
		setside(spoint[x],spoint[y],rightside);
	}

	xlong m = xlong(spoint[bot].y + spoint[top].y) >> 1;
	if(leftside[m].offset > rightside[m].offset)
	{
		//swap left and right per xor trick
		screenside *t = leftside;
		leftside = rightside;
		rightside = t;
	}

	xlong length;
	xlong offset;
	float actz;
	float zstep;
	for(y=xlong(spoint[top].y); y<=xlong(spoint[bot].y); y++)
	{
		length = rightside[y].offset - leftside[y].offset;
		if(length != 0) zstep = ((rightside[y].z - leftside[y].z) / length);
		else zstep = 0; //or 1 decide later
		actz = leftside[y].z;
		offset = leftside[y].offset;

		if(shadow==0)
		{
			while(length > 0)
			{
				if(actz < (*zbuffer)[offset])
				{
					(*doublebuffer)[offset] = shade;
					(*zbuffer)[offset] = actz;
				}
				
				offset++;
				actz += zstep;
				length--;
			}
		}
		if(shadow==1)
		{
			while(length > 0)
			{
				(*sbuffer)[offset] = scolor;
				
				offset++;
				length--;
			}
		}
	}
}

CLpolygon::CLpolygon(CLbuffer<xlong>* db,CLbuffer<float>* zb,CLbuffer<xlong>* sb,xlong ax,xlong ay,xlong az,xlong bx,xlong by,xlong bz,xlong cx,xlong cy,xlong cz,xlong dx,xlong dy,xlong dz,uxlong co,uxlong sc,CLmath* clm,CLlight* cll)
{
	clmath = clm;
	cllight = cll;

	color = co;
	scolor = sc;
	doublebuffer = db;
	zbuffer = zb;
	sbuffer = sb;
	cpointcount = 4;
	
	points[0].x = float(ax);
	points[0].y = float(ay);
	points[0].z = float(az);
	points[1].x = float(bx);
	points[1].y = float(by);
	points[1].z = float(bz);
	points[2].x = float(cx);
	points[2].y = float(cy);
	points[2].z = float(cz);
	points[3].x = float(dx);
	points[3].y = float(dy);
	points[3].z = float(dz);

	pointr[0].x = float(ax);
	pointr[0].y = float(ay);
	pointr[0].z = float(az);
	pointr[1].x = float(bx);
	pointr[1].y = float(by);
	pointr[1].z = float(bz);
	pointr[2].x = float(cx);
	pointr[2].y = float(cy);
	pointr[2].z = float(cz);
	pointr[3].x = float(dx);
	pointr[3].y = float(dy);
	pointr[3].z = float(dz);

	vector r;
	vector s;
	r.x = (bx - ax);
	r.y = (by - ay);
	r.z = (bz - az);
	s.x = (cx - ax);
	s.y = (cy - ay);
	s.z = (cz - az);

	//normal = clmath->crossproduct(r,s);

	normal.x = ( (r.y * s.z) - (r.z * s.y) );
	normal.y = ( (r.z * s.x) - (r.x * s.z) );
	normal.z = ( (r.x * s.y) - (r.y * s.x) );
	normal.l = clmath->vectorlength(normal);

	rnormal.x = normal.x;
	rnormal.y = normal.y;
	rnormal.z = normal.z;
	rnormal.l = normal.l;
}

CLpolygon::~CLpolygon() { }

void CLpolygon::display(xlong px,xlong py,xlong pz,bool center,bool flat,bool ambient,bool shadow,bool pixelshader,bool debug)
{
	if(shadow==0)
	{
		ppoint[0] = points[0];
		ppoint[1] = points[1];
		ppoint[2] = points[2];
		ppoint[3] = points[3];
	}
	else
	{
		ppoint[0] = pointt[0];
		ppoint[1] = pointt[1];
		ppoint[2] = pointt[2];
		ppoint[3] = pointt[3];
	}

	ppoint[0].z += float(pz);
	ppoint[1].z += float(pz);
	ppoint[2].z += float(pz);
	ppoint[3].z += float(pz);

	zclipping();
	project(px,py,0,0,center);
	xyclipping();
	if(cpointcount == 0) return;
	
	if(flat==false && shadow==0) //wireframe
	{
		shape();
	}
		
	else
	{
		if(visible())
		{
			if(debug==true)
			{
				shade=color;
				rasterize(0);
				shape();
			}
			if(shadow==false)	//default
			{
				flatshade(ambient);
				rasterize(0);
			}
			else			//shadow
			{
				rasterize(1);
			}
		}
	}
}

void CLpolygon::update(CLmatrix* m,bool i=0)
{

	if(i==0)
	{
		points[0] = m->transform(points[0]);
		points[1] = m->transform(points[1]);
		points[2] = m->transform(points[2]);
		points[3] = m->transform(points[3]);
		normal = m->transform(normal);
	}
	if(i==1)
	{
		pointt[0] = m->transform(points[0]);
		pointt[1] = m->transform(points[1]);
		pointt[2] = m->transform(points[2]);
		pointt[3] = m->transform(points[3]);
	}
}

vector CLpolygon::getnormal()
{
	return normal;
}

void CLpolygon::add(xlong x,xlong y,xlong z)
{
	if(x!=0)
	{
		float fx = float(x);
		points[0].x += fx;
		points[1].x += fx;
		points[2].x += fx;
		points[3].x += fx;
	}

	if(y!=0)
	{
		float fy = float(y);
		points[0].y += fy;
		points[1].y += fy;
		points[2].y += fy;
		points[3].y += fy;
	}

	if(z!=0)
	{
		float fz = float(z);
		points[0].z += fz;
		points[1].z += fz;
		points[2].z += fz;
		points[3].z += fz;
	}
}

void CLpolygon::reset()
{
	points[0].x = pointr[0].x;
	points[0].y = pointr[0].y;
	points[0].z = pointr[0].z;
	points[1].x = pointr[1].x;
	points[1].y = pointr[1].y;
	points[1].z = pointr[1].z;
	points[2].x = pointr[2].x;
	points[2].y = pointr[2].y;
	points[2].z = pointr[2].z;
	points[3].x = pointr[3].x;
	points[3].y = pointr[3].y;
	points[3].z = pointr[3].z;

	normal.x = rnormal.x;
	normal.y = rnormal.y;
	normal.z = rnormal.z;
	normal.l = rnormal.l;
}

void CLpolygon::setcolor(xlong c)
{
	color = c;
}

uxlong CLpolygon::getcolor()
{
	return color;
}

void CLpolygon::setactive(xlong b)
{
	if(b!=0) active=1;
	else active=0;
}

xlong CLpolygon::getactive()
{
	return active;
}

#endif

