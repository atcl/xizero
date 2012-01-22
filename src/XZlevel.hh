// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZlevel.hh
// Level Class 

///guard
#ifndef HH_XZLEVEL
#define HH_XZLEVEL
///*

///includes
#include "XZbasic.hh"
#include "XZsystem.hh"
#include "XZstring.hh"
#include "XZlist.hh"
#include "XZobject.hh"
#include "XZentity.hh"
///*

//declarations
#define LWIDTH 40
#define BWIDTH 20
#define BHEIGHT -10
#define CSHIFT (-XRES)>>1
#define GROUND 150
#define AFLOAT 50
///*

///definitions
class level
{
	private:
		object** terrain;
		entity* player;
		entity* boss;
		list enemies;
		long mark;
		long markmin;
		long markmax;

		static long ylevel;
		static long fheight;
	public:
		level(const char* o);
		~level();
		long update(long k);
		void display();
};
///*

///implementation
level::level(const char* o)
{
	//load lvl
	info* lvl = format::ini(o);
	//*

	//load player
	char*   ts = (*lvl)["player"];
	char**  ps = string::split(ts,',');
	object* pm = new object(system::ldf(ps[0]));
	object* pn = new object(system::ldf(ps[1]));
	info*   pi = format::ini(system::ldf(ps[2]));
	//*

	//load boss
	ts         = (*lvl)["boss"];
	char**  bs = string::split(ts,',');
	object* bm = new object(system::ldf(bs[0]));
	info*   bi = format::ini(system::ldf(bs[1]));
	//*

	//load enemy
	ts         = (*lvl)["enemy"];
	char**  es = string::split(ts,',');
	object* em = new object(system::ldf(es[0]));
	info*   ei = format::ini(system::ldf(es[1]));
	//*

	//load map
	ts            = (*lvl)["map"];
	const char* m = system::ldf(ts);
	const long l  = string::count(m,'\n');
	char** t      = string::split(m,'\n');
	//long n        = string::length(t[0]); //=LWIDTH

	markmax = 4*BWIDTH;
	markmin = mark = (l*BWIDTH)-YMAX;

	terrain    = new object*[l];
	lvector* a = new lvector[LWIDTH];
	lvector* b = new lvector[LWIDTH];
	lvector* c = new lvector[LWIDTH];
	lvector* d = new lvector[LWIDTH];
	lvector  e;
	lvector  f;

	for(long i=0,k=0;i<l;++i,k=0)
	{
		//load terrain stripe
		long v = CSHIFT;
		for(long j=1;j<LWIDTH;++j,v+=BWIDTH)		
		{
			a[k].set( v,       -(BWIDTH>>1),(string::toup(t[i+1][j-1])-'A')*BHEIGHT );
			b[k].set( v,         BWIDTH>>1, (string::toup(t[i][j-1])  -'A')*BHEIGHT );
			c[k].set( v+BWIDTH,  BWIDTH>>1, (string::toup(t[i][j])    -'A')*BHEIGHT );
			d[k].set( v+BWIDTH,-(BWIDTH>>1),(string::toup(t[i+1][j])  -'A')*BHEIGHT );

			if(a[k].z==d[k].z && b[k].z==c[k].z)
			{
				j++;
				v+=2*BWIDTH; 
				for(;j<LWIDTH;++j,v+=BWIDTH)
				{
					e.set( v,  BWIDTH>>1, (string::toup(t[i][j])  -'A')*BHEIGHT );
					f.set( v,-(BWIDTH>>1),(string::toup(t[i+1][j])-'A')*BHEIGHT );
					if(c[k].z==e.z && d[k].z==f.z)
					{
						c[k] = e;
						d[k] = f;
					}
					else break;
				}
				v-=2*BWIDTH;
				j--;
			}

			k += (a[k].z!=0 || b[k].z!=0 || c[k].z!=0 || d[k].z!=0);
		}
		//*

		//load entities
		for(long j=0;j<LWIDTH;++j)
		{
			switch(t[i][j] - math::set(62,t[i][j]>='a'))
			{
				case '!':
					boss = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),AFLOAT),bm,(*bi),2);
				break;

				case '"':
					player = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),GROUND),pm,pn,(*pi));
				break;

				case '#':
				case '$':
				case '%':
				case '&':
				case '\'':
				case '(':
					entity* enemy = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),AFLOAT),em,(*ei));
					enemies.append(enemy);
				break;
			}
		}
		//*

		terrain[i] = new object(a,b,c,d,k,OCHER);
	}
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
}

level::~level()
{
	//for(long i=enemies.first();i<enemies.length();i+=enemies.next())
	//{
	//	delete enemies.current(); 
	//	enemies.delcurrent();
	//}
	//delete boss
	//delete player
	//delete[] terrain
}

long level::update(long k)
{
	if(UNLIKELY(k==ESCAPE))
	{
		//show gui 
		//unpause all entities
	}

	for(long i=enemies.first();i<enemies.length();i+=enemies.next())
	{
		if(((entity*)enemies.current())->update(mark)<=0) { delete (entity*)enemies.delcurrent(); }
	}

	static long n = 0;
	long m = 0;
	const long r = (boss->update(mark)<=0)-(player->update(k,m)<=0);
	n = math::set(m+n,mark==markmax);
	mark = math::set(mark,markmin,mark<markmin);
	mark = math::set(mark,markmax,mark>markmax);
	mark+= math::set(m, (mark!=markmax)||(n>markmax-100) ); 
	return r;
}

void level::display()
{
	//draw background
	screen::back.clear(DRED);
	screen::depth.clear(fx::l2f(200));
	//*

	//render terrain
	const lvector p(400,300-BWIDTH-mark%BWIDTH,GROUND);
	const fixed yd = fx::l2f(-20);
	object::linear.clear();
	object::linear.translate(0,fx::l2f(300),0);
	long r = math::max((mark/BWIDTH)-3,0);
	for(long i=0;i<34;++i,++r)
	{
		terrain[r]->update();
		terrain[r]->display(p,R_F);
		terrain[r]->reset();
		object::linear.translate(0,yd,0);
	}
	//*

	//render shadows
	for(long i=enemies.first();i<enemies.length();i+=enemies.next()) { ((entity*)enemies.current())->display(mark,1); }
	boss->display(mark,1);
	player->display(mark,1);
	//*

	//render entities
	for(long i=enemies.first();i<enemies.length();i+=enemies.next()) { ((entity*)enemies.current())->display(mark,0); }
	boss->display(mark,0);
	player->display(mark,0);
	//*

	//render gui elements
	//for(long i=enemies.first();i<enemies.length();i+=enemies.next()) { ... }
	//
	//
	//*
}
///*

#endif

