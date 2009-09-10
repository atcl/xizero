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


struct CLbox
{
	CLfvector c[8]; //c[0-3] bottom, c[4-7] top
};

class CLobject : public virtual CLcl
{
	protected:
		CLpolygon** polyptr;
		CLfvector** dockptr;
		CLbox* boundingbox;
		CLbox* rboundingbox;

	private:
		xlong polycount;
		xlong dockcount;
		xlong name;
	
	public:
		CLobject(CLfile* fileptr,bool zs);
		CLobject(CLobject* obj);
		~CLobject();
		
		void update(CLmatrix* m);
		void partupdate(CLmatrix* m);
		void display(CLlvector p,xshort flags);
		void display(CLlvector p,screenside* l,screenside* r,CLfbuffer* b,xlong h);
		xlong getname();
		CLfvector* getdockingpoint(xlong t,xlong i);
		void translatealongnormals(float speed);
		CLbox* getboundingbox();
		void reset();
		void setcolor(uxlong co);
		void resetcolor();
};

CLobject::CLobject(CLfile* fileptr,bool zs)
{
	//init bounding box
	rboundingbox = new CLbox;
	boundingbox = new CLbox;
	boundingbox->c[0] = 0;
	boundingbox->c[1] = 0;
	boundingbox->c[2] = 0;
	boundingbox->c[3] = 0;
	boundingbox->c[4] = 0;
	boundingbox->c[5] = 0;
	boundingbox->c[6] = 0;
	boundingbox->c[7] = 0;
	//*

	xlong* dataptr = fileptr->data;

	xlong sobjcount = 0;

	//xlong sobjcounter = 0;
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

	//scale z values by 4 through left shifts by two if zs is set
	xlong zshift = 2;
	if(zs==0) zshift = 0;
	//*

	//check if first 4 bytes of file have y3d id
	if(dataptr[0] != '<CLY') CLsystem::exit(1,0,__func__,"wrong y3d format, may be endianess?");
	//*

	//check if second 4 bytes of file have binary id
	if(dataptr[1] == '3DB>')
	{
		//create polygon array
		polycount = dataptr[2];
		polyptr = new CLpolygon*[polycount];
		//*
		
		//dataptr[3] is empty
		
		//read OBJT tag ( 'OBJT' , object_name , subobject_count , dockingpoint_count )
		if(dataptr[4] != 'OBJT' ) CLsystem::exit(1,0,__func__,"No OBJT tag");
		name = dataptr[5];
		sobjcount = dataptr[6];
		dockcount = dataptr[7];
		if(dockcount!=0)
		{
			dockptr = new CLfvector*[dockcount];
		}
		else
		{
			dockptr=0;
		}
		//*
		
		//subobject read loop
		d = 8;
		for(uxlong i=0;i<sobjcount;i++)
		{
			//read SOBJ tag ( 'SOBJ' , subobject_name , subobject_polygon_count , subobject_dockingpoint_count )
			if(dataptr[d] != 'SOBJ' ) CLsystem::exit(1,0,__func__,"No SOBJ tag");
			d++; //"SOBJ"
			d++; //subobject identifier
			localpolycount = dataptr[d]; d++;
			localdockcount = dataptr[d]; d++;
			//*

			//read CONN tag ( 'CONN' , x_reference , y_reference , z_reference )
			if(dataptr[d] != 'CONN' ) CLsystem::exit(1,0,__func__,"No CONN tag");
			d++; //"CONN"
			xoff = dataptr[d]; d++;
			yoff = dataptr[d]; d++;
			zoff = dataptr[d]; d++;
			//*

			//polygon read loop
			for(uxlong j=0;j<localpolycount;j++,polycounter++)
			{
				//read POLY tag ( 'POLY' , polygon_name , polygon_color , 0 )
				if(dataptr[d] != 'POLY' ) CLsystem::exit(1,0,__func__,"No POLY tag");
				d++; //"POLY"
				d++; //identifier
				localcolor = dataptr[d]; d++; //color
				d++; //0
				//*

				//read 1st VECT tag ( 'VECT' , x_value , y_value , z_value )
				if(dataptr[d] != 'VECT' ) CLsystem::exit(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[0].x = dataptr[d] + xoff; d++; //x1
				t[0].y = dataptr[d] + yoff; d++; //y1
				t[0].z = (dataptr[d]>>zshift) + zoff; d++; //z1
				//*

				//bounding box generation
				if( t[0].x < boundingbox->c[4].x) { boundingbox->c[4].x = t[0].x; boundingbox->c[0].x = t[0].x; boundingbox->c[7].x = t[0].x; boundingbox->c[3].x = t[0].x; } 
				if( t[0].x > boundingbox->c[1].x) { boundingbox->c[5].x = t[0].x; boundingbox->c[1].x = t[0].x; boundingbox->c[6].x = t[0].x; boundingbox->c[2].x = t[0].x; } 
				if( t[0].y < boundingbox->c[4].y) { boundingbox->c[4].y = t[0].y; boundingbox->c[0].y = t[0].y; boundingbox->c[5].y = t[0].y; boundingbox->c[1].y = t[0].y; } 
				if( t[0].y > boundingbox->c[2].y) { boundingbox->c[6].y = t[0].y; boundingbox->c[2].y = t[0].y; boundingbox->c[7].y = t[0].y; boundingbox->c[3].y = t[0].y; } 
				if( t[0].z < boundingbox->c[4].z) { boundingbox->c[4].z = t[0].z; boundingbox->c[5].z = t[0].z; boundingbox->c[6].z = t[0].z; boundingbox->c[7].z = t[0].z; } 
				if( t[0].z > boundingbox->c[0].z) { boundingbox->c[0].z = t[0].z; boundingbox->c[1].z = t[0].z; boundingbox->c[2].z = t[0].z; boundingbox->c[3].z = t[0].z; } 
				//*

				//read 2nd VECT tag ( 'VECT' , x_value , y_value , z_value )
				if(dataptr[d] != 'VECT' ) CLsystem::exit(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[1].x = dataptr[d] + xoff; d++; //x2
				t[1].y = dataptr[d] + yoff; d++; //y2
				t[1].z = (dataptr[d]>>zshift) + zoff; d++; //z2
				//*

				//bounding box generation
				if( t[1].x < boundingbox->c[4].x) { boundingbox->c[4].x = t[1].x; boundingbox->c[0].x = t[1].x; boundingbox->c[7].x = t[1].x; boundingbox->c[3].x = t[1].x; } 
				if( t[1].x > boundingbox->c[1].x) { boundingbox->c[5].x = t[1].x; boundingbox->c[1].x = t[1].x; boundingbox->c[6].x = t[1].x; boundingbox->c[2].x = t[1].x; } 
				if( t[1].y < boundingbox->c[4].y) { boundingbox->c[4].y = t[1].y; boundingbox->c[0].y = t[1].y; boundingbox->c[5].y = t[1].y; boundingbox->c[1].y = t[1].y; } 
				if( t[1].y > boundingbox->c[2].y) { boundingbox->c[6].y = t[1].y; boundingbox->c[2].y = t[1].y; boundingbox->c[7].y = t[1].y; boundingbox->c[3].y = t[1].y; } 
				if( t[1].z < boundingbox->c[4].z) { boundingbox->c[4].z = t[1].z; boundingbox->c[5].z = t[1].z; boundingbox->c[6].z = t[1].z; boundingbox->c[7].z = t[1].z; } 
				if( t[1].z > boundingbox->c[0].z) { boundingbox->c[0].z = t[1].z; boundingbox->c[1].z = t[1].z; boundingbox->c[2].z = t[1].z; boundingbox->c[3].z = t[1].z; } 
				//*

				//read 3rd VECT tag ( 'VECT' , x_value , y_value , z_value )
				if(dataptr[d] != 'VECT' ) CLsystem::exit(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[2].x = dataptr[d] + xoff; d++; //x3
				t[2].y = dataptr[d] + yoff; d++; //y3
				t[2].z = (dataptr[d]>>zshift) + zoff; d++; //z3
				//*

				//bounding box generation
				if( t[2].x < boundingbox->c[4].x) { boundingbox->c[4].x = t[2].x; boundingbox->c[0].x = t[2].x; boundingbox->c[7].x = t[2].x; boundingbox->c[3].x = t[2].x; } 
				if( t[2].x > boundingbox->c[1].x) { boundingbox->c[5].x = t[2].x; boundingbox->c[1].x = t[2].x; boundingbox->c[6].x = t[2].x; boundingbox->c[2].x = t[2].x; } 
				if( t[2].y < boundingbox->c[4].y) { boundingbox->c[4].y = t[2].y; boundingbox->c[0].y = t[2].y; boundingbox->c[5].y = t[2].y; boundingbox->c[1].y = t[2].y; } 
				if( t[2].y > boundingbox->c[2].y) { boundingbox->c[6].y = t[2].y; boundingbox->c[2].y = t[2].y; boundingbox->c[7].y = t[2].y; boundingbox->c[3].y = t[2].y; } 
				if( t[2].z < boundingbox->c[4].z) { boundingbox->c[4].z = t[2].z; boundingbox->c[5].z = t[2].z; boundingbox->c[6].z = t[2].z; boundingbox->c[7].z = t[2].z; } 
				if( t[2].z > boundingbox->c[0].z) { boundingbox->c[0].z = t[2].z; boundingbox->c[1].z = t[2].z; boundingbox->c[2].z = t[2].z; boundingbox->c[3].z = t[2].z; } 
				//*

				//read 4th VECT tag ( 'VECT' , x_value , y_value , z_value )
				if(dataptr[d] != 'VECT' ) CLsystem::exit(1,0,__func__,"No VECT tag");
				d++; //"VECT"
				t[3].x = dataptr[d] + xoff; d++; //x4
				t[3].y = dataptr[d] + yoff; d++; //y4
				t[3].z = (dataptr[d]>>zshift) + zoff; d++; //z4
				//*

				//bounding box generation
				if( t[3].x < boundingbox->c[4].x) { boundingbox->c[4].x = t[3].x; boundingbox->c[0].x = t[3].x; boundingbox->c[7].x = t[3].x; boundingbox->c[3].x = t[3].x; } 
				if( t[3].x > boundingbox->c[1].x) { boundingbox->c[5].x = t[3].x; boundingbox->c[1].x = t[3].x; boundingbox->c[6].x = t[3].x; boundingbox->c[2].x = t[3].x; } 
				if( t[3].y < boundingbox->c[4].y) { boundingbox->c[4].y = t[3].y; boundingbox->c[0].y = t[3].y; boundingbox->c[5].y = t[3].y; boundingbox->c[1].y = t[3].y; } 
				if( t[3].y > boundingbox->c[2].y) { boundingbox->c[6].y = t[3].y; boundingbox->c[2].y = t[3].y; boundingbox->c[7].y = t[3].y; boundingbox->c[3].y = t[3].y; } 
				if( t[3].z < boundingbox->c[4].z) { boundingbox->c[4].z = t[3].z; boundingbox->c[5].z = t[3].z; boundingbox->c[6].z = t[3].z; boundingbox->c[7].z = t[3].y; } 
				if( t[3].z > boundingbox->c[0].z) { boundingbox->c[0].z = t[3].z; boundingbox->c[1].z = t[3].z; boundingbox->c[2].z = t[3].z; boundingbox->c[3].z = t[3].y; } 
				//*
				
				//set backup bounding box
				*rboundingbox = *boundingbox;
				//*
				
				//init polygon
				polyptr[polycounter] = new CLpolygon(t[0],t[1],t[2],t[3],localcolor,0x0000C000);
				//*
			}
			//*
			
			//docking point read loop
			for(uxlong k=0;k<localdockcount;k++)
			{
				//read DP tag ( 'DP' , docckingpoint_type , x_value , y_value , z_value )
				s.dd = dataptr[d]; d++; //"DP"+docktype
				localdocktype = s.dw[1];
				t[0].x = dataptr[d] + xoff; d++; //dx
				t[0].y = dataptr[d] + yoff; d++; //dy
				t[0].z = (dataptr[d]>>zshift) + zoff; d++; //dz
				//*

				//init dockingpoint
				dockptr[dockcounter] = new CLfvector(0,0,0);
				dockptr[dockcounter]->x = t[0].x;
				dockptr[dockcounter]->y = t[0].y;
				dockptr[dockcounter]->z = t[0].z;
				dockptr[dockcounter]->e = xlong(localdocktype);
				dockcounter++;
				//*
			}
			//*
		}
		//*

		//"ENDO"
	}
	//*
	
	//check if second 4 bytes have XML id
	else if(dataptr[1] == 'D_X>')
	{

	}
	//*
	
	//check if second 4 bytes have sequential id
	else if(dataptr[1] == 'D_T>')
	{

	}
	//*
	
	//! causes parts of player to be rendered wrong
	//~ //sort polygons by polygons normal.z
	//~ CLpolygon* temp;
	//~ CLfvector normi;
	//~ CLfvector normj;
	//~ 
	//~ for(uxlong i=0; i<(polycount-1); i++)
	//~ {
		//~ for(uxlong j=(i+1); j<polycount; j++)
		//~ {
			//~ normi = polyptr[i]->getnormal();
			//~ normj = polyptr[j]->getnormal();
			//~ if(normi.z<normj.z)
			//~ {
				//~ temp = polyptr[i];
				//~ polyptr[i] = polyptr[j];
				//~ polyptr[j] = temp;
			//~ }
		//~ }
	//~ }
	//~ //*
}

CLobject::CLobject(CLobject* obj)
{
		polycount = obj->polycount;
		polyptr = new CLpolygon*[polycount];
		dockcount = obj->dockcount;
		dockptr = new CLfvector*[dockcount];
		boundingbox = new CLbox(*(obj->boundingbox));
		rboundingbox = new CLbox(*(obj->rboundingbox));
		name = obj->name;
		
		for(uxlong i=0; i<polycount; i++)
		{
			polyptr[i] = new CLpolygon(*(obj->polyptr[i]));
		}
		
		for(uxlong j=0; j<dockcount; j++)
		{
			dockptr[j] = new CLfvector(*(obj->dockptr[j]));
		}
}

CLobject::~CLobject()
{
	delete boundingbox;
	delete rboundingbox;
}

void CLobject::update(CLmatrix* m)
{
	//transform each polygon
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->update(m,0);
	}
	//*

	//transform each docking point
	for(uxlong j=0;j<dockcount;j++)
	{
		*dockptr[j] = m->transform(*dockptr[j]);
	}
	//*

	//transform bounding box
	boundingbox->c[0] = m->transform(boundingbox->c[0]);
	boundingbox->c[1] = m->transform(boundingbox->c[1]);
	boundingbox->c[2] = m->transform(boundingbox->c[2]);
	boundingbox->c[3] = m->transform(boundingbox->c[3]);
	boundingbox->c[4] = m->transform(boundingbox->c[4]);
	boundingbox->c[5] = m->transform(boundingbox->c[5]);
	boundingbox->c[6] = m->transform(boundingbox->c[6]);
	boundingbox->c[7] = m->transform(boundingbox->c[7]);
	//*
}

void CLobject::partupdate(CLmatrix* m)
{
	//transform each polygon
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->partupdate(m);
	}
	//*

	//transform each docking point
	for(uxlong j=0;j<dockcount;j++)
	{
		*dockptr[j] = m->transform(*dockptr[j]);
	}
	//*

	//after partupdate boundingbox is without value
	boundingbox->c[0] = 0;
	boundingbox->c[1] = 0;
	boundingbox->c[2] = 0;
	boundingbox->c[3] = 0;
	boundingbox->c[4] = 0;
	boundingbox->c[5] = 0;
	boundingbox->c[6] = 0;
	boundingbox->c[7] = 0;
	//*
}

void CLobject::display(CLlvector p,xshort flags)
{
	CLfvector currnormal = CLfvector();
	
	//display objects shadow
	if(flags&SHADOW)
	{
		for(uxlong i=0;i<polycount;i++)
		{
			polyptr[i]->update(shadowM,1);
			polyptr[i]->display(p,flags);
		}
	}
	//*
	
	//display all polygons
	else
	{
		for(uxlong i=0;i<polycount;i++)
		{
			currnormal = polyptr[i]->getnormal();
			if( !( (flags&XPLUS  && currnormal.x>0 && currnormal.y==0 && currnormal.z==0) ||
				   (flags&XMINUS && currnormal.x<0 && currnormal.y==0 && currnormal.z==0) ||
				   (flags&YPLUS  && currnormal.x==0 && currnormal.y>0 && currnormal.z==0) ||
				   (flags&YMINUS && currnormal.x==0 && currnormal.y<0 && currnormal.z==0) ||
				   (flags&ZPLUS  && currnormal.x==0 && currnormal.y==0 && currnormal.z>0) ||
				   (flags&ZMINUS && currnormal.x==0 && currnormal.y==0 && currnormal.z<0) ) )
			{
				polyptr[i]->display(p,flags);
			}
		}
	}
	//*
}

void CLobject::display(CLlvector p,screenside* l,screenside* r,CLfbuffer* b,xlong h)
{
	//use special display method ONLY for zlevel map construction (see CLlevel Z179-2008)
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->display(p,l,r,b,h);
	}
	//*
}

