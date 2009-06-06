//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
#pragma message "Compiling " __FILE__ " ! TODO: interaction, new bcx,ev else,brking (speed=0),correct turning (speed)"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLapi.hh"
#include "CLgame.hh"


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
		CLmatrix* cllinear;
		CLlist*   ammolist;
		CLmath*   clmath;
		CLgame*   clgame;

	private:
		CLbox* boundingbox[2];

		xlong ammotype[4];
		xlong firerate[4];
		xlong ammoloadrate[4];

		vector ammodirection[4];
		
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;

		fvector speeddir;

		vector position;
		vector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		vector speed;
		fvector tilt; //meaning mainly z-tilt (ie on ramps)
		vector screenpos;

		bool  fwbw;
		xlong active;
		xlong points;
		xlong firing;
		float lastupdate;

		void setspeed(xlong s);
		void fire(xlong at);
		void hurt(xlong am);
		void transform(bool m);
		void collision(xchar*** levellayers);
	public:
		CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,CLmath* clm,CLgame* clg,xlong p=0);
		~CLplayer();

		void update(xchar input,char turbo,xchar*** levellayers);
		void display(xlong mark);

		void setxyz(xlong x,xlong y,xlong z);
		xlong gethealth();
		xlong getshield();
		xlong getx();
		xlong gety();
		xlong getz();
};

void CLplayer::setspeed(xlong s)
{
	switch(s)
	{
		case 0:
			speed.x = 0;
			speed.y = 0;
			speed.z = 0;
		break;

		case 1:
			speed.x = speeddir.x;
			speed.y = speeddir.y;
			speed.z = speeddir.z;
		break;

		case -1:
			speed.x = -speeddir.x;
			speed.y = -speeddir.y;
			speed.z = -speeddir.z;
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

void CLplayer::collision(xchar*** levellayers)
{
	//boundary check: (check if game screen is left)
	xlong bc = clgame->boundary(screenpos,boundingbox[0]);

	if(bc!=0)
	{
		if(bc==-1 && speed.x>=0) setspeed(0);
		if(bc==1 && speed.x<=0) setspeed(0);
		if(bc==-2 && speed.y<=0) setspeed(0);
		if(bc==2 && speed.y>=0) setspeed(0);
	}

	//terrain collision check: (check if player collides with terrain block)

	xlong blockwidth = 40;
	xlong blockheight = 40;
	xlong blockdepth = 40;

	xlong lx1 = ( ( position.x + clmath->max(boundingbox[0]->a.x,boundingbox[0]->a.x) + speed.x ) / blockwidth );
	xlong ly1 = ( ( position.y + clmath->max(boundingbox[0]->a.y,boundingbox[0]->a.y) + speed.y ) / blockheight );
	xlong lz1 = ( ( position.z + clmath->max(boundingbox[0]->a.z,boundingbox[0]->a.z) + speed.z ) / blockdepth );

	xlong lx2 = ( ( position.x + clmath->max(boundingbox[0]->a.x,boundingbox[0]->a.x) + speed.x ) / blockwidth );
	xlong ly2 = ( ( position.y + clmath->max(boundingbox[0]->a.y,boundingbox[0]->a.y) + speed.y ) / blockheight );
	xlong lz2 = ( ( position.z + clmath->max(boundingbox[0]->a.z,boundingbox[0]->a.z) + speed.z ) / blockdepth );

	//!continue here...

	//environment check: (check if player can drive up- or downhill)


	//enemy collision check: (check if player collides with enemy entity)


}

CLplayer::CLplayer(CLobject* cha,CLobject* tow,xlong** dat,xlong sx,xlong sy,xlong sz,CLmath* clm,CLgame* clg,xlong p)
{
	//set parameters to attributes:
	model[0] = cha;
	//model[1] = tow; //temp reactivate as soon as 2nd model avail
	boundingbox[0] = model[0]->getboundingbox();
	//boundingbox[1] = model[1]->getboundingbox(); //temp reactivate as soon as 2nd model avail

	clmath = clm;
	clgame = clg;
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

	speeddir.x  = 0;
	speeddir.y  = -4;
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

	fwbw = 1;
	active = true;
	lastupdate = CLgetmilliseconds_();
	firing=-1;
}

CLplayer::~CLplayer()
{
	delete cllinear;
	delete ammolist;
}

void CLplayer::update(xchar input,xchar turbo,xchar*** levellayers)
{

// 	switch(input)
// 	{
// 		
// 	}

	switch(turbo)
	{
		case 82: //arrow up -> accelerate
			setspeed(1);
			fwbw=1;
		break;

		case 84: //arrow down -> deccelerate
			setspeed(-1);
			fwbw=0;
		break;

		case 81: //arrow left -> turn left
			cllinear->rotate(0,0,8);
			transform(false);
			cllinear->unit();
		break;

		case 83: //arrow right -> turn right
			cllinear->rotate(0,0,-8);
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

	//processing new data:
	collision(levellayers);

	float temp = CLgetmilliseconds_();
	if(temp >= lastupdate + 25)
	{
		position.x -= speed.x;
		position.y += speed.y;
		position.z += speed.z;
		lastupdate = temp;
	}
}

void CLplayer::display(xlong mark)
{
	screenpos.x = position.x;
	screenpos.y = position.y - mark;
	screenpos.z = position.z;

	model[0]->setposition(screenpos.x,screenpos.y,100);
	model[0]->display(0,1,1,0,0,0);
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

