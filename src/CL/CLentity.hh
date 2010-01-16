///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guards 
#ifndef HH_CLENTITY
#define HH_CLENTITY
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLvector.hh"
#include "CLstruct.hh"
#include "CLar.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLexplosion.hh"
#include "CLammo.hh"
#include "CLstring.hh"
#include "CLformat.hh"
#include "CLgame.hh"
#include "CLmisc3d.hh"
#include "CLbase.hh"
#include "CLsound.hh"
#include "CLwindow.hh"
///*

///headers
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
///*

///definitions
template<int I>
class CLentity : public CLbase<CLentity<I>,0>
{
	private:
		static CLmisc3d* clmisc3d;
		static CLformat* clformat;
		static CLstring* clstring;
		static CLwindow* clwindow;
	protected:
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
		xlong sndfire;
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
		xlong gethealth() const { return health; };
		xlong getshield() const { return shield; };
		CLfvector* getposition() { return &position; };
		CLbox* getboundingbox() const { return boundingbox[0][0]; };
		bool isvisible() const { return visible; };
		xlong gettype() const { return type; };
		void start();
		void pause();
};

template<int I>CLmisc3d* CLentity<I>::clmisc3d = CLmisc3d::instance();
template<int I>CLformat* CLentity<I>::clformat = CLformat::instance();
template<int I>CLstring* CLentity<I>::clstring = CLstring::instance();
template<int I>CLwindow* CLentity<I>::clwindow = CLwindow::instance();
///*

///implementations
template<int I>
void CLentity<I>::setspeed() //! critical
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

template<int I>
void CLentity<I>::fire(xlong at,xlong d,xlong i,xlong tz,xlong m) //! critical
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
	//clsound->play(sndfire);
}

template<int I>
CLentity<I>::CLentity(CLfile* ea,xlong* markptr,xlong mm) //! noncritical
{	
	//set mark pointer from level
	mark = markptr;
	markmax = mm;
	//*
	
	//load entity archive 
	CLar* entitya = new CLar(ea);
	//*
	
	//for each model
	CLfile* entitymodelfile = 0;
	const xchar* testext[2] = { u8"0.y3d",u8"1.y3d" };
	if(I==1) testext[0] = u8".y3d";
	for(uxlong i=0; i<I; i++)
	{
		//find and load model(s) (*.y3d)
		entitymodelfile = entitya->findbyextension(testext[i]);
		if(entitymodelfile==0) err(__FILE__,__func__,u8"no entity model file found");
		model[i] = new CLobject(entitymodelfile,0);
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
	CLfile* entitydefinitionfile = entitya->findbyextension(u8".ini");
	if(entitydefinitionfile==0) err(__FILE__,__func__,u8"no entity definition found");
	def = clformat->loadini(entitydefinitionfile);
	//*
	
	//load entity attributes
	maxspeed    = clstring->tolong((*def)[u8"speed"]);
	healthmax = health = clstring->tolong((*def)[u8"health"]);
	shieldmax = shield = clstring->tolong((*def)[u8"shield"]);
	shieldrate	= clstring->tolong((*def)[u8"shieldrate"]);
	armor		= clstring->tolong((*def)[u8"armor"]);
	ammomounts	= clstring->tolong((*def)[u8"ammomounts"]);
	points		= 10; //CLstring->tolong((*def)["points"]);
	//*
	
	//load ammo types
	const xchar* ammoext[4] = { u8"ammotype0",u8"ammotype1",u8"ammotype2",u8"ammotype3" };
	const xchar* fireext[4] = { u8"firerate0",u8"firerate1",u8"firerate2",u8"firerate3" };
	ammotype = new xlong[ammomounts];
	firerate = new xlong[ammomounts];
	fireupdate = new xlong[ammomounts];
	for(xlong j=0; j<ammomounts; j++)
	{
		ammotype[j] = clstring->tolong((*def)[ammoext[j]]);
		firerate[j] = clstring->tolong((*def)[fireext[j]]);
		fireupdate[j] = clwindow->getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype,mark);
	//*
	
	//load csv if present (*.csv)
	csv = 0;
	CLfile* entityaifile = entitya->findbyextension(u8".csv");
	if(entityaifile!=0) csv = clformat->loadcsv(entityaifile);
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	speed = 0;
	gear = 0;
	visible = 0;
	active = 0;
	shieldupdate = lastupdate = clwindow->getmilliseconds();
	//*
}

template<int I>
CLentity<I>::CLentity(CLentity* entityptr) //! noncritical
{
	//set mark pointer from level
	mark = entityptr->mark;
	markmax = entityptr->markmax;
	//*
	
	//for each model
	for(xlong i=0; i<I; i++)
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
	for(xlong j=0; j<ammomounts; j++) { fireupdate[j] = clwindow->getmilliseconds(); }
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
	shieldupdate = lastupdate = clwindow->getmilliseconds();
	//*
}

template<int I>
CLentity<I>::~CLentity<I>() //! noncritical
{
	delete def;
	delete ammoman; 
	delete[] ammotype;
	delete[] firerate;
	delete[] fireupdate;
	
	for(xlong i=0; i<I; i++)
	{
		delete model[i];
		delete expl[i];
		delete boundingbox[1][i];
	}
}

template<int I>
void CLentity<I>::display(xlong modelorshadow) //! critical
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
				for(uxlong i=0; i<I; i++) { model[i]->display(sposition,FLAT + AMBIENT); }
				//*
				
				//display ammo
				ammoman->display();
				//*
			break;
			
			case 1:
				//display shadow(s)
				for(xlong i=0; i<I; i++) { model[i]->display(sposition,SHADOW); }
				//*
			break;
		}
	}
}

template<int I>
void CLentity<I>::hit(xlong h) //! critical
{
	if(shield>0) { shield -=h; }
	else { health -= h; }
	
	if(shield<0) { health -= -shield; } 
}

template<int I>
void CLentity<I>::start() //! noncritical
{
	xlong currtime = clwindow->getmilliseconds();
	lastupdate = currtime;
	shieldupdate = currtime;
	for(xlong i=0; i<ammomounts; i++) fireupdate[i] = currtime;
} 

template<int I>
void CLentity<I>::pause() //! noncritical
{
	xlong currtime = clwindow->getmilliseconds();
	lastupdate = currtime;
	shieldupdate = currtime;
	for(xlong i=0; i<ammomounts; i++) fireupdate[i] = currtime;
	ammoman->pause();
}
///*

#endif

