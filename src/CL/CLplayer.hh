//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPLAYER
#define HH_CLPLAYER
//#pragma message "Compiling " __FILE__ " ! TODO: interaction"

#include "CLtypes.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLcl.hh"
#include "CLapi.hh"
#include "CLgame.hh"
#include "CLformat.hh"
#include "CLammo.hh"


class CLplayer : public virtual CLcl
{
	protected:
		CLobject* model[2]; //0 is chassis,1 is tower
		CLmatrix* cllinear;
		CLammomanager* ammoman;

	private:
		CLbox* boundingbox[2];
		CLbox* oboundingbox[2];
		
		CLfvector position;
		CLfvector tposition;
		CLlvector sposition;
		CLfvector direction[2]; //0 is chassis, 1 is tower, whereas tilt in all but x,y-plane will be chained together, meaning tilt (ie on ramps) and rotating of ie tower
		CLfvector speed;
		CLfvector speeddir;
		CLfvector tilt; //meaning mainly z-tilt (ie on ramps)
		
		xlong ammotypecount;
		xlong* ammotypes;
		xlong* firerate;
		xlong health;
		xlong shield;
		xlong shieldrate;
		xlong armor;
		xlong gear;
		xlong active;
		xlong points;
		xlong lastupdate[3];

		void setspeed();
		void fire(xlong at,bool m,xlong d,xlong i);
		void hit();
		void pretransform(bool m);
		void transform(bool m);
		xlong collision(CLfbuffer* ll,xlong m);
		
	public:
		CLplayer(xchar* playerlib,CLlvector& s,xlong p=0);
		~CLplayer();
		
		xlong update(xchar input,xchar turbo,CLfbuffer* ll,xlong mark);
		void display(xlong mark);
		void shadow(xlong mark);
		xlong gethealth();
		xlong getshield();
		CLfvector& getposition();
		CLbox* getboundingbox();
		xlong getx();
		xlong gety();
		xlong getz();
};


void CLplayer::setspeed()
{
	switch(gear)
	{
		case 0: speed = 0; break;
		case 1: speed = speeddir; break;
		case -1: speed = -speeddir; break;
	}
}

void CLplayer::fire(xlong at,bool m,xlong d,xlong i)
{
	CLfvector startposition = position;
	CLfvector* ammodocking  = model[m]->getdockingpoint(d,i);
	startposition.x += ammodocking->x;
	startposition.y -= ammodocking->y;
	startposition.z += ammodocking->z;
	ammoman->fire(at,startposition,direction[m]);
}

void CLplayer::pretransform(bool m)
{
	//transform tower boundng box
	boundingbox[1]->c[0] = cllinear->transform(boundingbox[1]->c[0]);
	boundingbox[1]->c[1] = cllinear->transform(boundingbox[1]->c[1]);
	boundingbox[1]->c[2] = cllinear->transform(boundingbox[1]->c[2]);
	boundingbox[1]->c[3] = cllinear->transform(boundingbox[1]->c[3]);
	boundingbox[1]->c[4] = cllinear->transform(boundingbox[1]->c[4]);
	boundingbox[1]->c[5] = cllinear->transform(boundingbox[1]->c[5]);
	boundingbox[1]->c[6] = cllinear->transform(boundingbox[1]->c[6]);
	boundingbox[1]->c[7] = cllinear->transform(boundingbox[1]->c[7]);
	//*
	
	//transform chassis if wanted
	if(m==0)
	{
		boundingbox[0]->c[0] = cllinear->transform(boundingbox[0]->c[0]);
		boundingbox[0]->c[1] = cllinear->transform(boundingbox[0]->c[1]);
		boundingbox[0]->c[2] = cllinear->transform(boundingbox[0]->c[2]);
		boundingbox[0]->c[3] = cllinear->transform(boundingbox[0]->c[3]);
		boundingbox[0]->c[4] = cllinear->transform(boundingbox[0]->c[4]);
		boundingbox[0]->c[5] = cllinear->transform(boundingbox[0]->c[5]);
		boundingbox[0]->c[6] = cllinear->transform(boundingbox[0]->c[6]);
		boundingbox[0]->c[7] = cllinear->transform(boundingbox[0]->c[7]);
	}
	//*
}

