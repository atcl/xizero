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
		xlong fireupdate[I];
		
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
	ammotype = new xlong[ammomounts];
	firerate = new xlong[ammomounts];
	for(uxlong j=0; j<ammomounts; j++)
	{
		ammotype[j] = CLsystem::ato((*def)["ammotype"xchar(j)]);
		firerate[j] = CLsystem::ato((*def)["firerate"xchar(j)]);
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
	angles = 0;
	speed = 0;
	gear = 0;
	lastupdate = CLsystem::getmilliseconds();
	//*
}

template<int I>
CLentity::CLentity(CLentity* entityptr)
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
		*(model[i]) = *(entityptr->model[i]);
		//*
		
		//set model(s) bounding boxes
		boundingbox[0][i] = model[i]->getboundingbox()
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
	*(ammotype) = *(entityptr->ammotype);
	*(firerate) = *(entityptr->firerate);
	for(uxlong j=0; j<ammomounts; j++)
	{
		fireupdate[j] = CLsystem::getmilliseconds();
	}
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammomounts,ammotype);
	//*
	
	//load csv if present (*.csv)
	csv = entityptr->csv;
	//*
	
	//set remaining entity attributes
	speeddir = 0;
	angles = 0;
	speed = 0;
	gear = 0;
	lastupdate = CLsystem::getmilliseconds();
	//*
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
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLfbuffer* ll);
	
	public:
		CLplayer(CLfile playera,xlong* m,CLvector& playerp,xlong pts=0);
		~CLplayer();
		
		xlong update(xchar input,xchar turbo,CLfbuffer* ll);
};

void CLplayer::pretransform(bool m)
{
	//transform tower boundng box
	boundingbox[1][1]->c[0] = cllinear->transform(boundingbox[1][1]->c[0]);
	boundingbox[1][1]->c[1] = cllinear->transform(boundingbox[1][1]->c[1]);
	boundingbox[1][1]->c[2] = cllinear->transform(boundingbox[1][1]->c[2]);
	boundingbox[1][1]->c[3] = cllinear->transform(boundingbox[1][1]->c[3]);
	boundingbox[1][1]->c[4] = cllinear->transform(boundingbox[1][1]->c[4]);
	boundingbox[1][1]->c[5] = cllinear->transform(boundingbox[1][1]->c[5]);
	boundingbox[1][1]->c[6] = cllinear->transform(boundingbox[1][1]->c[6]);
	boundingbox[1][1]->c[7] = cllinear->transform(boundingbox[1][1]->c[7]);
	//*
	
	//transform chassis bounding box if wanted
	if(m==0)
	{
		boundingbox[1][0]->c[0] = cllinear->transform(boundingbox[1][0]->c[0]);
		boundingbox[1][0]->c[1] = cllinear->transform(boundingbox[1][0]->c[1]);
		boundingbox[1][0]->c[2] = cllinear->transform(boundingbox[1][0]->c[2]);
		boundingbox[1][0]->c[3] = cllinear->transform(boundingbox[1][0]->c[3]);
		boundingbox[1][0]->c[4] = cllinear->transform(boundingbox[1][0]->c[4]);
		boundingbox[1][0]->c[5] = cllinear->transform(boundingbox[1][0]->c[5]);
		boundingbox[1][0]->c[6] = cllinear->transform(boundingbox[1][0]->c[6]);
		boundingbox[1][0]->c[7] = cllinear->transform(boundingbox[1][0]->c[7]);
	}
	//*
}

void CLplayer::transform(bool m)
{
	//transform tower
	model[1]->update(cllinear);
	direction[1] = cllinear->transform(direction[1]);
	//*
	
	//transform chassis if wanted
	if(m==0)
	{
		model[0]->update(cllinear);
		direction[0] = cllinear->transform(direction[0]); 
		speeddir = cllinear->transform(speeddir);
	}
	//*
}

