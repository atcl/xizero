///license
//atCROSSLEVEL studios 2009
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
	protected:
		CLprogress* hprog;
		CLprogress* sprog;
	
	private:
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLfbuffer* ll);
	
	public:
		CLplayer(CLfile* playera,xlong* m,xlong mm,CLlvector* playerp,xlong pts=0);
		~CLplayer();
		xlong update(xchar input,xchar turbo,CLfbuffer* ll,CLenemylist* enemies,CLboss* boss,CLgamepadstate* p);
		void addpoints(xlong p);
		void showbox();
		void displayhud();
};
///*

///implementation
void CLplayer::pretransform(bool m) //! critical
{
	//transform tower bounding box
	boundingbox[1][1]->c[0] = linear->transform(boundingbox[1][1]->c[0]);
	boundingbox[1][1]->c[1] = linear->transform(boundingbox[1][1]->c[1]);
	boundingbox[1][1]->c[2] = linear->transform(boundingbox[1][1]->c[2]);
	boundingbox[1][1]->c[3] = linear->transform(boundingbox[1][1]->c[3]);
	boundingbox[1][1]->c[4] = linear->transform(boundingbox[1][1]->c[4]);
	boundingbox[1][1]->c[5] = linear->transform(boundingbox[1][1]->c[5]);
	boundingbox[1][1]->c[6] = linear->transform(boundingbox[1][1]->c[6]);
	boundingbox[1][1]->c[7] = linear->transform(boundingbox[1][1]->c[7]);
	//*
	
	//transform chassis bounding box if wanted
	if(m==0)
	{
		boundingbox[1][0]->c[0] = linear->transform(boundingbox[1][0]->c[0]);
		boundingbox[1][0]->c[1] = linear->transform(boundingbox[1][0]->c[1]);
		boundingbox[1][0]->c[2] = linear->transform(boundingbox[1][0]->c[2]);
		boundingbox[1][0]->c[3] = linear->transform(boundingbox[1][0]->c[3]);
		boundingbox[1][0]->c[4] = linear->transform(boundingbox[1][0]->c[4]);
		boundingbox[1][0]->c[5] = linear->transform(boundingbox[1][0]->c[5]);
		boundingbox[1][0]->c[6] = linear->transform(boundingbox[1][0]->c[6]);
		boundingbox[1][0]->c[7] = linear->transform(boundingbox[1][0]->c[7]);
	}
	//*
}

void CLplayer::transform(bool m) //! noncritical
{
	//transform tower
	model[1]->update(linear);
	direction[1] = linear->transform(direction[1]);
	//*
	
	//transform chassis if wanted
	if(m==0)
	{
		model[0]->update(linear);
		direction[0] = linear->transform(direction[0]); 
		speeddir = linear->transform(speeddir);
	}
	//*
}

xlong CLplayer::collision(CLfbuffer* ll) //! critical
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = clgame->boundary(tposition,*boundingbox[1][0]);
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
	xlong tc = clgame->terrain(ll,boundingbox[1][0],boundingbox[0][0],tposition,position,xangle,yangle,zdiff); 
 
	if(tc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*
	
	//adjust z position
	if(clmath->absolute(zdiff)>1) { tposition.z = zdiff-12; }
	//*	
	
	//adjust rotation around x andy axis
		//rotate x about xangle,y about yangle
		if(clmath->absolute(xangle)>=1) { linear->rotate(xangle,yangle,0); }
		//~ if(CLmath::absolute(xangle)>=1) CLsystem::print(xangle);
		//~ if(CLmath::absolute(yangle)>=1) CLsystem::print(yangle);
	//*

	return r;
}

CLplayer::CLplayer(CLfile* playera,xlong* m,xlong mm,CLlvector* playerp,xlong pts) : CLentity<2>(playera,m,mm) //! noncritical
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
	delete def;
	delete hprog;
	delete sprog;
}

