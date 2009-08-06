//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLEVEL
#define HH_CLLEVEL
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"
#include "CLapi.hh"
#include "CLmath.hh"
#include "CLformat.hh"
#include "CLutils.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLplayer.hh"
#include "CLenemy.hh"
#include "CLgame.hh"
#include "CLmacros.hh"


class CLlevel : public virtual CLcl
{
	protected:
		CLmatrix*  linear;
		CLplayer*  player;
		CLlist*    enemies;
		CLobject** terrain;

		CLfbuffer* levellandscape;

	private:
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

	public:
		CLlevel(xchar* terrainlib, xchar* enemylib, xchar* playerlib, xchar* levelcontainer);
		~CLlevel();

		void update(xchar input,xchar turbo);
		void display();
		void subsmark(xlong m);
		void setmark(xlong m);
		xlong getmark();
		CLfbuffer* getlandscape();
		CLplayer* getplayer();
};

xlong CLlevel::levelwidth = 20; //in blocks
xlong CLlevel::blockheight = 40;
xlong CLlevel::blockwidth = 40;
xlong CLlevel::blockdepth = 40;
xlong CLlevel::floorheight = 100;


CLlevel::CLlevel(xchar* terrainlib, xchar* enemylib, xchar* playerlib, xchar* levelcontainer)
{
	//matrix for linear transformations of level objects
	linear = new CLmatrix(1);
	//*
	
	//set screen boundaries
	CLgame::init(60,0,740,600);
	//*


//terrain:

	//load terrainlib from .ar to array of xlong* pointing to y3d objects
	CLfile* terrainraw = CLsystem::getfile(terrainlib);
	arfile* terraina = CLformat::loadar(terrainraw);
	//*

	//use array of y3ds to  create array of CLobjects
	terrain = new CLobject*[terraina->filecount];
	for(uxlong g=0; g<terraina->filecount; g++)
	{
		terrain[g] = new CLobject(terraina->members[g],1);
	}
	//*
	
	//release loaded files
	delete terraina;
	delete terrainraw;
	//*

//***

//level:

	//load levelmaps from .ar
	CLfile* levelraw = CLsystem::getfile(levelcontainer);
	arfile* levela = CLformat::loadar(levelraw);
	//*
	
	//terrain map:
	xlong tf = CLutils::findarmember(levela,".mapt");
	if(tf==-1) CLsystem::exit(1,0,__func__,"no terrain map found");
	xchar** terrainmap = CLformat::loadmap(levela->members[tf],33,' ',-1);
	//**

	//determine level constants
	levelheight = CLutils::getlinecount(levela->members[tf]);
	blocksperscreeny = yres / blockheight;
	blocksperscreenx = xres / blockwidth;
	blockmark = levelheight - blocksperscreeny;
	smoothmarkmin = 0;
	smoothmarkmax = blockmark * blockheight;
	smoothmark = smoothmarkmax;
	smoothlevelheight = levelheight * blockheight;
	smoothlevelwidth = levelwidth * blockwidth;
	if(blockmark < 0) CLsystem::exit(1,0,__func__,"Level too short");
	//*

	//height map:
	xlong hf = CLutils::findarmember(levela,".maph");
	if(hf==-1) CLsystem::exit(1,0,__func__,"no height map found");
	xchar** heightmap = CLformat::loadmap(levela->members[hf],48,' ',0);
	//**

	//entity map:
	xlong ef = CLutils::findarmember(levela,".mape");
	if(ef==-1) CLsystem::exit(1,0,__func__,"no entity map found");
	xchar** entitymap = CLformat::loadmap(levela->members[ef],34,'.',-2);
	//**

	//build levellayerscontaining all sub maps
	levellayers = new xchar**[4];
	levellayers[0] = terrainmap;
	levellayers[1] = heightmap;
	levellayers[2] = entitymap;
	//levellayers[3] = specialmap //later when needed.
	
	//release loaded files
	delete levela;
	delete levelraw;
	//*

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

	//the landscape screenside can retire now
	delete[] templevelrside;
	delete[] templevellside;
	//*
	
//***

//player:

	//search player start pos and set player pos to it
	bool startposfound = false;
	CLlvector playerp;
	for(uxlong h=0; h<levelheight; h++)
	{
		for(uxlong i=0; i<levelwidth; i++)
		{
			if(levellayers[2][h][i] == -1)
			{
				startposfound = true;
				playerp.x = i * blockwidth;
				playerp.y = h * blockheight;
				playerp.z = levellayers[1][h][i] * blockdepth;
				break;
			}
		}
	}
	if(startposfound==false) CLsystem::exit(1,0,__func__,"no player start position found in entity map");
	//*

	//create player
	player = new CLplayer(playerlib,playerp);
	playerscreenylevel = 3*(yres>>2);
	//*

//***

//enemies:

	//load enemy archive
	CLfile* enemiesraw = CLsystem::getfile(enemylib);
	arfile* enemiesa = CLformat::loadar(enemiesraw);
	//*
	
	//load all enemies in archive (base enemies) into array
	xlong differentenemies = enemiesa->filecount;
	CLenemy** baseenemies = new CLenemy*[differentenemies];
	for(uxlong k=0; k<enemiesa->filecount; k++)
	{
		baseenemies[k] = new CLenemy(enemiesa->members[k]);
	}
	//*
	
	//release loaded files
	delete enemiesa;
	delete enemiesraw;
	//*
	
	//create all enemies in level through copying from base enemies from archive
	//find enemy startpos in entity map and associate (copy) from baseenemy in enemy list
	enemies = new CLlist();
	CLenemy* currentenemy;
	CLlvector enemyp;
	for(uxlong l=0; l<differentenemies; l++)
	{ 
		for(uxlong m=0; m<levelheight; m++)
		{
			for(uxlong n=0; n<levelwidth; n++)
			{
				if(levellayers[2][m][n] == l)
				{
					enemyp.x = n * blockwidth;
					enemyp.y = m * blockheight;
					enemyp.z = levellayers[1][m][n];
					currentenemy = new CLenemy(baseenemies[l],enemyp);
					enemies->append(currentenemy);
				}
			}
		}
	}
	//*

//***
}

