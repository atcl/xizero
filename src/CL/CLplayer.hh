///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
///*

///includes
#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLvector.hh"
#include "CLgame.hh"
#include "CLammo.hh"
#include "CLentity.hh"
#include "CLenemy.hh"
#include "CLboss.hh"
#include "CLwindow.hh"
///*

///header
/* class name:	CLplayer
 * 
 * description:	The player (entity) class.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///declarations
typedef CLlist<CLenemy> CLenemylist;
///*

///definitions
class CLplayer : public CLentity<2>
{
	private:
		CLgame&   clgame;
		CLmath&   clmath;
		CLwindow& clwindow;
	protected:
		CLprogress* hprog;
		CLprogress* sprog;
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLobject** ll);	
	public:
		CLplayer(CLfile* playera,xlong* m,xlong mm,CLlvector* playerp,xlong pts=0);
		~CLplayer();
		xlong update(CLobject** ll,CLenemylist* enemies,CLboss* boss);
		void addpoints(xlong p);
		void showbox();
		void displayhud();
};
///*

///implementation
void CLplayer::pretransform(bool m) //! critical
{
	//transform tower bounding box
	boundingbox[1][1]->c[0] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[0]);
	boundingbox[1][1]->c[1] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[1]);
	boundingbox[1][1]->c[2] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[2]);
	boundingbox[1][1]->c[3] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[3]);
	boundingbox[1][1]->c[4] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[4]);
	boundingbox[1][1]->c[5] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[5]);
	boundingbox[1][1]->c[6] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[6]);
	boundingbox[1][1]->c[7] = model[1]->getmatrix()->transform(boundingbox[1][1]->c[7]);
	//*
	
	//transform chassis bounding box if requested
	if(m==0)
	{
		boundingbox[1][0]->c[0] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[0]);
		boundingbox[1][0]->c[1] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[1]);
		boundingbox[1][0]->c[2] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[2]);
		boundingbox[1][0]->c[3] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[3]);
		boundingbox[1][0]->c[4] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[4]);
		boundingbox[1][0]->c[5] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[5]);
		boundingbox[1][0]->c[6] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[6]);
		boundingbox[1][0]->c[7] = model[0]->getmatrix()->transform(boundingbox[1][0]->c[7]);
	}
	//*
}

void CLplayer::transform(bool m) //! noncritical
{
	//transform tower
	model[1]->update();
	direction[1] = model[1]->getmatrix()->transform(direction[1]);
	//*
	
	//transform chassis if wanted
	if(m==0)
	{
		model[0]->update();
		direction[0] = model[0]->getmatrix()->transform(direction[0]); 
		speeddir = model[0]->getmatrix()->transform(speeddir);
	}
	//*
}

xlong CLplayer::collision(CLobject** ll) //! critical
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = clgame.boundary(tposition,*boundingbox[1][0]);
	tposition.y += *mark;

	if(bc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*

	//terrain collision test
	float zdiff  = 0;
	float xangle = 0;
	float yangle = 0;
	xlong tc = 0; //clgame->terrain(ll,boundingbox[1][0],boundingbox[0][0],tposition,position,xangle,yangle,zdiff); 
 
	if(tc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*
	
	//adjust z position
	if(clmath.absolute(zdiff)>1) { tposition.z = zdiff-12; }
	//*	
	
	//adjust rotation around x and y axis
		//rotate x about xangle,y about yangle
		//~ if(clmath->absolute(xangle)>=1) { linear->rotate(xangle,yangle,0); }
	//*

	return r;
}

CLplayer::CLplayer(CLfile* playera,xlong* m,xlong mm,CLlvector* playerp,xlong pts) //! noncritical
: clgame(CLgame::instance()), clmath(CLmath::instance()), clwindow(CLwindow::instance()), CLentity<2>(playera,m,mm) 
{
	//set entity type
	type = 0;
	//*	
	
	//set and adjust (start) position to floor
	position.x = playerp->x;
	position.y = playerp->y;
	position.z = 100 - playerp->z - 12;
	tposition = position;
	//*
	
	//set players points from levels before
	points = pts;
	//*
	
	//set player specific attributes
	speeddir.y  = -maxspeed/20;
	direction[0].y = direction[1].y = 1;
	active = 1;
	visible = 1;
	//*
	
	//create progress bars
	hprog = new CLprogress(5,5,20,YRES-10,health,0,health,0x000000FF,1,1,0,0x00C0C0C0,0x00C0C0C0);
	sprog = new CLprogress(XRES-25,5,20,YRES-10,shield,0,shield,0x0000FF00,1,1,0,0x00C0C0C0,0x00C0C0C0);
	//*
}

CLplayer::~CLplayer() //! noncritical
{
	//~ delete def;
	//~ delete hprog;
	//~ delete sprog;
}

xlong CLplayer::update(CLobject** ll,CLenemylist* enemies,CLboss* boss) //! critical
{
	//update ammo
	CLenemy* currenemy = 0;
	ammoman->update();
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		currenemy = enemies->getcurrentdata();
		ammoman->collision(currenemy);
	}
	ammoman->collision(boss);
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

	if(active==1)
	{		
		//set progressbars
		hprog->setprogress(health);
		sprog->setprogress(shield);
		//*
		
		//init variables
		xlong time = clwindow.getmilliseconds();
		model[0]->getmatrix()->unit();
		model[1]->getmatrix()->unit();
		bool what = 0;
		xlong tempangle = 0;
		//*

		//shield regeneration
		if( (time >= (shieldupdate + shieldrate) ) && (shield < shieldmax) )
		{
			shield++;
			shieldupdate = time;
			if(shield>shieldmax) { shield = shieldmax; }
		}
		//*

		switch(clwindow.getinkey())
		{
			case UP: //stop backward driving and drive forward
				//~ if(gear==-1) { gear=0; setspeed(); }
				//~ else { gear=1; setspeed(); }
				gear++; setspeed();
			break; //*

			case DOWN: //stop forward driving and drive backward
				//~ if(gear==1) { gear=0; setspeed(); }
				//~ else { gear=-1; setspeed(); }
				gear--; setspeed();
			break; //*
		}

		switch(clwindow.getturbo())
		{
			case LEFT: //arrow left -> turn left
				tempangle = 3;
				model[0]->getmatrix()->rotate(0,0,3);
				model[1]->getmatrix()->rotate(0,0,3);
				pretransform(0);	
				setspeed();
			break; //*

			case RIGHT: //arrow right -> turn right
				tempangle = -3;
				model[0]->getmatrix()->rotate(0,0,-3);
				model[1]->getmatrix()->rotate(0,0,-3);
				pretransform(0);
				setspeed();
			break; //*

			case 'a': //a -> turn tower left
				tempangle = 5;
				model[1]->getmatrix()->rotate(0,0,5);
				pretransform(1);
				what=1;
			break; //*

			case 'd': //d -> turn tower right
				tempangle = -5;
				model[1]->getmatrix()->rotate(0,0,-5);
				pretransform(1);
				what=1;
			break; //*
			
			case 'w': //w -> reset tower
			if( (angles[1].z - angles[0].z) >  180) { angles[1].z -= 360; } 
			if( (angles[1].z - angles[0].z) < -180) { angles[1].z += 360; }
			tempangle = clmath.sign(angles[0].z-angles[1].z) * 5;
			model[1]->getmatrix()->rotate(0,0,tempangle);
			pretransform(1);
			what=1;
			break; //*

			
			case SPACE: //space -> fire tower weapon
				if(time >= fireupdate[1] + firerate[1])
				{
					fire(1,4,0,ENEMYZLEVEL,1);
					fireupdate[1] = time;
				}
			break; //*

			case '0': //0 -> fire chassis weapon(s)
				if(time >= fireupdate[0] + firerate[0])
				{
					fire(0,3,0,ENEMYZLEVEL,0);
					fire(0,3,1,ENEMYZLEVEL,0);
					fireupdate[0] = time;
				}
			break; //*

			case 'q': //fire (tachyon) laser
			break; //*

			case 's': //fire laser
			break; //*

			case 'e': //action (use) key
			break; //*
		}

		//update position
		float inter = (time-lastupdate);
		tposition.x = position.x - (inter*speed.x);
		tposition.y = position.y + (inter*speed.y);
		tposition.z = position.z - (inter*speed.z);
		lastupdate = time;	
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
		else { *(boundingbox[1][0]) = *(boundingbox[0][0]); }
		//*
	}
	else if(active==-1) { if(expl[0]->next()==1) { return points; } }
	
	return -1;
}

void CLplayer::addpoints(xlong p) {	points += p; }  //! noncritical

//for debug only:
void CLplayer::showbox() //! noncritical
{
	CLfvector bposition;
	bposition.x = position.x;
	bposition.y = position.y - *mark;
	bposition.z = position.z;
	
	CLfvector a = bposition;
	CLfvector b = bposition;
	CLfvector c = bposition;
	CLfvector d = bposition;
	a.x += boundingbox[1][0]->c[0].x;
	a.y -= boundingbox[1][0]->c[0].y;
	a.z += boundingbox[1][0]->c[0].z;
	b.x += boundingbox[1][0]->c[1].x;
	b.y -= boundingbox[1][0]->c[1].y;
	b.z += boundingbox[1][0]->c[1].z;
	c.x += boundingbox[1][0]->c[2].x;
	c.y -= boundingbox[1][0]->c[2].y;
	c.z += boundingbox[1][0]->c[2].z;
	d.x += boundingbox[1][0]->c[3].x;
	d.y -= boundingbox[1][0]->c[3].y;
	d.z += boundingbox[1][0]->c[3].z;
	
	CLgfx::instance().drawpolygon( a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,0x00FFFFFF );
	CLgfx::instance().drawpixel(d.x,d.y,0,1);
	CLgfx::instance().drawpixel( (a.x+b.x+c.x+d.x)/4,(a.y+b.y+c.y+d.y)/4,0,1);
}
//*

void CLplayer::displayhud() //! noncritical
{
	hprog->draw();
	sprog->draw();
}
///*

#endif

