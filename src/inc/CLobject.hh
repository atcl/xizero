//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLOBJECT
#define HH_CLOBJECT
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLpolygon.hh"
#include "CLmath.hh"
#include "CLmatrix.hh"
#include "CLlight.hh"
#include "CLstruct.hh"
#include "CLvector.hh"

class CLobject
{
	protected:
		CLmath* clmath;
		CLmatrix* shadowmatrix;
		CLlight*  cllight;

	private:
		xlong vertexcount;
		xlong polycount;
		xlong dockcount;
		xlong vertexptr;
		CLpolygon** polyptr;
		vertex** dockptr;
		vertex position;
		vertex rposition;
		xlong name;
	
	public:
		CLobject(xlong* db,float* zb,xlong* sb,xlong* dataptr,xlong x,xlong y,xlong z,CLmath* clm,CLmatrix* sm,CLlight* li);
		~CLobject();
		
		void update(CLmatrix* m);
		void display(bool center,bool flat,bool light,bool shadows,bool pixelshader,bool debug);
		vertex getdocking(xlong i);
		vertex getposition();
		xlong getpositionx();
		xlong getpositiony();
		xlong getpositionz();
		void setposition(xlong x,xlong y,xlong z);
		void setpositionx(xlong x);
		void setpositiony(xlong y);
		void setpositionz(xlong z);
		void addpositionx(xlong x);
		void addpositiony(xlong y);
		void addpositionz(xlong z);
		xlong getname();
		void translatealongnormals(float size);
		void reset();
};

CLobject::CLobject(xlong* db,float* zb,xlong* sb,xlong* dataptr,xlong x,xlong y,xlong z,CLmath* clm,CLmatrix* sm,CLlight* li)
{
	clmath = clm;
	shadowmatrix = sm;
	cllight = li;

	xlong sobjcount;

	xlong sobjcounter = 0;
	xlong polycounter = 0;
	xlong dockcounter = 0;

	xlong* sobjptr;

	xlong xoff;
	xlong yoff;
	xlong zoff;
	
	xlong localpolycount;
	xlong localdockcount;
	short localdocktype;

	xlong d;
	xlong t[17];
	doubleword s;

	position.x = x;
	position.y = y;
	position.z = z;

	rposition.x = x;
	rposition.y = y;
	rposition.z = z;

	if(dataptr[0] != '<CLY') CLexit_(__func__,"wrong y3d format, may be endianess?",1);

	if(dataptr[1] == '3DB>')
	{
		polycount = dataptr[2];
		polyptr = new CLpolygon*[polycount];
		//dataptr[3] is empty

		//dataptr[4] = "OBJT"
		if(dataptr[4] != 'OBJT' ) CLexit_(__func__,"No OBJT tag",1);
		name = dataptr[5];
		sobjcount = dataptr[6];
		dockcount = dataptr[7];
		dockptr = new vertex*[dockcount];

		d = 8;

		for(int i=0;i<sobjcount;i++)
		{
			if(dataptr[d] != 'SOBJ' ) CLexit_(__func__,"No SOBJ tag",1);
			d++; //"SOBJ"
			d++; //subobject identifier
			localpolycount = dataptr[d]; d++;
			localdockcount = dataptr[d]; d++;

			if(dataptr[d] != 'CONN' ) CLexit_(__func__,"No CONN tag",1);
			d++; //"CONN"
			xoff = dataptr[d]; d++;
			yoff = dataptr[d]; d++;
			zoff = dataptr[d]; d++;

			for(int j=0;j<localpolycount;j++,polycounter++)
			{
				if(dataptr[d] != 'POLY' ) CLexit_(__func__,"No POLY tag",1);
				d++; //"POLY"
				d++; //identifier
				t[12] = dataptr[d]; d++; //color
				d++; //0

				if(dataptr[d] != 'VECT' ) CLexit_(__func__,"No VECT tag",1);
				d++; //"VECT"
				t[0] = dataptr[d] + xoff; d++; //x1
				t[1] = dataptr[d] + yoff; d++; //y1
				t[2] = dataptr[d]/4 + zoff; d++; //z1
				//t[2] = dataptr[d] + zoff; d++; //z1

				if(dataptr[d] != 'VECT' ) CLexit_(__func__,"No VECT tag",1);
				d++; //"VECT"
				t[3] = dataptr[d] + xoff; d++; //x2
				t[4] = dataptr[d] + yoff; d++; //y2
				t[5] = dataptr[d]/4 + zoff; d++; //z2
				//t[5] = dataptr[d] + zoff; d++; //z2

				if(dataptr[d] != 'VECT' ) CLexit_(__func__,"No VECT tag",1);
				d++; //"VECT"
				t[6] = dataptr[d] + xoff; d++; //x3
				t[7] = dataptr[d] + yoff; d++; //y3
				t[8] = dataptr[d]/4 + zoff; d++; //z3
				//t[8] = dataptr[d] + zoff; d++; //z3

				if(dataptr[d] != 'VECT' )CLexit_(__func__,"No VECT tag",1);
				d++; //"VECT"
				t[9] = dataptr[d] + xoff; d++; //x4
				t[10] = dataptr[d] + yoff; d++; //y4
				t[11] = dataptr[d]/4 + zoff; d++; //z4
				//t[11] = dataptr[d] + zoff; d++; //z4
				
				polyptr[polycounter] = new CLpolygon(db,zb,sb,t[0],t[1],t[2],t[3],t[4],t[5],t[6],t[7],t[8],t[9],t[10],t[11],t[12],0x000000C0,clm,cllight);

			}

			for(int k=0;k<localdockcount;k++,dockcounter++)
			{
				s.dd = dataptr[d]; d++; //"DP"+docktype
				localdocktype = s.dw[1];
				
				t[0] = dataptr[d]; d++; //dx
				t[1] = dataptr[d]; d++; //dy
				t[2] = dataptr[d]; d++; //dz

				dockptr[dockcounter] = new vertex;
				dockptr[dockcounter]->x = t[0];
				dockptr[dockcounter]->y = t[1];
				dockptr[dockcounter]->z = t[2];
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

CLobject::~CLobject() { }

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
}

void CLobject::display(bool center,bool flat,bool light,bool shadows,bool pixelshader,bool debug)
{
	if(shadows==1)
	{
		for(int i=0;i<polycount;i++)
		{
			polyptr[i]->update(shadowmatrix,1);
			polyptr[i]->display(position.x,position.y,position.z,center,1,light,1,0,0);
		}
	}
	else
	{
		for(int i=0;i<polycount;i++)
		{
			polyptr[i]->display(position.x,position.y,position.z,center,flat,light,0,pixelshader,debug);
		}
	}
}

vertex CLobject::getdocking(xlong i)
{
	return *dockptr[i];
}

vertex CLobject::getposition()
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

void CLobject::addpositionx(xlong x)
{
	position.x += x;
}

void CLobject::addpositiony(xlong y)
{
	position.y += y;
}

void CLobject::addpositionz(xlong z)
{
	position.z += z;
}

xlong CLobject::getname()
{
	return name;
}

void CLobject::translatealongnormals(float size)
{
	xlong xa;
	xlong ya;
	xlong za;
	vector t;

	for(int i=0;i<polycount;i++)
	{
		t = polyptr[i]->getnormal();
		xa = xlong((t.x / t.l) * size);
		ya = xlong((t.y / t.l) * size);
		za = xlong((t.z / t.l) * size);

		polyptr[i]->add(xa,ya,za);
	}
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

#endif
