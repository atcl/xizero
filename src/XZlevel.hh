///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZlevel.hh
// Level Class 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: Bumpmapped Terrain"
///</header>

///<include>
#include "XZbasic.hh"
#include "XZsystem.hh"
#include "XZstring.hh"
#include "XZlist.hh"
#include "XZobject.hh"
#include "XZentity.hh"
#include "XZprogress.hh"
#include "XZvector.hh"
///</include>

//<declare>
#define LWIDTH 40
#define BWIDTH 20
#define BHEIGHT -10
#define CSHIFT (-XRES)>>1
#define GROUND 150
#define AFLOAT 50
#define OFFSET 4
///</declare>

///<define>
class level
{
	private:
		object** terrain;		//Level Terrain Stripe
		entity* player;			//Player Entity
		entity* boss;			//Boss Entity
		list enemies;			//List of Enemy Entities
		char** map;			//Text Map of Terrain
		sint mark;			//Current Level Position
		sint markmin;			//Lowest Level Position (Bottom)
		const sint markmax;		//Highest Level Position (Top)
		progress* pp;			//Player Health Gauge
		progress* sp;			//Player Shield Gauge
		progress* bp;			//Boss Gauge
		progress* ep;			//Enemy Gauge
		level(const level& l);
		level& operator=(const level& l);
	public:
		level(char* o);			//Constructor
		~level();			//Destructor
		sint update(sint k,sint j);	//Update All Entities
		void display();			//Display Terrain, Shadows, Entities
		void gauges();			//Display Gauges
		void resume();			//Resume After Pausing
		inline lvector ppos();		//get player position
};
///</define>

///<code>
level::level(char* o) : markmax(OFFSET*BWIDTH)
{
	//load lvl
	info* arc = format::ar(o);
	info* lvl = format::ini((*arc)["level0.lvl"]);
	//*

	//load intro message
	const char* tx = (*arc)[(*lvl)["intro"]];
	screen::back.clear(BLACK);
	font::draw(100,100,tx,ORANGE,BLACK);
	screen::_flush();
	//*

	//load player
	object* pm = new object((*arc)[(*lvl)["player_0"]]);
	object* pn = new object((*arc)[(*lvl)["player_1"]]);
	info*   pi = format::ini((*arc)[(*lvl)["player_i"]]);
 	        pp = new progress(0,string::str2int((*pi)["health"]),VER,10,20,20,YRES-40,GREEN,SYSCOL,WHITE,1);
 	        sp = new progress(0,string::str2int((*pi)["shield"]),VER,XRES-30,20,20,YRES-40,BLUE,SYSCOL,WHITE,1);
	gfx::fsprog(10);
	screen::_flush();
	//*

	//load boss
	object* bm = new object((*arc)[(*lvl)["boss_0"]]);
	info*   bi = format::ini((*arc)[(*lvl)["boss_i"]]); 
	        bp = new progress(0,string::str2int((*bi)["health"]),HOR,0,0,100,20,RED,SYSCOL,WHITE,0);
	gfx::fsprog(20);
	screen::_flush();
	//*

	//load enemy
	object* em = new object((*arc)[(*lvl)["enemy_0"]]);
	info*   ei = format::ini((*arc)[(*lvl)["enemy_i"]]);
	        ep = new progress(0,string::str2int((*ei)["health"]),HOR,0,0,50,10,GREEN,SYSCOL,WHITE,0);
	gfx::fsprog(30);
	screen::_flush();
	//*

	//load map
	const char* m = (*arc)[(*lvl)["map"]];
	const sint l  = string::count(m,'\n');
	map           = string::split(m,'\n');
	//long n        = string::length(t[0]); //=LWIDTH
	gfx::fsprog(40);
	screen::_flush();

	markmin = (l*BWIDTH)-YMAX;

	terrain    = new object*[l];
	lvector* a = new lvector[LWIDTH];
	lvector* b = new lvector[LWIDTH];
	lvector* c = new lvector[LWIDTH];
	lvector* d = new lvector[LWIDTH];
	lvector  e;
	lvector  f;

	for(sint i=0,k=0;i<l;++i,k=0)
	{
		//load terrain stripe
		sint v = CSHIFT;
		for(sint j=1;j<LWIDTH;++j,v+=BWIDTH)		
		{
			a[k].set( v,       -(BWIDTH>>1),(string::toup(map[i+1][j-1])-'A')*BHEIGHT );
			b[k].set( v,         BWIDTH>>1, (string::toup(map[i][j-1])  -'A')*BHEIGHT );
			c[k].set( v+BWIDTH,  BWIDTH>>1, (string::toup(map[i][j])    -'A')*BHEIGHT );
			d[k].set( v+BWIDTH,-(BWIDTH>>1),(string::toup(map[i+1][j])  -'A')*BHEIGHT );

			if(a[k].z==d[k].z && b[k].z==c[k].z)
			{
				++j;
				v+=2*BWIDTH; 
				for(;j<LWIDTH;++j,v+=BWIDTH)
				{
					e.set( v,  BWIDTH>>1, (string::toup(map[i][j])  -'A')*BHEIGHT );
					f.set( v,-(BWIDTH>>1),(string::toup(map[i+1][j])-'A')*BHEIGHT );
					if(c[k].z==e.z && d[k].z==f.z)
					{
						c[k] = e;
						d[k] = f;
					}
					else break;
				}
				v-=2*BWIDTH;
				--j;
			}

			k += (a[k].z!=0 || b[k].z!=0 || c[k].z!=0 || d[k].z!=0);
		}
		//*

		//load entities
		for(sint j=0;j<LWIDTH;++j)
		{
			switch(map[i][j] - math::set(62,map[i][j]>='a'))
			{
				case '!':
					boss = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),AFLOAT),(*bi),bm,0,2);
				break;

				case '"':
					player = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),GROUND),(*pi),pm,pn,0);
				break;

				case '#':
				case '$':
				case '%':
				case '&':
				case '\'':
				case '(':
					entity* enemy = new entity(lvector(j*BWIDTH-(BWIDTH>>1),i*BWIDTH-(BWIDTH>>1),AFLOAT),(*ei),em,0,1);
					enemies.append(enemy);
				break;
			}

			//map[i][j] = string::toup(map[i][j])-'A';
		}
		//*

		terrain[i] = new object(a,b,c,d,k,OCHER);
	}

	gfx::fsprog(95);
	font::draw(600,YRES-font::height(),"Press ENTER to start",GREEN,BLACK);
	screen::_flush();
	screen::wait(ENTER);
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
}

