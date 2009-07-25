//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
#pragma message "Compiling " __FILE__ " ! TODO: interaction, new bcx,ev else,brking (speed=0),correct turning (speed)"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLcl.hh"
#include "CLapi.hh"
#include "CLgame.hh"
#include "CLsprites.hh"


struct CLammo
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
	CLfvector d;
	float v;
};

class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
		CLmatrix* cllinear;
		CLlist*   ammolist;

	private:
		CLbox* boundingbox[2];
		CLbox* oboundingbox[2];

		CLammo* ammotype[4];
		CLammo* currammo;
		xlong firerate[4];
		
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;

		CLfvector position;
		CLfvector tposition;
		CLlvector sposition;
		CLfvector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		CLfvector speed;
		CLfvector speeddir;
		CLfvector tilt; //meaning mainly z-tilt (ie on ramps)

		xlong gear;
		xlong active;
		xlong points;
		xlong firing;
		xlong lastupdate;

		void setspeed();
		void fire(xlong at);
		void hurt(xlong am);
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLfbuffer* ll,xlong m);
		
	public:
		CLplayer(CLobject* cha,CLobject* tow,xlong** dat,CLlvector s,xlong p=0);
		~CLplayer();

		xlong update(xchar input,xchar turbo,CLfbuffer* ll,xlong mark);
		void display(xlong m);
		void setxyz(xlong x,xlong y,xlong z);
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
};


void CLplayer::setspeed()
{
	switch(gear)
	{
		case 0:
			speed = 0;
			break;

		case 1:
			speed = speeddir;
			break;

		case -1:
			speed = -speeddir;
			break;
	}
}

void CLplayer::fire(xlong at)
{

}

void CLplayer::hurt(xlong am)
{

}

void CLplayer::pretransform(bool m)
{
		boundingbox[1]->b1 = cllinear->transform(boundingbox[1]->b1);
		boundingbox[1]->b2 = cllinear->transform(boundingbox[1]->b2);
		boundingbox[1]->b3 = cllinear->transform(boundingbox[1]->b3);
		boundingbox[1]->b4 = cllinear->transform(boundingbox[1]->b4);
		boundingbox[1]->t1 = cllinear->transform(boundingbox[1]->t1);
		boundingbox[1]->t2 = cllinear->transform(boundingbox[1]->t2);
		boundingbox[1]->t3 = cllinear->transform(boundingbox[1]->t3);
		boundingbox[1]->t4 = cllinear->transform(boundingbox[1]->t4);
	
	if(m==0)
	{
		boundingbox[0]->b1 = cllinear->transform(boundingbox[0]->b1);
		boundingbox[0]->b2 = cllinear->transform(boundingbox[0]->b2);
		boundingbox[0]->b3 = cllinear->transform(boundingbox[0]->b3);
		boundingbox[0]->b4 = cllinear->transform(boundingbox[0]->b4);
		boundingbox[0]->t1 = cllinear->transform(boundingbox[0]->t1);
		boundingbox[0]->t2 = cllinear->transform(boundingbox[0]->t2);
		boundingbox[0]->t3 = cllinear->transform(boundingbox[0]->t3);
		boundingbox[0]->t4 = cllinear->transform(boundingbox[0]->t4);
	}
}

void CLplayer::transform(bool m)
{
	//bool decides what part, if complete or only tower

	if(m==0)
	{
		//transform model(s)
		model[0]->update(cllinear);
		model[1]->update(cllinear);
	
		//transform direction vector
		direction[0] = cllinear->transform(direction[0]);
		direction[1] = cllinear->transform(direction[1]);

		//transform (constant) speeddir vector
		speeddir = cllinear->transform(speeddir);

		//transform tilt vector
		//tilt = cllinear->transform(tilt);
	}
	else
	{
		//transform model(s)
		model[1]->update(cllinear);

		//transform direction vector
		direction[1] = cllinear->transform(direction[1]);
	}
}

