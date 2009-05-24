//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
#pragma message "Compiling " __FILE__ " ! TODO: interaction"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model;
		CLlist*   ammolist;

	private:
		xlong width;
		xlong height;
		xlong depth;

		xlong ammotype[4];
		xlong ammodirection[4];
		xlong firerate[4];
		xlong ammoload[4];
		xlong ammoloadrate[4];
		xlong firing;
		
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;

// 		vector dimension;
// 		vector postion;
// 		vector direction;
// 		vector speed;
// 		vector speedmax;
// 		vector acceleration;

		xlong xcoord;
		xlong ycoord;
		xlong zcoord;
		xlong xdirection;
		xlong ydirection;
		xlong speed;
		xlong speedmax;
		xlong xspeed;
		xlong yspeed;
		xlong xacc;
		xlong yacc;

		xlong tilt; //meaning z-tilt

		xlong points; //cumulative over all levels? then ctor para!

		xlong lastupdate;
	public:
		CLplayer(CLobject* obj,xlong** dat);
		~CLplayer();

		void draw();
		void move(xlong x,xlong y);
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
};

CLplayer::CLplayer(CLobject* obj,xlong** dat)
{
	ammolist = new CLlist();

	model = obj;
	points = 0;
	lastupdate=0;
	firing=-1;
	tilt = 0;
	speed = 1;
	speedmax = 6;

	//if( dat[1][0] != 'ENEM' ) exit(1) //wrong bcx type
	//if( dat[1][1] != model->getname() ) exit(1) //bcx and y3d are not fitting
	xcoord		= dat[1][2];
	ycoord		= dat[1][3];
	xdirection	= dat[1][4];
	ydirection	= dat[1][5];
	width		= dat[1][6];
	height		= dat[1][7];
	depth		= dat[1][8];
	xspeed		= dat[1][9];
	yspeed		= dat[1][10];
	ammotype[0]	= dat[1][11];
	ammodirection[0]= dat[1][12];
	firerate[0]	= dat[1][13];
	ammoload[0]	= dat[1][14];
	ammoloadrate[0]	= dat[1][15];
	ammotype[1]	= dat[1][16];
	ammodirection[1]= dat[1][17];
	firerate[1]	= dat[1][18];
	ammoload[1]	= dat[1][19];
	ammoloadrate[1]	= dat[1][20];
	ammotype[2]	= dat[1][21];
	ammodirection[2]= dat[1][22];
	firerate[2]	= dat[1][23];
	ammoload[2]	= dat[1][24];
	ammoloadrate[2]	= dat[1][25];
	ammotype[3]	= dat[1][26];
	ammodirection[3]= dat[1][27];
	firerate[3]	= dat[1][28];
	ammoload[3]	= dat[1][29];
	ammoloadrate[3]= dat[1][30];
	health		= dat[1][31];
	shield		= dat[1][32];
	shieldrate	= dat[1][33];
	armor		= dat[1][34];
}

CLplayer::~CLplayer() { }

void CLplayer::draw()
{
	//if(tilt!=0) model->rotate(0,0,tilt);
	//model->setposition()
	//model->display()
}

void CLplayer::move(xlong x,xlong y)
{
	//not to be used for regular player movement!
	//For regular movement x and y are controlled indirectly through the speed.
	//This is only to be used for special purposes like resetting after death.

	xcoord = x;
	ycoord = y;
}

void CLplayer::incxspeed()
{
	if(xspeed<speedmax) xspeed += xacc;
}

void CLplayer::incyspeed()
{
	if(yspeed<speedmax) yspeed += yacc;
}

void CLplayer::decxspeed()
{
	if(xspeed>-speedmax) xspeed -= xacc;
}

void CLplayer::decyspeed()
{
	if(yspeed>-speedmax) yspeed -= yacc;
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
	return xcoord;
}

xlong CLplayer::gety()
{
	return ycoord;
}

#endif

