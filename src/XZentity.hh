///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Entity Class ( XZentity.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZfixed.hh"
#include "XZvector.hh"
#include "XZlist.hh"
#include "XZformat.hh"
#include "XZobject.hh"
#include "XZgame.hh"
#include "XZscreen.hh"
///</include>

//<declare>
#define ROTANG   2
#define BWIDTH  16
#define MAXSTEP  5
///</declare>

///<define>
struct ammo
{
	vector pos;
	vector dir;
};

class entity
{
	private:
		static const matrix turn[2];
		static list<ammo> ammo_list[2];

		static fixed ymark; // move to level

		object*  model[2];
		vector   position[2];
		vector   direction[2];
		vector** ammomount;
		xint     angle;

		const bool type;
		const xint shieldmax;
		const xint shieldrate;
		const xint ammomounts;
		const xint ammotype;
		const xint firerate;

		      yint lastupdate;
		      yint lastfire;

		      xint health;
		      xint shield;

		      yint points;

		void fire(xint i);

		xint terrain(const char** m,vector& n);

		entity(const entity& e);
		entity& operator=(const entity& e);

	public:
		entity(const vector& p,const info& v,object* m,object* n,xint s);
		~entity();

		xint update(xint k,xint j,fixed m,fixed n);
		xint update();

		void display(xint m,bool t);

		void check_ammo();
		static void display_ammo(bool t,xint m);

		void resume();

		inline void addpoints(xint a);
		inline yint getpoints() const;

		inline vector data(xint m) const; // w/o m 

		inline static xint ylevel(); // remove
};
///</define>

///<code>
const matrix entity::turn[2] = { []()->matrix { matrix m; m.rotatez(FX(-ROTANG)); return m; }(),[]()->matrix { matrix m; m.rotatez(FX(ROTANG)); return m; }() };
list<ammo>   entity::ammo_list[2]{ list<ammo>(), list<ammo>() };
fixed        entity::ymark    = 0;

void entity::fire(xint i)
{
	const bool j = (ammomount[i]->z) || (type!=0);

	ammo* cur = new ammo{ vector{position[0].x+ammomount[i]->x,position[0].y-ammomount[i]->y,0,0 },
	                      vector{direction[j].x,-direction[j].y,0,(FXONE<<2)} };

	ammo_list[type].append(cur);
}

void entity::check_ammo()
{
	list<ammo>& a = ammo_list[bool(type)];

	for(a.first();a.notlast();a.next())
	{
		const xint h = model[0]->collision(position[0],a.current()->pos)*4;
		ifu(h!=0) { delete a.delcurrent(); health = math::max(0,health-h); }
	}
}

void entity::display_ammo(bool t,xint m)
{
	list<ammo>& a = ammo_list[t];

	for(a.first();a.notlast();a.next())
	{
		const vector& dir = a.current()->dir;
		const vector& cur = a.current()->pos -= fx::mul(dir,dir.e);

		const xint cx = fx::r2l(cur.x);
		const xint cy = fx::r2l(cur.y) - m;

		switch( (screen::onscreen(cx-4,cy-4)&screen::onscreen(cx+4,cy+4))<<t )
		{
			case 0: delete a.delcurrent(); break;
			case 1: game::compiled(cx,cy,BLUE,YELLOW); break;
			case 2: game::compiled(cx,cy,GREEN,ORANGE); break;
		}
	}
}

xint entity::terrain(const char** m,vector& n) //TODO actual angle computation
{
	const fixed r = model[0]->bounding();

	const yint l0 = fx::f2l(position[0].x+r)/BWIDTH;
	const yint r0 = fx::f2l(position[0].x-r)/BWIDTH;
	const yint u0 = fx::f2l(position[0].y+r)/BWIDTH;
	const yint d0 = fx::f2l(position[0].y-r)/BWIDTH;

	const yint l1 = fx::f2l(n.x+r)/BWIDTH;
	const yint r1 = fx::f2l(n.x-r)/BWIDTH;
	const yint u1 = fx::f2l(n.y+r)/BWIDTH;
	const yint d1 = fx::f2l(n.y-r)/BWIDTH;
//REWORK!!! map members are corners!!!
	return	math::abs(m[l0][u0]-m[l1][u1])<=MAXSTEP &&
		math::abs(m[r0][u0]-m[r1][u1])<=MAXSTEP &&
		math::abs(m[l0][d0]-m[l1][d1])<=MAXSTEP &&
		math::abs(m[r0][d0]-m[r1][d1])<=MAXSTEP;
}

entity::entity(const vector& p,const info& v,object* m,object* n,xint s)
 : model{new object(*m),0},
   position{ p, vector{0,0,0,0} },
   direction{ vector{0,FXONE,0,FXONE}, vector{0,FXONE,0,FXONE} },
   ammomount(new vector*[string::str2int(v["mounts"])]),
   angle(0),
   type(s),
   lastupdate(screen::time()),
   lastfire(screen::time()),
   health(string::str2int(v["health"])),
   shield(string::str2int(v["shield"])),
   shieldmax(string::str2int(v["shield"])),
   shieldrate(string::str2int(v["srate"])),
   ammomounts(string::str2int(v["mounts"])),
   ammotype(string::str2int(v["atype"])),
   firerate(string::str2int(v["frate"])),
   points(string::str2int(v["points"]))
{
	switch(s)
	{
		case 2:
			object::linear.clear();
			//object::linear.scale(FXTWO,FXTWO,FXTWO);
			//scale bounding circle radius
			//model[0]->update();
			direction[0] = vector{-65536,0,0,FXONE}; //FXMON
			break;
		case 0:
			model[1] = new object(*n);
			ymark = position[0].y;

			position[1] = (*model[0]->docktype(3,0)) - (*model[1]->docktype(3,0));

			direction[0] = vector{0,-65536,0,0}; //FXMON
			direction[1] = vector{0,-65536,0,0}; //FXMON
	}

	const xint z = (model[1]!=0);

	for(xint i=0,j=0;i+j<ammomounts;++i)
	{
		vector* t = model[0]->docktype(z,i);
		const bool mt = (t==0);
		if(model[1]!=0 && mt==1) { t = model[1]->docktype(z,j); ++j; --i; }

		ammomount[i+j] = t;
		ammomount[i+j]->z = mt;	
	}
}

