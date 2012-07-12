// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZentity.hh
// Entity Class 

///guard
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ymark as reference"
///*

///includes
#include "XZbasic.hh"
#include "XZvector.hh"
#include "XZlist.hh"
#include "XZformat.hh"
#include "XZobject.hh"
#include "XZgame.hh"
#include "XZcompiled.hh"
///*

//declarations
struct ammo
{
	fvector pos;
	fvector dir;
};

#define ROTANG 2
///*

///definitions
class entity
{
	private:
		static const fmatrix rot[2];
		static const fmatrix exp[2];
		static list  ammos[2];
		static fixed ymark;

		object* model[2];
		fvector position;
		fvector direction[2];
		lvector towpos;
		sint    angle;

		bool type;
		sint lastupdate;
		sint lastfire;

		sint health;
		sint shield;
		sint shieldmax;
		sint shieldrate;
		sint ammomounts;
		sint ammotype;
		sint firerate;
		sint points;
		fvector** ammomount;

		void fire(sint i);
		void checkammo();
		entity(const entity& e);
		entity& operator=(const entity& e);
	public:
		entity(const lvector& p,const info& v,object* m,object* n,sint s);
		~entity();
		sint update(sint k,sint j,fixed m,fixed n);
		sint update();
		void display(sint m,bool t);
		inline void resume();
		inline void addpoints(sint a);
		inline lvector data(sint m) const;
		inline static sint ylevel();
};
///*

///implementation
const fmatrix entity::rot[2]   = { []()->fmatrix { fmatrix m; m.rotatez(FX(ROTANG)); return m; }(),[]()->fmatrix { fmatrix m; m.rotatez(FX(-ROTANG)); return m; }() };
const fmatrix entity::exp[2]   = { []()->fmatrix { fmatrix m; m.dyadic(fvector(FXHLF,FXQRT,FXHLF),fvector(FXHLF,FXHLF,FXHLF)); return m; }(),[]()->fmatrix { fmatrix m; m.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); return m; }() };
list          entity::ammos[2] = { list(), list() };
fixed         entity::ymark    = 0;

void entity::fire(sint i)
{
	const bool j = (ammomount[i]->z)||(type!=0);
	ammo* cur = new ammo{ fvector(position.x+ammomount[i]->x,position.y-ammomount[i]->y,0,0 ),fvector(direction[j].x,-(direction[j].y),0,(FXONE<<2)) }; 
	ammos[type].append(cur);
}

void entity::checkammo()
{
	list& a = ammos[!type];
	for(a.first();a.notlast();a.next())
	{
		const sint h = game::collision(position,((ammo*)a.current())->pos,model[0]->bounding())<<2;
		ifu(h!=0) { delete (ammo*)a.delcurrent(); health = math::max(0,health-h); }
	}
}

entity::entity(const lvector& p,const info& v,object* m,object* n,sint s)
 : model{new object(*m),0},
   position(p),
   direction{ fvector(0,FXONE,0,FXONE),fvector(0,FXONE,0,FXONE) },
   towpos(),
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

	const sint z = (model[1]!=0);
	for(sint i=0,j=0;i+j<ammomounts;++i)
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

