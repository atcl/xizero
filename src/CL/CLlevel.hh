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
#include "CLgamepad.hh"
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
	protected:
		CLmatrix*    linear;
		CLplayer*    player;
		CLenemylist* enemies;
		CLboss*      boss;
		CLobject**   terrain;
		CLfbuffer*   levellandscape;
		static xlong levelwidth;
		static xlong blockheight;
		static xlong blockwidth;
		static xlong blockdepth;
		static xlong floorheight;
		xchar*** levellayers;
		xlong levelheight;
		xlong blocksperscreeny;
		xlong blocksperscreenx;
		xlong blockmark;
		xlong smoothmark;
		xlong smoothmarkmin;
		xlong smoothmarkmax;
		xlong smoothlevelheight;
		xlong smoothlevelwidth;
		xlong playerscreenylevel;
		static bool  paused;
	public:
		CLlevel(CLfile* terrainlib,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,CLfile* levelcontainer,xlong bosstype);
		~CLlevel();
		xlong update(xchar input,xchar turbo,CLgamepadstate* p);
		void display();
		void subsmark(xlong m);
		void setmark(xlong m);
		void start();
		static void pause();
		xlong getmark() const { return smoothmark; };
		CLfbuffer* getlandscape() const { return levellandscape; };
		CLplayer* getplayer() const { return player; };
};

CLgame*   CLlevel::clgame   = CLgame::instance();
CLformat* CLlevel::clformat = CLformat::instance();
CLstring* CLlevel::clstring = CLstring::instance();
CLscreen* CLlevel::clscreen = CLscreen::instance();
xlong CLlevel::levelwidth = 20; //in blocks
xlong CLlevel::blockheight = 40;
xlong CLlevel::blockwidth = 40;
xlong CLlevel::blockdepth = 40;
xlong CLlevel::floorheight = 100;
bool  CLlevel::paused = 0;
///*

