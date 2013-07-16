///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZentity.hh
// Entity Class 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZfixed.hh"
#include "XZvector.hh"
#include "XZlist.hh"
#include "XZformat.hh"
#include "XZobject.hh"
#include "XZgame.hh"
///</include>

//<declare>
#define ROTANG   2
#define BWIDTH  16
#define MAXSTEP  5
///</declare>

///<define>
struct ammo
{
	//ammo(const fvector& p,const fvector& d) : pos(p),dir(d) { ; }
	fvector pos;
	fvector dir;
};

class entity
{
	private:
		static const fmatrix rot[2];
		static const fmatrix exp;
		static list<ammo> ammos[2];
		static fixed ymark;

		object* model[2];
		fvector position;
		fvector direction[2];
		lvector towpos;
		xint    angle;

		/*const*/ bool type;
		xint lastupdate;
		xint lastfire;

		xint death; //remove?
		xint health;
		xint shield;
		/*const*/ xint shieldmax;
		/*const*/ xint shieldrate;
		/*const*/ xint ammomounts;
		/*const*/ xint ammotype;
		/*const*/ xint firerate;
		xint points;
		fvector** ammomount;

		void fire(xint i);
		void checkammo();
		xint terrain(const char** m,fvector& n);
		entity(const entity& e);
		entity& operator=(const entity& e);
	public:
		entity(const lvector& p,const info& v,object* m,object* n,xint s);
		~entity();
		xint update(xint k,xint j,fixed m,fixed n);
		xint update();
		void display(xint m,bool t);
		inline void resume();
		inline void addpoints(xint a);
		inline lvector data(xint m) const;
		inline static xint ylevel();
};
///</define>

///<code>
const fmatrix entity::rot[2]   = { []()->fmatrix { fmatrix m; m.rotatez(FX(-ROTANG)); return m; }(),[]()->fmatrix { fmatrix m; m.rotatez(FX(ROTANG)); return m; }() };
const fmatrix entity::exp      = []()->fmatrix { fmatrix m; m.scale(FXONE-FXCEN,FXONE-FXCEN,FXONE-FXCEN); return m; }();
list<ammo>    entity::ammos[2] = { list<ammo>(), list<ammo>() };
fixed         entity::ymark    = 0;

void entity::fire(xint i)
{
	const bool j = (ammomount[i]->z)||(type!=0);
	ammo* cur = new ammo{ fvector(position.x+ammomount[i]->x,position.y-ammomount[i]->y,0,0 ),fvector(direction[j].x,-(direction[j].y),0,(FXONE<<2)) }; 
	ammos[type].append(cur);
}

void entity::checkammo()
{
	list<ammo>& a = ammos[!type];
	for(a.first();a.notlast();a.next())
	{
		const xint h = model[0]->collision(position,a.current()->pos)*4;
		ifu(h!=0) { delete a.delcurrent(); health = math::max(0,health-h); }
	}
}