xlong CLplayer::collision(CLfbuffer* ll)
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= mark;
	xlong bc = CLgame::boundary(tposition,*boundingbox[1][0]);
	tposition.y += mark;

	if(bc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*

	//terrain collision test
	float zdiff  = 0;
	xlong xangle = 0;
	xlong yangle = 0;
	xlong tc = CLgame::terrain(ll,boundingbox[1][0],boundingbox[0][0],tposition,position,xangle,yangle,zdiff); 
 
	if(tc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*
	
	//adjust z position
	if(CLmath::absolute(zdiff)>1) tposition.z = zdiff-12;
	//*	
	
	//adjust rotation around x andy axis
		//rotate x about xangle,y about yangle
		//if(CLmath::absolute(xangle)>=1) cllinear->rotate(xangle,yangle,0);
		//if(CLmath::absolute(xangle)>=1) CLsystem::print(xangle);
		//if(CLmath::absolute(yangle)>=1) CLsystem::print(yangle);
	//*

	return r;
}

CLplayer::CLplayer(CLfile playera,xlong* m,CLvector& playerp,xlong pts) : CLentity(playera,m)
{
	//set and adjust (start) position to floor
	position = playerp;
	position.z = 100 - position.z - 12;
	tposition = position;
	//*
	
	//set players points from levels before
	points = pts;
	//*
	
	//set player specific attributes
	speeddir.y  = -CLsystem::ato((*def)["speed"]);
	direction[0].y = direction[1].y = 1;
	active = 1;
	//*
}

CLplayer::~CLplayer()
{
	delete def;
}

xlong update(xchar input,xchar turbo,CLfbuffer* ll)
{
	//update ammo
	ammoman->update();
	//*
	
	//init variables
	xlong time = CLsystem::getmilliseconds();
	cllinear->unit();
	bool what = 0;
	xlong tempangle = 0;
	//*
	
	switch(input)
	{
		//stop backward driving and drive forward
		case 82:
			if(gear==-1) { gear=0; setspeed(); }
			else { gear=1; setspeed(); }
		break;
		//*

		//stop forward driving and drive backward
		case 84:
			if(gear==1) { gear=0; setspeed(); }
			else { gear=-1; setspeed(); }
		break;
		//*
	}
	
	switch(turbo)
	{
		//arrow left -> turn left
		case 81: 
			tempangle = 5;
			cllinear->rotate(0,0,5);
			pretransform(0);	
			setspeed();
		break;
		//*

		//arrow right -> turn right
		case 83: 
			tempangle = -5;
			cllinear->rotate(0,0,-5);
			pretransform(0);
			setspeed();
		break;
		//*

		//a -> turn tower left
		case 97: 
			tempangle = 5;
			cllinear->rotate(0,0,5);
			pretransform(1);
			what=1;
		break;
		//*

		//d -> turn tower right
		case 100: 
			tempangle = -5;
			cllinear->rotate(0,0,-5);
			pretransform(1);
			what=1;
		break;
		//*
		
		//reset tower
		case 'w':
		if(angles[1].z>180) angles[1].z-=360;
		if(angles[1].z<-180) angles[1].z+=360;
		tempangle = CLmath::sign(angles[0].z-angles[1].z) * 5;
		cllinear->rotate(0,0,tempangle);
		pretransform(1);
		what=1;
		break;
		//*

		//space -> fire tower weapon
		case 32: 
			if(time >= fireupdate[1] + firerate[1])
			{
				fire(1,4,0,1);
				fireupdate[1] = time;
			}
		break;
		//*

		//strg -> fire chassis weapon(s)
		case -29: 
			if(time >= fireupdate[0] + firerate[0])
			{
				fire(0,3,0,0);
				fire(0,3,1,0);
				fireupdate[0] = time;
			}
		break;
		//*

		case 'q': //fire (tachyon) laser
		break; //*

		case 's': //fire laser
		break; //*

		case 'e': //action (use) key
		break; //*
	}

	//update test position
	if(time >= lastupdate + 20)
	{
		tposition.x = position.x - speed.x;
		tposition.y = position.y + speed.y;
		tposition.z = position.z - speed.z;
		lastupdate = time;	
	}
	//*

	//check if test position doesn't collide with anything
	if(collision(ll)==0)
	{	
		transform(what);
		if(what==0) { angles[0].z = (angles[0].z + tempangle)%360; angles[1].z = (tempangle + angles[1].z)%360; }
		else angles[1].z = (tempangle + angles[1].z)%360;
		position = tposition;
	}
	//*
	
	//if collision reset bounding box to state before pretransform
	else
	{
		*(boundingbox[1][0]) = *(boundingbox[0][0]);
	}
	//*
}

//********

class CLenemy : CLentity<1>
{
	private:
		xlong* aiarray;
		xlong aitype; //0=straight, 1=vary x along aiarray, 2=aiarray to polygon 
		xlong aggrolevel;
		CLbox* aggrobox;
	
		void pretransform();
		void transform();
		xlong collision();
	
	public:
		CLenemy(CLfile enemya);
		CLenemy(CLenemy* enemyptr,CLvector& enemyp);
		~CLenemy();
		
		xlong update();
};

void CLenemy::pretransform()
{
	//transform enemy bounding box
	boundingbox[1]->c[0] = cllinear->transform(boundingbox[1]->c[0]);
	boundingbox[1]->c[1] = cllinear->transform(boundingbox[1]->c[1]);
	boundingbox[1]->c[2] = cllinear->transform(boundingbox[1]->c[2]);
	boundingbox[1]->c[3] = cllinear->transform(boundingbox[1]->c[3]);
	boundingbox[1]->c[4] = cllinear->transform(boundingbox[1]->c[4]);
	boundingbox[1]->c[5] = cllinear->transform(boundingbox[1]->c[5]);
	boundingbox[1]->c[6] = cllinear->transform(boundingbox[1]->c[6]);
	boundingbox[1]->c[7] = cllinear->transform(boundingbox[1]->c[7]);
	//*
}

void CLenemy::transform()
{
	//transform enemy
	model->update(cllinear);
	direction = cllinear->transform(direction);
	speeddir = cllinear->transform(speeddir);
	//*
}

xlong CLenemy::collision()
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = CLgame::boundary(tposition,*boundingbox[1],1);
	tposition.y += *mark;

	if(bc==0)
	{
		visible = 1;
	}
	//*
	
	return r;
}