///implementation
CLlevel::CLlevel(CLfile* terrainlib,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,CLfile* levelcontainer,xlong bosstype) //! noncritical
{
	//matrix for linear transformations of level objects
	linear = new CLmatrix(1);
	//*
	
	//set screen boundaries
	clgame->setboundaries(60,0,740,600);
	//*

	//game is not paused from beginning
	paused = 0;
	//*

//terrain:

	//load terrainlib from .ar to array of CLobject* pointing to y3d objects
	CLar* terraina = new CLar(terrainlib);
	uxlong terraincount = terraina->getfilecount();
	terrain = new CLobject*[terraincount];
	for(uxlong g=0; g<terraincount; g++) { terrain[g] = new CLobject(terraina->getmember(g),1); }
	//*

//***

//level:

	//load levelmaps from .ar
	CLar* levela = new CLar(levelcontainer);
	//*
	
	//terrain map:
	CLfile* tf = levela->findbyextension(u8".mapt");
	if(tf==0) err(__func__,u8"no terrain map found");
	xchar** terrainmap = clformat->loadmap(tf,33,' ',-1);
	//*

	//determine level constants
	levelheight = clstring->linecount(tf->text);
	blocksperscreeny = YRES / blockheight;
	blocksperscreenx = XRES / blockwidth;
	blockmark = levelheight - blocksperscreeny;
	smoothmarkmin = 0;
	smoothmarkmax = blockmark * blockheight;
	smoothmark = smoothmarkmax;
	smoothlevelheight = levelheight * blockheight;
	smoothlevelwidth = levelwidth * blockwidth;
	if(blockmark < 0) err(__func__,u8"Level too short");
	//*

	//height map:
	CLfile* hf = levela->findbyextension(u8".maph");
	if(hf==0) err(__func__,u8"no height map found");
	xchar** heightmap = clformat->loadmap(hf,48,' ',0);
	//*

	//entity map:
	CLfile* ef = levela->findbyextension(u8".mape");
	if(ef==0) err(__func__,u8"no entity map found");
	xchar** entitymap = clformat->loadmap(ef,35,'.',-3);
	//*

	//build levellayerscontaining all sub maps
	levellayers = new xchar**[4];
	levellayers[0] = terrainmap;
	levellayers[1] = heightmap;
	levellayers[2] = entitymap;
	//levellayers[3] = specialmap //later when needed.

//***

//level landscape generation:

	//create landscape buffer and virtual screenside for rendering to landscape buffer
	xlong templevelheight = smoothlevelheight;
	levellandscape = new CLfbuffer(smoothlevelwidth*templevelheight);
	levellandscape->clear(floorheight);
	screenside* templevelrside = new screenside[templevelheight];
	screenside* templevellside = new screenside[templevelheight];
	//*

	//set variables for controlling the render loop
	xlong currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1;
	xlong localfloorheight = floorheight - 5;
	CLlvector current(blockoffsetx,blockoffsety,localfloorheight);
	//*

	//render landscape
	for(uxlong i=0; i<levelheight; i++)
	{
		for(uxlong j=0; j<levelwidth; j++)
		{
			currentterrain = xlong(levellayers[0][i][j]);
			if(currentterrain != -1)
			{
				currentheight = levellayers[1][i][j];
				currententity = levellayers[2][i][j];
				if(currentheight!=0)
				{
					for(int k=1; k<=currentheight; k++)
					{
						terrain[0]->display(current,templevellside,templevelrside,levellandscape,templevelheight);
						current.z -= blockdepth>>2;
					}
				}

				terrain[currentterrain]->display(current,templevellside,templevelrside,levellandscape,templevelheight);
				terrain[currentterrain]->reset();
				linear->unit();
				current.z = localfloorheight;
			}
			current.x += blockwidth;
		}
		current.x = blockoffsetx;
		current.y += blockheight;	
	}
	//*
	
//***

//player:

	//search player start pos and set player pos to it
	bool startposfound = false;
	CLlvector* playerp = new CLlvector();
	for(uxlong h=0; h<levelheight; h++)
	{
		for(uxlong i=0; i<levelwidth; i++)
		{
			if(levellayers[2][h][i] == -2)
			{
				startposfound = true;
				playerp->x = i * blockwidth;
				playerp->y = h * blockheight;
				playerp->z = levellayers[1][h][i] * blockdepth;
				break;
			}
		}
	}
	if(startposfound==false) err(__func__,u8"no player start position found in entity map");
	//*

	//create player
	player = new CLplayer(playerlib,&smoothmark,smoothlevelheight+10,playerp);
	playerscreenylevel = 3*(YRES>>2);
	//*

//***

//enemies:

	//load enemy archive
	CLar* enemiesa = new CLar(enemylib);
	//*
	
	//load all enemies in archive (base enemies) into array
	xlong differentenemies = enemiesa->getfilecount();
	CLenemy** baseenemies = new CLenemy*[differentenemies];
	for(uxlong k=0; k<differentenemies; k++) { baseenemies[k] = new CLenemy(enemiesa->getmember(k),&smoothmark,smoothlevelheight+10); }
	//*

	//create all enemies in level through copying from base enemies from archive
	//find enemy startpos in entity map and associate (copy) from baseenemy in enemy list
	enemies = new CLenemylist();
	CLenemy* currentenemy;
	CLlvector* enemyp = new CLlvector();
	for(uxlong l=0; l<differentenemies; l++)
	{ 
		for(uxlong m=0; m<levelheight; m++)
		{
			for(uxlong n=0; n<levelwidth; n++)
			{
				if(levellayers[2][m][n] == l)
				{
					enemyp->x = n * blockwidth;
					enemyp->y = m * blockheight;
					enemyp->z = levellayers[1][m][n];
					currentenemy = new CLenemy(baseenemies[l],enemyp);
					enemies->append(currentenemy);
				}
			}
		}
	}
	//*
	
//***
	
//boss

	//load enemy archive
	CLar* bossa = new CLar(bosslib);
	//*

	//search player start pos and set player pos to it
	bool bossposfound = false;
	CLlvector* bossp = new CLlvector();
	for(uxlong h=0; h<levelheight; h++)
	{
		for(uxlong i=0; i<levelwidth; i++)
		{
			if(levellayers[2][h][i] == -1)
			{
				bossposfound = true;
				bossp->x = i * blockwidth;
				bossp->y = h * blockheight;
				bossp->z = levellayers[1][h][i] * blockdepth;
				break;
			}
		}
	}
	if(bossposfound==false) err(__func__,u8"no boss position found in entity map");
	//*

	//load boss
	boss = new CLboss(bossa->getmember(bosstype),&smoothmark,smoothlevelheight+10,bossp);
	//*

//***
	
	//release loaded files
	delete enemiesa;
	delete bossa;
	delete levela;
	delete enemyp;
	delete bossp;
	delete playerp;
	delete[] templevelrside;
	delete[] templevellside;
	//*

//***
}

CLlevel::~CLlevel() //! noncritical
{
	delete player;
	delete linear;
	delete enemies;
	delete boss;
	delete levellandscape;
	delete terrain;
}

xlong CLlevel::update(xchar input,xchar turbo,CLgamepadstate* p) //! critical
{
	if(paused)
	{
		player->pause();
		for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext()) { enemies->getcurrentdata()->pause(); }
		boss->pause();
		return 1;
	}
	
	xlong isdead = 0;
	
	//update player
	isdead = player->update(input,turbo,levellandscape,enemies,boss,p);
	if(isdead != -1) { return -1; }
	//*

	//update enemies
	CLenemy* currentenemy;
	bool listfix = 0;
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		if(listfix) { i+=enemies->setprev(); listfix=0; }
		currentenemy = enemies->getcurrentdata();
		isdead = currentenemy->update(player);
		if(isdead!=-1)
		{
			//delete static_cast<CLenemy*>(enemies->delcurrent(1));
			player->addpoints(isdead);
			enemies->delcurrent(0);
			listfix = enemies->isfirst();
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
	if(py<playerscreenylevel) setmark(smoothmarkmin);
	else if(py>(smoothmarkmax+playerscreenylevel)) setmark(smoothmarkmax);
	else setmark(py - playerscreenylevel);
	//*
	
	return 1;
}

