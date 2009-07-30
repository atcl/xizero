//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLENEMY
#define HH_CLENEMY
//#pragma message "Compiling " __FILE__ " ! TODO: interaction, see player for ctor and attr"


#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLcl.hh"
#include "CLapi.hh"
#include "CLgame.hh"
#include "CLsprites.hh"
#include "CLformat.hh"

#ifndef CLAMMO
#define CLAMMO
struct CLammo
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
	CLfvector d;
	float v;
};
#endif

class CLenemy : public virtual CLcl
{
	protected:
		CLobject* model;
		CLmatrix* cllinear;
		CLlist*   ammolist;

	private:
		CLbox* boundingbox;
		CLbox* oboundingbox;
		
		CLfvector position;
		CLfvector tposition;
		CLlvector sposition;
		CLfvector direction;
		CLfvector speed;
		CLfvector speeddir;
		CLfvector tilt; //meaning mainly z-tilt (ie on ramps)
		
		CLammo* ammotype[2];
		xlong firerate[2];
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;
		xlong gear;
		xlong active;
		xlong points;
		xlong lastupdate[3];
		xlong* aiarray;
		xlong airightside;

		void setspeed();
		void fire(xlong at,xlong d,xlong i);
		void hit();
		void pretransform();
		void transform();
		xlong collision(CLfbuffer* ll,xlong m);
		xlong aihelper();
		
	public:
		CLenemy(CLfile* enemylib,CLlvector& s);
		CLenemy(CLenemy* e);
		~CLenemy();
		
		xlong update(CLfbuffer* ll,xlong mark);
		void display(xlong m);
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
		bool  isactive();
};


void CLenemy::setspeed()
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

void CLenemy::fire(xlong at,xlong d,xlong i)
{
	if(at<4)
	{
		CLammo* currammo = new CLammo();
		currammo->comsprite = ammotype[at]->comsprite;
		currammo->v = ammotype[at]->v;
		CLfvector* ta = model->getdockingpoint(d,i);
		currammo->p.x = position.x + ta->x;
		currammo->p.y = position.y - ta->y;
		currammo->p.z = position.z + ta->z;
		currammo->d = direction;
		ammolist->append(currammo,"at" + xchar(at+30) );
	}
}

void CLenemy::pretransform()
{
	//transform enemy bounding box
	boundingbox->c[0] = cllinear->transform(boundingbox->c[0]);
	boundingbox->c[1] = cllinear->transform(boundingbox->c[1]);
	boundingbox->c[2] = cllinear->transform(boundingbox->c[2]);
	boundingbox->c[3] = cllinear->transform(boundingbox->c[3]);
	boundingbox->c[4] = cllinear->transform(boundingbox->c[4]);
	boundingbox->c[5] = cllinear->transform(boundingbox->c[5]);
	boundingbox->c[6] = cllinear->transform(boundingbox->c[6]);
	boundingbox->c[7] = cllinear->transform(boundingbox->c[7]);
	//*
}

void CLenemy::transform()
{
	//transform enemy
	model->update(cllinear); //transform model(s)
	direction = cllinear->transform(direction); //transform direction vector
	speeddir = cllinear->transform(speeddir); //transform (constant) speeddir vector
	//tilt = cllinear->transform(tilt); //transform tilt vector
	//*
}

