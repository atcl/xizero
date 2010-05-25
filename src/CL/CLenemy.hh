///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLENEMY
#define HH_CLENEMY
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLgame.hh"
#include "CLammo.hh"
#include "CLentity.hh"
#include "CLwindow.hh"
///*

///header
/* class name:	CLenemy
 * 
 * description:	The enemy (entity) class.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///declarations
#define ENEMYZLEVEL 55
///*

///definitions
class CLenemy : public CLentity<1>
{
	private:
		static CLgame&   clgame;
		static CLstring& clstring;
	protected:
		CLprogress* hprog;
		xlong*      aiarray;
		xlong       aitype;
		xlong       aggrolevel;
		CLbox*      aggrobox;
		
		void  cruise();
		void  pretransform();
		void  transform();
		xlong collision();
	public:
		CLenemy(CLfile* enemya,xlong* m,xlong mm,CLlvector* enemyp=0);
		CLenemy(CLenemy* enemyptr,CLlvector* enemyp);
		~CLenemy();
		template<int I>xlong update(CLentity<I>* p);
		void displayhud() const;
};

CLgame&   CLenemy::clgame   = CLgame::instance();
CLstring& CLenemy::clstring = CLstring::instance();
///*

///implementation
void CLenemy::cruise() //! critical
{
	//update enemy through ai array
	switch(aitype)
	{
		case 0: //straight
			gear=1;
			setspeed(); 
		break;
		
		case 1: //vary x along aiarray
		
		break;
		
		case 2: //aiarray to polygon
		
		break;
		
		case 3: //1D random walk
		
		break;
		
		case 4: //oscillate
			if(speed.x==0) { speed.x = -maxspeed/20; }
			if(position.x<=200 && speed.x==maxspeed/20) { speed.x = -maxspeed/20; }
			if(position.x>=600 && speed.x==-maxspeed/20) { speed.x = maxspeed/20; }
		break;
		
		case 5: //oscillate following
			if(speed.x==0) { speed.x = -maxspeed/20; }
			if(position.x<=200 && speed.x==maxspeed/20) { speed.x = -maxspeed/20; }
			if(position.x>=600 && speed.x==-maxspeed/20) { speed.x = maxspeed/20; }
		break;
	}		
	//*
}

void CLenemy::pretransform() //! critical
{
	//transform enemy bounding box
	boundingbox[1][0]->c[0] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[0]);
	boundingbox[1][0]->c[1] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[1]);
	boundingbox[1][0]->c[2] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[2]);
	boundingbox[1][0]->c[3] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[3]);
	boundingbox[1][0]->c[4] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[4]);
	boundingbox[1][0]->c[5] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[5]);
	boundingbox[1][0]->c[6] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[6]);
	boundingbox[1][0]->c[7] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[7]);
	//*
}

void CLenemy::transform() //! critical
{
	//transform enemy
	model[0]->update();
	direction[0] = model[0]->getmatrix()->transform(direction[0]);
	speeddir = model[0]->getmatrix()->transform(speeddir);
	//*
}

xlong CLenemy::collision() //! critical
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = clgame.boundary(tposition,*boundingbox[1][0],1);
	tposition.y += *mark;
	//terrain collision does not apply
	if(bc!=0) { visible = 1; }
	//*
	
	return r;
}

CLenemy::CLenemy(CLfile* enemya,xlong* m,xlong mm,CLlvector* enemyp) : CLentity<1>(enemya,m,mm) //! noncritical
{
	//set entity type
	type = 1;
	//*
	
	//set and adjust (start) position to floating X pixel above ground
	if(enemyp!=0)
	{
		position.x = enemyp->x;
		position.y = enemyp->y;
		position.z = ENEMYZLEVEL;
		tposition = position;
	}
	//*
	
	//load enemy ai
	aitype = csv[1];
	aggrolevel = csv[2];
	aiarray = &csv[3];
	//*
	
	//create agrobox
	aggrobox = new CLbox;
	aggrobox->c[0] = aggrobox->c[4] = CLfvector(boundingbox[0][0]->c[0].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[1] = aggrobox->c[5] = CLfvector(boundingbox[0][0]->c[3].x,boundingbox[0][0]->c[0].y,0);
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox[0][0]->c[3].x,YRES>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox[0][0]->c[0].x,YRES>>1,0);
	//*
	
	//set enemy specific attributes
	points = clstring.tolong((*def)[u8"points"]);
	speeddir.y  = maxspeed/20;
	direction[0].y = -1;
	//*
	
	//create progress bar
	hprog = new CLprogress(0,0,40,10,health+shield,0,health+shield,0x00FF0000,1,1,0,0,0);
	//*
}

CLenemy::CLenemy(CLenemy* enemyptr,CLlvector* enemyp) : CLentity<1>(enemyptr) //! noncritical
{
	//set and adjust (start) position to floating X pixel above ground
	position.x = enemyp->x;
	position.y = enemyp->y;
	position.z = ENEMYZLEVEL;
	tposition = position;
	//*
	
	//set entity type
	type = enemyptr->type;
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
	aggrobox->c[2] = aggrobox->c[6] = CLfvector(boundingbox[0][0]->c[3].x,YRES>>1,0);
	aggrobox->c[3] = aggrobox->c[7] = CLfvector(boundingbox[0][0]->c[0].x,YRES>>1,0);
	//*
	
	//set enemy specific attributes
	points = enemyptr->points;
	speeddir.y  = enemyptr->speeddir.y;
	direction[0].y = -1;
	//*	
	
	//create progress bar
	hprog = new CLprogress(*(enemyptr->hprog));
	hprog->reset(0,0,40,10,health+shield,0,health+shield,0x0000FF00,0,1,0,0x00C0C0C0,0x00C0C0C0);
	//*
}

CLenemy::~CLenemy() //! noncritical
{
	delete aiarray;
	delete aggrobox;
	delete hprog;
}

template<int I>
xlong CLenemy::update(CLentity<I>* p) //! critical
{
	//update ammo
	ammoman->update();
	ammoman->collision(p);
	//*
	
	//check if to activate
	if(active==0 && ( (*mark)-100)<position.y) { active = 1; }
	//*

	//check if screen is left behind player
	else if(active==1 && position.y>markmax ) { return (active = visible = 0); }
	//*

	//check if destroyed
	if(health<=0 && active!=-1)
	{
		//deactivate and start explosion
		active = -1;
		expl[0]->first();
		//*
	}
	//*
	
	//set progressbars
	hprog->setprogress(health+shield);
	//*
	
	if(active==1)
	{
		xlong time = clwindow.getmilliseconds();

		model[0]->getmatrix()->unit();
		
		cruise();
		
		//fire at player?
		xlong fc = clgame.collision(position,*aggrobox,*(p->getposition()),*(p->getboundingbox()),1);
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
		else { *boundingbox[1] = *boundingbox[0]; }
		//*
	}
	else if(active==-1) { if(expl[0]->next()==1) return points; }
	else { lastupdate = clwindow.getmilliseconds(); }
	
	return -1;
}

void CLenemy::displayhud() const //! critical
{
	if(active==1)
	{
		//draw enemy's healthbar
		xlong tempx = (boundingbox[0][0]->c[0].x + boundingbox[0][0]->c[1].x) / 2;
		hprog->setx(tempx + sposition.x - (hprog->getwidth()>>1));
		hprog->sety(position.y - *mark - 25);
		hprog->draw();
		//*
	}
}
///*

#endif