xlong CLplayer::update(xchar input,xchar turbo,CLfbuffer* ll,CLenemylist* enemies,CLboss* boss,CLgamepadstate* p) //! critical
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
		expl[0]->first(0);
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
		xlong time = clsystem->getmilliseconds();
		linear->unit();
		bool what = 0;
		xlong tempangle = 0;
		//*

		//shield regeneration
		if( (time >= (shieldupdate + shieldrate) ) && (shield < shieldmax) )
		{
			shield++;
			shieldupdate = time;
			if(shield>shieldmax) shield = shieldmax;
		}
		//*

		//temp use gamepad
		if(p!=0)
		{
			if(p->axis[1]<0) input = UP;
			if(p->axis[1]>0) input = DOWN;
			if(p->axis[0]<0) turbo = LEFT;
			if(p->axis[0]>0) turbo = RIGHT;
			if(p->button[0]!=0) turbo = LCTRL;
			if(p->button[1]!=0) turbo = SPACE;
			if(p->button[2]!=0) turbo = 'q';
			if(p->button[3]!=0) turbo = 'w';
			if(p->button[4]!=0) turbo = 'e';
			if(p->button[5]!=0) turbo = 's';
			if(p->button[6]!=0) turbo = 'a';
			if(p->button[7]!=0) turbo = 'd';
			//if(p->button[8]!=0) turbo = ?;
			//if(p->button[9]!=0) turbo = ?;
		}
		//*

		switch(turbo)
		{
			//stop backward driving and drive forward
			case UP:
				if(gear==-1) { gear=0; setspeed(); }
				else { gear=1; setspeed(); }
			break;
			//*

			//stop forward driving and drive backward
			case DOWN:
				if(gear==1) { gear=0; setspeed(); }
				else { gear=-1; setspeed(); }
			break;
			//*
		}
		
		switch(input)
		{
			//arrow left -> turn left
			case LEFT: 
				tempangle = 3;
				linear->rotate(0,0,3);
				pretransform(0);	
				setspeed();
			break;
			//*

			//arrow right -> turn right
			case RIGHT: 
				tempangle = -3;
				linear->rotate(0,0,-3);
				pretransform(0);
				setspeed();
			break;
			//*

			//a -> turn tower left
			case 'a': 
				tempangle = 5;
				linear->rotate(0,0,5);
				pretransform(1);
				what=1;
			break;
			//*

			//d -> turn tower right
			case 'd': 
				tempangle = -5;
				linear->rotate(0,0,-5);
				pretransform(1);
				what=1;
			break;
			//*
			
			//w -> reset tower
			case 'w':
			if( (angles[1].z - angles[0].z) >  180) { angles[1].z -= 360; } 
			if( (angles[1].z - angles[0].z) < -180) { angles[1].z += 360; }
			tempangle = clmath->sign(angles[0].z-angles[1].z) * 5;
			linear->rotate(0,0,tempangle);
			pretransform(1);
			what=1;
			break;
			//*

			//space -> fire tower weapon
			case SPACE: 
				if(time >= fireupdate[1] + firerate[1])
				{
					fire(1,4,0,ENEMYZLEVEL,1);
					fireupdate[1] = time;
				}
			break;
			//*

			//strg -> fire chassis weapon(s)
			case LCTRL: 
				if(time >= fireupdate[0] + firerate[0])
				{
					fire(0,3,0,ENEMYZLEVEL,0);
					fire(0,3,1,ENEMYZLEVEL,0);
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
	
	CLgfx1::instance()->drawpolygon( a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,0x00FFFFFF );
	CLgfx1::instance()->drawpixel(d.x,d.y,0,1);
	CLgfx1::instance()->drawpixel( (a.x+b.x+c.x+d.x)/4,(a.y+b.y+c.y+d.y)/4,0,1);
}
//*

void CLplayer::displayhud() //! noncritical
{
	hprog->draw();
	sprog->draw();
}
///*

#endif

