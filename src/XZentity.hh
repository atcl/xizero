// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZentity.hh
// Entity Class 

///guard
#ifndef HH_XZENTITY
#define HH_XZENTITY
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
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
		static list _ammo[2];

		object* _model[2];
		fvector _position;
		fvector _direction[2];
		fvector** _ammomount;
		lvector _towpos;
		sint    _angle;

		bool _unique;
		bool _active;
		sint _lastupdate;

		sint _health;
		
		sint _shield;
		sint _shieldmax;
		sint _shieldrate;

		sint _ammomounts;
		sint _ammotype;
		sint _firerate;
		sint _lastfire;

		sint _points;

		void setup(const lvector& p,object* m,const info& v);
		void fire(sint h,sint i);
		void checkammo(sint t,bool j);
	public:
		entity(const lvector& p,object* m,object* n,const info& v);
		entity(const lvector& p,object* m,const info& v,sint s);
		entity(const lvector& p,object* m,const info& v);
		~entity();
		sint update(sint k,sint j);
		sint update();
		void display(sint m,bool t);
		inline void resume();
		inline void addpoints(sint a);
		inline lvector data(sint m) const;

		static sint ylevel;
		static fixed ymark;
};
///*

///implementation
const fmatrix entity::rp    = []()->fmatrix { fmatrix m; m.rotatez(fx::l2f(ROTANG)); return m; }();
const fmatrix entity::rm    = []()->fmatrix { fmatrix m; m.rotatez(fx::l2f(-ROTANG)); return m; }();
list          entity::_ammo[2] = { list(), list() };
sint          entity::ylevel = 0;
fixed         entity::ymark  = 0;

void entity::setup(const lvector& p,object* m,const info& v)
{
	_model[0] = new object(*m);

	_position = p;
	_direction[0].set(0,FXONE,0,FXONE);
	_direction[1].set(0,FXONE,0,FXONE);

	_health = string::str2int(v["health"]);
	_shield = _shieldmax = string::str2int(v["shield"]);
	_shieldrate = string::str2int(v["srate"]);
	_ammomounts = string::str2int(v["mounts"]);
	_ammotype = string::str2int(v["atype"]);
	_firerate = string::str2int(v["frate"]);
	_points = string::str2int(v["points"]);

	_ammomount = new fvector*[_ammomounts];
	const sint s = (_model[1]!=0);
	for(sint i=0,j=0;i+j<_ammomounts;++i)
	{
		fvector* t = _model[0]->docktype(s,i);
		const bool mt = (t==0);
		if(_model[1]!=0 && mt==1) { t = _model[1]->docktype(s,j); j++; i--; }
		_ammomount[i+j] = t;
		_ammomount[i+j]->z = mt;	
	}

	_active = 0;
	_lastupdate = _lastfire = screen::time();
}

void entity::fire(sint h,sint i)
{
	const bool j = _ammomount[i]->z;
	ammo* cur = new ammo({{_position.x+_ammomount[i]->x,_position.y-_ammomount[i]->y,0,h },{_direction[j].x,-(_direction[j].y),0,(FXONE<<2)}}); 
	//cur->pos -= (cur->dir*cur->dir.e);
	_ammo[(_model[1]!=0)].append(cur);
}

void entity::checkammo(sint t,bool j)
{
	for(sint i=_ammo[j].first();i<_ammo[j].length();i+=_ammo[j].next())
	{
		ammo* ca = (ammo*)_ammo[j].current();
		const long h = game::collision(_position,_model[0]->boundingbox(),ca->pos,i==0)<<2;
		if(h!=0) { delete (ammo*)_ammo[j].delcurrent(); }
		//_ammo[j].prefn();
		_health = math::max(0,_health-h);
	}
}

entity::entity(const lvector& p,object* m,object* n,const info& v)
{
	_model[1] = new object(*n);
	_unique = 1;
	setup(p,m,v);

	_towpos = *_model[1]->docktype(3,0)-*_model[0]->docktype(3,0);	
	_direction[0].set(0,FXMON,0,0);
	_direction[1].set(0,FXMON,0,0);
	_active = 1;
	_angle = 0;
}

entity::entity(const lvector& p,object* m,const info& v,sint s)
{
	_model[1] = 0;
	_unique = 1;
	setup(p,m,v);
	_direction[1].set(FXMON,0,0,FXONE);
	
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
	_unique = 0;
	setup(p,m,v);
}

