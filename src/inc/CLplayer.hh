//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
#pragma message "Compiling " __FILE__ " ! TODO: interaction, new bcx,ev else"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
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

		vector speedmax;
		vector acceleration;

		vector position;
		vector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		vector speed;
		vector tilt; //meaning mainly z-tilt (ie on ramps)

		xlong active;
		xlong points;
		xlong firing;
		xlong lastupdate;
	public:
		CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,xlong p=0);
		~CLplayer();

		void draw();
		void move(xlong x,xlong y,xlong z=0);
		void incxspeed();
		void incyspeed();
		void decxspeed();
		void decyspeed();
		void fire(xlong at);
		void hurt(xlong am);
		void update();
		void collision();
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
};

CLplayer::CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,xlong p)
{
	//set parameters to attributes:
	model[0] = cha;
	//model[1] = tow; //temp reactivate as soon as 2nd model avail
	position.x = sx;
	position.y = sy;
	position.z = sz;
	points = p;

	//create attribute objects:
	ammolist = new CLlist();

	//construct bounding box from model:
	//CLbox = model->getboundingbox();

	//load player attribtes from data-file provided in parameters (dat)

	if( dat[1][0] != 'PLYR' ) CLexit_(1,__func__,"wrong bcx type");
	//if( dat[1][1] != model[0]->getname() ) CLexit_(__func__,1,"bcx and y3d are not fitting");
	health		= dat[1][2];
	shield		= dat[1][3];
	shieldrate	= dat[1][4];
	armor		= dat[1][5];

	speedmax.x      = dat[1][6];
	speedmax.y      = dat[1][7];
	speedmax.z      = dat[1][8];
	acceleration.x  = dat[1][9];
	acceleration.y  = dat[1][10];
	acceleration.z  = dat[1][11];

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
	direction[0].y = 0;
	direction[0].z = 0;
	direction[1].x = 0;
	direction[1].y = 0;
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
	lastupdate = 0;
	firing=-1;
}

CLplayer::~CLplayer() { }

void CLplayer::draw()
{
	//if(tilt!=0) model->rotate(0,0,tilt);
	//model->setposition()
	//model->display()
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

void CLplayer::incxspeed()
{

}

void CLplayer::incyspeed()
{

}

void CLplayer::decxspeed()
{

}

void CLplayer::decyspeed()
{

}

void CLplayer::fire(xlong at)
{

}

void CLplayer::hurt(xlong am)
{

}

void CLplayer::update()
{

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

