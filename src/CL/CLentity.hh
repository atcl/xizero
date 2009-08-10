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


class CLentity : public virtual CLcl
{
	protected:
		CLmatrix* linear;
		CLammomanager* ammoman;
		CLobject** model;
		xmap* def;
		xlong* csv;
		
		CLbox** boundingbox[2];
		CLfvector position;
		CLfvector tposition;
		CLlvector sposition;
		CLfvector* direction;
		CLfvector speed;
		CLfvector speeddir;
		CLfvector* angles;
		uxlong models;
		xlong* mark;
		xlong gear;
		xlong active;
		xlong lastupdate;
		
		xlong ammotypecount;
		CLlvector* ammotypes;
		xlong* firerate;
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;
		xlong points;
		
		void setspeed();
		void fire();
		void hit();
		void pretransform(xlong m);
		void transform(xlong m);
		xlong collision();
	
	public:
		CLentity(CLfile* entitya,xlong* markptr);
		~CLentity();
		
		xlong update();
		void display(bool modelorshadow=0);
		xlong gethealth() return health;
		xlong getshield() return shield;
		CLfvector* getposition() return &position
		CLbox* getboundingbox() return boundingbox[0][0];
};

void CLentity::setspeed()
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

void CLentity::fire()
{
	
}

void CLentity::hit()
{
	
}

void CLentity::pretransform(xlong m)
{
	
}

void CLentity::transform(xlong m)
{
	
}

xlong CLentity::collision()
{
	
}

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

	//count models
	while(CLutils::findarmember(entitya,".y3d")!=-1)
	{
		models++;
	}
	model = new CLobject*[models];
	boundingbox[0] = new CLbox*[models]; 
	boundingbox[1] = new CLbox*[models]; 
	direction = new CLfvector[models];
	angles = new CLfvector[models];
	//*

	//for each model
	for(uxlong i=0; i<models; i++)
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
	//*
	
	//load ammo types
	
	//*
	
	//create ammo manager
	
	//*
	
	//load csv if present (*.csv)
	xlong ec = CLutils::findarmember(entitya,".csv");
	if(ec!=-1) csv = CLformat::loadcsv(entitya->members[ec]);
	//*
	
	//set remaining entity attributes
	angles = 0;
	speed = 0;
	gear = 0;
	lastupdate[2] = CLsystem::getmilliseconds();
	//*
}

CLentity::~CLentity()
{
	delete linear;
	for(uxlong i=0; i<models; i++)
	{
		delete model[i];
		delete boundingbox[1];
		delete model;
		delete boundingbox[0]; 
		delete boundingbox[1];
		delete direction;
		delete angles;
	}
	
	//...
}

xlong CLentity::update()
{
	
}

void CLentity::display(bool modelorshadow)
{
	
}

#endif
