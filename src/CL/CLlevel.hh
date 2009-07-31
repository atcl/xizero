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
		CLmatrix*  cllinear;
		CLplayer*  clplayer;
		CLlist*    clenemy;
		CLobject** clterrain;

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

	public:
		CLlevel(xchar* terrainlib, xchar* enemylib, xchar* playerlib, xchar* levelcontainer);
		~CLlevel();

		void update(xchar input,xchar turbo);
		void display();
		void subsmark(xlong m);
		void setmark(xlong m);
		xlong getmark();
		CLfbuffer* getlandscape();
};

xlong CLlevel::levelwidth = 20; //in blocks
xlong CLlevel::blockheight = 40;
xlong CLlevel::blockwidth = 40;
xlong CLlevel::blockdepth = 40;
xlong CLlevel::floorheight = 100;


CLlevel::CLlevel(xchar* terrainlib, xchar* enemylib, xchar* playerlib, xchar* levelcontainer)
{
	//matrix for linear transformations of level objects
	cllinear = new CLmatrix(1);
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
	clterrain = new CLobject*[terraina->filecount];
	for(uxlong g=0; g<terraina->filecount; g++)
	{
		clterrain[g] = new CLobject(terraina->members[g],1);
	}
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
	xchar** entitymap = CLformat::loadmap(levela->members[ef],34,' ',-1);
	//**

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
						clterrain[0]->display(current,templevellside,templevelrside,levellandscape,templevelheight);
						current.z -= blockdepth>>2;
					}
				}

				clterrain[currentterrain]->display(current,templevellside,templevelrside,levellandscape,templevelheight);
				clterrain[currentterrain]->reset();
				cllinear->unit();
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
	clplayer = new CLplayer(playerlib,playerp);
	//*

//***

//enemies:

	//load enemy archive
	CLfile* enemyraw = CLsystem::getfile(enemylib);
	arfile* enemya = CLformat::loadar(enemyraw);
	//*
	
	//load all enemies in archive (base enemies) into list
	clenemy = new CLlist();
	CLenemy* tempenemy;
	startposfound = 0;
	CLlvector enemyp;
	for(uxlong k=0; k<enemya->filecount; k++)
	{
		//find enemy startpos with index fitting current enemy indey in enemya
		for(uxlong l=0; l<levelheight; l++)
		{
			for(uxlong m=0; m<levelwidth; m++)
			{
				if(levellayers[2][l][m] == k)
				{
					startposfound = true;
					enemyp.x = m * blockwidth;
					enemyp.y = l * blockheight;
					enemyp.z = levellayers[1][l][m] * blockdepth + 100; //correct or - 100 ???
					break;
				}
			}
		}
		if(startposfound==0) CLsystem::exit(1,0,__func__,"no (suitable) enemy start position found in entity map");
		//*

		tempenemy = new CLenemy(enemya->members[k],enemyp);
		clenemy->append(tempenemy);

		startposfound = 0;
	}
	//*
	
	//create all enemies in level through copying from base enemies from archive

	//...

//***
}

CLlevel::~CLlevel()
{
	delete clplayer;
	delete cllinear;
	delete levellandscape;
}

void CLlevel::update(xchar input,xchar turbo)
{
	//update player
	clplayer->update(input,turbo,levellandscape,smoothmark);
	//*
	
	//update enemies
	
	//*
	
	//adjust section of level t be displayed by ("new") player position
	xlong tempy = 3*(yres>>2);
	xlong py = clplayer->gety();
	if(py<tempy) setmark(smoothmarkmin);
	else if(py>(smoothmarkmax+tempy)) setmark(smoothmarkmax);
	else setmark(py - tempy);
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
	//*

	//render level to screen block by block
	cllinear->unit();
	for(int i=-1; i<blocksperscreeny+2; i++)
	{
		if( !( i<0                   && blockmark<=0 ) )
		{
		if( !( i==blocksperscreeny   && smoothmark>=smoothmarkmax ) )
		{
		if( !( i==blocksperscreeny+1 && smoothmark>=smoothmarkmax-blockheight ) )
		{
			for(uxlong j=0; j<blocksperscreenx; j++)
			{
				currentterrain = xlong(levellayers[0][blockmark+i][j]);
				if(currentterrain!=-1)
				{
					currentheight = levellayers[1][blockmark+i][j];
					currententity = levellayers[2][blockmark+i][j];
					cllinear->translate(ckeeper.x,ckeeper.y,0);

					if(currentheight!=0)
					{
						clterrain[0]->update(cllinear);
						for(int k=1; k<=currentheight; k++)
						{
							clterrain[0]->display(current,CENTER + FLAT + AMBIENT);
							current.z -= blockdepth>>2;
						}
						clterrain[0]->reset();
					}
					
					clterrain[currentterrain]->update(cllinear);
					clterrain[currentterrain]->display(current,CENTER + FLAT + AMBIENT);
					clterrain[currentterrain]->reset();
					current.z = ckeeper.z;
					cllinear->unit();
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

	//display player:
	clplayer->display(smoothmark);
	//*
	
	//display enemies:
		//...
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

#endif

