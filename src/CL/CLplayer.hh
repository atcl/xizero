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


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
		CLmatrix* cllinear;
		CLlist*   ammolist;
		CLgame*   clgame;

	private:
		CLbox* boundingbox[2];

		xlong ammotype[4];
		xlong firerate[4];
		xlong ammoloadrate[4];

		CLfvector ammodirection[4];
		
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;

		CLfvector position;
		CLfvector tposition;
		CLfvector lposition;
		CLlvector sposition;
		CLfvector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		CLfvector speed;
		CLfvector speeddir;
		CLfvector tilt; //meaning mainly z-tilt (ie on ramps)


		xlong gear;
		xlong active;
		xlong points;
		xlong firing;
		float lastupdate;

		void setspeed();
		void fire(xlong at);
		void hurt(xlong am);
		void transform(bool m);
		xlong collision(xlong mark);
	public:
		CLplayer(CLobject* cha,CLobject* tow,xlong** dat,CLlvector s,CLgame* clg,xlong imark,xlong p=0);
		~CLplayer();

		void update(xchar input,char turbo,xchar*** levellayers,xlong mark);
		void display();

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

void CLplayer::transform(bool m)
{
	//bool decides what part, if complete or only tower

	if(m==false)
	{
		//transform model(s)
		model[0]->update(cllinear);
		//model[1]->update(cllinear);
	
		//transform direction vector
		direction[0] = cllinear->transform(direction[0]);
		direction[1] = cllinear->transform(direction[1]);
	
		//transform speed vector
		speed = cllinear->transform(speed);

		//transform (constant) speeddir vector
		speeddir = cllinear->transform(speeddir);

		//transform tilt vector
		tilt = cllinear->transform(tilt);

		//transform ammo direction(s)

	}
	else
	{
		//transform model(s)
		//model[1]->update(cllinear);

		//transform direction vector
		direction[1] = cllinear->transform(direction[1]);

		//transform ammo direction(s)

	}
}

xlong CLplayer::collision(xlong mark)
{
	if(gear==0) return 0;

	xlong r = 0;

	//boundary check: (check if game screen is left)
	xlong bc = 0; //clgame->boundary(tposition,boundingbox[0]);

	if(bc!=0)
	{
		if(bc==-1 && speed.x>=0) { gear=0; setspeed(); r++; }
		if(bc==1  && speed.x<=0) { gear=0; setspeed(); r++; }
		if(bc==-2 && speed.y<=0) { gear=0; setspeed(); r++; }
		if(bc==2  && speed.y>=0) { gear=0; setspeed(); r++; }
	}
	//*

	//terrain collision check: (check if player collides with terrain block)
	xlong tc = 0; //clgame->impact(tposition,lposition,boundingbox[0],clzbuffer);

// 	CLprint_(tc);
// 	CLprint_(speeddir);

	 //compare player current z with surrounding terrain (in zbuffer,since terrain is rendered first)
	if(tc!=0)
	{
		if(tc==1) { gear=0; setspeed(); r++; }
		if(tc==2) { gear=0; setspeed(); r++; }
		if(tc==4) { gear=0; setspeed(); r++; }
		if(tc==8) { gear=0; setspeed(); r++; }
	}

	//environment check: (check if player can drive up- or downhill)


	//enemy collision check: (check if player collides with enemy entity)

	return r;
}

CLplayer::CLplayer(CLobject* cha,CLobject* tow,xlong** dat,CLlvector s,CLgame* clg,xlong imark,xlong p)
{
	//set parameters to attributes:
	model[0] = cha;

	//model[1] = tow; //temp reactivate as soon as 2nd model avail
	boundingbox[0] = model[0]->getboundingbox();
	//boundingbox[1] = model[1]->getboundingbox(); //temp reactivate as soon as 2nd model avail

	clgame = clg;
	cllinear = new CLmatrix(1);

	position = s;
	lposition = s;
	sposition = s;
	sposition.y -= imark;

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

	ammotype[0]	= dat[1][12];
	firerate[0]	= dat[1][13];
	ammoloadrate[0]	= dat[1][14];

	ammotype[1]	= dat[1][15];
	firerate[1]	= dat[1][16];
	ammoloadrate[1]	= dat[1][17];

	ammotype[2]	= dat[1][18];
	firerate[2]	= dat[1][19];
	ammoloadrate[2]	= dat[1][20];

	ammotype[3]	= dat[1][21];
	firerate[3]	= dat[1][22];
	ammoloadrate[3] = dat[1][23];

	//set other attributes to init:
	speed = 0;

	direction[0].x = 0;
	direction[0].y = 1;
	direction[0].z = 0;
	direction[1].x = 0;
	direction[1].y = 1;
	direction[1].z = 0;

	ammodirection[0].x = 0;
	ammodirection[0].y = 0;
	ammodirection[0].z = 0;
	
	ammodirection[1].x = 0;
	ammodirection[1].y = 0;
	ammodirection[1].z = 0;

	ammodirection[2].x = 0;
	ammodirection[2].y = 0;
	ammodirection[2].z = 0;

	ammodirection[3].x = 0;
	ammodirection[3].y = 0;
	ammodirection[3].z = 0;

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

void CLplayer::update(xchar input,xchar turbo,xchar*** levellayers,xlong mark)
{

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

	switch(turbo)
	{
		case 81: //arrow left -> turn left
			cllinear->rotate(0,0,5);
			transform(false);
			cllinear->unit();
		break;

		case 83: //arrow right -> turn right
			cllinear->rotate(0,0,-5);
			transform(false);
			cllinear->unit();
		break;

		case 97: //a -> turn tower left
			cllinear->rotate(0,0,4);
			transform(true);
			cllinear->unit();
		break;

		case 100: //d -> turn tower right
			cllinear->rotate(0,0,-4);
			transform(true);
			cllinear->unit();
		break;

		case 32: //space -> fire tower weapon
		break;

		case -29: //strg -> fire chassis weapon(s)
		break;

		case 119: //w -> fire tachyon laser
		break;

		case 115: //s -> fire laser
		break;

		case 101: //e -> action key
		break;
	}

	//
	float temp = CLsystem::getmilliseconds();
	if(temp >= lastupdate + 20)
	{
		tposition.x = position.x - speed.x;
		tposition.y = position.y + speed.y - mark;
		tposition.z = position.z + speed.z;
		lastupdate = temp;

		if(collision(mark)==0)
		{
			lposition = sposition;
			position = tposition;

			position.y += mark;
		
			sposition.x = xlong(tposition.x);
			sposition.y = xlong(tposition.y);
			sposition.z = xlong(tposition.z);
		}
	}
	//*
}

void CLplayer::display()
{
	model[0]->setposition(sposition.x,sposition.y,100);
	model[0]->display(FLAT + AMBIENT);

	//temp!
	CLgfx1::drawpolygon(
sposition.x+boundingbox[0]->b1.x,
sposition.y-boundingbox[0]->b1.y,
sposition.x+boundingbox[0]->b2.x,
sposition.y-boundingbox[0]->b2.y,
sposition.x+boundingbox[0]->b3.x,
sposition.y-boundingbox[0]->b3.y,
sposition.x+boundingbox[0]->b4.x,
sposition.y-boundingbox[0]->b4.y,
0x00FFFFFF);

	CLgfx1::drawrectangle(65,0,735,599,0x00FF00FF);

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

