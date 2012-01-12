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
		static fmatrix rp;
		static fmatrix rm;
		static list _ammo;

		object* _model[2];
		fvector _position;
		fvector _direction;
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
		INLINE void fire(long i,bool j);
	public:
		entity(const lvector& p,object* m,object* n,const info& v);
		entity(const lvector& p,object* m,const info& v,long s);
		entity(const lvector& p,object* m,const info& v);
		~entity();
		long update(long k,long& m);
		long update(long m);
		void display(long m,bool t);
		void pause();
};
///*

///implementation
fmatrix entity::rp    = fmatrix();
fmatrix entity::rm    = fmatrix();
list    entity::_ammo = list();

void entity::setup(const lvector& p,object* m,const info& v)
{
	_model[0] = new object(*m);
	_model[1] = 0;

	_position = p;
	_direction.set(0,FXONE,0,FXONE);

	_health = _healthmax = string::conl(v["health"]);
	_healthrate = string::conl(v["hrate"]);
	_shield = _shieldmax = string::conl(v["shield"]);
	_shieldrate = string::conl(v["srate"]);
	_ammomounts = string::conl(v["mounts"]);
	_ammotype = string::conl(v["atype"]);
	_firerate = string::conl(v["frate"]);
	_points = string::conl(v["points"]);

	_active = 0;
	_lastupdate = system::clk();
}

void entity::fire(long i,bool j)
{
	//...
	//ammo cur = new ammo();
	//_ammo.append(cur);
}

entity::entity(const lvector& p,object* m,object* n,const info& v)
{
	setup(p,m,v);

	rp.clear();
	rp.rotatez(fx::l2f(ROTANG));
	rm.clear();
	rm.rotatez(fx::l2f(-ROTANG));

	_direction.set(0,FXMON,0,0);
	_active = 1;
	_model[1] = new object(*n);
	_angle = 0;
}

entity::entity(const lvector& p,object* m,const info& v,long s)
{
	setup(p,m,v);
	
	//scale model by s
	s = fx::l2f(s);
	_model[0]->linear().clear(FXONE);
	_model[0]->linear().scale(s,s,s);
	_model[0]->update();
	_model[0]->set();
	//
}

entity::entity(const lvector& p,object* m,const info& v)
{
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
	//for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
	//{
	//	ammo* ca = _ammo.current();
	//	_health -= game::collision(_position,_model[0]->boundingbox(),ca->pos,i==0)<<2;
	//	ca->pos += ca->dir;
	//}

	//destroy ani if health below zeros
	if(_health<0)
	{
		
	}
	//

	switch(k)
	{
		case LEFT:
			_model[0]->linear() = rp;
			_model[0]->update();
			_model[1]->update();
			_model[0]->linear() = rm;
			_direction = _model[0]->linear().transform(_direction);
		break;

		case RIGHT:
			_model[0]->linear() = rm;
			_model[0]->update();
			_model[1]->update();
			_model[0]->linear() = rp;
			_direction = _model[0]->linear().transform(_direction);
		break;

		case UP:
			_direction.e = math::set( fx::l2f(_direction.e>=0),_direction.e,l==1);
		break;

		case DOWN:
			_direction.e = math::set(-fx::l2f(_direction.e<=0),_direction.e,l==1);
		break;

		case 'A':
			_model[1]->linear() = rp;
			_model[1]->update();
			_angle+=ROTANG;
		break;

		case 'D':
			_model[1]->linear() = rm;
			_model[1]->update();
			_angle-=ROTANG;
		break;

		case 'W':
			if(_angle>=0) { _model[1]->linear()=rm; } else { _model[1]->linear() = rp; }
			_model[1]->update();
			_angle+=math::neg(ROTANG,_angle>=0);
		break;

		case SPACE:
			for(long i=0;i<_ammomounts;++i) { fire(i,0); }
		break;
	}

	const fixed py = _position.y;
	_position += _direction * _direction.e;
	m = fx::r2l(fx::mul(PRJY<<FX,fx::div(_position.y - py,_position.z))); //PRJY from polygon

	last = k;

	return _health;
}

long entity::update(long m)
{
	_active |= (_position.y>m || _position.y<m) && (_position.y>0);

	if(_active!=0)
	{
		//for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
		//{
		//	_health -= game::collision(_position,_model[0]->boundingbox(),_ammo.current()->pos,i==0)<<2;
		//}

		for(long i=0;i<_ammomounts;++i) { fire(i,1); }
		_position += _direction * _direction.e;
	}

	if(_health<0)
	{
		
	}

	return _health;
}

void entity::display(long m,bool t)
{
	guard(fx::r2l(_position.y)<m-100&&fx::r2l(_position.y)>m+YRES);

	lvector p(fx::r2l(_position.x),fx::r2l(_position.y),fx::r2l(_position.z));
	p.y -= m;
	const long r = math::set(R_B,R_F,t);
	_model[0]->display(p,r);
	if(_model[1]!=0)
	{
		_model[1]->display(p,r);
		//for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
		//{
		//	const lvector = project(_ammo.current());
		//	compiled::ammo(ca.x,ca.y,YELLOW,RED);
		//}
	}
}

void entity::pause()
{
	//...
}
///*

#endif
