//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLOBJECT
#define HH_CLOBJECT
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLmath.hh"
#include "CLcl.hh"
#include "CLvector.hh"
#include "CLbuffer.hh"
#include "CLglobal.hh"
#include "CLpolygon.hh"
#include "CLmatrix.hh"
#include "CLlight.hh"



class CLobject : public virtual CLcl
{
	protected:
		CLpolygon** polyptr;

	private:
		xlong vertexcount;
		xlong polycount;
		xlong dockcount;
		xlong vertexptr;
		CLlvector** dockptr;
		CLlvector position;
		CLlvector rposition;
		CLbox* boundingbox;
		xlong name;
	
	public:
		CLobject(CLfile* fileptr,xlong x,xlong y,xlong z,bool zs);
		~CLobject();
		
		void update(CLmatrix* m);
		void display(xchar flags);
		void display(screenside* l,screenside* r,CLfbuffer* b,xlong h);
		CLlvector getposition();
		xlong getpositionx();
		xlong getpositiony();
		xlong getpositionz();
		void setposition(xlong x,xlong y,xlong z);
		void setpositionx(xlong x);
		void setpositiony(xlong y);
		void setpositionz(xlong z);
		void addposition(xlong x,xlong y,xlong z);
		xlong getname();
		CLlvector* getdockingpoint(xlong i);
		CLlvector* getdockingpoint(xlong t,xlong i);
		void translatealongnormals(float size);
		CLbox* getboundingbox();
		void reset();
		void setminz(xlong z);
		xlong getminz();
};

