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
		static const fmatrix rot[2];
		static const fmatrix exp[2];
		static list _ammo[2];
		static fixed ymark;

		object* _model[2];
		fvector _position;
		fvector _direction[2];
		fvector** _ammomount;
		lvector _towpos;
		sint    _angle;

		sint _type;
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
		void fire(sint i);
		void checkammo();
	public:
		entity(const lvector& p,object* m,object* n,const info& v);
		entity(const lvector& p,object* m,const info& v,sint s=1);
		~entity();
		sint update(sint k,sint j);
		sint update();
		void display(sint m,bool t);
		inline void resume();
		inline void addpoints(sint a);
		inline lvector data(sint m) const;

		static sint ylevel;
};
///*

///implementation
const fmatrix entity::rot[2]   = { []()->fmatrix { fmatrix m; m.rotatez(fx::l2f(ROTANG)); return m; }(),[]()->fmatrix { fmatrix m; m.rotatez(fx::l2f(-ROTANG)); return m; }() };
const fmatrix entity::exp[2]   = { []()->fmatrix { fmatrix m; m.dyadic(fvector(FXTWO,FXONE,FXHLF),fvector(FXHLF,FXHLF,FXHLF)); return m; }(),[]()->fmatrix { fmatrix m; m.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); return m; }() };
list          entity::_ammo[2] = { list(), list() };
fixed         entity::ymark  = 0;
sint          entity::ylevel = 0;


void entity::setup(const lvector& p,object* m,const info& v)
{
	_model[0] = new object(*m);

	_position = p;
	_direction[0].set(math::set(FXMON,_type==-1),math::neg(math::set(FXONE,_type!=-1),_type==0),0,FXONE);
	_direction[1].set(0,FXONE,0,FXONE);

	_health     = string::str2int(v["health"]);
	_shieldmax  = string::str2int(v["shield"]);
	_shieldrate = string::str2int(v["srate"]);
	_ammomounts = string::str2int(v["mounts"]);
	_ammotype   = string::str2int(v["atype"]);
	_firerate   = string::str2int(v["frate"]);
	_points     = string::str2int(v["points"]);
	_shield     = _shieldmax;

	_ammomount = new fvector*[_ammomounts];
	const sint s = (_model[1]!=0);
	for(sint i=0,j=0;i+j<_ammomounts;++i)
	{
		fvector* t = _model[0]->docktype(s,i);
		const bool mt = (t==0);
		if(_model[1]!=0 && mt==1) { t = _model[1]->docktype(s,j); ++j; --i; }
		_ammomount[i+j] = t;
		_ammomount[i+j]->z = mt;	
	}

	_lastupdate = _lastfire = screen::time();
}

void entity::fire(sint i)
{
	const bool j = (_ammomount[i]->z)||(_type!=0);
	ammo* cur = new ammo({{_position.x+_ammomount[i]->x,_position.y-_ammomount[i]->y,0,0 },{_direction[j].x,-(_direction[j].y),0,(FXONE<<2)}}); 
	_ammo[(bool)_type].append(cur);
}

void entity::checkammo()
{
	list& a = _ammo[!(bool)_type];
	for(a.first();a.notlast();a.next())
	{
		const sint h = game::collision(_position,_model[0]->bounding(),((ammo*)a.current())->pos)<<2;
		if(h!=0) { delete (ammo*)a.delcurrent(); _health = math::max(0,_health-h); }
	}
}

entity::entity(const lvector& p,object* m,object* n,const info& v)
{
	_model[1] = new object(*n);
	_type = 0;
	setup(p,m,v);

	_towpos = *_model[1]->docktype(3,0)-*_model[0]->docktype(3,0);	
	_direction[0].set(0,FXMON,0,0);
	_direction[1].set(0,FXMON,0,0);
	_angle = 0;
	ymark = _position.y;
}

