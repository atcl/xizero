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
		object*  model[2];
		vector   pos[2];
		vector   dir[2];
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

		      xint mhealth;
		      xint mshield;
		      xint mpoints;

		static const matrix turn[2];
		static list<ammo> ammo_list[2];

		void fire(xint i);

		xint terrain(const char** m,vector& n);

		entity(const entity& e);
		entity& operator=(const entity& e);

	public:
		entity(const vector& p,const info& v,object* m,object* n,xint s);
		~entity();

		xint update();
		xint update(bool v);

		void display(const vector& p,bool t);

		void check_ammo();

		void resume();

		inline vector position() const;
		inline void points(xint a);
		inline xint points() const;
		inline xint health() const;
		inline xint shield() const;

		static void display_ammo(bool t,xint m);
};
///</define>

///<code>
const matrix entity::turn[2] = { []()->matrix { matrix m; m.rotatez(FX(-ROTANG)); return m; }(),
                                 []()->matrix { matrix m; m.rotatez(FX(ROTANG)); return m; }() };
list<ammo>   entity::ammo_list[2]{ list<ammo>(), list<ammo>() };

void entity::fire(xint i)
{
	const bool j = (ammomount[i]->z) || (type!=0);

	ammo* cur = new ammo{ vector{pos[0].x+ammomount[i]->x,pos[0].y-ammomount[i]->y,0,0 },
	                      vector{dir[j].x,-dir[j].y,0,(FXONE<<2)} };

	ammo_list[type].append(cur);
}

void entity::check_ammo()
{
	list<ammo>& a = ammo_list[bool(type)];

	for(a.first();a.notlast();a.next())
	{
		const xint h = model[0]->collision(pos[0],a.current()->pos)*4;
		ifu(h!=0) { delete a.delcurrent(); mhealth = math::max(0,mhealth-h); }
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

	const yint l0 = fx::f2l(pos[0].x+r)/BWIDTH;
	const yint r0 = fx::f2l(pos[0].x-r)/BWIDTH;
	const yint u0 = fx::f2l(pos[0].y+r)/BWIDTH;
	const yint d0 = fx::f2l(pos[0].y-r)/BWIDTH;

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
   pos{ fx::l2f(p), vector{0,0,0,0} },
   dir{ vector{0,FXONE,0,FXONE}, vector{0,FXONE,0,FXONE} },
   ammomount(new vector*[string::str2int(v["mounts"])]),
   angle(0),
   type(s),
   lastupdate(screen::time()),
   lastfire(screen::time()),
   mhealth(string::str2int(v["health"])),
   mshield(string::str2int(v["shield"])),
   shieldmax(string::str2int(v["shield"])),
   shieldrate(string::str2int(v["srate"])),
   ammomounts(string::str2int(v["mounts"])),
   ammotype(string::str2int(v["atype"])),
   firerate(string::str2int(v["frate"])),
   mpoints(string::str2int(v["points"]))
{
	switch(s)
	{
		case 2:
			//object::linear.clear();
			//object::linear.scale(FXTWO,FXTWO,FXTWO);
			//scale bounding circle radius
			//model[0]->update();
			dir[0] = vector{FXMON,0,0,FXONE};
			break;
		case 0:
			model[1] = new object(*n);

			pos[1] = (*model[0]->docktype(3,0)) - (*model[1]->docktype(3,0));

			dir[0] = vector{0,FXONE,0,0};
			dir[1] = vector{0,FXONE,0,0};
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

xint entity::update()
{
	xint k = screen::turbo();
	const xint curr = screen::time();

	const bool mat = (angle>=0&&angle<=180) || (angle<=-180&&angle>=-360);

	//checkammo();

	ifu(mhealth<=0)
	{
		model[0]->explode(-FXHLF+FXTNT);
		model[1]->explode(-FXHLF);
		return mhealth--<-250;
	}
	
	static fixed gear = 0;

	k = math::set(0,k,k=='w' && angle==0);
	switch(k)
	{
		case RIGHT:
			model[0]->update(turn[0]);
			model[1]->update(turn[0]);
			dir[0] = turn[1]*dir[0];
			dir[1] = turn[1]*dir[1];
		break;

		case LEFT:
			model[0]->update(turn[1]);
			model[1]->update(turn[1]);
			dir[0] = turn[0]*dir[0];
			dir[1] = turn[0]*dir[1];
		break;

		case UP:
			gear = math::set(0,FXMON,gear==FXONE);
		break;

		case DOWN:
			gear = math::set(0,FXONE,gear==FXMON);
		break;

		case 'a':
			model[1]->update(turn[1]);
			angle += ROTANG;
			dir[1] = turn[1]*dir[1];
		break;

		case 'd':
			model[1]->update(turn[0]);
			angle -= ROTANG;
			dir[1] =turn[0]*dir[1];
		break;

		case 'w':
			model[1]->update(turn[mat]);
			angle += math::neg(ROTANG,mat);
			dir[1] = turn[mat]*dir[1];
		break;

		case SPACE:
			for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
			lastfire = math::set(curr+firerate,lastfire,curr>lastfire);
		break;
	}
 
	angle -= math::set(360,angle>=360);
			
	const vector tp = pos[0] + fx::mul(dir[0],gear);

	const fixed r = model[0]->bounding();
	//terrain collision here:
	//terrain(map,tp);
	const bool t = (tp.x-r>=0) && (tp.x+r<=FX(XRES)); //TODO

	pos[0] = tp; //vector{math::set(tp.x,pos[0].x,t),
	             //   math::set(tp.y,pos[0].y,t),
	             //   math::set(tp.z,pos[0].z,t),
	             //   0 };

	lastupdate = curr;

	return mhealth;
}

xint entity::update(bool v)
{
	const xint curr = screen::time();

	ifu(mhealth<=0)
	{
		model[0]->implode(FXCEN);
		return mhealth--<-250;		
	}

	if( v ) //check
	{
		check_ammo();

		for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
		lastfire = math::set(curr+firerate,lastfire,curr>lastfire);

		pos[0].x -= fx::mul(dir[0].x,dir[0].e);
		//pos[0].y += fx::mul(dir[0].y,dir[0].e); //temp
		pos[0].z += fx::mul(dir[0].z,dir[0].e);

		dir[0].x = math::set(-dir[0].x,dir[0].x,pos[0].x<=FX(XRES>>2));
		dir[0].x = math::set(-dir[0].x,dir[0].x,pos[0].x>=FX(XRES-(XRES>>2)));
	}

	lastupdate = curr;
	return mhealth;
}

void entity::display(const vector& p,bool s)
{
	const xint r = math::set(R_B,R_F,s);
	model[0]->display(p,r);
	if(model[1]!=0) { model[1]->display(pos[1]+p,r); }

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

vector entity::position() const
{
	return fx::f2l(pos[0]);
}

void entity::points(xint a)
{
	mpoints += a;
}

xint entity::points() const
{
	return mpoints;
}

xint entity::health() const
{
	return mhealth;
}

xint entity::shield() const
{
	return mshield;
}
///</code>