void CLplayer::transform(bool m)
{
	//transform tower
	model[1]->update(cllinear); //transform model(s)
	direction[1] = cllinear->transform(direction[1]); //transform direction vector
	//*
	
	//transform chassis if wanted
	if(m==0)
	{
		model[0]->update(cllinear); //transform model(s)
		direction[0] = cllinear->transform(direction[0]); //transform direction vector
		speeddir = cllinear->transform(speeddir); //transform (constant) speeddir vector
		//tilt = cllinear->transform(tilt); //transform tilt vector
	}
	//*
}

xlong CLplayer::collision(CLfbuffer* ll,xlong m)
{
	xlong r = 0;

	//screen boundary collision test
	tposition.y -= m;
	xlong bc = CLgame::boundary(tposition,*boundingbox[0]); //boundary check: (check if game screen is left)
	tposition.y += m;

	if(bc!=0)
	{
		gear=0;
		setspeed();
		r++;
	}
	//*

	//terrain collision test
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
	//*
	
	//temp
		//tposition.z += zdiff; //only growth when uphill, constant on downhill, funny :)
		if(zdiff!=0) { CLsystem::print("z level change: ",0); CLsystem::print(zdiff); }
		
		//rotate x about xangle,y about yangle
		//cllinear->rotate(xangle,0,0);
		//if(xangle!=0) CLsystem::print(xangle);
		//if(yangle!=0) CLsystem::print(yangle);
	//*

	//enemy collision check
	
		//...
	
	//*

	return r;
}

CLplayer::CLplayer(xchar* playerlib,CLlvector& s,xlong p)
{
	//matrix for player transformations
	cllinear = new CLmatrix(1);
	//*
	
	//set points
	points = p;
	//*
	
	//load player archive 
	CLfile* playerraw = CLsystem::getfile(playerlib);
	arfile* playera = CLformat::loadar(playerraw);
	//*

	//find player definition, has to have extension .ini
	xlong pd = CLutils::findarmember(playera,".ini");
	if(pd==-1) CLsystem::exit(1,0,__func__,"no player definition found");
	xmap* pini = CLformat::loadini(playera->members[pd]);
	//*
	
	//find first part of player model
	xlong pm = CLutils::findarmember(playera,"0.y3d");
	if(pm==-1) CLsystem::exit(1,0,__func__,"no player model part I file found");
	model[0] = new CLobject(playera->members[pm],0);
	//*
	
	//find second part of player model
	xlong pn = CLutils::findarmember(playera,"1.y3d");
	if(pn==-1) CLsystem::exit(1,0,__func__,"no player model part I file found");
	model[1] = new CLobject(playera->members[pn],0);
	//*

	//set and copy bounding box of first model
	boundingbox[0] = new CLbox;
	oboundingbox[0] = model[0]->getboundingbox();
	*boundingbox[0] = *oboundingbox[0];
	//*

	//set and copy bounding box of second model
	boundingbox[1] = new CLbox;
	oboundingbox[1] = model[1]->getboundingbox();
	*boundingbox[1] = *oboundingbox[1];
	//*
	
	//set and adjust (start) position to floor
	position = s;
	position.z += 95;
	tposition = position;
	//*
	
	//load player info from definition file (...later ini)
	health		= CLsystem::ato((*pini)["health"]);
	shield		= CLsystem::ato((*pini)["shield"]);
	shieldrate	= CLsystem::ato((*pini)["shieldrate"]);
	armor		= CLsystem::ato((*pini)["armor"]);
	speeddir.x  = 0;
	speeddir.y  = -CLsystem::ato((*pini)["speed"]);
	speeddir.z  = 0;
	ammotypecount = CLsystem::ato((*pini)["ammotypecount"]);
	ammotypes  = new xlong[ammotypecount];
	firerate   = new xlong[ammotypecount];
	ammotypes[0] = CLsystem::ato((*pini)["ammo0"]);
	firerate[0]	= CLsystem::ato((*pini)["firerate0"]); //every X ms
	ammotypes[1] = CLsystem::ato((*pini)["ammo1"]);
	firerate[1]	= CLsystem::ato((*pini)["firerate1"]); //every X ms
	//*
	
	//create ammo manager
	ammoman = new CLammomanager(ammotypecount,ammotypes);
	//*
	
	//set remaining player attributes
	speed = 0;
	gear = 0;
	active = 1;
	direction[0].x = direction[1].x = 0;
	direction[0].y = direction[1].y = 1;
	direction[0].z = direction[1].z = 0;	
	lastupdate[0] = CLsystem::getmilliseconds();
	lastupdate[1] = CLsystem::getmilliseconds();
	lastupdate[2] = CLsystem::getmilliseconds();
	//*
	
	delete pini;
}

