//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLENTITY
#define HH_CLENTITY
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLvector.hh"
#include "CLstruct.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLammo.hh"
#include "CLapi.hh"
#include "CLformat.hh"

template<int I>
class CLentity : public virtual CLcl
{
	protected:
		CLmatrix* linear;
		CLammomanager* ammoman;
		CLobject* model[I];
		xmap* def;
		xlong* csv;
		
		CLbox* boundingbox[2][I];
		CLfvector position;
		CLfvector tposition;
		CLlvector sposition;
		CLfvector direction[I];
		CLfvector speed;
		CLfvector speeddir;
		CLfvector angles[I];
		xlong* mark;
		xlong gear;
		xlong active;
		xlong lastupdate;
		
		xlong ammomounts;
		xlong* ammotype;
		xlong* firerate;
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;
		xlong points;
		
		void setspeed();
		void fire(xlong at,xlong d,xlong i,xlong m=0);
		void pretransform() virtual;
		void transform() virtual;
		xlong collision() virtual;
	
	public:
		CLentity(CLfile* entitya,xlong* markptr);
		CLentity(CLentity* entityptr);
		~CLentity();
		
		xlong update() virtual;
		void display(bool modelorshadow=0);
		
		void hit(xlong h) health -= h;
		xlong gethealth() return health;
		xlong getshield() return shield;
		CLfvector* getposition() return &position
		CLbox* getboundingbox() return boundingbox[0][0];
};

template<int I>
void CLentity::setspeed()
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

template<int I>
void CLentity::fire(xlong at,xlong d,xlong i,xlong m)
{
	if( (d+i)>=ammomounts && m>=I) return;
	CLfvector startposition = position;
	CLfvector* ammodocking  = model[m]->getdockingpoint(d,i);
	startposition.x = ammodocking->x;
	startposition.y = ammodocking->y;
	startposition.z += ammodocking->z;
	startposition = CLmisc3d::project(startposition,position);
	ammoman->fire(at,startposition,direction[m],mark);
}

template<int I>
CLentity::CLentity(CLfile* ea,xlong* markptr)
{
	//create transformation matrix
	linear = new CLmatrix(1);
	//*
	
	//set mark pointer from level
	mark = markptr;
	//*
	
	//load entity archive 
	arfile* entitya = CLformat::loadar(ea);
	//*

	//for each model
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		xlong em = CLutils::findarmember(entitya,xchar(48+i)".y3d");
		if(em==-1) CLsystem::exit(1,0,__func__,"no entity model file found");
		model[i] = new CLobject(entitya->members[],0);
		//*
		
		//set model(s) bounding boxes
		boundingbox[0][i] = model[i]->getboundingbox()
		//*
		
		//set testing bounding boxes
		boundingbox[1][i] = new CLbox();
		*boundingbox[1][i] = *boundingbox[0][i];
		//*
	}
	//*
	
	//find and load definition (*.ini)
	xlong ed = CLutils::findarmember(entitya,".ini");
	if(ed==-1) CLsystem::exit(1,0,__func__,"no entity definition found");
	def = CLformat::loadini(entitya->members[ed]);
	//*
	
	//load entity attributes
	health		= CLsystem::ato((*def)["health"]);
	shield		= CLsystem::ato((*def)["shield"]);
	shieldrate	= CLsystem::ato((*def)["shieldrate"]);
	armor		= CLsystem::ato((*def)["armor"]);
	ammomounts	= CLsystem::ato((*def)["ammomounts"]);
	//*
	
	//load ammo types
	ammotype = new xlong[ammomounts];
	firerate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		ammotype[j] = CLsystem::ato((*def)["ammotype"xchar(j)]);
		firerate[j] = CLsystem::ato((*def)["firerate"xchar(j)]);
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype);
	//*
	
	//load csv if present (*.csv)
	xlong ec = CLutils::findarmember(entitya,".csv");
	if(ec!=-1) csv = CLformat::loadcsv(entitya->members[ec]);
	//*
	
	//set remaining entity attributes
	angles = 0;
	speed = 0;
	gear = 0;
	lastupdate = CLsystem::getmilliseconds();
	//*
}

template<int I>
CLentity::CLentity(CLentity* entityptr)
{
	
}

template<int I>
CLentity::~CLentity()
{
	delete linear;
	delete ammoman;
	delete[] model;
	delete[] boundingbox[0];
	delete[] boundingbox[1]; 
	delete[] ammotype;
	delete[] firerate;
	
	//...
}

template<int I>
void CLentity::display(bool modelorshadow)
{
	//set screen position
	sposition.x = position.x;
	sposition.y = position.y - *mark;
	sposition.z = position.z;
	//sposition = CLmisc3d::project(sposition); //fix point projections
	//*
	
	switch(modelorshadow)
	{
		case 0:
			//display model(s)
			for(uxlong i=0; i<I; i++) model[i]->display(sposition,FLAT + AMBIENT);
			//*
			
			//display ammo
			ammoman->display();
			//*
		break;
		
		case 1:
			//display shadow(s)
			for(uxlong i=0; i<I; i++) model[i]->display(sposition,SHADOW);
			//*
		break;
	}
}

//********

class CLplayer : CLentity<2>
{
	private:
		void pretransform();
		void transform();
		xlong collision();
	
	public:
		CLplayer();
		~CLplayer();
		
		xlong update();
};

//********

class CLenemy : CLentity<1>
{
	private:
		void pretransform();
		void transform();
		xlong collision();
	
	public:
		CLenemy();
		CLenemy();
		~CLenemy();
		
		xlong update();
};

#endif
