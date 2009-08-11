//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLENTITY
#define HH_CLENTITY
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLvector.hh"
#include "CLstruct.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLammo.hh"
#include "CLapi.hh"
#include "CLformat.hh"
#include "CLgame.hh"
#include "CLdiv3d.hh"
#include "CLcl.hh"


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
		CLlvector angles[I];
		xlong* mark;
		xlong gear;
		xlong active;
		xlong visible;
		xlong lastupdate;
		xlong* fireupdate;
		
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
		//virtual void pretransform() = 0;
		//virtual void transform() = 0;
		//virtual xlong collision() = 0;
	
	public:
		CLentity(CLfile* entitya,xlong* markptr);
		CLentity(CLentity* entityptr);
		~CLentity();
		
		//virtual xlong update() = 0;
		void display(bool modelorshadow=0);
		
		void hit(xlong h) { health -= h; }
		xlong gethealth() { return health; }
		xlong getshield() { return shield; }
		CLfvector* getposition() { return &position; }
		CLbox* getboundingbox() { return boundingbox[0][0]; }
};

template<int I>
void CLentity<I>::setspeed()
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

template<int I>
void CLentity<I>::fire(xlong at,xlong d,xlong i,xlong m)
{
	if( (d+i)>=ammomounts && m>=I) return;
	CLfvector startposition = position;
	CLfvector* ammodocking  = model[m]->getdockingpoint(d,i);
	startposition.x = ammodocking->x;
	startposition.y = ammodocking->y;
	startposition.z += ammodocking->z;
	startposition = CLmisc3d::project(startposition,position);
	ammoman->fire(at,startposition,direction[m],*mark);
}

template<int I>
CLentity<I>::CLentity(CLfile* ea,xlong* markptr)
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
	xchar* testext[2] = { "0.y3d","1.y3d" };
	if(I==1) testext[0] = ".y3d";
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		xlong em = CLutils::findarmember(entitya,testext[i]);
		if(em==-1) CLsystem::exit(1,0,__func__,"no entity model file found");
		model[i] = new CLobject(entitya->members[em],0);
		//*
		
		//set model(s) bounding boxes
		boundingbox[0][i] = model[i]->getboundingbox();
		//*
		
		//set testing bounding boxes
		boundingbox[1][i] = new CLbox();
		*boundingbox[1][i] = *boundingbox[0][i];
		//*
		
		//set initial model rotation angles
		angles[i] = 0;
		direction[i] = 0;
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
	xchar* ammoext[4] = { "ammotype0","ammotype1","ammotype2","ammotype3" };
	xchar* fireext[4] = { "firerate0","firerate1","firerate2","firerate3" };
	ammotype = new xlong[ammomounts];
	firerate = new xlong[ammomounts];
	fireupdate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		ammotype[j] = CLsystem::ato((*def)[ammoext[j]]);
		firerate[j] = CLsystem::ato((*def)[fireext[j]]);
		fireupdate[j] = CLsystem::getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype);
	//*
	
	//load csv if present (*.csv)
	csv=0;
	xlong ec = CLutils::findarmember(entitya,".csv");
	if(ec!=-1) csv = CLformat::loadcsv(entitya->members[ec]);
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	speed = 0;
	gear = 0;
	lastupdate = CLsystem::getmilliseconds();
	//*
}

template<int I>
CLentity<I>::CLentity(CLentity* entityptr)
{
	//create transformation matrix
	linear = new CLmatrix(1);
	//*
	
	//set mark pointer from level
	mark = entityptr->mark;
	//*
	
	//for each model
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		model[i] = entityptr->model[i];
		//*
		
		//set model(s) bounding boxes
		boundingbox[0][i] = model[i]->getboundingbox();
		//*
		
		//set testing bounding boxes
		boundingbox[1][i] = new CLbox();
		*boundingbox[1][i] = *boundingbox[0][i];
		//*
		
		//set initial model rotation angles
		angles[i] = 0;
		direction[i] = 0;
		//*
	}
	//*
	
	//find and load definition (*.ini)
	def = entityptr->def;
	//*
	
	//load entity attributes
	health		= entityptr->health;
	shield		= entityptr->shield;
	shieldrate	= entityptr->shieldrate;
	armor		= entityptr->armor;
	ammomounts	= entityptr->ammomounts;
	//*
	
	//load ammo types
	ammotype = entityptr->ammotype;
	firerate = entityptr->firerate;
	fireupdate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		fireupdate[j] = CLsystem::getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype);
	//*

	//load csv if present
	csv = entityptr->csv;
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	speed = 0;
	gear = 0;
	lastupdate = CLsystem::getmilliseconds();
	//*
}

template<int I>
CLentity<I>::~CLentity<I>()
{
	delete linear;
	delete ammoman; 
	delete[] ammotype;
	delete[] firerate;
	delete[] fireupdate;
	
	for(uxlong i=0; i<I; i++)
	{
		delete model[i];
		delete boundingbox[1][i];
	}
}

template<int I>
void CLentity<I>::display(bool modelorshadow)
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

#endif