entity::~entity()
{
	delete _model[0];
	if(_model[1]!=0) { delete _model[1]; }
	//delete[] _ammomount;
}

sint entity::update(sint k,sint j)
{
	static sint last = 0;
	const bool l = k^last;
	const sint curr = screen::time();

	checkammo(curr,0);

	if(_health==0)
	{
		static sint dm = 0;
		_model[0]->pull(-FXHLF+FXTNT);
		_model[1]->pull(-FXHLF);
		return _health-(dm++>250);
	}

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
				for(sint i=0;i<_ammomounts;++i) { fire(1,i); }
				_lastfire = curr+_firerate;
			}
		break;
	}
 
	_angle -= math::set(360,_angle>=360);

	const fvector temp(_position.x-fx::mul(_direction[0].x,_direction[0].e),_position.y+fx::mul(_direction[0].y,_direction[0].e),_position.z+fx::mul(_direction[0].z,_direction[0].e)); 
	_position = temp * FXONE; //math::set(FXONE,0,(temp.x<=0||temp.x>=XRES)||(temp.y<=0||temp.y>=m));
	ymark  = _position.y;
	ylevel = fx::r2l(fx::mul(PRJY<<FX,fx::div(_position.y,_position.z))); //PRJY from polygon

	last = k;
	_lastupdate = curr;
	return _health;
}

sint entity::update() //check, because ammo is in list even though level just started
{
	const sint curr = screen::time();

	if( (_health>0) && (_position.y>0) && (_position.y+(YRES<<FX)>ymark) ) //check
	{
		//_active = 1;
		checkammo(curr,1);

		if(curr>_lastfire)
		{
			for(sint i=0;i<_ammomounts;++i) { fire(0,i); }
			_lastfire = curr+_firerate;
		}

		_position.x -= fx::mul(_direction[1].x,_direction[1].e);
		_position.y += fx::mul(_direction[1].y,_direction[1].e);
		_position.z += fx::mul(_direction[1].z,_direction[1].e);

		_direction[1].x = math::set(-_direction[1].x,_direction[1].x,_position.x<=fx::l2f(150));
		_direction[1].x = math::set(-_direction[1].x,_direction[1].x,_position.x>=fx::l2f(650));
	}

	if(_health==0)
	{
		
	}

	_lastupdate = curr;
	return _health;
}

void entity::display(sint m,bool t)
{
	guard(fx::r2l(_position.y)<m-100&&fx::r2l(_position.y)>m+YRES);

	const lvector p(fx::r2l(_position.x),fx::r2l(_position.y)-m,fx::r2l(_position.z));
	const sint r = math::set(R_B,R_F,t);
	_model[0]->display(p,r);
	if(_model[1]!=0)
	{
		_model[1]->display(p+_towpos,r);
		for(sint h=0;h<2;++h)
		{
			for(sint i=_ammo[h].first();i<_ammo[h].length()&&r==R_F;i+=_ammo[h].next())
			{
				fvector* cur = &((ammo*)_ammo[h].current())->pos;
				const fvector* dir = &((ammo*)_ammo[h].current())->dir;
				(*cur) -= (*dir) * dir->e;

				const sint cx = fx::r2l(cur->x);
				const sint cy = fx::r2l(cur->y)-m;
				switch(game::onscreen(cx,cy))
				{
					case 0: delete (ammo*)_ammo[h].delcurrent(); break;
					case 1: compiled::ammo(cx,cy,BLUE,YELLOW); break;
				}
			}
		}
	}

	//temp: draw 2d bounding box
	const lvector b0 = polygon::project(p,_model[0]->boundingbox()[0]);
	const lvector b1 = polygon::project(p,_model[0]->boundingbox()[1]);
	const lvector b2 = polygon::project(p,_model[0]->boundingbox()[2]);
	const lvector b3 = polygon::project(p,_model[0]->boundingbox()[3]);
	gfx::line(b0.x,b0.y,b1.x,b1.y,GREEN);
	gfx::line(b1.x,b1.y,b2.x,b2.y,GREEN);
	gfx::line(b2.x,b2.y,b3.x,b3.y,GREEN);
	gfx::line(b3.x,b3.y,b0.x,b0.y,GREEN);
	//*
}

void entity::resume()
{
	_lastfire = _lastupdate = screen::time();
}

void entity::addpoints(sint a)
{
	_points += a;
}

lvector entity::data(sint m) const
{
	return lvector(fx::r2l(_position.x),fx::r2l(_position.y)-m,_health,_shield);
}
///*

#endif