xint entity::terrain(const char** m,fvector& n) //TODO actual angle computation
{
	const fixed r = model[0]->bounding();

	const yint l0 = fx::f2l(position.x+r)/BWIDTH;
	const yint r0 = fx::f2l(position.x-r)/BWIDTH;
	const yint u0 = fx::f2l(position.y+r)/BWIDTH;
	const yint d0 = fx::f2l(position.y-r)/BWIDTH;

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

entity::entity(const lvector& p,const info& v,object* m,object* n,xint s)
 : model{new object(*m),0},
   position(p),
   direction{ fvector(0,FXONE,0,FXONE),fvector(0,FXONE,0,FXONE) },
   towpos(),
   angle(0),
   type(s),
   lastupdate(screen::time()),
   lastfire(screen::time()),
   death(0),
   health(string::str2int(v["health"])),
   shield(string::str2int(v["shield"])),
   shieldmax(string::str2int(v["shield"])),
   shieldrate(string::str2int(v["srate"])),
   ammomounts(string::str2int(v["mounts"])),
   ammotype(string::str2int(v["atype"])),
   firerate(string::str2int(v["frate"])),
   points(string::str2int(v["points"])),
   ammomount(new fvector*[ammomounts])
{
	switch(s)
	{
		case 2:
			object::linear.clear();
			object::linear.scale(FXTWO,FXTWO,FXTWO);
			//scale bounding circle radius
			model[0]->update();
			direction[0].set(FXMON,0,0,FXONE);
			break;
		case 0:
			model[1] = new object(*n);
			ymark = position.y;

			towpos = *model[1]->docktype(3,0)-*model[0]->docktype(3,0);	
			direction[0].set(0,FXMON,0,0);
			direction[1].set(0,FXMON,0,0);
	}

	const xint z = (model[1]!=0);
	for(xint i=0,j=0;i+j<ammomounts;++i)
	{
		fvector* t = model[0]->docktype(z,i);
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

	ifu(health==0)
	{
		model[0]->pull(-FXHLF+FXTNT);
		model[1]->pull(-FXHLF);
		return health-(death++>250);
	}

	k = math::set(k,k!='w'||angle!=0);
	switch(k)
	{
		case RIGHT:
			model[0]->update(rot[0]);
			model[1]->update(rot[0]);
			direction[0] = rot[0]*direction[0];
			direction[1] = rot[0]*direction[1];
		break;

		case LEFT:
			model[0]->update(rot[1]);
			model[1]->update(rot[1]);
			direction[0] = rot[1]*direction[0];
			direction[1] = rot[1]*direction[1];
		break;

		case UP:
			direction[0].e = math::set(fx::l2f(direction[0].e>=0),direction[0].e,l==1);
		break;

		case DOWN:
			direction[0].e = math::set(fx::l2f(-(direction[0].e<=0)),direction[0].e,l==1);
		break;

		case 'a':
			model[1]->update(rot[0]);
			angle += ROTANG;
			direction[1] = rot[0]*direction[1];
		break;

		case 'd':
			model[1]->update(rot[1]);
			angle -= ROTANG;
			direction[1] =rot[1]*direction[1];
		break;

		case 'w':
			model[1]->update(rot[mat]);
			angle += math::neg(ROTANG,mat);
			direction[1] = rot[mat]*direction[1];
		break;

		case SPACE:
			for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
			lastfire = math::set(curr+firerate,lastfire,curr>lastfire);
		break;
	}
 
	angle -= math::set(360,angle>=360);

	const fvector tp(position.x - fx::mul(direction[0].x,direction[0].e),position.y + fx::mul(direction[0].y,direction[0].e),position.z + fx::mul(direction[0].z,direction[0].e));
	const fixed r = model[0]->bounding();
	//terrain collision here:
	//terrain(map,tp);
	const bool t = (tp.x-r>=0)&&(tp.x+r<=FX(XRES))&&((tp.y-r>=m)&&(tp.y+r<=n)); //TODO
	position.x = math::set(tp.x,position.x,t);
	position.y = math::set(tp.y,position.y,t);
	position.z = math::set(tp.z,position.z,t);

	ymark  = position.y; //TODO: ymark as reference

	last = k;
	lastupdate = curr;
	return health;
}

xint entity::update()
{
	const xint curr = screen::time();

	ifu(health==0)
	{
		model[0]->update(exp,death==0);
		return health-=(death++>250);		
	}

	if( (health>0) && (position.y>0) && ((position.y+FX(YRES))>ymark) ) //check
	{
		checkammo();

		for(xint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
		lastfire = math::set(curr+firerate,lastfire,curr>lastfire);

		position.x -= fx::mul(direction[0].x,direction[0].e);
		//position.y += fx::mul(direction[0].y,direction[0].e); //temp
		position.z += fx::mul(direction[0].z,direction[0].e);

		direction[0].x = math::set(-direction[0].x,direction[0].x,position.x<=FX(XRES>>2));
		direction[0].x = math::set(-direction[0].x,direction[0].x,position.x>=FX(XRES-(XRES>>2)));
	}

	lastupdate = curr;
	return health;
}

void entity::display(xint m,bool t)
{
	guard(fx::r2l(position.y)<m-100&&fx::r2l(position.y)>m+YRES);

	const lvector p(fx::r2l(position.x),fx::r2l(position.y)-m,fx::r2l(position.z));
	const xint r = math::set(R_B,R_F,t);
	model[0]->display(p,r);
	if(model[1]!=0)
	{
		model[1]->display(p+towpos,r);
		for(xint h=0;h<2&&t==0;++h)
		{
			list<ammo>& a = ammos[h];
			for(a.first();a.notlast();a.next())
			{
				const fvector& dir = a.current()->dir;
				const fvector& cur = a.current()->pos -= dir * dir.e;

				const xint cx = fx::r2l(cur.x);
				const xint cy = fx::r2l(cur.y)-m;
				switch( (game::onscreen(cx-4,cy-4)&game::onscreen(cx+4,cy+4))<<h )
				{
					case 0: delete a.delcurrent(); break;
					case 1: game::compiled(cx,cy,BLUE,YELLOW); break;
					case 2: game::compiled(cx,cy,GREEN,ORANGE); break;
				}
			}
		}
	}

//temp
const fixed y(model[0]->bounding());
fvector q(p);
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

lvector entity::data(xint m) const
{
	return lvector(fx::r2l(position.x),fx::r2l(position.y)-m,health,shield);
}

xint entity::ylevel()
{
	return fx::r2l(ymark);
}
///</code>