void CLlevel::display() //! critical
{
	//set variables controlling render loop
	xlong currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1; 
	xlong yoffset = smoothmark % blockheight;
	xlong localfloorheight = floorheight - 5;
	CLlvector ckeeper( -(XRES >> 1) + blockoffsetx , (YRES >> 1) - blockoffsety + yoffset  + blockheight , localfloorheight );
	CLlvector current( 0 , 0 , localfloorheight );
	xshort dontrender = 0; 
	uxlong ii = 0;
	//*

	//render level to screen block by block
	linear->unit();
	for(int i=-1; i<blocksperscreeny+2; i++)
	{
		ii = blockmark + i;
		
		if( !( i<0                   && blockmark<=0 ) )
		{
		if( !( i==blocksperscreeny   && smoothmark>=smoothmarkmax ) )
		{
		if( !( i==blocksperscreeny+1 && smoothmark>=smoothmarkmax-blockheight ) )
		{
			for(uxlong j=0; j<blocksperscreenx; j++)
			{
				currentterrain = xlong(levellayers[0][ii][j]);
				if(currentterrain!=-1)
				{
					currentheight = levellayers[1][ii][j];
					currententity = levellayers[2][ii][j];
					linear->translate(ckeeper.x,ckeeper.y,0);

					if(currentheight!=0)
					{
						terrain[0]->update(linear);
						for(int k=1; k<=currentheight; k++)
						{
							//~ if( (k<currentheight-1) || (k<currentheight && currentterrain==0) ) dontrender += ZMINUS;
							terrain[0]->display(current,CENTER + FLAT + AMBIENT + ZLIGHT);
							current.z -= blockdepth>>2;
						}
						terrain[0]->reset();
						dontrender = 0;
					}
					
					terrain[currentterrain]->update(linear);
					//~ if( (j>0                && currentterrain == levellayers[0][ii][j-1] && currentheight == levellayers[1][ii][j-1]) || j==0 ) dontrender += XMINUS;
					//~ if( (j<blocksperscreenx && currentterrain == levellayers[0][ii][j+1] && currentheight == levellayers[1][ii][j+1]) || j==blocksperscreenx ) dontrender += XPLUS;
					//~ if( (i>0                && currentterrain == levellayers[0][ii-1][j] && currentheight == levellayers[1][ii-1][j]) || i==-1 ) dontrender += YPLUS;
					//~ if( (ii<levelheight-1   && currentterrain == levellayers[0][ii+1][j] && currentheight == levellayers[1][ii+1][j]) || ii==levelheight-1 ) dontrender += YMINUS;
					terrain[currentterrain]->display(current,CENTER + FLAT + AMBIENT + ZLIGHT + dontrender);
					terrain[currentterrain]->reset();
					current.z = ckeeper.z;
					linear->unit();
					dontrender = 0;
				}
				ckeeper.x += blockwidth;
			}
		}
		}
		}
		ckeeper.x = -(XRES >> 1) + blockoffsetx;
		ckeeper.y -= blockheight;	
	}
	//*
	
	CLenemy* currentenemy;
	
	//cast shadows of entities
	player->display(1);
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		currentenemy = enemies->getcurrentdata();
		currentenemy->display(1);
	}
	boss->display(1);
	clscreen->clstencilbuffer.blendcopy(clscreen->cldoublebuffer.getbuffer(),9);
	//*

	//display player:
	player->display();
	//*
	
	//display enemies:
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		currentenemy = enemies->getcurrentdata();
		currentenemy->display();
	}
	//*
	
	//display boss
	boss->display();
	//*
	
	//display hud
	player->displayhud();
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext())
	{
		currentenemy = enemies->getcurrentdata();
		currentenemy->displayhud();
	}
	boss->displayhud();
	//*
}

void CLlevel::subsmark(xlong m) //! noncritical
{
	xlong sm = smoothmark - m;

	if( sm<smoothmarkmax && sm>smoothmarkmin )
	{
		smoothmark = sm;
		blockmark = smoothmark / blockheight;
	}
}

void CLlevel::setmark(xlong m) //! noncritical
{
	smoothmark = m;
	blockmark = smoothmark / blockheight;
}

void CLlevel::pause() { paused = !paused; } //! noncritical

void CLlevel::start() //! noncritical
{
	player->start();
	for(xlong i=enemies->setfirst(); i<enemies->getlength(); i+=enemies->setnext()) { enemies->getcurrentdata()->start(); }
	boss->start();
}
///*

#endif