CLlevel::~CLlevel()
{
	delete player;
	delete linear;
	delete enemies;
	delete levellandscape;
	delete[] terrain;
}

void CLlevel::update(xchar input,xchar turbo)
{
	//update player
	player->update(input,turbo,levellandscape,smoothmark);
	//*
	
	//update enemies
	CLenemy* currentenemy;
	xlong    isenemydead;
	for(uxlong i=0; i<enemies->getlength();i++)
	{
		enemies->setindex(i);
		currentenemy = static_cast<CLenemy*>(enemies->getcurrentdata());
		isenemydead = currentenemy->update(smoothmark,player);
		if(isenemydead!=0)
		{
			delete static_cast<CLenemy*>(enemies->delcurrent(1));
		}
	}
	//*
	
	//adjust section of level to be displayed by ("new") player position
	xlong py = player->gety();
	if(py<playerscreenylevel) setmark(smoothmarkmin);
	else if(py>(smoothmarkmax+playerscreenylevel)) setmark(smoothmarkmax);
	else setmark(py - playerscreenylevel);
	//*
}

void CLlevel::display()
{
	//set variables controlling render loop
	xlong currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1; 
	xlong yoffset = smoothmark % blockheight;
	xlong localfloorheight = floorheight - 5;
	CLlvector ckeeper( -(xres >> 1) + blockoffsetx , (yres >> 1) - blockoffsety + yoffset  + blockheight , localfloorheight );
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
		ckeeper.x = -(xres >> 1) + blockoffsetx;
		ckeeper.y -= blockheight;	
	}
	//*
	
	CLenemy* currentenemy;
	
	//cast shadows of entities
	player->shadow(smoothmark);
	for(uxlong i=0; i<enemies->getlength();i++)
	{
		enemies->setindex(i);
		currentenemy = static_cast<CLenemy*>(enemies->getcurrentdata());
		currentenemy->shadow(smoothmark);
	}
	CLstencilbuffer->blendcopy(CLdoublebuffer->getbuffer(),4);
	//*

	//display player:
	player->display(smoothmark);
	//*
	
	//display enemies:
	for(uxlong i=0; i<enemies->getlength();i++)
	{
		enemies->setindex(i);
		currentenemy = static_cast<CLenemy*>(enemies->getcurrentdata());
		currentenemy->display(smoothmark);
	}
	//*
}

void CLlevel::subsmark(xlong m)
{
	xlong sm = smoothmark - m;

	if( sm<smoothmarkmax && sm>smoothmarkmin )
	{
		smoothmark = sm;
		blockmark = smoothmark / blockheight;
	}
}

void CLlevel::setmark(xlong m)
{
	smoothmark = m;
	blockmark = smoothmark / blockheight;
}

xlong CLlevel::getmark()
{
	return smoothmark;
}

CLfbuffer* CLlevel::getlandscape()
{
	return levellandscape;	
}

CLplayer* CLlevel::getplayer()
{
	return player;
}

#endif