CLobject::CLobject(CLfile* fileptr,xlong x,xlong y,xlong z,bool zs)
{
	position.x = rposition.x = x;
	position.y = rposition.y = y;
	position.z = rposition.z = z;

	boundingbox = new CLbox;
	boundingbox->t1 = 0;
	boundingbox->t2 = 0;
	boundingbox->t3 = 0;
	boundingbox->t4 = 0;
	boundingbox->b1 = 0;
	boundingbox->b2 = 0;
	boundingbox->b3 = 0;
	boundingbox->b4 = 0;

	xlong* dataptr = fileptr->data;

	xlong sobjcount = 0;

	xlong sobjcounter = 0;
	xlong polycounter = 0;
	xlong dockcounter = 0;

	xlong localpolycount = 0;
	xlong localdockcount = 0;
	short localdocktype = 0;
	uxlong localcolor = 0;

	doubleword s = { 0 };

	xlong xoff = 0;
	xlong yoff = 0;
	xlong zoff = 0;

	xlong d = 0;
	CLlvector t[4];

	xlong zshift = 2;
	if(zs==0) zshift = 0;

	if(dataptr[0] != '<CLY') CLsystem::CLexit_(1,0,__func__,"wrong y3d format, may be endianess?");

	if(dataptr[1] == '3DB>')
	{
		polycount = dataptr[2];
		polyptr = new CLpolygon*[polycount];
		//dataptr[3] is empty

		//dataptr[4] = "OBJT"
		if(dataptr[4] != 'OBJT' ) CLsystem::CLexit_(1,0,__func__,"No OBJT tag");
		name = dataptr[5];
		sobjcount = dataptr[6];
		dockcount = dataptr[7];
		dockptr = new CLlvector*[dockcount];

		d = 8;

		for(int i=0;i<sobjcount;i++)
		{
			if(dataptr[d] != 'SOBJ' ) CLsystem::CLexit_(1,0,__func__,"No SOBJ tag");
			d++; //"SOBJ"
			d++; //subobject identifier
			localpolycount = dataptr[d]; d++;
			localdockcount = dataptr[d]; d++;

			if(dataptr[d] != 'CONN' ) CLsystem::CLexit_(1,0,__func__,"No CONN tag");
			d++; //"CONN"
			xoff = dataptr[d]; d++;
			yoff = dataptr[d]; d++;
			zoff = dataptr[d]; d++;

			for(int j=0;j<localpolycount;j++,polycounter++)
			{
				if(dataptr[d] != 'POLY' ) CLsystem::CLexit_(1,0,__func__,"No POLY tag");
				d++; //"POLY"
				d++; //identifier
				localcolor = dataptr[d]; d++; //color
				d++; //0

				if(dataptr[d] != 'VECT' ) CLsystem::CLexit_(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[0].x = dataptr[d] + xoff; d++; //x1
				t[0].y = dataptr[d] + yoff; d++; //y1
				t[0].z = (dataptr[d]>>zshift) + zoff; d++; //z1

				//bounding box generation
				if( t[0].x < boundingbox->t1.x) { boundingbox->t1.x = t[0].x; boundingbox->b1.x = t[0].x; boundingbox->t4.x = t[0].x; boundingbox->b4.x = t[0].x; } 
				if( t[0].x > boundingbox->b2.x) { boundingbox->t2.x = t[0].x; boundingbox->b2.x = t[0].x; boundingbox->t3.x = t[0].x; boundingbox->b3.x = t[0].x; } 
				if( t[0].y < boundingbox->t1.y) { boundingbox->t1.y = t[0].y; boundingbox->b1.y = t[0].y; boundingbox->t2.y = t[0].y; boundingbox->b2.y = t[0].y; } 
				if( t[0].y > boundingbox->b3.y) { boundingbox->t3.y = t[0].y; boundingbox->b3.y = t[0].y; boundingbox->t4.y = t[0].y; boundingbox->b4.y = t[0].y; } 
				if( t[0].z < boundingbox->t1.z) { boundingbox->t1.z = t[0].z; boundingbox->t2.z = t[0].z; boundingbox->t3.z = t[0].z; boundingbox->t4.z = t[0].z; } 
				if( t[0].z > boundingbox->b1.z) { boundingbox->b1.z = t[0].z; boundingbox->b2.z = t[0].z; boundingbox->b3.z = t[0].z; boundingbox->b4.z = t[0].z; } 
				//*

				if(dataptr[d] != 'VECT' ) CLsystem::CLexit_(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[1].x = dataptr[d] + xoff; d++; //x2
				t[1].y = dataptr[d] + yoff; d++; //y2
				t[1].z = (dataptr[d]>>zshift) + zoff; d++; //z2

				//bounding box generation
				if( t[1].x < boundingbox->t1.x) { boundingbox->t1.x = t[1].x; boundingbox->b1.x = t[1].x; boundingbox->t4.x = t[1].x; boundingbox->b4.x = t[1].x; } 
				if( t[1].x > boundingbox->b2.x) { boundingbox->t2.x = t[1].x; boundingbox->b2.x = t[1].x; boundingbox->t3.x = t[1].x; boundingbox->b3.x = t[1].x; } 
				if( t[1].y < boundingbox->t1.y) { boundingbox->t1.y = t[1].y; boundingbox->b1.y = t[1].y; boundingbox->t2.y = t[1].y; boundingbox->b2.y = t[1].y; } 
				if( t[1].y > boundingbox->b3.y) { boundingbox->t3.y = t[1].y; boundingbox->b3.y = t[1].y; boundingbox->t4.y = t[1].y; boundingbox->b4.y = t[1].y; } 
				if( t[1].z < boundingbox->t1.z) { boundingbox->t1.z = t[1].z; boundingbox->t2.z = t[1].z; boundingbox->t3.z = t[1].z; boundingbox->t4.z = t[1].z; } 
				if( t[1].z > boundingbox->b1.z) { boundingbox->b1.z = t[1].z; boundingbox->b2.z = t[1].z; boundingbox->b3.z = t[1].z; boundingbox->b4.z = t[1].z; } 
				//*

				if(dataptr[d] != 'VECT' ) CLsystem::CLexit_(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[2].x = dataptr[d] + xoff; d++; //x3
				t[2].y = dataptr[d] + yoff; d++; //y3
				t[2].z = (dataptr[d]>>zshift) + zoff; d++; //z3

				//bounding box generation
				if( t[2].x < boundingbox->t1.x) { boundingbox->t1.x = t[2].x; boundingbox->b1.x = t[2].x; boundingbox->t4.x = t[2].x; boundingbox->b4.x = t[2].x; } 
				if( t[2].x > boundingbox->b2.x) { boundingbox->t2.x = t[2].x; boundingbox->b2.x = t[2].x; boundingbox->t3.x = t[2].x; boundingbox->b3.x = t[2].x; } 
				if( t[2].y < boundingbox->t1.y) { boundingbox->t1.y = t[2].y; boundingbox->b1.y = t[2].y; boundingbox->t2.y = t[2].y; boundingbox->b2.y = t[2].y; } 
				if( t[2].y > boundingbox->b3.y) { boundingbox->t3.y = t[2].y; boundingbox->b3.y = t[2].y; boundingbox->t4.y = t[2].y; boundingbox->b4.y = t[2].y; } 
				if( t[2].z < boundingbox->t1.z) { boundingbox->t1.z = t[2].z; boundingbox->t2.z = t[2].z; boundingbox->t3.z = t[2].z; boundingbox->t4.z = t[2].z; } 
				if( t[2].z > boundingbox->b1.z) { boundingbox->b1.z = t[2].z; boundingbox->b2.z = t[2].z; boundingbox->b3.z = t[2].z; boundingbox->b4.z = t[2].z; } 
				//*

				if(dataptr[d] != 'VECT' ) CLsystem::CLexit_(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[3].x = dataptr[d] + xoff; d++; //x4
				t[3].y = dataptr[d] + yoff; d++; //y4
				t[3].z = (dataptr[d]>>zshift) + zoff; d++; //z4

				//bounding box generation
				if( t[3].x < boundingbox->t1.x) { boundingbox->t1.x = t[3].x; boundingbox->b1.x = t[3].x; boundingbox->t4.x = t[3].x; boundingbox->b4.x = t[3].x; } 
				if( t[3].x > boundingbox->b2.x) { boundingbox->t2.x = t[3].x; boundingbox->b2.x = t[3].x; boundingbox->t3.x = t[3].x; boundingbox->b3.x = t[3].x; } 
				if( t[3].y < boundingbox->t1.y) { boundingbox->t1.y = t[3].y; boundingbox->b1.y = t[3].y; boundingbox->t2.y = t[3].y; boundingbox->b2.y = t[3].y; } 
				if( t[3].y > boundingbox->b3.y) { boundingbox->t3.y = t[3].y; boundingbox->b3.y = t[3].y; boundingbox->t4.y = t[3].y; boundingbox->b4.y = t[3].y; } 
				if( t[3].z < boundingbox->t1.z) { boundingbox->t1.z = t[3].z; boundingbox->t2.z = t[3].z; boundingbox->t3.z = t[3].z; boundingbox->t4.z = t[3].y; } 
				if( t[3].z > boundingbox->b1.z) { boundingbox->b1.z = t[3].z; boundingbox->b2.z = t[3].z; boundingbox->b3.z = t[3].z; boundingbox->b4.z = t[3].y; } 
				//*

				polyptr[polycounter] = new CLpolygon(t[0],t[1],t[2],t[3],localcolor,0x000000C0);
			}

			for(int k=0;k<localdockcount;k++,dockcounter++)
			{
				s.dd = dataptr[d]; d++; //"DP"+docktype
				localdocktype = s.dw[1];
				
				t[0].x = dataptr[d] + xoff; d++; //dx
				t[0].y = dataptr[d] + yoff; d++; //dy
				t[0].z = (dataptr[d]>>zshift) + zoff; d++; //dz

				dockptr[dockcounter] = new CLlvector;
				dockptr[dockcounter]->x = t[0].x;
				dockptr[dockcounter]->y = t[0].y;
				dockptr[dockcounter]->z = t[0].z;
				dockptr[dockcounter]->e = xlong(localdocktype);
			}
		}

		//"ENDO"

	}
	else if(dataptr[1] == 'D_X>')
	{

	}
	else if(dataptr[1] == 'D_T>')
	{

	}
}

CLobject::~CLobject()
{
	delete boundingbox;
}

void CLobject::update(CLmatrix* m)
{
	for(int i=0;i<polycount;i++)
	{
		polyptr[i]->update(m,0);
	}

	for(int j=0;j<dockcount;j++)
	{
		*dockptr[j] = m->transform(*dockptr[j]);
	}

	boundingbox->t1 = m->transform(boundingbox->t1);
	boundingbox->t2 = m->transform(boundingbox->t2);
	boundingbox->t3 = m->transform(boundingbox->t3);
	boundingbox->t4 = m->transform(boundingbox->t4);
	boundingbox->b1 = m->transform(boundingbox->b1);
	boundingbox->b2 = m->transform(boundingbox->b2);
	boundingbox->b3 = m->transform(boundingbox->b3);
	boundingbox->b4 = m->transform(boundingbox->b4);
}

void CLobject::display(xchar flags)
{
	if(flags&SHADOW)
	{
		for(int i=0;i<polycount;i++)
		{
			polyptr[i]->update(shadowM,1);
			polyptr[i]->display(position,( (flags&CENTER) + FLAT + (flags&AMBIENT) + SHADOW ));
		}
	}
	else
	{
		for(int i=0;i<polycount;i++)
		{
			polyptr[i]->display(position,( (flags&CENTER) + (flags&FLAT) + (flags&AMBIENT) + (flags&SHADER) + (flags&DEBUG) ));
		}
	}
}

void CLobject::display(screenside* l,screenside* r,CLfbuffer* b,xlong h)
{
	for(int i=0;i<polycount;i++)
	{
		polyptr[i]->display(position,l,r,b,h);
	}
}

CLlvector CLobject::getposition()
{
	return position;
}

xlong CLobject::getpositionx()
{
	return position.x;
}

xlong CLobject::getpositiony()
{
	return position.y;
}

xlong CLobject::getpositionz()
{
	return position.z;
}

void CLobject::setposition(xlong x,xlong y,xlong z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void CLobject::setpositionx(xlong x)
{
	position.x = x;
}

void CLobject::setpositiony(xlong y)
{
	position.y = y;
}

void CLobject::setpositionz(xlong z)
{
	position.z = z;
}

void CLobject::addposition(xlong x,xlong y,xlong z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

xlong CLobject::getname()
{
	return name;
}

CLlvector* CLobject::getdockingpoint(xlong i) //get i-th docking point
{
	return dockptr[i];
}

CLlvector* CLobject::getdockingpoint(xlong t,xlong i) //get i-th docking point of type t, return 0 if not found, i= 0 means first of sort
{
	xlong c=-1;

	for(int j=0;j<dockcount;j++)
	{
		if(dockptr[j]->e == t)
		{
			c++;

			if(c==i)
			{
				break;
			}
		}
	}

	if(c==-1) return 0;
	else return dockptr[c]; 
}

void CLobject::translatealongnormals(float size)
{
	CLfvector t;

	for(int i=0;i<polycount;i++)
	{
		t = polyptr[i]->getnormal();
		t.x = (t.x / !t) * size;
		t.y = (t.y / !t) * size;
		t.z = (t.z / !t) * size;

		polyptr[i]->add(t);
	}
}

CLbox* CLobject::getboundingbox()
{
	return boundingbox;
}

void CLobject::reset()
{
	position.x = rposition.x;
	position.y = rposition.y;
	position.z = rposition.z;

	for(int i=0;i<polycount;i++)
	{
		polyptr[i]->reset();
	}
}

void CLobject::setminz(xlong z)
{
	xlong temp = CLmath::min(boundingbox->t1.z,boundingbox->b1.z);

	xlong diff = z - (position.z + temp);

	position.z += diff; 
}

xlong CLobject::getminz()
{
	xlong temp = CLmath::min(boundingbox->t1.z,boundingbox->b1.z);

	return position.z + temp;
}

#endif