sint entity::update(sint k,sint j,fixed m,fixed n)
{
	static sint last = 0;
	const bool l = k^last;
	const sint curr = screen::time();

	const bool mat = (angle>=0&&angle<=180) || (angle<=-180&&angle>=-360);

	//checkammo();

	ifu(health==0)
	{
		static sint dm = 0;
		model[0]->pull(-FXHLF+FXTNT);
		model[1]->pull(-FXHLF);
		return health-(dm++>250);
	}

	switch(k)
	{
		case RIGHT:
			model[0]->update(rot[0]);
			model[1]->update(rot[0]);
			direction[0] = rot[0].transform(direction[0]);
			direction[1] = rot[0].transform(direction[1]);
		break;

		case LEFT:
			model[0]->update(rot[1]);
			model[1]->update(rot[1]);
			direction[0] = rot[1].transform(direction[0]);
			direction[1] = rot[1].transform(direction[1]);
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
			direction[1] = rot[0].transform(direction[1]);
		break;

		case 'd':
			model[1]->update(rot[1]);
			angle -= ROTANG;
			direction[1] = rot[1].transform(direction[1]);
		break;

		case 'w':
			model[1]->update(rot[mat]);
			angle += math::neg(ROTANG,mat);
			direction[1] = rot[mat].transform(direction[1]);
		break;

		case SPACE:
			for(sint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
			lastfire = math::set(curr+firerate,lastfire,curr>lastfire);
		break;
	}
 
	angle -= math::set(360,angle>=360);

	const fvector tp(position.x - fx::mul(direction[0].x,direction[0].e),position.y + fx::mul(direction[0].y,direction[0].e),position.z + fx::mul(direction[0].z,direction[0].e));
	//terrain collision here
	const fixed r = model[0]->bounding();
	const bool t = (tp.x-r>=0)&&(tp.x+r<=FX(XRES))&&((tp.y-r>=m)&&(tp.y+r<=n));
	position.x = math::set(tp.x,position.x,t);
	position.y = math::set(tp.y,position.y,t);
	position.z = math::set(tp.z,position.z,t); 

	ymark  = position.y;

	last = k;
	lastupdate = curr;
	return health;
}

sint entity::update()
{
	const sint curr = screen::time();

	ifu(health==0) //todo: dyadic explosion
	{
		static sint dm = 0;
		model[0]->update(exp[dm!=0],dm==0);
		return health-(dm++>250);		
	}

	if( (health>0) && (position.y>0) && ((position.y+FX(YRES))>ymark) ) //check
	{
		checkammo();

		for(sint i=0;i<ammomounts&&curr>lastfire;++i) { fire(i); }
		lastfire = math::set(curr+firerate,lastfire,curr>lastfire);

		position.x -= fx::mul(direction[0].x,direction[0].e);
		//position.y += fx::mul(direction[0].y,direction[0].e); //temp
		position.z += fx::mul(direction[0].z,direction[0].e);

		direction[0].x = math::set(-direction[0].x,direction[0].x,position.x<=FX(150));
		direction[0].x = math::set(-direction[0].x,direction[0].x,position.x>=FX(650));
	}

	lastupdate = curr;
	return health;
}

void entity::display(sint m,bool t)
{
	guard(fx::r2l(position.y)<m-100&&fx::r2l(position.y)>m+YRES);

	const lvector p(fx::r2l(position.x),fx::r2l(position.y)-m,fx::r2l(position.z));
	const sint r = math::set(R_B,R_F,t);
	model[0]->display(p,r);
	if(model[1]!=0)
	{
		model[1]->display(p+towpos,r);
		for(sint h=0;h<2&&r!=R_B;++h)
		{
			list& a = ammos[h];
			for(a.first();a.notlast();a.next())
			{
				const fvector& dir = ((ammo*)a.current())->dir;
				const fvector& cur = ((ammo*)a.current())->pos -= dir * dir.e;

				const sint cx = fx::r2l(cur.x);
				const sint cy = fx::r2l(cur.y)-m;
				switch(game::onscreen(cx,cy)<<h)
				{
					case 0: delete (ammo*)a.delcurrent(); break;
					case 1: compiled::ammo(cx,cy,BLUE,YELLOW); break;
					case 2: compiled::ammo(cx,cy,GREEN,ORANGE); break;
				}
			}
		}
	}

const fixed y(model[0]->bounding());
fvector q(p);
gfx::rect(fx::f2l(q.x-y),fx::f2l(q.y-y),fx::f2l(q.x+y),fx::f2l(q.y+y),BLUE,0,0,0);
}

void entity::resume()
{
	lastfire = lastupdate = screen::time();
}

void entity::addpoints(sint a)
{
	points += a;
}

lvector entity::data(sint m) const
{
	return lvector(fx::r2l(position.x),fx::r2l(position.y)-m,health,shield);
}

sint entity::ylevel()
{
	return fx::r2l(ymark);
}
///*

