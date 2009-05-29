//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
#pragma message "Compiling " __FILE__ " ! TODO: interaction, new bcx,ev else"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLapi.hh"


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
		CLmatrix* cllinear;
		CLlist*   ammolist;

	private:
		CLbox bbox;

		xlong ammotype[4];
		xlong firerate[4];
		xlong ammoloadrate[4];

		vector ammodirection[4];
		
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;

		float speedmax;
		fvector acceleration;

		vector position;
		vector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		fvector speed;
		fvector tilt; //meaning mainly z-tilt (ie on ramps)
		vector screenpos;

		xlong active;
		xlong points;
		xlong firing;
		float lastupdate;

		void incspeed();
		void decspeed();
		void fire(xlong at);
		void hurt(xlong am);
		void transform(bool m);
		void collision();
	public:
		CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,CLmath* clm,xlong p=0);
		~CLplayer();

		void display(xlong mark);
		void move(xlong x,xlong y,xlong z=0);
		void update(xchar input);
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
};

CLplayer::CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,CLmath* clm,xlong p)
{
	//set parameters to attributes:
	model[0] = cha;
	//model[1] = tow; //temp reactivate as soon as 2nd model avail

	cllinear = new CLmatrix(1,clm);

	position.x = sx;
	position.y = sy;
	position.z = sz;
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

	speedmax        = 8; //dat[1][6];

	acceleration.x  = 0; //dat[1][9];
	acceleration.y  = -1; //dat[1][10];
	acceleration.z  = 0; //dat[1][11];

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
	speed.x = 0;
	speed.y = 0;
	speed.z = 0;

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

	active = true;
	lastupdate = CLgetmilliseconds_();
	firing=-1;
}

CLplayer::~CLplayer() { }

void CLplayer::display(xlong mark)
{
	screenpos.x = position.x;
	screenpos.y = position.y - mark;
	screenpos.z = position.z;

	model[0]->setposition(screenpos.x,screenpos.y,100);
	model[0]->display(0,1,1,0,0,0);
}

void CLplayer::move(xlong x,xlong y,xlong z)
{
	//not to be used for regular player movement!
	//For regular movement x and y are controlled indirectly through the speed.
	//This is only to be used for special purposes like resetting after death.

	position.x = x;
	position.y = y;
	if(z!=0) position.z = z;
}

void CLplayer::incspeed()
{
	if(speed.l <= speedmax)
	{
		speed.x += acceleration.x;
		speed.y += acceleration.y;
		speed.z += acceleration.z;
	}
}

void CLplayer::decspeed()
{
	if(speed.l > acceleration.l)
	{
		speed.x -= acceleration.x;
		speed.y -= acceleration.y;
		speed.z -= acceleration.z;
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
	//bool decdes what part, if complete or only tower

	if(m==false)
	{
		//transform model(s)
		model[0]->update(cllinear);
		//model[1]->update(cllinear);

		//transform bounding box
		//bbox.a = cllinear->transform(bbox.a);
		//bbox.b = cllinear->transform(bbox.b);
	
		//transform direction vector
		direction[0] = cllinear->transform(direction[0]);
		direction[1] = cllinear->transform(direction[1]);
	
		//transform speed vector
		speed = cllinear->transform(speed);

		//transform (constant) acceleration vector
		acceleration = cllinear->transform(acceleration);

		//transform tilt vector
		tilt = cllinear->transform(tilt);

		//transform ammo direction(s)

	}
	else
	{
		//transform model(s)
		//model[1]->update(cllinear);

		//transform bounding box
		//bbox.a = cllinear->transform(bbox.a);
		//bbox.b = cllinear->transform(bbox.b);

		//transform direction vector
		direction[1] = cllinear->transform(direction[1]);

		//transform ammo direction(s)

	}
}

void CLplayer::update(xchar input)
{
	switch(input)
	{
		case 82: //arrow up -> accelerate
		incspeed();
		break;

		case 84: //arrow down -> deccelerate
		decspeed();
		break;

		case 81: //arrow left -> turn left
		decspeed();
		cllinear->rotate(0,0,5);
		transform(false);
		cllinear->unit();
		break;

		case 83: //arrow right -> turn right
		decspeed();
		cllinear->rotate(0,0,-5);
		transform(false);
		cllinear->unit();
		break;

		case 97: //a -> turn tower left
		cllinear->rotate(0,0,5);
		transform(true);
		cllinear->unit();
		break;

		case 100: //d -> turn tower right
		cllinear->rotate(0,0,-5);
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

	float temp = CLgetmilliseconds_();
	if(temp >= lastupdate + 50)
	{
		position.x -= speed.x;
		position.y += speed.y;
		position.z += speed.z;
		lastupdate = temp;
	}
}

void CLplayer::collision()
{

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

