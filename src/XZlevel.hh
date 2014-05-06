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
#include "XZpatch.hh"
///</include>

//<declare>
#define LWIDTH   40
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
		patch** terrain;		// Level Terrain Normal Map
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

		void landscape() const;		// Display Terrain
		void entities(bool s);		// Display Entities
		void gauges();			// Display Gauges

		vector camera(const vector& v) const;

		level(const level& l);
		level& operator=(const level& l);
	public:
		level(char* o);			// Constructor
		~level();			// Destructor

		xint update();			// Update Entities
		void display();			// Display Scene
		void resume();			// Resume After Pausing

		inline vector ppos();		//get player position
};
///</define>

///<code>
void level::load()
{

}

void level::landscape() const
{
	for(xint i=0,p=0,y=0;i<30;++i,y+=BWIDTH)
	{
		for(xint j=0,x=0;j<LWIDTH;++j,++p,x+=BWIDTH)
		{
			terrain[p]->display(vector{x,y,0,BWIDTH},-polygon::light,ORANGE);
		}
	}
}

void level::entities(bool s)
{
	for(enemies.first();enemies.notlast();enemies.next())
	{
		enemies.current()->display(camera(enemies.current()->position()),s);
	}
	boss->display(camera(boss->position()),s);
	player->display(camera(player->position()),s);
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

vector level::camera(const vector& v) const
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

	terrain = new patch*[l*LWIDTH];

	for(xint i=0,p=0;i<l;++i)
	{
		//load entities
		for(xint j=0;j<(LWIDTH+1);++j)
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
		
		for(xint j=1;j<(LWIDTH+1);++j,++p)		
		{
			const xint az = map[i-1][j-1];
			const xint bz = map[i-1][j];
			const xint cz = map[i][j];
			const xint dz = map[i][j-1];
			
			const vector x0 = { FX(BWIDTH), 0, fx::l2f(dz-cz) };
			const vector y0 = { 0, FX(BWIDTH), fx::l2f(az-dz) };

			const vector x1 = { FX(BWIDTH), 0, fx::l2f(az-bz) };
			const vector y1 = { 0, FX(BWIDTH), fx::l2f(bz-cz) };

			terrain[p] = new patch(x0,y0,x1,y1);
		}
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

xint level::update()
{
	//for(enemies.first();enemies.notlast();enemies.next())
	//{
	//	ifu(enemies.current()->update()<0) { /*delete*/ enemies.delcurrent(); }
	//}

	//const xint b = boss->update();
	//const xint p = player->update();

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
	//entities(0); // Render Entities
	
	//entities::display_ammo(0);
	//entiites::display_ammo(1);
	
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

