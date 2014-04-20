///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Level Class ( XZlevel.hh )
#pragma once
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
#include "XZscreen.hh"
///</include>

//<declare>
#define LWIDTH  40
#define BWIDTH  16
#define BHEIGHT 10
#define GROUND  300 
#define AFLOAT  50
#define OFFSET   4
#define MAXSTEP  5

#define BOSS   '!'
#define PLAYER '"'
#define ENEMY0 '#'
#define ENEMY1 '$'
#define ENEMY2 '%'
#define ENEMY3 '&'
#define ENEMY4 '\''
#define ENEMY5 '('
///</declare>

///<define>
class level
{
	private:
		object** terrain;		//Level Terrain Stripe
		entity* player;			//Player Entity
		entity* boss;			//Boss Entity
		list<entity> enemies;		//List of Enemy Entities
		char** map;			//Text Map of Terrain
		xint mark;			//Current Level Position
		xint markmin;			//Lowest Level Position (Bottom)
		const xint markmax;		//Highest Level Position (Top)
		progress* pp;			//Player Health Gauge
		progress* sp;			//Player Shield Gauge
		progress* bp;			//Boss Gauge
		progress* ep;			//Enemy Gauge
		level(const level& l);
		level& operator=(const level& l);
	public:
		level(char* o);			//Constructor
		~level();			//Destructor
		xint update(xint j);		//Update All Entities
		void display();			//Display Terrain, Shadows, Entities
		//void gauges();			//Display Gauges
		void resume();			//Resume After Pausing
		inline vector ppos();		//get player position
};
///</define>

///<code>
level::level(char* o) : markmax(OFFSET*BWIDTH)
{
	//load lvl
	info arc = format::ar(o);
	info lvl = format::ini(arc["level0.lvl"]);
system::say("hi");
	//load intro message
	const char* tx = arc[lvl["intro"]];
	screen::frame.clear(BLACK);
	font::draw(XRES>>3,100,tx,ORANGE,BLACK);
	screen::run();
system::say("hi");
	//load player
	object* pm = new object(arc[lvl["player_0"]]);
	object* pn = new object(arc[lvl["player_1"]]);
	info    pi = format::ini(arc[lvl["player_i"]]);
 	        pp = new progress(0,string::str2int(pi["health"]),VER,16,16,16,YRES-32,GREEN,RED,GREY,WHITE,1);
 	        sp = new progress(0,string::str2int(pi["shield"]),VER,XRES-32,16,16,YRES-32,BLUE,RED,GREY,WHITE,1);
	gfx::fsprog(10);
	screen::run();
system::say("hi");
	//load boss
	object* bm = new object(arc[lvl["boss_0"]]);
	info    bi = format::ini(arc[lvl["boss_i"]]); 
	        bp = new progress(0,string::str2int(bi["health"]),HOR,0,0,96,16,RED,ORANGE,GREY,WHITE,0);
	gfx::fsprog(20);
	screen::run();
system::say("hi");
	//load enemy
	object* em = new object(arc[lvl["enemy_0"]]);
	info    ei = format::ini(arc[lvl["enemy_i"]]);
	        ep = new progress(0,string::str2int(ei["health"]),HOR,0,0,48,8,GREEN,ORANGE,GREY,WHITE,0);
	gfx::fsprog(30);
	screen::run();
system::say("hi");
	//load map
	const char* m = arc[lvl["map"]];
	const xint l  = string::count(m,'\n');
	map           = string::split(m,'\n');
	gfx::fsprog(40);
	screen::run();

	markmin = (l*BWIDTH)-YMAX; //Level Start

	terrain = new object*[l];
system::say("hi");
	vector* a = new vector[LWIDTH];
	vector* b = new vector[LWIDTH];
	vector* c = new vector[LWIDTH];
	vector* d = new vector[LWIDTH];
	vector e;
	vector f;

	for(xint i=0,k=0;i<l;++i,k=0)
	{
		//load entities
		for(xint j=0;j<LWIDTH;++j)
		{
			switch(map[i][j] - math::set(62,map[i][j]>='a'))
			{
				case BOSS:
					boss = new entity( vector{j*BWIDTH-(BWIDTH/2),i*BWIDTH-(BWIDTH/2),AFLOAT,0}, bi,bm,0,2);
				break;

				case PLAYER:
					player = new entity( vector{j*BWIDTH-(BWIDTH/2),i*BWIDTH-(BWIDTH/2),GROUND,0}, pi,pm,pn,0);
				break;

				case ENEMY0: 
				case ENEMY1:
				case ENEMY2:
				case ENEMY3:
				case ENEMY4:
				case ENEMY5:
					enemies.append(new entity( vector{j*BWIDTH-(BWIDTH/2),i*BWIDTH-(BWIDTH/2),AFLOAT,0}, ei,em,0,1));
				break;
			}

			map[i][j] = string::toup(map[i][j])-'A';
		}
		//*

		//load terrain stripe
		xint v = -XRES/2;
		for(xint j=1;j<LWIDTH&&i>1;++j,v+=BWIDTH)		
		{
			(a[k]) = vector{ v,       -BWIDTH/2, BHEIGHT*map[i][j-1],   0 };
			(b[k]) = vector{ v,        BWIDTH/2, BHEIGHT*map[i-1][j-1], 0 };
			(c[k]) = vector{ v+BWIDTH, BWIDTH/2, BHEIGHT*map[i-1][j],   0 };
			(d[k]) = vector{ v+BWIDTH,-BWIDTH/2, BHEIGHT*map[i][j],     0 };

			if(a[k].z==d[k].z && b[k].z==c[k].z)
			{
				++j;
				v += BWIDTH*2; 
				for(;j<LWIDTH;++j,v+=BWIDTH) //TODO v+= weg wg v-=
				{
					e = vector{ v,  BWIDTH/2, (map[i-1][j])*BHEIGHT, 0 };
					f = vector{ v,-(BWIDTH/2),(map[i][j])  *BHEIGHT, 0 };
					if(c[k].z==e.z && d[k].z==f.z)
					{
						c[k] = e;
						d[k] = f;
					}
					else break;
				}
				v -= BWIDTH*2;
				--j;
			}

			k += (a[k].z!=0 || b[k].z!=0 || c[k].z!=0 || d[k].z!=0);
		}
		//*

		terrain[i] = new object(a,b,c,d,k,VIOLET);
	}

	gfx::fsprog(95);
	font::draw(XRES-200,YRES-font::height(),"Press SPACE to start",GREEN,BLACK);
	screen::run();
	//screen::depth.clear(0); //z
	screen::key();
	screen::wait(SPACE);

	//delete[] a;
	//delete[] b;
	//delete[] c;
	//delete[] d;
}