level::~level()
{
	//for(long i=enemies.first();i<enemies.length();i+=enemies.next())
	//{ 
	//	delete (entity*)enemies.delcurrent();
	//}
	delete boss;
	delete player;
	//delete[] terrain
	//delete map
	delete pp;
	delete sp;
	delete bp;
	delete ep;
}

sint level::update(sint k,sint j)
{
	for(enemies.first();enemies.notlast();enemies.next())
	{
		ifu(((entity*)enemies.current())->update()<0) { /*delete*/ enemies.delcurrent(); }
	}

	return (boss->update()<0)-(player->update(k,j,fx::l2f(markmax),fx::l2f(markmin+YMAX))<0);
}

void level::display()
{
	//draw background
	screen::back.clear(DRED);
	screen::depth.clear(FX(200));
	//*

	//render terrain //fix
	mark = math::lim(markmax,entity::ylevel()-450,markmin);
	const lvector p(400,300+BWIDTH-mark%BWIDTH,GROUND);
	object::linear.clear();
	object::linear.translate(0,FX(300),0);
	sint r = math::max((mark/BWIDTH)-OFFSET,0);
	for(uint i=0;i<34;++i,++r)
	{
		object temp(*terrain[r]);
		temp.update();
		temp.display(p,R_F);
		object::linear.translate(0,FX(-20),0);
	}
	//*

	//add glow to terrain

	//*

	//render shadows
	for(enemies.first();enemies.notlast();enemies.next()) { ((entity*)enemies.current())->display(mark,1); }
	boss->display(mark,1);
	player->display(mark,1);
	//*

	//render entities
	for(enemies.first();enemies.notlast();enemies.next()) { ((entity*)enemies.current())->display(mark,0); }
	boss->display(mark,0);
	player->display(mark,0);
	//*
}

void level::gauges()
{
	//render gui elements
	for(enemies.first();enemies.notlast();enemies.next())
	{
		const lvector e(((entity*)enemies.current())->data(mark));
		if(e.z>0)
		{
			ep->visible = (game::onscreen(e.x,e.y));
			ep->left = (e.x-25);
			ep->top = (e.y-10);
			ep->set(e.z+e.e);
			ep->draw();
		}
	}
	const lvector b(boss->data(mark));
	if(b.z>0)
	{
		bp->visible = (game::onscreen(b.x,b.y));
		bp->left = (b.x-50);
		bp->top = (b.y-20);
		bp->set(b.z+b.e);
		bp->draw();
	}
	const lvector p(player->data(mark));
	pp->set(p.z);
	pp->draw();
	sp->set(p.e);
	sp->draw();
	//*
}

void level::resume()
{
	//resume all entities
	for(enemies.first();enemies.notlast();enemies.next()) { ((entity*)enemies.current())->resume(); }
	boss->resume();
	player->resume();
	//*
}

lvector level::ppos()
{
	return player->data(mark);
}
///</code>