entity::entity(const lvector& p,object* m,const info& v,sint s)
{
	_model[1] = 0;
	_type = math::neg(1,s!=1);
	setup(p,m,v);
	
	//scale model by s
	s = fx::l2f(s);
	object::linear.clear();
	object::linear.scale(s,s,s);
	//scale bounding circle radius
	_model[0]->update();
	//*
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

	const bool mat = (_angle>=0&&_angle<=180) || (_angle<=-180&&_angle>=-360);

	//checkammo();

	ifu(_health==0)
	{
		static sint dm = 0;
		_model[0]->pull(-FXHLF+FXTNT);
		_model[1]->pull(-FXHLF);
		return _health-(dm++>250);
	}

	switch(k)
	{
		case RIGHT:
			_model[0]->update(rot[0]);
			_model[1]->update(rot[0]);
			_direction[0] = rot[0].transform(_direction[0]);
			_direction[1] = rot[0].transform(_direction[1]);
		break;

		case LEFT:
			_model[0]->update(rot[1]);
			_model[1]->update(rot[1]);
			_direction[0] = rot[1].transform(_direction[0]);
			_direction[1] = rot[1].transform(_direction[1]);
		break;

		case UP:
			_direction[0].e = math::set(fx::l2f(_direction[0].e>=0),_direction[0].e,l==1);
		break;

		case DOWN:
			_direction[0].e = math::set(fx::l2f(-(_direction[0].e<=0)),_direction[0].e,l==1);
		break;

		case 'a':
			_model[1]->update(rot[0]);
			_angle += ROTANG;
			_direction[1] = rot[0].transform(_direction[1]);
		break;

		case 'd':
			_model[1]->update(rot[1]);
			_angle -= ROTANG;
			_direction[1] = rot[1].transform(_direction[1]);
		break;

		case 'w':
			_model[1]->update(rot[mat]);
			_angle += math::neg(ROTANG,mat);
			_direction[1] = rot[mat].transform(_direction[1]);
		break;

		case SPACE:
			for(sint i=0;i<_ammomounts&&curr>_lastfire;++i) { fire(i); }
			_lastfire = math::set(curr+_firerate,_lastfire,curr>_lastfire);
		break;
	}
 
	_angle -= math::set(360,_angle>=360);

	const fvector tp(_position.x - fx::mul(_direction[0].x,_direction[0].e),_position.y + fx::mul(_direction[0].y,_direction[0].e),_position.z + fx::mul(_direction[0].z,_direction[0].e));
	//terrain collision here
	const bool t = (tp.x>=0)&&(tp.x<=(XRES<<FX));//||(ty<=0||temp.y>=m);
	_position.x = math::set(tp.x,_position.x,t);
	_position.y = math::set(tp.y,_position.y,t);
	_position.z = math::set(tp.z,_position.z,t); 

	ymark  = _position.y;
	ylevel = fx::r2l(fx::mul(PRJY<<FX,fx::div(_position.y,_position.z))); //PRJY from polygon

	last = k;
	_lastupdate = curr;
	return _health;
}

sint entity::update()
{
	const sint curr = screen::time();

	ifu(_health==0) //todo: dyadic explosion
	{
		static sint dm = 0;
		_model[0]->update(exp[dm!=0],dm==0);
		return _health-(dm++>250);		
	}

	if( (_health>0) && (_position.y>0) && (_position.y+(YRES<<FX)>ymark) ) //check
	{
		checkammo();

		for(sint i=0;i<_ammomounts&&curr>_lastfire;++i) { fire(i); }
		_lastfire = math::set(curr+_firerate,_lastfire,curr>_lastfire);

		_position.x -= fx::mul(_direction[0].x,_direction[0].e);
		//_position.y += fx::mul(_direction[0].y,_direction[0].e);
		_position.z += fx::mul(_direction[0].z,_direction[0].e);

		_direction[0].x = math::set(-_direction[0].x,_direction[0].x,_position.x<=fx::l2f(150));
		_direction[0].x = math::set(-_direction[0].x,_direction[0].x,_position.x>=fx::l2f(650));
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
		for(sint h=0;h<2&&r!=R_B;++h)
		{
			list& a = _ammo[h];
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
/*fvector q;
q = p;
fvector y(_model[0]->bounding());
gfx::rect(fx::f2l(q.x+y.x-y.e),fx::f2l(q.y+y.y-y.e),fx::f2l(q.x+y.x+y.e),fx::f2l(q.y+y.y+y.e),BLUE,0,0,0);*/
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