level::~level()
{
	//for(long i=enemies.first();i<enemies.length();i+=enemies.next())
	//{ 
	//	delete enemies.delcurrent();
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

xint level::update(xint j)
{
	for(enemies.first();enemies.notlast();enemies.next())
	{
		ifu(enemies.current()->update()<0) { /*delete*/ enemies.delcurrent(); }
	}

	return (boss->update()<0)-(player->update(screen::turbo(),j,fx::l2f(markmax),fx::l2f(markmin+YMAX))<0);
}

void level::display()
{
	//draw background 
	screen::frame.clear(VIOLET>>1);
	screen::depth.clear(0); //TODO: remove the clear
/*
	//render terrain //fix
	mark = math::lim(markmax,entity::ylevel()-YRES+(YRES>>2),markmin);
	const tuple pos{(XRES>>1)+(BWIDTH/2),(YRES>>1)+(BWIDTH/2)-mark%BWIDTH,GROUND};
	object::linear.clear();
	object::linear.translate(0,FX(YRES>>1),0);
	xint r = math::max((mark/BWIDTH)-OFFSET,0);
	for(yint i=0;i<31;++i)
	{
		object temp(*terrain[r++]);
		temp.update();
		temp.display(pos,R_F);
		object::linear.translate(0,FX(-BWIDTH),0);
	}
	//*

	//render shadows
	for(enemies.first();enemies.notlast();enemies.next()) { enemies.current()->display(mark,1); }
	boss->display(mark,1);
	player->display(mark,1);*/
	//*

	//render entities
	//for(enemies.first();enemies.notlast();enemies.next()) { enemies.current()->display(mark,0); }
	//	boss->display(mark,0);
		player->display(mark,0);
	//*

	//render enemy gauges
	for(enemies.first();enemies.notlast();enemies.next())
	{
		const vector e(enemies.current()->data(mark));
		if(e.z>0)
		{
			ep->vis(screen::onscreen(e.x,e.y));
			ep->pos(e.x-24,e.y-10);
			ep->set(e.z+e.e);
			ep->draw();
		}
	}

	//render boss gauge
	const vector b(boss->data(mark));
	if(b.z>0)
	{
		bp->vis(screen::onscreen(b.x,b.y));
		bp->pos(b.x-48,b.y-20);
		bp->set(b.z+b.e);
		bp->draw();
	}

	//render player gauge
	const vector p(player->data(mark));
	pp->set(p.z);
	pp->draw();
	sp->set(p.e);
	sp->draw();
}

void level::resume()
{
	//resume all entities after a pause
	for(enemies.first();enemies.notlast();enemies.next()) { enemies.current()->resume(); }
	boss->resume();
	player->resume();
}

vector level::ppos()
{
	return player->data(mark);
}
///</code>

