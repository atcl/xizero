//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLENEMY
#define HH_CLENEMY
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLsystem.hh"
#include "CLgame.hh"
#include "CLammo.hh"
#include "CLentity.hh"

#define ENEMYZLEVEL 55

class CLenemy : public CLentity<1>
{
	private:
		CLprogress* hprog;
		xlong* aiarray;
		xlong aitype; //0=straight, 1=vary x along aiarray, 2=aiarray to polygon, 3=1D random walk
		xlong aggrolevel;
		CLbox* aggrobox;
		void pretransform();
		void transform();
		xlong collision();
	public:
		CLenemy(CLfile* enemya,xlong* m,xlong mm);
		CLenemy(CLenemy* enemyptr,CLlvector& enemyp);
		~CLenemy();
		template<int I>xlong update(CLentity<I>* p);
		void displayhud();
};

void CLenemy::pretransform()
{
	//transform enemy bounding box
	boundingbox[1][0]->c[0] = linear->transform(boundingbox[1][0]->c[0]);
	boundingbox[1][0]->c[1] = linear->transform(boundingbox[1][0]->c[1]);
	boundingbox[1][0]->c[2] = linear->transform(boundingbox[1][0]->c[2]);
	boundingbox[1][0]->c[3] = linear->transform(boundingbox[1][0]->c[3]);
	boundingbox[1][0]->c[4] = linear->transform(boundingbox[1][0]->c[4]);
	boundingbox[1][0]->c[5] = linear->transform(boundingbox[1][0]->c[5]);
	boundingbox[1][0]->c[6] = linear->transform(boundingbox[1][0]->c[6]);
	boundingbox[1][0]->c[7] = linear->transform(boundingbox[1][0]->c[7]);
	//*
}

void CLenemy::transform()
{
	//transform enemy
	model[0]->update(linear);
	direction[0] = linear->transform(direction[0]);
	speeddir = linear->transform(speeddir);
	//*
}

xlong CLenemy::collision()
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = CLgame::boundary(tposition,*boundingbox[1][0],1);
	tposition.y += *mark;

	if(bc==0)
	{
		visible = 1;
	}
	//*
	
	return r;
}

CLenemy::CLenemy(CLfile* enemya,xlong* m,xlong mm) : CLentity<1>(enemya,m,mm)
{
	//load enemy ai
	aitype = csv[1];
	aggrolevel = csv[2];
	aiarray = &csv[3];
	//*
	
	//create agrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox[0][0]->c[0].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox[0][0]->c[3].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox[0][0]->c[3].x,yres>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox[0][0]->c[0].x,yres>>1,0);
	//*
	
	//set enemy specific attributes
	points = system->ato((*def)["points"]);
	speeddir.y  = system->ato((*def)["speed"]);
	speeddir.y /= 20;
	direction[0].y = -1;
	//*
	
	//create progress bar
	hprog = new CLprogress(0,0,0,0,health+shield,0,health+shield,0x00FF0000,1,1,0,0,0);
	//*
}

CLenemy::CLenemy(CLenemy* enemyptr,CLlvector& enemyp) : CLentity<1>(enemyptr)
{
	//set and adjust (start) position to floating X pixel above ground
	position = enemyp;
	position.z = ENEMYZLEVEL;
	tposition = position;
	//*
	
	//load enemy ai
	aitype = enemyptr->aitype;
	aggrolevel = enemyptr->aggrolevel;
	aiarray = enemyptr->aiarray;
	//*
	
	//create aggrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox[0][0]->c[0].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox[0][0]->c[3].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox[0][0]->c[3].x,yres>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox[0][0]->c[0].x,yres>>1,0);
	//*
	
	//set enemy specific attributes
	points = enemyptr->points;
	speeddir.y  = enemyptr->speeddir.y;
	direction[0].y = -1;
	//*	
	
	//create progress bar
	hprog = new CLprogress(*(enemyptr->hprog));
	hprog->reset(0,0,40,10,health+shield,0,health+shield,0x00FF0000,0,1,0,0x00C0C0C0,0x00C0C0C0);
	//*
}

CLenemy::~CLenemy()
{
	//delete all allocated objects
	delete def;
	delete[] aiarray;
	delete aggrobox;
	delete hprog;
	//*
}

template<int I>
xlong CLenemy::update(CLentity<I>* p)
{
	//update ammo
	ammoman->update();
	ammoman->collision(p);
	//*
	
	//check if to activate
	if(active==0 && ( (*mark)-100)<position.y)
	{
		active = 1;
	}
	//*

	//check if screen is left behind player
	else if(active==1 && position.y>markmax )
	{
		active = 0;
		visible = 0;
		return 0;
	}
	//*

	//check if destroyed
	if(health<=0 && active!=-1)
	{
		//deactivate and start explosion
		active = -1;
		expl[0]->first(1);
		//*
	}
	//*
	
	//set progressbars
	hprog->setprogress(health+shield);
	//*
	
	if(active==1)
	{
		xlong time = system->getmilliseconds();

		linear->unit();
		
		//update enemy through ai array
		switch(aitype)
		{
			case 0: gear=1; setspeed(); break;
		}		
		//*
		
		//fire at player?
		xlong fc = CLgame::collision(position,*aggrobox,*(p->getposition()),*(p->getboundingbox()),1);
		if( fc != 0 )
		{
			if(time >= fireupdate[0] + firerate[0])
			{
				fire(0,4,0,p->getposition()->z);
				fire(0,4,1,p->getposition()->z);
				fireupdate[0] = time;
			}
		}

		//update position
		float inter = time-lastupdate;
		tposition.x = position.x - (inter*speed.x);
		tposition.y = position.y + (inter*speed.y);
		tposition.z = position.z - (inter*speed.z);
		lastupdate = time;	
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
	else if(active==-1)
	{
		if(expl[0]->next()==1) return points;
	}
	else lastupdate = system->getmilliseconds();
	
	return -1;
}

void CLenemy::displayhud()
{
	if(active==1)
	{
		//draw enemy's healthbar
		xlong tempx = (boundingbox[0][0]->c[0].x + boundingbox[0][0]->c[1].x) / 2;
		hprog->setx(tempx + sposition.x - 20);
		hprog->sety(position.y - *mark - 20);
		hprog->draw();
		//*
	}
}

#endif