xlong CLenemy::collision(CLfbuffer* ll,xlong m)
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= m;
	xlong bc = CLgame::boundary(tposition,*boundingbox); //boundary check: (check if game screen is left)
	tposition.y += m;

	if(bc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*

	//terrain collision test
	xlong xangle = 0;
	xlong yangle = 0;
	xlong zdiff  = 0;
	xlong tc = CLgame::terrain(ll,boundingbox,oboundingbox,tposition,position,xangle,yangle,zdiff); //terrain collision check: (check if player collides with terrain block)
 
	if(tc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*
	
	//temp
		//tposition.z += zdiff; //only growth when uphill, constant on downhill, funny :)
		if(zdiff!=0) { CLsystem::print("z level change: ",0); CLsystem::print(zdiff); }
		
		//rotate x about xangle,y about yangle
		//cllinear->rotate(xangle,0,0);
		//if(xangle!=0) CLsystem::print(xangle);
		//if(yangle!=0) CLsystem::print(yangle);
	//*

	//enemy collision check
	
		//...
	
	//*

	return r;
}

xlong CLenemy::aihelper()
{
	return airightside;	
}

CLenemy::CLenemy(CLfile* enemylib,CLlvector& s)
{
	//matrix for enemy transformations
	cllinear = new CLmatrix(1);
	//*
	
	//load enemy archive 
	//CLfile* enemyraw = CLsystem::getfile(enemylib);
	arfile* enemya = CLformat::loadar(enemylib);
	//*

	//find enemy definition, has to have extension .ini
	xlong ed = CLutils::findarmember(enemya,".ini");
	say(ed);
	if(ed==-1) CLsystem::exit(1,0,__func__,"no enemy definition found");
	xmap* eini = CLformat::loadini(enemya->members[ed]);
	//*

	//find enemy ai, has to have extension .csv
	xlong pa = CLutils::findarmember(enemya,".csv");
	if(pa==-1) CLsystem::exit(1,0,__func__,"no enemy ai found");
	aiarray = CLformat::loadcsv(enemya->members[pa]);
	//*
	
	//find enemy model
	xlong pm = CLutils::findarmember(enemya,"0.y3d");
	if(pm==-1) CLsystem::exit(1,0,__func__,"no enemy model file found");
	model = new CLobject(enemya->members[pm],0);
	//*

	//set and copy bounding box of model
	boundingbox = new CLbox;
	oboundingbox = model->getboundingbox();
	*boundingbox = *oboundingbox;
	//*
	
	//~ //set and adjust (start) position to floating X pixel above ground
	//~ position = s;
	//~ position.z += 95;
	//~ tposition = position;
	//~ //*
	
	//create list for all ammo fired by enemy
	ammolist = new CLlist();
	//*
	
	//load enemy info from definition file (...later ini)
	health		= CLsystem::ato((*eini)["health"]);
	shield		= CLsystem::ato((*eini)["shield"]);
	shieldrate	= CLsystem::ato((*eini)["shieldrate"]);
	armor		= CLsystem::ato((*eini)["armor"]);
	speeddir.x  = 0;
	speeddir.y  = -CLsystem::ato((*eini)["speed"]);
	speeddir.z  = 0;
	ammotype[0] = new CLammo;
	switch(CLsystem::ato((*eini)["ammo0"])) { case 0: ammotype[0]->comsprite = CLsprites::drawplasma; break; }
	ammotype[0]->v = 16;
	ammotype[0]->p = CLfvector();
	ammotype[0]->d = CLfvector();
	firerate[0]	= CLsystem::ato((*eini)["firerate0"]); //every X ms
	ammotype[1] = new CLammo;
	switch(CLsystem::ato((*eini)["ammo1"])) { case 0: ammotype[1]->comsprite = CLsprites::drawplasma; break; }
	ammotype[1]->v = 16;
	ammotype[1]->p = CLfvector();
	ammotype[1]->d = CLfvector();
	firerate[1]	= CLsystem::ato((*eini)["firerate1"]); //every X ms
	//*
	
	//set remaining enemy attributes
	speed = 0;
	gear = 0;
	active = 0;
	direction.x = 0;
	direction.y = -1;
	direction.z = 0;	
	lastupdate[0] = CLsystem::getmilliseconds();
	lastupdate[1] = CLsystem::getmilliseconds();
	lastupdate[2] = CLsystem::getmilliseconds();
	//*
	
	delete eini;
}

CLenemy::CLenemy(CLenemy* e)
{
	//matrix for enemy transformations
	cllinear = new CLmatrix(1);
	//*
	
	//copy aiarray and model
	aiarray = e->aiarray;
	model = e->model;
	//

	//set and copy bounding box of model
	boundingbox = new CLbox;
	oboundingbox = model->getboundingbox();
	*boundingbox = *oboundingbox;
	//*
	
	//~ //set and adjust (start) position to floating X pixel above ground
	//~ position = s;
	//~ position.z += 95;
	//~ tposition = position;
	//~ //*
	
	//create list for all ammo fired by enemy
	ammolist = new CLlist();
	//*
	
	//load enemy info from definition file (...later ini)
	health		= e->health;
	shield		= e->shield;
	shieldrate	= e->shieldrate;
	armor		= e->armor;
	speeddir.x  = 0;
	speeddir.y  = e->speed.y;
	speeddir.z  = 0;
	ammotype[0] = new CLammo;
	ammotype[0]->comsprite = e->ammotype[0]->comsprite;
	ammotype[0]->v = 16;
	ammotype[0]->p = CLfvector();
	ammotype[0]->d = CLfvector();
	firerate[0]	= e->firerate[0]; //every X ms
	ammotype[1] = new CLammo;
	ammotype[1]->comsprite = e->ammotype[0]->comsprite;
	ammotype[1]->v = 16;
	ammotype[1]->p = CLfvector();
	ammotype[1]->d = CLfvector();
	firerate[1]	= e->firerate[1]; //every X ms
	//*
	
	//set remaining enemy attributes
	speed = 0;
	gear = 0;
	active = 0;
	direction.x = 0;
	direction.y = -1;
	direction.z = 0;	
	lastupdate[0] = CLsystem::getmilliseconds();
	lastupdate[1] = CLsystem::getmilliseconds();
	lastupdate[2] = CLsystem::getmilliseconds();
	//*
}

CLenemy::~CLenemy()
{
	//release everything this class grabbed
	delete cllinear;
	delete ammolist;
	delete ammotype[0];
	delete ammotype[1];
	delete boundingbox;
	delete model;
	//*
}

xlong CLenemy::update(CLfbuffer* ll,xlong mark)
{
	xlong time = CLsystem::getmilliseconds();
	
	//check if active
	
	//*
	
	//ammo update
	CLammo* currammo;
	if(time >= lastupdate[0] + 20)
	{
		for(int i=0; i<ammolist->getlength();i++)
		{
			ammolist->setindex(i);
			currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
			currammo->p.y -= mark;
			if(CLgame::boundary(currammo->p)!=0) ammolist->delcurrent(0);
			else
			{
				currammo->p.x += currammo->v * currammo->d.x;
				currammo->p.y -= currammo->v * currammo->d.y;
				currammo->p.z += currammo->v * currammo->d.z;
			}
			currammo->p.y += mark;
		}
		lastupdate[0] = time;
	}
	//*
	


	cllinear->unit();

	//update enemy through ai array
	
		//! todo
	
	//

	//update test position
	if(time >= lastupdate[2] + 20)
	{
		tposition.x = position.x - speed.x;
		tposition.y = position.y + speed.y;
		tposition.z = position.z + speed.z;
		
		lastupdate[2] = time;	
	}
	//*

	//check if test position doesn't collide with anything
	if(collision(ll,mark)==0)
	{	
		transform();
		
		position = tposition;
	}
	//*
	
	//if collision reset bounding box to state before pretransformation
	else
	{
		*boundingbox = *oboundingbox;
	}
	//*
}

void CLenemy::display(xlong m)
{
	//!todo: check wether enemy is on screen or not
	
	//model display
	sposition.x = position.x;
	sposition.y = position.y - m;
	sposition.z = position.z;
	model->display(sposition,FLAT + AMBIENT);
	//*
	
	//ammo display
	CLammo* currammo;
	for(int i=0; i<ammolist->getlength();i++)
	{
		ammolist->setindex(i);
		currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
		currammo->p.y -= m;
		currammo->comsprite(currammo->p.x,currammo->p.y);
		currammo->p.y += m;
	}
	//*

	//temp!
	//~ CLgfx1::drawpolygon(
//~ sposition.x+boundingbox[0]->b1.x,
//~ sposition.y-boundingbox[0]->b1.y,
//~ sposition.x+boundingbox[0]->b2.x,
//~ sposition.y-boundingbox[0]->b2.y,
//~ sposition.x+boundingbox[0]->b3.x,
//~ sposition.y-boundingbox[0]->b3.y,
//~ sposition.x+boundingbox[0]->b4.x,
//~ sposition.y-boundingbox[0]->b4.y,
//~ 0x00FFFFFF);

	//CLgfx1::drawrectangle(65,0,735,599,0x00FF00FF);

	//*
}

xlong CLenemy::gethealth()
{
	return health;
}

xlong CLenemy::getshield()
{
	return shield;
}

xlong CLenemy::getx()
{
	return position.x;
}

xlong CLenemy::gety()
{
	return position.y;
}

xlong CLenemy::getz()
{
	return position.z;
}

bool CLenemy::isactive()
{
	return active;
}

#endif