xlong CLobject::getname()
{
	return name;
}

CLfvector* CLobject::getdockingpoint(xlong t,xlong i) //get i-th docking point of type t, return 0 if not found, i= 0 means first of sort
{
	xlong c= -1;
	xlong d= 0;

	//search requested docking point
	for(uxlong j=0;j<dockcount;j++)
	{
		//requested docking point type found
		if(dockptr[j]->e == t)
		{
			c=j;
			d++;
			
			//requested docking point type with correct index found
			if(d==i)
			{
				break;
			}
			//*
		}
		//*
	}
	//*
	
	//requested docking point not found
	if(c==-1) return 0;
	//*
	
	//return found docking point
	else
	{
		CLfvector* temp = new CLfvector();
		temp->x = dockptr[c]->x;
		temp->y = dockptr[c]->y;
		temp->z = dockptr[c]->z;
		return temp;
	}
	//*
}

void CLobject::translatealongnormals(float speed)
{
	CLfvector t;

	//move all polygons along their normal by given speed
	for(uxlong i=0;i<polycount;i++)
	{
		t = polyptr[i]->getnormal();
		t.x = (t.x / !t) * speed;
		t.y = (t.y / !t) * speed;
		t.z = (t.z / !t) * speed;

		polyptr[i]->add(t);
	}
	//*
}

CLbox* CLobject::getboundingbox()
{
	return boundingbox;
}

void CLobject::reset()
{
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->reset();
	}
	
	*boundingbox = *rboundingbox;
}

void CLobject::setcolor(uxlong co)
{
	//set colors of all polygons
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->setcolor(co);
	}
	//*
}

void CLobject::resetcolor()
{
	//reset colors of all polygons
	for(uxlong i=0;i<polycount;i++)
	{
		polyptr[i]->resetcolor();
	}
	//*
}
#endif

