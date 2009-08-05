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
#include "CLformat.hh"
#include "CLammo.hh"
#include "CLplayer.hh"


class CLenemy : public virtual CLcl
{
	protected:
		CLobject* model;
		CLmatrix* cllinear;
		CLammomanager* ammoman;

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
		
		xlong ammotypecount;
		xlong* ammotypes;
		xlong* firerate;
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;
		xlong gear;
		xlong active;
		xlong visible;
		xlong points;
		xlong lastupdate[2];
		xlong* aiarray;
		xlong airightside;
		xlong aitype; //0=straight, 1=vary x along aiarray, 2=aiarray to polygon 
		xlong aggrolevel;
		CLbox* aggrobox;

		void setspeed();
		void fire(xlong at,xlong d,xlong i,xlong mark);
		void hit();
		void pretransform();
		void transform();
		xlong collision(xlong m);
		xlong aihelper();
		
	public:
		CLenemy(CLfile* enemylib);
		CLenemy(CLenemy* e,CLlvector& s);
		~CLenemy();
		
		xlong update(xlong mark,CLplayer* p);
		void display(xlong mark);
		void shadow(xlong mark);
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
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

void CLenemy::fire(xlong at,xlong d,xlong i,xlong mark)
{
	CLfvector startposition = position;
	CLfvector* ammodocking  = model->getdockingpoint(d,i);
	startposition.x += ammodocking->x;
	startposition.y -= ammodocking->y;
	startposition.z += ammodocking->z;
	ammoman->fire(at,startposition,direction,mark);
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

xlong CLenemy::collision(xlong m)
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= m;
	xlong bc = CLgame::boundary(tposition,*boundingbox,1); //boundary check: (check if completely outside screen)
	tposition.y += m;

	if(bc==0)
	{
		visible = 1;
	}
	//*

	//~ //terrain collision test
	//~ xlong xangle = 0;
	//~ xlong yangle = 0;
	//~ xlong zdiff  = 0;
	//~ xlong tc = CLgame::terrain(ll,boundingbox,oboundingbox,tposition,position,xangle,yangle,zdiff); //terrain collision check: (check if player collides with terrain block)
 //~ 
	//~ if(tc!=0)
	//~ {
		//~ gear=0;
		//~ setspeed();
		//~ r++;
	//~ }
	//~ //*
	
	//~ //temp
		//~ //tposition.z += zdiff; //only growth when uphill, constant on downhill, funny :)
		//~ if(zdiff!=0) { CLsystem::print("z level change: ",0); CLsystem::print(zdiff); }
		//~ 
		//~ //rotate x about xangle,y about yangle
		//~ //cllinear->rotate(xangle,0,0);
		//~ //if(xangle!=0) CLsystem::print(xangle);
		//~ //if(yangle!=0) CLsystem::print(yangle);
	//~ //*
//~ 
	//~ //enemy collision check
	//~ 
		//~ //...
	//~ 
	//~ //*
	
	return r;
}

xlong CLenemy::aihelper()
{
	return airightside;	
}

CLenemy::CLenemy(CLfile* enemylib)
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
	if(ed==-1) CLsystem::exit(1,0,__func__,"no enemy definition found");
	xmap* eini = CLformat::loadini(enemya->members[ed]);
	//*

	//find enemy ai, has to have extension .csv
	xlong pa = CLutils::findarmember(enemya,".csv");
	if(pa==-1) CLsystem::exit(1,0,__func__,"no enemy ai found");
	aiarray = CLformat::loadcsv(enemya->members[pa]);
	aitype = aiarray[0];
	aggrolevel = aiarray[1];
	aiarray = &aiarray[2];
	//*
	
	//find enemy model
	xlong pm = CLutils::findarmember(enemya,".y3d");
	if(pm==-1) CLsystem::exit(1,0,__func__,"no enemy model file found");
	model = new CLobject(enemya->members[pm],0);
	//*

	//set and copy bounding box of model
	boundingbox = new CLbox;
	oboundingbox = model->getboundingbox();
	*boundingbox = *oboundingbox;
	//*
	
	position = tposition = 0;
	
