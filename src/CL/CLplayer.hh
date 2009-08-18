//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
//#pragma message "Compiling " __FILE__ " ! TODO: interaction"

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLvector.hh"
#include "CLgame.hh"
#include "CLammo.hh"
#include "CLentity.hh"
#include "CLenemy.hh"


class CLplayer : public CLentity<2>
{
	private:
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLfbuffer* ll);
	
	public:
		CLplayer(CLfile* playera,xlong* m,CLlvector& playerp,xlong pts=0);
		~CLplayer();

		xlong update(xchar input,xchar turbo,CLfbuffer* ll,CLlist* enemies);
		void addpoints(xlong p);
		void showbox();
};

void CLplayer::pretransform(bool m)
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

void CLplayer::transform(bool m)
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

xlong CLplayer::collision(CLfbuffer* ll)
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= *mark;
	xlong bc = CLgame::boundary(tposition,*boundingbox[1][0]);
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
		if(CLmath::absolute(xangle)>=1) linear->rotate(xangle,yangle,0);
		//~ if(CLmath::absolute(xangle)>=1) CLsystem::print(xangle);
		//~ if(CLmath::absolute(yangle)>=1) CLsystem::print(yangle);
	//*

	return r;
}

CLplayer::CLplayer(CLfile* playera,xlong* m,CLlvector& playerp,xlong pts) : CLentity<2>(playera,m)
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
	speeddir.y /= 20;
	direction[0].y = direction[1].y = 1;
	active = 1;
	visible = 1;
	//*
}

CLplayer::~CLplayer()
{
	delete def;
}

xlong CLplayer::update(xchar input,xchar turbo,CLfbuffer* ll,CLlist* enemies)
{
	//update ammo
	CLenemy* currenemy = 0;
	ammoman->update();
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		currenemy = static_cast<CLenemy*>(enemies->getcurrentdata());
		ammoman->collision(currenemy);
	}
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
		//init variables
		xlong time = CLsystem::getmilliseconds();
		linear->unit();
		bool what = 0;
		xlong tempangle = 0;
		//*

		//temp use gamepad	
		if(CLgamepad::axis[1]<0) input = 82;
		if(CLgamepad::axis[1]>0) input = 84;
		if(CLgamepad::axis[0]<0) turbo = 81;
		if(CLgamepad::axis[0]>0) turbo = 83;
		if(CLgamepad::button[0]!=0) turbo = 97;
		if(CLgamepad::button[1]!=0) turbo = 100;
		if(CLgamepad::button[2]!=0) turbo = 'w';
		if(CLgamepad::button[3]!=0) turbo = 32;
		if(CLgamepad::button[4]!=0) turbo = -29;
		if(CLgamepad::button[5]!=0) turbo = 'q';
		if(CLgamepad::button[6]!=0) turbo = 's';
		if(CLgamepad::button[7]!=0) turbo = 'e';
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
				linear->rotate(0,0,5);
				pretransform(0);	
				setspeed();
			break;
			//*

			//arrow right -> turn right
			case 83: 
				tempangle = -5;
				linear->rotate(0,0,-5);
				pretransform(0);
				setspeed();
			break;
			//*

			//a -> turn tower left
			case 97: 
				tempangle = 5;
				linear->rotate(0,0,5);
				pretransform(1);
				what=1;
			break;
			//*

			//d -> turn tower right
			case 100: 
				tempangle = -5;
				linear->rotate(0,0,-5);
				pretransform(1);
				what=1;
			break;
			//*
			
			//reset tower
			case 'w':
			if(angles[1].z>180) angles[1].z-=360;
			if(angles[1].z<-180) angles[1].z+=360;
			tempangle = CLmath::sign(angles[0].z-angles[1].z) * 5;
			linear->rotate(0,0,tempangle);
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

		//update position
		float inter = time-lastupdate;
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
		else
		{
			*(boundingbox[1][0]) = *(boundingbox[0][0]);
		}
		//*
	}
	else if(active==-1)
	{
		if(expl[0]->next()==1) return points;
	}
	
	return -1;
}

void CLplayer::addpoints(xlong p)
{
	points += p;
}

//for debug only:
void CLplayer::showbox()
{
	CLfvector bposition;
	bposition.x = position.x;
	bposition.y = position.y - *mark;
	bposition.z = position.z;
	
	CLfvector a = bposition;
	a.x += boundingbox[1][0]->c[0].x;
	a.y -= boundingbox[1][0]->c[0].y;
	a.z -= boundingbox[1][0]->c[0].z;
	CLfvector b = bposition;
	b.x += boundingbox[1][0]->c[1].x;
	b.y -= boundingbox[1][0]->c[1].y;
	b.z -= boundingbox[1][0]->c[1].z;
	CLfvector c = bposition;
	c.x += boundingbox[1][0]->c[2].x;
	c.y -= boundingbox[1][0]->c[2].y;
	c.z -= boundingbox[1][0]->c[2].z;
	CLfvector d = bposition;
	d.x += boundingbox[1][0]->c[3].x;
	d.y -= boundingbox[1][0]->c[3].y;
	d.z -= boundingbox[1][0]->c[3].z;
	
	CLgfx1::drawpolygon( a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,0x00FFFFFF );
	CLgfx1::drawbigpixel(d.x,d.y,0);
	CLgfx1::drawbigpixel( (a.x+b.x+c.x+d.x)/4,(a.y+b.y+c.y+d.y)/4,0);
}
//*

#endif