entity::~entity()
{
	delete model[0];
	if(model[1]!=0) { delete model[1]; }
	//delete[] ammomount;
}

xint entity::update(xint k,xint j,fixed m,fixed n)
{
	static xint last = 0;
	const bool l = k^last;
	const xint curr = screen::time();

	const bool mat = (angle>=0&&angle<=180) || (angle<=-180&&angle>=-360);

	//checkammo();

	ifu(health<=0)
	{
		model[0]->explode(-FXHLF+FXTNT);
		model[1]->explode(-FXHLF);
		return health--<-250;
	}

	k = math::set(0,k,k=='w' && angle==0);
	switch(k)
	{
		case RIGHT:
			model[0]->update(turn[0]);
			model[1]->update(turn[0]);
			direction[0] = turn[0]*direction[0];
			direction[1] = turn[0]*direction[1];
		break;

		case LEFT:
			model[0]->update(turn[1]);
			model[1]->update(turn[1]);
			direction[0] = turn[1]*direction[0];
			direction[1] = turn[1]*direction[1];
		break;

		case UP:
			direction[0].e = math::set(fx::l2f(direction[0].e>=0),direction[0].e,l==1);
		break;

		case DOWN:
			direction[0].e = math::set(fx::l2f(-(direction[0].e<=0)),direction[0].e,l==1);
		break;

		case 'a':
			model[1]->update(turn[1]);
			angle += ROTANG;
			direction[1] = turn[1]*direction[1];
		break;

		case 'd':
			model[1]->update(turn[0]);
			angle -= ROTANG;
			direction[1] =turn[0]*direction[1];
		break;

		case 'w':
			model[1]->update(turn[mat]);
			angle += math::neg(ROTANG,mat);
			direction[1] = turn[mat]*direction[1];
		break;

		case SPACE:
			for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
			lastfire = math::set(curr+firerate,lastfire,curr>lastfire);
		break;
	}
 
	angle -= math::set(360,angle>=360);

	const vector tp{position[0].x - fx::mul(direction[0].x,direction[0].e),
	                position[0].y + fx::mul(direction[0].y,direction[0].e),
	                position[0].z + fx::mul(direction[0].z,direction[0].e),
	                0 };

	const fixed r = model[0]->bounding();
	//terrain collision here:
	//terrain(map,tp);
	const bool t = (tp.x-r>=0)&&(tp.x+r<=FX(XRES))&&((tp.y-r>=m)&&(tp.y+r<=n)); //TODO

	position[0] = vector{math::set(tp.x,position[0].x,t),
	                     math::set(tp.y,position[0].y,t),
	                     math::set(tp.z,position[0].z,t),
	                     0 };

	ymark = position[0].y;
	last  = k;
	lastupdate = curr;

	return health;
}

xint entity::update()
{
	const xint curr = screen::time();

	ifu(health<=0)
	{
		model[0]->implode(FXCEN);
		return health--<-250;		
	}

	if( (health>0) && (position[0].y>0) && ((position[0].y+FX(YRES))>ymark) ) //check
	{
		check_ammo();

		for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
		lastfire = math::set(curr+firerate,lastfire,curr>lastfire);

		position[0].x -= fx::mul(direction[0].x,direction[0].e);
		//position[0].y += fx::mul(direction[0].y,direction[0].e); //temp
		position[0].z += fx::mul(direction[0].z,direction[0].e);

		direction[0].x = math::set(-direction[0].x,direction[0].x,position[0].x<=FX(XRES>>2));
		direction[0].x = math::set(-direction[0].x,direction[0].x,position[0].x>=FX(XRES-(XRES>>2)));
	}

	lastupdate = curr;
	return health;
}

void entity::display(xint m,bool t)
{
	guard(fx::r2l(position[0].y)<m-100&&fx::r2l(position[0].y)>m+YRES);

	//const vector p{fx::r2l(position[0].x),fx::r2l(position.y)-m,fx::r2l(position.z)};

	const vector p{fx::r2l(position[0].x),YRES/2,fx::r2l(position[0].z)}; //temp

	const xint r = math::set(R_B,R_F,t);
	model[0]->display(p,r);

	if(model[1]!=0) { model[1]->display(position[1]+p,r); }

//temp
const fixed y(model[0]->bounding());
vector q(p);
gfx::rect(fx::f2l(q.x-y),fx::f2l(q.y-y),fx::f2l(q.x+y),fx::f2l(q.y+y),BLUE,0,0,0);
//*
}

void entity::resume()
{
	lastfire = lastupdate = screen::time();
}

void entity::addpoints(xint a)
{
	points += a;
}

yint entity::getpoints() const
{
	return points;
}

vector entity::data(xint m) const
{
	return vector{fx::r2l(position[0].x),fx::r2l(position[0].y)-m,health,shield};
}

xint entity::ylevel()
{
	return fx::r2l(ymark);
}
///</code>

