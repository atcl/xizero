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
	fvector pos;
	fvector dir;
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
		fvector** _ammomount;
		lvector _towpos;
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
		long _lastfire;

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
		void resume();
		INLINE long health() { return _health; }
		INLINE long shield() { return _shield; }
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

	_ammomount = new fvector*[_ammomounts];
	const long s = (_model[1]!=0);
	for(long i=0,j=0;i+j<_ammomounts;++i)
	{
		fvector* t = _model[0]->docktype(s,i);
		const bool mt = (t==0);
		if(_model[1]!=0 && mt==1) { t = _model[1]->docktype(s,j); j++; i--; }
		_ammomount[i+j] = t;
		_ammomount[i+j]->z = mt;	
	}

	_active = 0;
	_lastupdate = _lastfire = system::clk();
}

void entity::fire(long h,long i)
{
	const bool j = _ammomount[i]->z;
	ammo* cur = new ammo({{_position.x+_ammomount[i]->x,_position.y-_ammomount[i]->y,0,h },{_direction[j].x,-(_direction[j].y),0,fx::div(FXONE,_direction[j].length())<<2}}); 
	_ammo.append(cur);
}

entity::entity(const lvector& p,object* m,object* n,const info& v)
{
	_model[1] = new object(*n);
	setup(p,m,v);

	_towpos = *_model[1]->docktype(3,0)-*_model[0]->docktype(3,0);	
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
	//delete ammomounts
}

long entity::update(long k,long& m)
{
	static long last = 0;
	const bool l = k^last;
	const long curr = system::clk();

	for(long i=_ammo.first();i<_ammo.length();i+=_ammo.next())
	{
		ammo* ca = (ammo*)_ammo.current();
		//const long h = game::collision(_position,_model[0]->boundingbox(),ca->pos,i==0)<<2;
		//if(h!=0) { _ammo.delcurrent(); }
		//health -= h;
		/*if(curr>_lastupdate+ammorate)*/ ca->pos -= ca->dir*ca->dir.e;
	}

	//destroy ani if health below zeros
	if(_health<0)
	{
		
	}
	//

	switch(k)
	{
		case LEFT:
			_model[0]->update(rp);
			_model[1]->update(rp);
			_direction[0] = rp.transform(_direction[0]);
			_direction[1] = rp.transform(_direction[1]);
		break;

		case RIGHT:
			_model[0]->update(rm);
			_model[1]->update(rm);
			_direction[0] = rm.transform(_direction[0]);
			_direction[1] = rm.transform(_direction[1]);
		break;

		case UP:
			_direction[0].e = math::set(fx::l2f(_direction[0].e>=0),_direction[0].e,l==1);
		break;

		case DOWN:
			_direction[0].e = math::set(fx::l2f(-(_direction[0].e<=0)),_direction[0].e,l==1);
		break;

		case 'A':
			_model[1]->update(rp);
			_angle+=ROTANG;
			_direction[1] = rp.transform(_direction[1]);
		break;

		case 'D':
			_model[1]->update(rm);
			_angle-=ROTANG;
			_direction[1] = rm.transform(_direction[1]);
		break;

		case 'W':
			if( (_angle>=0&&_angle<=180) || (_angle<=-180&&_angle>=-360) )
			{ 
				_model[1]->update(rm);
				_direction[1] = rm.transform(_direction[1]);
				_angle -= ROTANG;
			}
			else
			{ 
				_model[1]->update(rp);
				_direction[1] = rp.transform(_direction[1]);
				_angle += ROTANG;
			}
		break;

		case SPACE:
			if(curr>_lastfire)
			{
				for(long i=0;i<_ammomounts;++i) { fire(1,i); }
				_lastfire = curr+_firerate;
			}
		break;
	}

	_angle = _angle%360;
	const fixed py = _position.y;
	_position.x -= fx::mul(_direction[0].x,_direction[0].e);
	_position.y += fx::mul(_direction[0].y,_direction[0].e);
	_position.z += fx::mul(_direction[0].z,_direction[0].e);
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
			//const long h = game::collision(_position,_model[0]->boundingbox(),_ammo.current()->pos,i==0)<<2;
			//if(h!=0) { _ammo.delcurrent(); }
			//health -= h;
		}

		if(curr>_lastfire)
		{
			for(long i=0;i<_ammomounts;++i) { fire(0,i); }
			_lastfire = curr+_firerate;
		}

		_position.x -= fx::mul(_direction[0].x,_direction[0].e);
		_position.y += fx::mul(_direction[0].y,_direction[0].e);
		_position.z += fx::mul(_direction[0].z,_direction[0].e);
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
		_model[1]->display(p+_towpos,r);
		for(long i=_ammo.first();i<_ammo.length()&&r==R_F;i+=_ammo.next())
		{
			const fvector* cur = &((ammo*)_ammo.current())->pos;
			const long cx = fx::r2l(cur->x);
			const long cy = fx::r2l(cur->y)-m;
			switch(game::onscreen(cx,cy))
			{
				case 0: /*delete*/ _ammo.delcurrent(); break;
				case 1: compiled::ammo(cx,cy,BLUE,YELLOW); break; //compiled::ammo(cx,cy,compiled::type[cur->e][0],compiled::[cur->e][1]); break;
			}
		}
	}
}

void entity::resume()
{
	_lastfire = _lastupdate = system::clk();
}
///*

#endif
