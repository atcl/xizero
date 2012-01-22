// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZentity.hh
// Entity Class 

///guard
#ifndef HH_XZENTITY
#define HH_XZENTITY
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
	lvector pos;
	lvector dir;
};

#define ROTANG 1
///*

///definitions
class entity
{
	private:
		static const fmatrix rp;
		static const fmatrix rm;
		static list _ammo;

		object* _model[2];
		fvector _position;
		fvector _direction[2];
		lvector** _ammomount;
		long _angle;

		bool _active;
		long _lastupdate;

		long _health;
		long _healthmax;
		long _healthrate;
		
		long _shield;
		long _shieldmax;
		long _shieldrate;

		long _ammomounts;
		long _ammotype;
		long _firerate;

		long _points;

		INLINE void setup(const lvector& p,object* m,const info& v);
		INLINE void fire(long h,long i);
	public:
		entity(const lvector& p,object* m,object* n,const info& v);
		entity(const lvector& p,object* m,const info& v,long s);
		entity(const lvector& p,object* m,const info& v);
		~entity();
		long update(long k,long& m);
		long update(long m);
		void display(long m,bool t);
		void unpause();
};
///*

///implementation
const fmatrix entity::rp    = []()->fmatrix { fmatrix m; m.rotatez(fx::l2f(ROTANG)); return m; }();
const fmatrix entity::rm    = []()->fmatrix { fmatrix m; m.rotatez(fx::l2f(-ROTANG)); return m; }();
list          entity::_ammo = list();

void entity::setup(const lvector& p,object* m,const info& v)
{
	_model[0] = new object(*m);

	_position = p;
	_direction[0].set(0,FXONE,0,FXONE);

	_health = _healthmax = string::conl(v["health"]);
	_healthrate = string::conl(v["hrate"]);
	_shield = _shieldmax = string::conl(v["shield"]);
	_shieldrate = string::conl(v["srate"]);
	_ammomounts = string::conl(v["mounts"]);
	_ammotype = string::conl(v["atype"]);
	_firerate = string::conl(v["frate"]);
	_points = string::conl(v["points"]);

	_ammomount = new lvector*[_ammomounts];
	const long s = (_model[1]!=0);
	for(long i=0,j=0;i<_ammomounts;++i)
	{
		lvector* t = _model[0]->docktype(s,i); t->e=0;
		if(t==0) { t = _model[1]->docktype(s,j); t->e=1; j++; }
		_ammomount[i] = t;		
	}

	_active = 0;
	_lastupdate = system::clk();
}

void entity::fire(long h,long i)
{
	const bool j = _ammomount[i]->e;
	ammo* cur = new ammo({{fx::f2l(_position.x)+_ammomount[i]->x,fx::f2l(_position.y)+_ammomount[i]->y,fx::f2l(_position.z)+_ammomount[i]->z,h },{fx::f2l(_direction[j].x),fx::f2l(_direction[j].y),fx::f2l(_direction[j].z)}}); 
	_ammo.append(cur);
}

entity::entity(const lvector& p,object* m,object* n,const info& v)
{
	_model[1] = new object(*n);
	setup(p,m,v);

	_direction[0].set(0,FXMON,0,0);
	_direction[1].set(0,FXMON,0,0);
	_active = 1;
	_angle = 0;
}

entity::entity(const lvector& p,object* m,const info& v,long s)
{
	_model[1] = 0;
	setup(p,m,v);
	
	//scale model by s
	s = fx::l2f(s);
	object::linear.clear();
	object::linear.scale(s,s,s);
	_model[0]->update();
	//
}

entity::entity(const lvector& p,object* m,const info& v)
{
	_model[1] = 0;
	setup(p,m,v);
}

entity::~entity()
{
	delete _model[0];
	if(_model[1]!=0) { delete _model[1]; }
}

long entity::update(long k,long& m)
{
	static long last = 0;
	const bool l = k^last;
	const long curr = system::clk();

	for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
	{
		ammo* ca = (ammo*)_ammo.current();
		//_health -= game::collision(_position,_model[0]->boundingbox(),ca->pos,i==0)<<2;
		//ca->pos -= ca->dir;
	}

	//destroy ani if health below zeros
	if(_health<0)
	{
		
	}
	//

	switch(k)
	{
		case LEFT:
			object::linear = rp;
			_model[0]->update();
			_model[1]->update();
			object::linear = rm;
			_direction[0] = object::linear.transform(_direction[0]);
			_direction[1] = object::linear.transform(_direction[1]);
		break;

		case RIGHT:
			object::linear = rm;
			_model[0]->update();
			_model[1]->update();
			object::linear = rp;
			_direction[0] = object::linear.transform(_direction[0]);
			_direction[1] = object::linear.transform(_direction[1]);
		break;

		case UP:
			_direction[0].e = math::set( fx::l2f(_direction[0].e>=0),_direction[0].e,l==1);
		break;

		case DOWN:
			_direction[0].e = math::set(-fx::l2f(_direction[0].e<=0),_direction[0].e,l==1);
		break;

		case 'A':
			object::linear = rp;
			_model[1]->update();
			_angle+=ROTANG;
			_direction[1] = object::linear.transform(_direction[1]);
		break;

		case 'D':
			object::linear = rm;
			_model[1]->update();
			_angle-=ROTANG;
			_direction[1] = object::linear.transform(_direction[1]);
		break;

		case 'W':
			if(_angle>=0) { object::linear = rm; } else { object::linear = rp; }
			_model[1]->update();
			_direction[1] = object::linear.transform(_direction[1]);
			_angle+=math::neg(ROTANG,_angle>=0);
		break;

		case SPACE:
			for(long i=0;i<_ammomounts;++i) { fire(1,i); }
		break;
	}

	const fixed py = _position.y;
	_position += _direction[0] * _direction[0].e;
	m = fx::r2l(fx::mul(PRJY<<FX,fx::div(_position.y - py,_position.z))); //PRJY from polygon

	last = k;
	_lastupdate = curr;
	return _health;
}

long entity::update(long m)
{
	const long curr = system::clk();

	_active |= (_position.y>m || _position.y<m) && (_position.y>0);

	if(_active!=0)
	{
		for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
		{
		//	_health -= game::collision(_position,_model[0]->boundingbox(),_ammo.current()->pos,i==0)<<2;
		}

		for(long i=0;i<_ammomounts;++i) { /*fire(0,i);*/ }
		_position += _direction[0] * _direction[0].e;
	}

	if(_health<0)
	{
		
	}

	_lastupdate = curr;
	return _health;
}

void entity::display(long m,bool t)
{
	guard(fx::r2l(_position.y)<m-100&&fx::r2l(_position.y)>m+YRES);

	const lvector p(fx::r2l(_position.x),fx::r2l(_position.y)-m,fx::r2l(_position.z));
	const long r = math::set(R_B,R_F,t);
	_model[0]->display(p,r);
	if(_model[1]!=0)
	{
		_model[1]->display(p,r);
		for(long i=_ammo.first();i<_ammo.length()&&r==R_F;i+=_ammo.next())
		{
			const lvector* cur = &((ammo*)_ammo.current())->pos;
			const long cx = cur->x;
			const long cy = cur->y-m;
			switch(game::onscreen(cx,cy))
			{
				case 0: /*delete*/ _ammo.delcurrent(); break;
				case 1: compiled::ammo(cx,cy,BLUE,YELLOW); break; //compiled::ammo(cx,cy,compiled::type[cur->e][0],compiled::[cur->e][1]); break;
			}
		}
	}
}

void entity::unpause()
{
	_lastupdate = system::clk();
}
///*

#endif
