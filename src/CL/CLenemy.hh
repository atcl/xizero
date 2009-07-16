//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLENEMY
#define HH_CLENEMY
#pragma message "Compiling " __FILE__ " ! TODO: interaction, see player for ctor and attr"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"


class CLenemy : public virtual CLcl
{
	protected:
		CLobject* model;
		CLlist*   ammolist;

	private:
		xlong name;
		xlong xcoord;
		xlong ycoord;
		xlong xdirection;
		xlong ydirection;
		xlong width;
		xlong height;
		xlong depth;
		//xlong xdelay;
		//xlong ydelay;
		//xlong xcount;
		//xlong ycount;
		xlong xspeed;
		xlong yspeed;

		xlong ammotype[2];
		xlong ammodirection[2];
		xlong firerate[2];
		
		xlong health;
		xlong shield;
		xlong armor;
		xlong points;

		xlong aitype;

		xlong* aiarray;
		xlong lastupdate;
	public:
		CLenemy(CLobject* obj, xlong** dat);
		~CLenemy();
		void draw();
		void move();
		void fire();
		void hurt(xlong am);
		void update();
		void collision();
		xlong gethealth();
};

CLenemy::CLenemy(CLobject* obj, xlong** dat)
{
	model = obj;
	lastupdate = 0;

	//if( dat[1][0] != 'ENEM' ) exit(1) //wrong bcx type
	//if( dat[1][1] != model->getname() ) exit(1) //bcx and y3d are not fitting
	xcoord      = dat[1][2];
	ycoord      = dat[1][3];
	xdirection  = dat[1][4];
	ydirection  = dat[1][5];
	width       = dat[1][6];
	height      = dat[1][7];
	depth       = dat[1][8];
	xspeed      = dat[1][9];
	yspeed      = dat[1][10];
	ammotype[0] = dat[1][11];
	ammodirection[0] = dat[1][12];
	firerate[0]  = dat[1][13];
	ammotype[1] = dat[1][14];
	ammodirection[1] = dat[1][15];
	firerate[1]  = dat[1][16];
	health      = dat[1][17];
	shield      = dat[1][18];
	armor       = dat[1][19];
	aitype      = dat[1][20];
}

CLenemy::~CLenemy() { }

void CLenemy::draw()
{

}

void CLenemy::move()
{

}

void CLenemy::fire()
{

}

void CLenemy::hurt(xlong am)
{

}

void CLenemy::update()
{

}

void CLenemy::collision()
{

}

xlong CLenemy::gethealth()
{
	return health;
}

#endif