CLenemy::CLenemy(CLfile enemya) : CLentity(enemya)
{
	//load enemy ai
	aitype = csv[0];
	aggrolevel = csv[1];
	aiarray = &csv[2];
	//*
	
	//create agrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox->c[0].x,boundingbox->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox->c[3].x,boundingbox->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox->c[3].x,yres>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox->c[0].x,yres>>1,0);
	//*
	
	//set enemy specific attributes
	points = enemyptr->points;CLsystem::ato((*def)["points"]);
	speeddir.y  = CLsystem::ato((*def)["speed"]);
	direction.y = -1;
	visible = 0;
	//*	
}

CLenemy::CLenemy(CLenemy* enemyptr,CLvector& enemyp) : CLentity(enemyptr)
{
	//load enemy ai
	aitype = enemyptr->aitype;
	aggrolevel = enemyptr->aggrolevel;
	aiarray = enemyptr->aiarray;
	//*
	
	//create agrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox->c[0].x,boundingbox->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox->c[3].x,boundingbox->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox->c[3].x,yres>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox->c[0].x,yres>>1,0);
	//*
	
	//set enemy specific attributes
	points = enemyptr->points;
	speeddir.y  = enemyptr->speeddir.y;
	direction.y = -1;
	visible = 0;
	//*	
}

CLenemy::~CLenemy()
{
	delete def;
	delete[] aiarray;
	delete aggrobox;
}

xlong CLenemy::update(CLplayer* p)
{
	//check if to activate
	if(active!=1 && (*mark-100)<position.y)
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
			if(time >= fireupdate[0] + firerate[0])
			{
				fire(0,4,0);
				fire(0,4,1);
				fireupdate[0] = time;
			}
		}

		//update test position
		if(time >= lastupdate + 20)
		{
			tposition.x = position.x - speed.x;
			tposition.y = position.y + speed.y;
			tposition.z = position.z + speed.z;
			
			lastupdate = time;	
		}
		//*

		//check if test position doesn't collide with anything
		if(collision()==0)
		{	
			transform();
			position = tposition;
		}
		//*
		
		//if collision reset bounding box to state before pretransformation
		else
		{
			*boundingbox[1] = *boundingbox[0];
		}
		//*
	}
	
	return 0;
}

#endif