xlong CLplayer::collision(CLfbuffer* ll,xlong m)
{
	xlong r = 0;

	tposition.y -= m;
	xlong bc = CLgame::boundary(tposition,*boundingbox[0]); //boundary check: (check if game screen is left)
	tposition.y += m;

	if(bc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}

	xlong xangle = 0;
	xlong yangle = 0;
	xlong zdiff  = 0;

	xlong tc = CLgame::terrain(ll,boundingbox[0],oboundingbox[0],tposition,position,xangle,yangle,zdiff); //terrain collision check: (check if player collides with terrain block)
 
	if(tc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	
	//tposition.z += zdiff; //only growth when uphill, constant on downhill, funny :)
	if(zdiff!=0) { CLsystem::print("z level change: ",0); CLsystem::print(zdiff); }
	
	//rotate x about xangle,y about yangle
	//cllinear->rotate(xangle,0,0);
	//if(xangle!=0) CLsystem::print(xangle);
	//if(yangle!=0) CLsystem::print(yangle);


	//enemy collision check: (check if player collides with enemy entity)

	return r;
}

CLplayer::CLplayer(CLobject* cha,CLobject* tow,xlong** dat,CLlvector s,xlong p)
{
	model[0] = cha;
	model[1] = tow;
	
	boundingbox[0] = new CLbox;
	oboundingbox[0] = model[0]->getboundingbox();
	boundingbox[0]->b1 = oboundingbox[0]->b1;
	boundingbox[0]->b2 = oboundingbox[0]->b2;
	boundingbox[0]->b3 = oboundingbox[0]->b3;
	boundingbox[0]->b4 = oboundingbox[0]->b4;
	boundingbox[0]->t1 = oboundingbox[0]->t1;
	boundingbox[0]->t2 = oboundingbox[0]->t2;
	boundingbox[0]->t3 = oboundingbox[0]->t3;
	boundingbox[0]->t4 = oboundingbox[0]->t4;

	boundingbox[1] = model[1]->getboundingbox();

	cllinear = new CLmatrix(1);

	position = s;
	position.z += 95;
	tposition = position;

	points = p;

	//create attribute objects:
	ammolist = new CLlist();

	//construct bounding box from model:
	//CLbox = model->getboundingbox();

	//load player attribtes from data-file provided in parameters (dat)

	//if( dat[1][0] != 'PLYR' ) CLexit_(1,__func__,"wrong bcx type");
	//if( dat[1][1] != model[0]->getname() ) CLexit_(__func__,1,"bcx and y3d are not fitting");
	health		= dat[1][2];
	shield		= dat[1][3];
	shieldrate	= dat[1][4];
	armor		= dat[1][5];

	speeddir.x  = 0;
	speeddir.y  = -6;
	speeddir.z  = 0;

	//~ ammotype[0]	= dat[1][12];
	//~ ammotype[1]	= dat[1][15];
	//~ ammotype[2]	= dat[1][18];
	//~ ammotype[3]	= dat[1][21];
	
	ammotype[0] = new CLammo;
	ammotype[0]->comsprite = CLsprites::drawplasma;
	ammotype[0]->v = 4;
	ammotype[0]->p = CLfvector();
	ammotype[0]->d = CLfvector();
	
	ammotype[1] = new CLammo;
	ammotype[1]->comsprite = CLsprites::drawplasma;
	ammotype[1]->v = 4;
	ammotype[1]->p = CLfvector();
	ammotype[1]->d = CLfvector();
	
	firerate[0]	= dat[1][13];
	firerate[1]	= dat[1][16];
	firerate[2]	= dat[1][19];
	firerate[3]	= dat[1][22];



	//set other attributes to init:
	speed = 0;

	direction[0].x = 0;
	direction[0].y = 1;
	direction[0].z = 0;
	direction[1].x = 0;
	direction[1].y = 1;
	direction[1].z = 0;

	gear = 0;
	active = true;
	lastupdate = CLsystem::getmilliseconds();
	firing=-1;
}

CLplayer::~CLplayer()
{
	delete cllinear;
	delete ammolist;
}

xlong CLplayer::update(xchar input,xchar turbo,CLfbuffer* ll,xlong mark)
{
	//ammo update
	for(int i=0; i<ammolist->getlength();i++)
	{
		ammolist->setindex(i);
		currammo = reinterpret_cast<CLammo*>(ammolist->getcurrentdata());
		//add time dependency
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
	//*
	
	switch(input)
	{
		case 82:
			if(gear==-1) { gear=0; setspeed(); }
			else { gear=1; setspeed(); }
		break;

		case 84:
			if(gear==1) { gear=0; setspeed(); }
			else { gear=-1; setspeed(); }
		break;	
	}

	cllinear->unit();
	bool what = 0;
	
	CLfvector* ta;
		
	switch(turbo)
	{
		case 81: //arrow left -> turn left
			cllinear->rotate(0,0,5);
			pretransform(0);	
			setspeed();
		break;

		case 83: //arrow right -> turn right
			cllinear->rotate(0,0,-5);
			pretransform(0);
			setspeed();
		break;

		case 97: //a -> turn tower left
			cllinear->rotate(0,0,5);
			pretransform(1);
			what=1;
		break;

		case 100: //d -> turn tower right
			cllinear->rotate(0,0,-5);
			pretransform(1);
			what=1;
		break;

		case 32: //space -> fire tower weapon
			//add time dependency
			currammo = new CLammo();
			currammo->comsprite = ammotype[1]->comsprite;
			currammo->v = ammotype[1]->v;
			ta = model[1]->getdockingpoint(4,0);
			currammo->p.x = position.x + ta->x;
			currammo->p.y = position.y - ta->y;
			currammo->p.z = position.z + ta->z;
			currammo->d = direction[1];
			ammolist->append(currammo,"at1");
		break;

		case -29: //strg -> fire chassis weapon(s)
			//add time dependency
			currammo = new CLammo();
			currammo->comsprite = ammotype[0]->comsprite;
			currammo->v = ammotype[0]->v;
			ta = model[0]->getdockingpoint(3,0);
			currammo->p.x = position.x + ta->x;
			currammo->p.y = position.y - ta->y;
			currammo->p.z = position.z + ta->z;
			currammo->d = direction[0];
			ammolist->append(currammo,"at0");
			currammo = new CLammo();
			currammo->comsprite = ammotype[0]->comsprite;
			currammo->v = ammotype[0]->v;
			ta = model[0]->getdockingpoint(3,1);
			currammo->p.x = position.x + ta->x;
			currammo->p.y = position.y - ta->y;
			currammo->p.z = position.z + ta->z;
			currammo->d = direction[0];
			ammolist->append(currammo,"at0");
		break;

		case 119: //w -> fire (tachyon) laser
		break;

		case 115: //s -> fire laser
		break;

		case 101: //e -> action key
		break;
	}
	
	xmark = mark;

	xlong temp = CLsystem::getmilliseconds();
	if(temp >= lastupdate + 20)
	{
		tposition.x = position.x - speed.x;
		tposition.y = position.y + speed.y;
		tposition.z = position.z + speed.z;
		
		lastupdate = temp;	
	}

	if(collision(ll,mark)==0)
	{	
		transform(what);
		
		position = tposition;
	}
	else
	{
		boundingbox[0]->b1 = oboundingbox[0]->b1;
		boundingbox[0]->b2 = oboundingbox[0]->b2;
		boundingbox[0]->b3 = oboundingbox[0]->b3;
		boundingbox[0]->b4 = oboundingbox[0]->b4;
		boundingbox[0]->t1 = oboundingbox[0]->t1;
		boundingbox[0]->t2 = oboundingbox[0]->t2;
		boundingbox[0]->t3 = oboundingbox[0]->t3;
		boundingbox[0]->t4 = oboundingbox[0]->t4;
	}
}

void CLplayer::display(xlong m)
{
	//model display
	sposition.x = position.x;
	sposition.y = position.y - m;
	sposition.z = position.z;
	
	model[0]->display(sposition,FLAT + AMBIENT);
	model[1]->display(sposition,FLAT + AMBIENT);
	//*
	
	//ammo display
	for(int i=0; i<ammolist->getlength();i++)
	{
		ammolist->setindex(i);
		currammo = reinterpret_cast<CLammo*>(ammolist->getcurrentdata());
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

	//~ CLgfx1::drawpolygon(
//~ tposition.x+boundingbox[0]->b1.x,
//~ tposition.y-boundingbox[0]->b1.y - xmark,
//~ tposition.x+boundingbox[0]->b2.x,
//~ tposition.y-boundingbox[0]->b2.y - xmark,
//~ tposition.x+boundingbox[0]->b3.x,
//~ tposition.y-boundingbox[0]->b3.y - xmark,
//~ tposition.x+boundingbox[0]->b4.x,
//~ tposition.y-boundingbox[0]->b4.y - xmark,
//~ 0x0000FFFF);

	//~ CLgfx1::drawbigpixel(tv.x,tv.y-xmark,0x000FFFFFF);
	//~ CLgfx1::drawbigpixel(sposition.x,sposition.y,0x000FFFFFF);

	//CLgfx1::drawrectangle(65,0,735,599,0x00FF00FF);

	//CLgfx1::drawbigpixel(sposition.x+boundingbox[0]->b1.x,sposition.y-boundingbox[0]->b1.y,0x00FF00FF);
	//*
}

void CLplayer::setxyz(xlong x,xlong y,xlong z)
{
	//not to be used for regular player movement!
	//For regular movement x and y are controlled indirectly through the speed.
	//This is only to be used for special purposes like resetting after death.

	position.x = x;
	position.y = y;
	if(z!=0) position.z = z;
}

xlong CLplayer::gethealth()
{
	return health;
}

xlong CLplayer::getshield()
{
	return shield;
}

xlong CLplayer::getx()
{
	return position.x;
}

xlong CLplayer::gety()
{
	return position.y;
}

xlong CLplayer::getz()
{
	return position.z;
}

#endif

