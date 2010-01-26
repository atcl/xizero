///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLLEVEL
#define HH_CLLEVEL
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLbuffer.hh"
#include "CLsystem.hh"
#include "CLmath.hh"
#include "CLar.hh"
#include "CLformat.hh"
#include "CLutils.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLplayer.hh"
#include "CLenemy.hh"
#include "CLboss.hh"
#include "CLgame.hh"
///*

///header
/* class name:	CLlevel
 * 
 * description:	This class manages the complete game inside a level
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///declarations
#define UNITHEIGHT 20
#define UNITWIDTH  20
typedef CLlist<CLenemy> CLenemylist;
///*

///definitions
class CLlevel : public CLbase<CLlevel,0>
{
	private:
		static CLgame*   clgame;
		static CLformat* clformat;
		static CLstring* clstring;
		static CLscreen* clscreen;
		static CLsystem* clsystem; //temp!
	protected:
		CLobject**   levelmap;
		CLplayer*    player;
		CLenemylist* enemylist;
		CLboss*      boss;
		xlong levelwidth;
		xlong levelheight;
		xlong levelmarkmax;
		xlong levelmarkmin;
		xlong levelmark;
		xlong stripewidth;
		xlong stripeheight;
		xlong stripemarkmax;
		xlong stripemarkmin;
		xlong stripemark;
		xlong striperest;
		static xlong floorheight;
		xlong playerscreenylevel;
		static bool paused;
		
		inline xlong extract(sprite* h,xlong y,xlong x,xlong i) { return (xlong(((uxchar*)(&h->data[(y*h->width)+x]))[i])); }
	public:
		CLlevel(CLfile* terrainlib,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,CLfile* levelcontainer,xlong bosstype);
		CLlevel(CLfile* maplib,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,xlong level,xlong bosstype);
		~CLlevel();
		xlong update();
		void display();
		void setmark(xlong m);
		void start();
		static void pause();
		xlong getmark() const { return levelmark; };
		CLplayer* getplayer() const { return player; };
};

CLgame*   CLlevel::clgame   = CLgame::instance();
CLformat* CLlevel::clformat = CLformat::instance();
CLstring* CLlevel::clstring = CLstring::instance();
CLscreen* CLlevel::clscreen = CLscreen::instance();
CLsystem* CLlevel::clsystem = CLsystem::instance();
xlong CLlevel::floorheight = 100;
bool  CLlevel::paused = 0;
///*

///implementation
CLlevel::CLlevel(CLfile* maplib,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,xlong level,xlong bosstype) //! noncritical
{
	//set screen boundaries
	clgame->setboundaries(60,0,740,600);
	//*

	//game is not paused from beginning
	paused = 0;
	//*
	
	//load combined level map file
	//******************************************************************
	CLar* levela = new CLar(maplib);
	sprite* h = clformat->loadras(levela->findbyname("level000.map"));
	xlong width = h->width;
	xlong height = h->height;
	uxlong* data = h->data;
	
	xlong stripesperscreen = YRES / UNITHEIGHT;
	levelwidth = width * UNITWIDTH;
	levelheight = height * UNITHEIGHT;
	levelmarkmax = levelheight - (stripesperscreen * UNITHEIGHT);
	levelmarkmin = 1;
	levelmark = levelmarkmax;
	stripewidth = width;
	stripeheight = height;
	stripemarkmax = height - stripesperscreen;
	stripemarkmin = 1;
	stripemark = stripemarkmax;
	striperest = 0;
	
	if(stripemark < 0) err(__FILE__,__func__,u8"Level too short");
	playerscreenylevel = 3*(YRES>>2);
	//******************************************************************
	
	//load heightmap
	//******************************************************************
	rawpoly* grid = new rawpoly[4*width];
	xlong curr = 0;
	levelmap = new CLobject*[height];
	
	xlong hordiff[4];
	xlong verdiff[4];
	bool  unbalanced = 0;
	
	for(xlong i=0; i<height-1; i++)
	{
	
		//generate stripe horizontally
		for(xlong j=0; j<width-1; j++)
		{			
			//setup poly
			grid[curr].v[0].x = (j*20)-400;
			grid[curr].v[0].y = 10;
			grid[curr].v[0].z = -extract(h,i,j,2)/4;
			grid[curr].v[1].x = ((j+1)*20)-400;
			grid[curr].v[1].y = 10;
			grid[curr].v[1].z = -extract(h,i,j+1,2)/4;
			grid[curr].v[2].x = ((j+1)*20)-400;
			grid[curr].v[2].y = -10;
			grid[curr].v[2].z = -extract(h,i+1,j+1,2)/4;
			grid[curr].v[3].x = (j*20)-400;
			grid[curr].v[3].y = -10;
			grid[curr].v[3].z = -extract(h,i+1,j,2)/4;
			//*
			
			//split by balance
			if( ( (grid[curr].v[0].z==grid[curr].v[1].z) && (grid[curr].v[0].z==grid[curr].v[3].z) && (grid[curr].v[0].z!=grid[curr].v[2].z) )
			||  ( (grid[curr].v[2].z==grid[curr].v[1].z) && (grid[curr].v[2].z==grid[curr].v[3].z) && (grid[curr].v[0].z!=grid[curr].v[2].z) ) )
			{
				curr++;
				grid[curr].v[0].x = grid[curr-1].v[1].x;
				grid[curr].v[0].y = grid[curr-1].v[1].y;
				grid[curr].v[0].z = grid[curr-1].v[1].z;
				grid[curr].v[1].x = grid[curr-1].v[2].x;
				grid[curr].v[1].y = grid[curr-1].v[2].y;
				grid[curr].v[1].z = grid[curr-1].v[2].z;
				grid[curr].v[2].x = grid[curr-1].v[3].x;
				grid[curr].v[2].y = grid[curr-1].v[3].y;
				grid[curr].v[2].z = grid[curr-1].v[3].z;
				grid[curr].v[3].x = grid[curr-1].v[3].x;
				grid[curr].v[3].y = grid[curr-1].v[3].y;
				grid[curr].v[3].z = grid[curr-1].v[3].z;
				
				//grid[curr-1].v[0].x = grid[curr-1].v[0].x;
				//grid[curr-1].v[0].y = grid[curr-1].v[0].y;
				//grid[curr-1].v[0].z = grid[curr-1].v[0].z;
				//grid[curr-1].v[1].x = grid[curr-1].v[1].x;
				//grid[curr-1].v[1].y = grid[curr-1].v[1].y;
				//grid[curr-1].v[1].z = grid[curr-1].v[1].z;
				grid[curr-1].v[2].x = grid[curr-1].v[3].x;
				grid[curr-1].v[2].y = grid[curr-1].v[3].y;
				grid[curr-1].v[2].z = grid[curr-1].v[3].z;
				//grid[curr-1].v[3].x = grid[curr-1].v[3].x;
				//grid[curr-1].v[3].y = grid[curr-1].v[3].y;
				//grid[curr-1].v[3].z = grid[curr-1].v[3].z;
				
				unbalanced = 1;
			}
			else if( ( (grid[curr].v[1].z==grid[curr].v[0].z) && (grid[curr].v[1].z==grid[curr].v[2].z) && (grid[curr].v[1].z!=grid[curr].v[3].z) )
			     ||  ( (grid[curr].v[3].z==grid[curr].v[0].z) && (grid[curr].v[3].z==grid[curr].v[2].z) && (grid[curr].v[3].z!=grid[curr].v[1].z) ) )
			{
				curr++;
				grid[curr].v[0].x = grid[curr-1].v[0].x;
				grid[curr].v[0].y = grid[curr-1].v[0].y;
				grid[curr].v[0].z = grid[curr-1].v[0].z;
				grid[curr].v[1].x = grid[curr-1].v[1].x;
				grid[curr].v[1].y = grid[curr-1].v[1].y;
				grid[curr].v[1].z = grid[curr-1].v[1].z;
				grid[curr].v[2].x = grid[curr-1].v[2].x;
				grid[curr].v[2].y = grid[curr-1].v[2].y;
				grid[curr].v[2].z = grid[curr-1].v[2].z;
				grid[curr].v[3].x = grid[curr-1].v[2].x;
				grid[curr].v[3].y = grid[curr-1].v[2].y;
				grid[curr].v[3].z = grid[curr-1].v[2].z;
				
				//grid[curr-1].v[0].x = grid[curr-1].v[0].x;
				//grid[curr-1].v[0].y = grid[curr-1].v[0].y;
				//grid[curr-1].v[0].z = grid[curr-1].v[0].z;
				grid[curr-1].v[1].x = grid[curr-1].v[2].x;
				grid[curr-1].v[1].y = grid[curr-1].v[2].y;
				grid[curr-1].v[1].z = grid[curr-1].v[2].z;
				grid[curr-1].v[2].x = grid[curr-1].v[3].x;
				grid[curr-1].v[2].y = grid[curr-1].v[3].y;
				grid[curr-1].v[2].z = grid[curr-1].v[3].z;
				//grid[curr-1].v[3].x = grid[curr-1].v[3].x;
				//grid[curr-1].v[3].y = grid[curr-1].v[3].y;
				//grid[curr-1].v[3].z = grid[curr-1].v[3].z;
				
				unbalanced = 1;
			}
			//*
			
			//unify horizontally
			else if(curr>0 && unbalanced==0)
			{
				hordiff[0] = grid[curr-1].v[1].z - grid[curr-1].v[0].z;
				hordiff[1] = grid[curr-1].v[2].z - grid[curr-1].v[3].z;
				hordiff[2] = grid[curr].v[1].z   - grid[curr].v[0].z;
				hordiff[3] = grid[curr].v[2].z   - grid[curr].v[3].z;
				
				verdiff[0] = grid[curr-1].v[3].z - grid[curr-1].v[0].z;
				verdiff[1] = grid[curr-1].v[2].z - grid[curr-1].v[1].z;
				verdiff[2] = grid[curr].v[3].z   - grid[curr].v[0].z;
				verdiff[3] = grid[curr].v[2].z   - grid[curr].v[1].z;
				
				if( hordiff[0]==hordiff[2] && hordiff[1]==hordiff[3]
				&&  verdiff[0]==verdiff[2] && verdiff[1]==verdiff[3] )
				{
					grid[curr-1].v[1].x = grid[curr].v[1].x;
					grid[curr-1].v[2].x = grid[curr].v[2].x;
					grid[curr-1].v[1].z = grid[curr].v[1].z;
					grid[curr-1].v[2].z = grid[curr].v[2].z;
					curr--;
				}
			}
			//*
			
			else { unbalanced = 0; }
			
			curr++;
		}
		//*
		
		//generate object
		levelmap[i] = new CLobject(grid,curr,0xFFFFFFFF,0x000000FF);
		//*
		
		//reset for next stripe
		curr = 0;
		//* 			
	}
	
	delete[] grid;
	//******************************************************************
	
	
	//load player, enemies and boss
	//******************************************************************
	uxchar type = 0;
	
	CLar* bossa = new CLar(bosslib);
	CLar* enemiesa = new CLar(enemylib);
	xlong differentenemies = enemiesa->getfilecount();
	CLenemy** baseenemies = new CLenemy*[differentenemies];
	for(uxlong k=0; k<differentenemies; k++) { baseenemies[k] = new CLenemy(enemiesa->getmember(k),&levelmark,levelheight+10); }
	
	CLlvector* ppos = new CLlvector(0,0,0,-1);
	CLlvector* epos = new CLlvector(0,0,0,-1);
	CLlvector* bpos = new CLlvector(0,0,0,-1);
	enemylist = new CLenemylist();
	CLenemy* currentenemy;
	
	for(xlong i=0; i<height; i++)
	{
		for(xlong j=0; j<width; j++)
		{
			type = extract(h,i,j,1);
			
			switch(type)
			{
				case 0: //nothing
				
				case 63: //player start position
					if(ppos->e==-1)
					{
						ppos->x = j * UNITHEIGHT;
						ppos->y = i * UNITWIDTH;
						ppos->z = -extract(h,i,j,2)/4;;
						ppos->e = 0;
						player = new CLplayer(playerlib,&levelmark,levelheight+10,ppos);
					}
				break;
				
				case 255: //boss start position
					if(bpos->e==-1)
					{
						bpos->x = j * UNITHEIGHT;
						bpos->y = i * UNITWIDTH;
						bpos->z = -extract(h,i,j,2)/4;
						bpos->e = 0;
						boss = new CLboss(bossa->getmember(bosstype),&levelmark,levelheight+10,bpos);
					}
				break;
				
				default: //enemy start position
					bpos->x = j * UNITHEIGHT;
					bpos->y = i * UNITWIDTH;
					bpos->z = -extract(h,i,j,2)/4;
					bpos->e = 0;
					currentenemy = new CLenemy(baseenemies[127+type],epos);
					enemylist->append(currentenemy);
				break;
			}
		}
	}
	
	if(ppos->e==-1) { err(__FILE__,__func__,u8"no player start position found in combined map"); }
	if(epos->e==-1) { err(__FILE__,__func__,u8"no enemy start position found in combined map"); }
	if(bpos->e==-1) { err(__FILE__,__func__,u8"no boss start position found in combined map"); }
	
	delete bossa;
	delete enemiesa;
	delete[] baseenemies;
	//******************************************************************
	
	
}

CLlevel::~CLlevel() //! noncritical
{
	delete player;
	delete enemylist;
	delete boss;
}

xlong CLlevel::update() //! critical
{
	//check if game paused
	if(paused)
	{
		player->pause();
		for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext()) { enemylist->getcurrentdata()->pause(); }
		boss->pause();
		return 1;
	}
	//*
	
	//update terrain
	for(xlong i=-1; i<31; i++)
	{
		levelmap[stripemark+i]->getmatrix()->translate(0,i,0);
		levelmap[stripemark+i]->update();
	}
	//*
	
	xlong isdead = 0;
	
	//update player
	isdead = player->update(levelmap,enemylist,boss);
	if(isdead != -1) { return -1; }
	//*

	//update enemies
	CLenemy* currentenemy;
	bool listfix = 0;
	for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext())
	{
		if(listfix) { i+=enemylist->setprev(); listfix=0; }
		currentenemy = enemylist->getcurrentdata();
		isdead = currentenemy->update(player);
		if(isdead!=-1)
		{
			//delete static_cast<CLenemy*>(enemies->delcurrent(1));
			player->addpoints(isdead);
			enemylist->delcurrent(0);
			listfix = enemylist->isfirst();
		}
	}
	//*
	
	//update boss
	isdead = boss->update(player);
	if(isdead!=-1)
	{
		player->addpoints(isdead);
		return 0;
	}
	//*

	//adjust section of level to be displayed by ("new") player position
	xlong py = player->getposition()->y;
	     if(py<playerscreenylevel)                 { setmark(levelmarkmin); }
	else if(py>(levelmarkmax+playerscreenylevel))  { setmark(levelmarkmax); }
	else                                           { setmark(py - playerscreenylevel); }
	//*
	
	return 1;
}

void CLlevel::display() //! critical
{
	//render terrain
	CLlvector p(400,300-striperest,100);
	for(xlong i=-1; i<31; i++)
	{
		levelmap[stripemark+i]->display(p,AMBIENT + FLAT + ZLIGHT);
		levelmap[stripemark+i]->reset();
	}
	//*
	
	//cast shadows of entities
	player->display(1);
	for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext()) { enemylist->getcurrentdata()->display(1); }
	boss->display(1);
	
	clscreen->clstencilbuffer.copy(&clscreen->cldoublebuffer,12);
	//*

	//display enitities
	player->display();
	for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext()) { enemylist->getcurrentdata()->display(); }
	boss->display();
	//*
	
	//display hud
	player->displayhud();
	for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext()) { enemylist->getcurrentdata()->displayhud(); }
	boss->displayhud();
	//*
}

void CLlevel::setmark(xlong m) //! noncritical
{
	levelmark = m;
	stripemark = levelmark / UNITHEIGHT;
	striperest = levelmark % UNITHEIGHT;
}

void CLlevel::pause() { paused = !paused; } //! noncritical

void CLlevel::start() //! noncritical
{
	player->start();
	for(xlong i=enemylist->setfirst(); i<enemylist->getlength(); i+=enemylist->setnext()) { enemylist->getcurrentdata()->start(); }
	boss->start();
}
///*

#endif