CLplayer::~CLplayer()
{
	//release everything this class grabbed
	delete cllinear;
	delete ammoman;
	delete boundingbox[0];
	delete boundingbox[1];
	delete model[0];
	delete model[1];
	//*
}

xlong CLplayer::update(xchar input,xchar turbo,CLfbuffer* ll,xlong mark)
{
	ammoman->update(mark);
	
	xlong time = CLsystem::getmilliseconds();
	
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

	cllinear->unit();
	bool what = 0;
	
	switch(turbo)
	{
		//arrow left -> turn left
		case 81: 
			cllinear->rotate(0,0,5);
			pretransform(0);	
			setspeed();
		break;
		//*

		//arrow right -> turn right
		case 83: 
			cllinear->rotate(0,0,-5);
			pretransform(0);
			setspeed();
		break;
		//*

		//a -> turn tower left
		case 97: 
			cllinear->rotate(0,0,5);
			pretransform(1);
			what=1;
		break;
		//*

		//d -> turn tower right
		case 100: 
			cllinear->rotate(0,0,-5);
			pretransform(1);
			what=1;
		break;
		//*

		//space -> fire tower weapon
		case 32: 
			if(time >= lastupdate[1] + firerate[1])
			{
				fire(1,1,4,0);
				lastupdate[1] = time;
			}
		break;
		//*

		//strg -> fire chassis weapon(s)
		case -29: 
			if(time >= lastupdate[1] + firerate[0])
			{
				fire(0,0,3,0);
				fire(0,0,3,1);
				lastupdate[1] = time;
			}
		break;
		//*

		case 119: //w -> fire (tachyon) laser
		break; //*

		case 115: //s -> fire laser
		break; //*

		case 101: //e -> action key
		break; //*
	}
	
	xmark = mark; //temp

	//update test position
	if(time >= lastupdate[0] + 20)
	{
		tposition.x = position.x - speed.x;
		tposition.y = position.y + speed.y;
		tposition.z = position.z + speed.z;
		
		lastupdate[0] = time;	
	}
	//*

	//check if test position doesn't collide with anything
	if(collision(ll,mark)==0)
	{	
		transform(what);
		
		position = tposition;
	}
	//*
	
	//if collision reset bounding box to state before pretransformation
	else
	{
		*boundingbox[0] = *oboundingbox[0];
	}
	//*
}

void CLplayer::display(xlong mark)
{
	//model display
	sposition.x = position.x;
	sposition.y = position.y - mark;
	sposition.z = position.z;
	//~ model[0]->display(sposition,SHADOW);
	//~ model[1]->display(sposition,SHADOW);
	//~ CLstencilbuffer->blendcopy(CLdoublebuffer->getbuffer(),4);
	model[0]->display(sposition,FLAT + AMBIENT);
	model[1]->display(sposition,FLAT + AMBIENT);
	//*
	
	//ammo display
	ammoman->display(mark);
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

	//CLgfx1::drawrectangle(65,0,735,599,0x00FF00FF);

	//*
}

void CLplayer::shadow(xlong mark)
{
	sposition.x = position.x;
	sposition.y = position.y - mark;
	sposition.z = position.z;
	model[0]->display(sposition,SHADOW);
	model[1]->display(sposition,SHADOW);
}

xlong CLplayer::gethealth()
{
	return health;
}

xlong CLplayer::getshield()
{
	return shield;
}

CLfvector& CLplayer::getposition()
{
	return position;
}

CLbox* CLplayer::getboundingbox()
{
	return boundingbox[0];
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