	//load enemy info from ini definition file
	health		= CLsystem::ato((*eini)["health"]);
	shield		= CLsystem::ato((*eini)["shield"]);
	shieldrate	= CLsystem::ato((*eini)["shieldrate"]);
	armor		= CLsystem::ato((*eini)["armor"]);
	speeddir.x  = 0;
	speeddir.y  = CLsystem::ato((*eini)["speed"]);
	speeddir.z  = 0;
	points = CLsystem::ato((*eini)["points"]);
	ammotypecount = CLsystem::ato((*eini)["ammotypecount"]);
	ammotypes  = new xlong[ammotypecount];
	firerate   = new xlong[ammotypecount];
	ammotypes[0] = CLsystem::ato((*eini)["ammo0"]);
	firerate[0]	= CLsystem::ato((*eini)["firerate0"]); //every X ms
	ammotypes[1] = CLsystem::ato((*eini)["ammo1"]);
	firerate[1]	= CLsystem::ato((*eini)["firerate1"]); //every X ms
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammotypecount,ammotypes);
	//*
	
	//create agrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox->c[0].x,boundingbox->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox->c[3].x,boundingbox->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox->c[3].x,yres>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox->c[0].x,yres>>1,0);
	//*
	
	//set remaining enemy attributes
	speed = 0;
	gear = 0;
	active = 0;
	visible = 0;
	direction.x = 0;
	direction.y = -1;
	direction.z = 0;	
	lastupdate[0] = CLsystem::getmilliseconds();
	lastupdate[1] = CLsystem::getmilliseconds();
	//*
	
	delete eini;
}

CLenemy::CLenemy(CLenemy* e,CLlvector& s)
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
	
	//set and adjust (start) position to floating X pixel above ground
	position = s;
	position.z = 100 - position.z - 12 - 30;
	tposition = position;
	//*
	
	//copy enemy info from argument enemy
	health		= e->health;
	shield		= e->shield;
	shieldrate	= e->shieldrate;
	armor		= e->armor;
	speeddir.x  = 0;
	speeddir.y  = e->speeddir.y;
	speeddir.z  = 0;
	firerate    = e->firerate;
	ammotypecount = e->ammotypecount;
	ammotypes   = e->ammotypes;
	ammoman = new CLammomanager(ammotypecount,ammotypes);
	aggrobox = new CLbox(*(e->aggrobox));
	points      = e->points;
	//*
	
	//set remaining enemy attributes
	speed = 0;
	gear = 0;
	active = 0;
	visible = 0;
	direction.x = 0;
	direction.y = -1;
	direction.z = 0;	
	lastupdate[0] = CLsystem::getmilliseconds();
	lastupdate[1] = CLsystem::getmilliseconds();
	//*
}

CLenemy::~CLenemy()
{
	//release everything this class grabbed
	delete cllinear;
	delete ammoman;
	delete boundingbox;
	delete model;
	//*
}

xlong CLenemy::update(xlong mark,CLplayer* p)
{
	//check if to activate
	if(active!=1 && (mark-100)<position.y)
	{
		active=1;
	}
	//*
	
	//check if destroyed (return points)
	
	//*
	
	//check if level is left (return -1)
	
	//*
	
	
	if(active==1)
	{
		ammoman->update();

		xlong time = CLsystem::getmilliseconds();

		cllinear->unit();
		
		//update enemy through ai array
		switch(aitype)
		{
			case 0: gear=1; setspeed(); break;
		}		
		//*
		
		//fire at player?
		xlong fc = CLgame::collision(position,*aggrobox,p->getposition(),*(p->getboundingbox()),1);
		if( fc != 0 )
		{
			if(time >= lastupdate[1] + firerate[0])
			{
				fire(0,4,0,mark);
				fire(0,4,1,mark);
				lastupdate[1] = time;
			}
		}

		//update test position
		if(time >= lastupdate[0] + 20)
		{
			tposition.x = position.x - speed.x;
			tposition.y = position.y + speed.y;
			tposition.z = position.z + speed.z;
			
			lastupdate[0] = time;	
		}
		//*

		//check if test position doesn't collide with anything
		if(collision(mark)==0)
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
	
	return 0;
}

void CLenemy::display(xlong mark)
{
	if(active==1 && visible==1)
	{
		//model display
		sposition.x = position.x;
		sposition.y = position.y - mark;
		sposition.z = position.z;
		model->display(sposition,FLAT + AMBIENT);
		//*
		
		//ammo display
		ammoman->display();
		//*
	}

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

void CLenemy::shadow(xlong mark)
{
	sposition.x = position.x;
	sposition.y = position.y - mark;
	sposition.z = position.z;
	model->display(sposition,SHADOW);
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

#endif


