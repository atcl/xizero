//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLENTITY
#define HH_CLENTITY
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLvector.hh"
#include "CLstruct.hh"
#include "CLar.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLexplosion.hh"
#include "CLammo.hh"
#include "CLsystem.hh"
#include "CLformat.hh"
#include "CLgame.hh"
#include "CLmisc3d.hh"
#include "CLcl.hh"

/* class name:	CLentity
 * 
 * description:	base class for enemies and player.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

template<int I>
class CLentity : public virtual CLcl
{
	protected:
		CLmatrix* linear;
		CLammomanager* ammoman;
		CLobject* model[I];
		CLexplosion* expl[I];
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
		xlong type;
		xlong* mark;
		xlong markmax;
		xlong gear;
		float maxspeed;
		xlong active;
		xlong visible;
		xlong lastupdate;
		xlong* fireupdate;
		xlong ammomounts;
		xlong* ammotype;
		xlong* firerate;
		xlong health;
		xlong healthmax;
		xlong shield;
		xlong shieldmax;
		xlong shieldrate;
		xlong shieldupdate;
		xlong armor;
		xlong points;
		void setspeed();
		void fire(xlong at,xlong d,xlong i,xlong tz,xlong m=0);
		//virtual void pretransform() = 0;
		//virtual void transform() = 0;
		//virtual xlong collision() = 0;
	public:
		CLentity(CLfile* entitya,xlong* markptr,xlong mm);
		CLentity(CLentity* entityptr);
		virtual ~CLentity();
		//virtual xlong update() = 0;
		void display(xlong modelorshadow=0);
		void hit(xlong h);
		xlong gethealth() const;
		xlong getshield() const;
		CLfvector* getposition();
		CLbox* getboundingbox() const;
		bool isvisible() const;
		xlong gettype() const;
		void start();
		void pause();
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
void CLentity<I>::fire(xlong at,xlong d,xlong i,xlong tz,xlong m)
{
	if( (d+i)>=ammomounts && m>=I) return;
	CLfvector startposition = position;
	CLfvector targetdirection = direction[m];
	//targetdirection.z = -startposition.z + tz;
	CLfvector* ammodocking  = model[m]->getdockingpoint(d,i);
	startposition.x = ammodocking->x;
	startposition.y = ammodocking->y;
	startposition.z += ammodocking->z;
	clmisc3d->project(startposition,position);
	ammoman->fire(at,startposition,targetdirection);
}

template<int I>
CLentity<I>::CLentity(CLfile* ea,xlong* markptr,xlong mm)
{
	//create transformation matrix
	linear = new CLmatrix(1);
	//*
	
	//set mark pointer from level
	mark = markptr;
	markmax = mm;
	//*
	
	//load entity archive 
	CLar* entitya = new CLar(ea);
	//*

	//for each model
	xchar* testext[2] = { "0.y3d","1.y3d" };
	if(I==1) testext[0] = ".y3d";
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		CLfile* entitymodelfile = entitya->findbyextension(testext[i]);
		if(entitymodelfile==0) clsystem->exit(1,0,__func__,"no entity model file found");
		model[i] = new CLobject(entitymodelfile,0,0x00404040);
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
		
		//create explosion
		expl[i] = new CLexplosion(model[i]);
		//*
	}
	//*
	
	//find and load definition (*.ini)
	CLfile* entitydefinitionfile = entitya->findbyextension(".ini");
	if(entitydefinitionfile==0) clsystem->exit(1,0,__func__,"no entity definition found");
	def = clformat->loadini(entitydefinitionfile);
	//*
	
	//load entity attributes
	maxspeed    = clsystem->ato((*def)["speed"]);
	healthmax = health = clsystem->ato((*def)["health"]);
	shieldmax = shield = clsystem->ato((*def)["shield"]);
	shieldrate	= clsystem->ato((*def)["shieldrate"]);
	armor		= clsystem->ato((*def)["armor"]);
	ammomounts	= clsystem->ato((*def)["ammomounts"]);
	points		= 10; //system->ato((*def)["points"]);
	//*
	
	//load ammo types
	xchar* ammoext[4] = { "ammotype0","ammotype1","ammotype2","ammotype3" };
	xchar* fireext[4] = { "firerate0","firerate1","firerate2","firerate3" };
	ammotype = new xlong[ammomounts];
	firerate = new xlong[ammomounts];
	fireupdate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		ammotype[j] = clsystem->ato((*def)[ammoext[j]]);
		firerate[j] = clsystem->ato((*def)[fireext[j]]);
		fireupdate[j] = clsystem->getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype,mark);
	//*
	
	//load csv if present (*.csv)
	csv = 0;
	CLfile* entityaifile = entitya->findbyextension(".csv");
	if(entityaifile!=0) csv = clformat->loadcsv(entityaifile);
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	speed = 0;
	gear = 0;
	visible = 0;
	active = 0;
	shieldupdate = lastupdate = clsystem->getmilliseconds();
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
	markmax = entityptr->markmax;
	//*
	
	//for each model
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		model[i] = new CLobject(entityptr->model[i]);
		//*
		
		//set model(s) bounding boxes
		boundingbox[0][i] = new CLbox(*model[i]->getboundingbox());
		//*
		
		//set testing bounding boxes
		boundingbox[1][i] = new CLbox();
		*boundingbox[1][i] = *boundingbox[0][i];
		//*
		
		//set initial model rotation angles
		angles[i] = 0;
		direction[i] = 0;
		//*
		
		//create explosion
		expl[i] = new CLexplosion(model[i]);
		//*
	}
	//*
	
	//find and load definition (*.ini)
	def = entityptr->def;
	//*
	
	//load entity attributes
	maxspeed  = entityptr->maxspeed;
	healthmax = health = entityptr->health;
	shieldmax = shield = entityptr->shield;
	shieldrate	= entityptr->shieldrate;
	armor		= entityptr->armor;
	ammomounts	= entityptr->ammomounts;
	points		= entityptr->points;
	//*
	
	//load ammo types
	ammotype = entityptr->ammotype;
	firerate = entityptr->firerate;
	fireupdate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		fireupdate[j] = clsystem->getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype,mark);
	//*

	//load csv if present
	csv = entityptr->csv;
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	speed = 0;
	gear = 0;
	visible = 0;
	active = 0;
	shieldupdate = lastupdate = clsystem->getmilliseconds();
	//*
}

template<int I>
CLentity<I>::~CLentity<I>()
{
	delete def;
	delete linear;
	delete ammoman; 
	delete[] ammotype;
	delete[] firerate;
	delete[] fireupdate;
	
	for(uxlong i=0; i<I; i++)
	{
		delete model[i];
		delete expl[i];
		delete boundingbox[1][i];
	}
}

template<int I>
void CLentity<I>::display(xlong modelorshadow)
{
	//set screen position
	sposition.x = position.x;
	sposition.y = position.y - *mark;
	sposition.z = position.z;
	//CLmisc3d::project2(sposition); //fix point projections
	//*

	if(active!=0)
	{
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
}

template<int I>
void CLentity<I>::hit(xlong h)
{
	if(shield>0) shield -=h;
	else health -= h; 
	
	if(shield<0)
	{
		health -= -shield; 
	}
}

template<int I>
xlong CLentity<I>::gethealth() const { return health; }

template<int I>
xlong CLentity<I>::getshield() const { return shield; }

template<int I>
CLfvector* CLentity<I>::getposition() { return &position; }

template<int I>
CLbox* CLentity<I>::getboundingbox() const { return boundingbox[0][0]; }

template<int I>
bool CLentity<I>::isvisible() const { return visible; } 

template<int I>
xlong CLentity<I>::gettype() const { return type; } 

template<int I>
void CLentity<I>::start()
{
	xlong currtime = clsystem->getmilliseconds();
	lastupdate = currtime;
	shieldupdate = currtime;
	for(uxlong i=0; i<ammomounts; i++) fireupdate[i] = currtime;
} 

template<int I>
void CLentity<I>::pause()
{
	xlong currtime = clsystem->getmilliseconds();
	lastupdate = currtime;
	shieldupdate = currtime;
	for(uxlong i=0; i<ammomounts; i++) fireupdate[i] = currtime;
	ammoman->pause();
} 

#endif

