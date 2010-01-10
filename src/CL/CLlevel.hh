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
		static CLsystem* clsystem; //temp!
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
		CLlevel(CLfile* map,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,xlong bosstype);
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
CLsystem* CLlevel::clsystem = CLsystem::instance();
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
	if(tf==0) err(__FILE__,__func__,u8"no terrain map found");
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
	if(blockmark < 0) err(__FILE__,__func__,u8"Level too short");
	//*

	//height map:
	CLfile* hf = levela->findbyextension(u8".maph");
	if(hf==0) err(__FILE__,__func__,u8"no height map found");
	xchar** heightmap = clformat->loadmap(hf,48,' ',0);
	//*

	//entity map:
	CLfile* ef = levela->findbyextension(u8".mape");
	if(ef==0) err(__FILE__,__func__,u8"no entity map found");
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
	if(startposfound==false) err(__FILE__,__func__,u8"no player start position found in entity map");
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
	if(bossposfound==false) err(__FILE__,__func__,u8"no boss position found in entity map");
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

CLlevel::CLlevel(CLfile* map,CLfile* enemylib,CLfile* playerlib,CLfile* bosslib,xlong bosstype)
{
	//load level map
	CLfile* testim24 = clsystem->getfile("dat/maps/test.im24");
	sprite* testlevel = clformat->loadras(testim24);
	
		//load heightmap (plain)
		xlong rows = testlevel->height;
		xlong cols = testlevel->width;
		uxlong* hmap = testlevel->data;
		doubleword currz = { 0 };
		doubleword lastz = { 0 };
		xlong vertcount = 0;
		xlong polycount = 0;
		xlong opolycount = 0;
		xlong oldpolycount = 0;
		xlong ooldpolycount = 0;
		rawpoly* polys = new rawpoly[cols*3];
		rawpoly* opolys = new rawpoly[cols*3];
		
		CLobject** terrows = new CLobject*[rows-1];
		
		xlong jj = 0;
		xlong kk = 0;
		xlong j = 0;
		xlong k = 0;
		xlong l = 0;
		
		xlong x0 = 0;
		xlong x1 = 0;
		xlong x2 = 0;
		xlong x3 = 0;
		xlong xz = 0;
		
		xlong o0 = 0;
		xlong o1 = 0;
		xlong o2 = 0;
		xlong o3 = 0;
		xlong oz = 0;
		
		bool resetx2 = 0;
		bool resetx3 = 0;
		bool resetx22 = 0;
		xlong y1 = 0;
		xlong y2 = 0;

		//check each row of pixels
		for(xlong i=0; i<rows-1; i++)
		{
			while(o1<cols && o2<cols)
			{
				//set first vertex
				x0 = o1;
				currz.dd = hmap[i*cols+x0];
				xz = currz.db[2];
				polys[polycount].v[0].x = (x0*20)-400;
				polys[polycount].v[0].y = 10;
				polys[polycount].v[0].z = -(xz/4);
				//*
				
				//find second vertex
				if(x0<cols)
				{
					for(x1=x0; x1<cols; x1++) //->cols-1 
					{
						currz.dd = hmap[i*cols+x1];
						if(currz.db[2]!=xz) break;
					}
					
					//inter gap
					for(y1=x1; y1<cols; y1++)
					{
						currz.dd = hmap[(i*cols)+y1];
						if(currz.db[2]==xz)
						{
							resetx22 = 1;
							break;
						}
					}
					//*
				}
				else
				{
					x1 = x0;
				}
				//*
				
				//set second vertex
				polys[polycount].v[1].x = (x1*20)-400;
				polys[polycount].v[1].y = 10;
				polys[polycount].v[1].z = -(xz/4);
				//*
				
				//find third vertex
				currz.dd = hmap[((i+1)*cols)+x0];
				if(currz.db[2]==xz)
				{
					for(x3=x0; x3>o2; x3--)
					{
						currz.dd = hmap[((i+1)*cols)+x3];
						if(currz.db[2]!=xz) break;
					}
				}
				else
				{
					for(x3=x0; x3<cols; x3++)
					{
						currz.dd = hmap[((i+1)*cols)+x3];
						if(currz.db[2]==xz) break;
					}
					if(x3>x1)
					{
						x3 = x0;
						currz.dd = hmap[((i+1)*cols)+x3];
						xz = currz.db[2];
						polys[polycount].v[0].z = -(xz/4);
						polys[polycount].v[1].z = -(xz/4);
						resetx2 = 1;
					}
				}
				//*

				//set third vertex
				polys[polycount].v[3].x = (x3*20)-400;
				polys[polycount].v[3].y = -10;
				polys[polycount].v[3].z = -(xz/4);
				//*
				
				//find fourth vertex
				if(resetx2==0)
				{
					for(x2=x3; x2<cols; x2++)
					{
						currz.dd = hmap[((i+1)*cols)+x2];
						if(currz.db[2]!=xz) break;
					}
					
					//inter gap
					if(resetx3!=0)
					{
						x3 = x0;
						polys[polycount].v[3].x = (x3*20)-400;
						resetx3 = 0;
					}
					
					for(y2=x2; y2<cols; y2++)
					{
						currz.dd = hmap[((i+1)*cols)+y2];
						if(currz.db[2]==xz && y2<x1)
						{
							x1 = x2;
							polys[polycount].v[1].x = (x1*20)-400;
							resetx3 = 1;
							break;
						}
					}
					
					if(resetx22==1 && y1<x2)
					{
						x2 = x1;
						resetx22 = 0;
					}
					//*
				}
				else
				{
					x2 = x1;
				}
				//*
				
				//set fourth vertex
				polys[polycount].v[2].x = (x2*20)-400;
				polys[polycount].v[2].y = -10;
				polys[polycount].v[2].z = -(xz/4);
				//*
				
				//fix start and end of rows
				if(x0==0)
				{
					polys[polycount].v[3].x = -400;
				}
				//~ else if(x1==cols-1)
				//~ {
					//~ polys[polycount].v[2].x = (x1*20)-400;
				//~ }
				//~ else if(x2==cols-1)
				//~ {
					//~ polys[polycount].v[1].x = (x2*20)-400;
				//~ }
				//*
				
				//merge if mergeable with previous polygon and prepare for next polygon
				if(oz==xz && o1==x0 && o2==x3)
				{
					polys[polycount-1].v[1].x = polys[polycount].v[1].x;
					polys[polycount-1].v[2].x = polys[polycount].v[2].x;
					o1 = x1;
					o2 = x2;
				}
				else
				{
					polycount++;
					o0 = x0;
					o1 = x1;
					o2 = x2;
					o3 = x3;
					oz = xz;
				}
					
				resetx2 = 0;
				//*
			}
			
			//insert horizontal connecting polygons
			oldpolycount = polycount;
			for(xlong j=1; j<oldpolycount; j++)
			{
				polys[polycount].v[0].x = polys[j-1].v[1].x;
				polys[polycount].v[1].x = polys[j].v[0].x;
				polys[polycount].v[2].x = polys[j].v[3].x;
				polys[polycount].v[3].x = polys[j-1].v[2].x;
				
				polys[polycount].v[0].y = 10;
				polys[polycount].v[1].y = 10;
				polys[polycount].v[2].y = -10;
				polys[polycount].v[3].y = -10;
				
				polys[polycount].v[0].z = polys[j-1].v[0].z;
				polys[polycount].v[1].z = polys[j].v[0].z;
				polys[polycount].v[2].z = polys[j].v[0].z;
				polys[polycount].v[3].z = polys[j-1].v[0].z;
				
				polycount++;
			}
			//*
			
			//insert lower vertical connecting polygons
			if(i>0 && i<(rows-1))
			{
				xlong cp0 = 0;
				xlong cp1 = ooldpolycount - 1;
				xlong cp = 0;
				
				for(xlong k=0; k<ooldpolycount; k++)
				{
					//~ while(cp0 < ooldpolycount) { if(polys[cp0].v[0].x <= opolys[k].v[3].x && polys[cp0+1].v[0].x > opolys[k].v[3].x) { break; } cp0++; }
					//~ while(cp1 >= 0) { if(polys[cp1].v[1].x >= opolys[k].v[2].x && polys[cp1-1].v[1].x < opolys[k].v[3].x) { break; } cp1--; }
					//~ 
					//~ if(opolys[k].v[0].z != polys[cp0].v[0].z || opolys[k].v[0].z != polys[cp1].v[0].z)
					//~ {
						//~ cp = cp0;
						//~ if( polys[cp1].v[0].z < polys[cp0].v[0].z) cp = cp1;
						//~ 
						//~ polys[polycount].v[0].x = opolys[k].v[3].x;
						//~ polys[polycount].v[1].x = opolys[k].v[2].x;
						//~ polys[polycount].v[2].x = opolys[k].v[2].x;
						//~ polys[polycount].v[3].x = opolys[k].v[3].x;
						//~ 
						//~ polys[polycount].v[0].y = 10;
						//~ polys[polycount].v[1].y = 10;
						//~ polys[polycount].v[2].y = 10;
						//~ polys[polycount].v[3].y = 10;
						//~ 
						//~ polys[polycount].v[0].z = opolys[k].v[0].z;
						//~ polys[polycount].v[1].z = opolys[k].v[0].z;
						//~ polys[polycount].v[2].z = polys[cp].v[0].z;
						//~ polys[polycount].v[3].z = polys[cp].v[0].z;
						//~ 
						//~ polycount++;
					//~ }
					//~ 
					//~ cp0 = 0;
					//~ cp1 = ooldpolycount - 1;
				//~ }
				
				//~ for(xlong k=0; k<oldpolycount; k++)
				//~ {
					//~ while(cp0 < oldpolycount) { if(polys[cp0].v[0].x <= opolys[k].v[3].x && polys[cp0+1].v[0].x > opolys[k].v[3].x) { break; } cp0++; }
					//~ while(cp1 >= 0) { if(polys[cp1].v[1].x >= opolys[k].v[2].x && polys[cp1-1].v[1].x < opolys[k].v[3].x) { break; } cp1--; }
					//~ 
					//~ if(opolys[k].v[0].z != polys[cp0].v[0].z || opolys[k].v[0].z != polys[cp1].v[0].z)
					//~ {
						//~ cp = cp0;
						//~ if( polys[cp1].v[0].z < polys[cp0].v[0].z) cp = cp1;
						//~ 
						//~ polys[polycount].v[0].x = polys[k].v[0].x;
						//~ polys[polycount].v[1].x = polys[k].v[1].x;
						//~ polys[polycount].v[2].x = polys[k].v[1].x;
						//~ polys[polycount].v[3].x = polys[k].v[0].x;
						//~ 
						//~ polys[polycount].v[0].y = 10;
						//~ polys[polycount].v[1].y = 10;
						//~ polys[polycount].v[2].y = 10;
						//~ polys[polycount].v[3].y = 10;
						//~ 
						//~ polys[polycount].v[0].z = polys[k].v[0].z;
						//~ polys[polycount].v[1].z = ppolys[k].v[0].z;
						//~ polys[polycount].v[2].z = opolys[cp].v[0].z;
						//~ polys[polycount].v[3].z = opolys[cp].v[0].z;
				//~ 
						//~ polycount++;
					//~ }
					//~ 
					//~ cp0 = 0;
					//~ cp1 = oldpolycount - 1;
				}
			}
			//*
			
			terrows[i] = new CLobject(polys,polycount,0x000000FF,0);
			for(xlong l=0; l<polycount; l++) { opolys[l] = polys[l]; }
			opolycount = polycount;
			ooldpolycount = oldpolycount;
			polycount = x0 = x1 = x2 = x3 = xz = o0 = o1 = o2 = o3 = oz = y1 = y2 = 0;
		}
		//*
		
		//load heightmap (ramps)
		
		//*
		
		//load entites
		
		//*
	
	//*
	
	//load player
	
	//*
	
	//load enemies
	
	//*
	
	//load boss
	
	//*
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
	clscreen->clstencilbuffer.copy(&clscreen->cldoublebuffer,12);
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

