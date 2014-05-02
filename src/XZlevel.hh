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
#define LWIDTH   41
#define BWIDTH   16
#define BHEIGHT  10
#define GROUND  360 
#define AFLOAT  380
#define OFFSET    3
#define MAXSTEP   5

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
		object** terrain;		// Level Terrain Stripes
		entity* player;			// Player Entity
		entity* boss;			// Boss Entity
		list<entity> enemies;		// Enemy Entities List

		      xint marker;		// Current Level Position
		      xint marker_bot;		// Bottom Level Position
		const xint marker_top;		// Top Level Position

		progress* pp;			// Player Health Gauge
		progress* sp;			// Player Shield Gauge
		progress* bp;			// Boss Gauge
		progress* ep;			// Enemy Gauge

		void load();

		void landscape();		// Display Terrain
		void entities(bool s);		// Display Entities
		void gauges();			// Display Gauges

		vector camera(const vector& v);

		level(const level& l);
		level& operator=(const level& l);
	public:
		level(char* o);			// Constructor
		~level();			// Destructor

		xint update(xint j);		// Update Entities
		void display();			// Display Scene
		void resume();			// Resume After Pausing

		inline vector ppos();		//get player position
};
///</define>

///<code>
void level::load()
{

}

void level::landscape()
{
	//mark = math::lim(markmax,entity::ylevel()-YRES+(YRES>>2),markmin);
	//const vector pos{(XRES>>1)+(BWIDTH/2),(YRES>>1)+(BWIDTH/2)-mark%BWIDTH,GROUND};

	const vector pos{XRES/2,YRES/2,GROUND,0};
	object::linear.clear();
	object::linear.translate(0,FX(YRES/2+OFFSET*BWIDTH),0);

	xint r = math::max((marker/BWIDTH)-OFFSET,0);

	for(xint i=0;i<(30+OFFSET);++i)
	{
		object temp(*terrain[r++]);
		temp.update();
		temp.display(pos,R_F);
		object::linear.translate(0,FX(-BWIDTH),0);
	}
}

void level::entities(bool s)
{
	for(enemies.first();enemies.notlast();enemies.next())
	{
		enemies.current()->display(marker,s);
	}
	boss->display(marker,s);
	player->display(marker,s);
}

void level::gauges()
{
	//render enemy gauges
	for(enemies.first();enemies.notlast();enemies.next())
	{
		const entity& enemy = *enemies.current();
		const vector e(camera(enemy.position()));
		if(e.z>0)
		{
			ep->vis(screen::onscreen(e.x,e.y));
			ep->pos(e.x-24,e.y-10);
			ep->set(e.z+e.e);
			ep->draw();
		}
	}

	//render boss gauge
	const vector b(camera(boss->position()));
	if(b.z>0)
	{
		bp->vis(screen::onscreen(b.x,b.y));
		bp->pos(b.x-48,b.y-20);
		bp->set(boss->health()+boss->shield());
		bp->draw();
	}

	//render player gauge
	pp->set(player->health());
	pp->draw();
	sp->set(player->shield());
	sp->draw();
}

vector level::camera(const vector& v)
{
	return vector{v.x,v.y-marker,v.z,0};
}

level::level(char* o) : marker_top(OFFSET*BWIDTH)
{
	//load lvl
	info arc = format::ar(o);
	info lvl = format::ini(arc["level0.lvl"]);

	//load intro message
	const char* tx = arc[lvl["intro"]];
	screen::frame.clear(BLACK);
	font::draw(XRES>>3,100,tx,ORANGE);
	screen::run();

	//load player
	object* pm = new object(arc[lvl["player_0"]]);
	object* pn = new object(arc[lvl["player_1"]]);
	info    pi = format::ini(arc[lvl["player_i"]]);
 	        pp = new progress(0,string::str2int(pi["health"]),VER,16,16,16,YRES-32,GREEN,RED,GREY,WHITE,1);
 	        sp = new progress(0,string::str2int(pi["shield"]),VER,XRES-32,16,16,YRES-32,BLUE,RED,GREY,WHITE,1);
	gfx::fsprog(10);
	screen::run();

	//load boss
	object* bm = new object(arc[lvl["boss_0"]]);
	info    bi = format::ini(arc[lvl["boss_i"]]); 
	        bp = new progress(0,string::str2int(bi["health"]),HOR,0,0,96,16,RED,ORANGE,GREY,WHITE,0);
	gfx::fsprog(20);
	screen::run();

	//load enemy
	object* em = new object(arc[lvl["enemy_0"]]);
	info    ei = format::ini(arc[lvl["enemy_i"]]);
	        ep = new progress(0,string::str2int(ei["health"]),HOR,0,0,48,8,GREEN,ORANGE,GREY,WHITE,0);
	gfx::fsprog(30);
	screen::run();

	//load map
	const char* m = arc[lvl["map"]];
	const xint  l = string::count(m,'\n');
	char** map    = string::split(m,'\n');
	gfx::fsprog(40);
	screen::run();

	marker = marker_bot = (l*BWIDTH)-YRES; //Level Start

	terrain = new object*[l];

	vector* a = new vector[LWIDTH];
	vector* b = new vector[LWIDTH];
	vector* c = new vector[LWIDTH];
	vector* d = new vector[LWIDTH];

	for(xint i=0;i<l;++i)
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

		//load terrain, stripe by stripe
		if(i==0) continue;
		xint v = -XRES/2;
		xint k = 0;

		for(xint j=1;j<LWIDTH;++j)		
		{
			a[k] = vector{ fx::l2f(v),        FX(BWIDTH/2),  fx::l2f(BHEIGHT*map[i-1][j-1]), 0 };
			b[k] = vector{ fx::l2f(v),        FX(-BWIDTH/2), fx::l2f(BHEIGHT*map[i][j-1]),   0 };
			c[k] = vector{ fx::l2f(v+BWIDTH), FX(-BWIDTH/2), fx::l2f(BHEIGHT*map[i][j]),     0 };
			d[k] = vector{ fx::l2f(v+BWIDTH), FX(BWIDTH/2),  fx::l2f(BHEIGHT*map[i-1][j]),   0 };

			//join same level blocks
			if(k>0 && a[k].z==d[k].z && b[k].z==c[k].z && a[k].z==a[k-1].z && b[k].z==b[k-1].z)
			{
				c[k-1].x = c[k].x;
				d[k-1].x = d[k].x;
			}
			else
			{
				k += (a[k].z!=0 || b[k].z!=0 || c[k].z!=0 || d[k].z!=0);
			}

			v += BWIDTH;
			
		}

		terrain[i] = new object(a,b,c,d,k,VIOLET);
	}

	gfx::fsprog(95);
	font::draw(XRES-200,YRES-font::height(),"Press SPACE to start",GREEN);
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
	//for(enemies.first();enemies.notlast();enemies.next())
	//{
	//	ifu(enemies.current()->update()<0) { /*delete*/ enemies.delcurrent(); }
	//}

	//const xint b = boss->update();
	//const xint p = player->update(screen::turbo(),j,fx::l2f(markmax),fx::l2f(markmin+YMAX));

	//return (b<0)-(p<0);

	return 0;
}

void level:: display()
{
	//draw background 
	screen::frame.clear(VIOLET>>1);
	screen::depth.clear(0); //TODO: remove the clear

	landscape();
	//entities(1); // Render Shadows
	entities(0); // Render Entities
	//gauges();
}

void level::resume()
{
	for(enemies.first();enemies.notlast();enemies.next())
	{
		enemies.current()->resume();
	}
	boss->resume();
	player->resume();
}

vector level::ppos()
{
	return player->position();
}
///</code>

