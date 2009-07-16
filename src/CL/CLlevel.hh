//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLEVEL
#define HH_CLLEVEL
#pragma message "Compiling " __FILE__ " ! TODO: "

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
#include "CLlight.hh"
#include "CLmacros.hh"


class CLlevel : public virtual CLcl
{
	protected:
		CLmatrix*  cllinear;
		CLplayer*  clplayer;
		CLenemy**  clenemy;
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
		CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* levelcontainer);
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


CLlevel::CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* levelcontainer)
{
	cllinear = new CLmatrix(1);
	CLgame::init(60,0,740,600);

//terrain:
	//load terrainlib from .ar to array of xlong* to y3d objects
	CLfile* terrainraw = CLsystem::getfile(terrainlib);
	arfile* terraina = CLformat::loadar(terrainraw);
	//*

	//convert array of y3ds to array of CLobjects
	clterrain = new CLobject*[terraina->filecount];
	for(int g=0; g<terraina->filecount; g++)
	{
		clterrain[g] = new CLobject(terraina->members[g],1);
	}
	//*

//level:
	//load levelmaps from .ar
	CLfile* levelraw = CLsystem::getfile(levelcontainer);
	arfile* levela = CLformat::loadar(levelraw);
	//*

	//find terrain map, has to have extension .mapt
	xlong tf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(CLutils::checkextension(levela->members[h]->name,16,".mapt",5)==true)
		{
			tf=h;
			break;
		}
	}
	//tf holds index of armember terrain map

	if(tf==-1) CLsystem::exit(1,0,__func__,"no terrain map found");

	//convert terrain map to 2d xchar array, and remove lineends
	xchar** terrainmap = CLformat::loadmap(levela->members[tf],33,' ',-1);
	//now terrain map holds 2d xchar array of terrain objects

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

	//find height map, has to have extension .maph
	xlong hf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(CLutils::checkextension(levela->members[h]->name,16,".maph",5)==true)
		{
			hf=h;
			break;
		}
	}
	//tf holds index of armember height map

	if(tf==-1) CLsystem::exit(1,0,__func__,"no height map found");

	//convert height map to 2d xchar array, and remove lineends
	xchar** heightmap = CLformat::loadmap(levela->members[hf],48,' ',0);
	//now height map holds 2d xchar array of height levels

	//find entity map, has to have extension .mape
	xlong ef = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(CLutils::checkextension(levela->members[h]->name,16,".mape",5)==true)
		{
			ef=h;
			break;
		}
	}
	//tf holds index of armember entity map

	if(ef==-1) CLsystem::exit(1,0,__func__,"no entitiy map found");

	//convert entity map to 2d xchar array, and remove lineends
	xchar** entitymap = CLformat::loadmap(levela->members[ef],33,'.',-1);
	//now entity map holds 2d xchar array of entity objects

	//build levellayerscontaining all sub maps
	levellayers = new xchar**[4];
	levellayers[0] = terrainmap;
	levellayers[1] = heightmap;
	levellayers[2] = entitymap;
	//levellayers[3] = specialmap //later when needed.

//***

//level landscape generation:
	xlong templevelheight = smoothlevelheight;
	levellandscape = new CLfbuffer(smoothlevelwidth*templevelheight);
	levellandscape->clear(floorheight);
	screenside* templevelrside = new screenside[templevelheight];
	screenside* templevellside = new screenside[templevelheight];

	xlong currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1;
	xlong localfloorheight = floorheight - 5;
	CLlvector current(blockoffsetx,blockoffsety,localfloorheight);

	for(int i=0; i<levelheight; i++)
	{
		for(int j=0; j<levelwidth; j++)
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

	delete[] templevelrside;
	delete[] templevellside;
//*

//player:

	//load playerstuff 
	CLfile* playerraw = CLsystem::getfile(playerlib);
	arfile* playera = CLformat::loadar(playerraw);
// 	//*

	//find player definition, has to have extension .bcx
	xlong pd = -1;
	for(int h=0; h<playera->filecount; h++)
	{
		if(CLutils::checkextension(playera->members[h]->name,16,".bcx",4)==true)
		{
			pd=h;
			break;
		}
	}
	//pd holds index of player definition

	if(pd==-1) CLsystem::exit(1,0,__func__,"no player definition file found");

	xlong** playerd = CLformat::loadbcx(playera->members[pd]);

	//find player model part I, has to have extension .y3d
	xlong pm = -1;
	for(int h=0; h<playera->filecount; h++)
	{
		if(CLutils::checkextension(playera->members[h]->name,16,".y3d",4)==true)
		{
			pm=h;
			break;
		}
	}
	//pm holds index of first model

	if(pm==-1) CLsystem::exit(1,0,__func__,"no player model part I file found");

	CLobject* playerm = new CLobject(playera->members[pm],0);
	CLobject* playern = NULL; //2nd part of player model as soon as avail.

	bool startposfound = false;
	CLlvector playerp;
	for(int h=0; h<levelheight; h++)
	{
		for(int i=0; i<levelwidth; i++)
		{
			if(levellayers[2][h][i] == 0)
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

	clplayer = new CLplayer(playerm,playern,playerd,playerp,smoothmark);

//***

//***

//enemies: (test)
// 	//load enemylib from .ar
// 	xchar *enemyraw = CLgetcharfile_(enemylib);
// 	xlong enemysize = CLgetfilesize_(enemylib);
// 	arfile *enemya = clformat->loadar(enemyraw,enemysize);
// 	//*
// 
// 	//divide in y3d and data
// 	if(enemya->filecount%2 != 0) CLexit_(__func__,"unequal enemy models and enemy data",1);
// 	xlong ey = (enemya->filecount)>>1;
// 	xlong ed = ((enemya->filecount)>>1)+1;
// 	//*
// 
// 	//load enemies:
// 	clenemy = new CLenemy*[ey];
// 	CLobject** enemyy3d = new CLobject*[ey];
// 	xlong*** enemybcx = new xlong**[ey];
// 	for(int i=0; i<ey; i++, ed++)
// 	{
// 		enemyy3d[i] = new CLobject(cldouble,clzbuffer,clstencil,enemya->members[i].filedata,400,300,100,clmath,clshadow,cllight);
// 		enemybcx[i] = clformat->loadbcx(enemya->members[ed].filedata,enemya->members[ed].filesizedata);
// 		clenemy[i] = new CLenemy(enemyy3d[i],enemybcx[i]);
// 	}
// 	//*
//***	

//***

}

CLlevel::~CLlevel()
{
	delete clplayer;
}

void CLlevel::update(xchar input,xchar turbo)
{
	xlong tempy = 3*(yres>>2);
	xlong py = clplayer->gety();
	if( (py<smoothlevelheight+tempy) && (py>tempy) ) subsmark(smoothmark+tempy-py);

	clplayer->update(input,turbo,levellandscape,smoothmark);
}

void CLlevel::display()
{
	//display level:
	xlong currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1; 
	xlong yoffset = smoothmark % blockheight;
	xlong localfloorheight = floorheight - 5;
	CLlvector ckeeper( -(xres >> 1) + blockoffsetx , (yres >> 1) - blockoffsety + yoffset  + blockheight , localfloorheight );
	CLlvector current( 0 , 0 , localfloorheight );

	cllinear->unit();
	for(int i=-1; i<blocksperscreeny+2; i++)
	{
		if( !( i<0                   && blockmark<=0 ) )
		{
		if( !( i==blocksperscreeny   && smoothmark>=smoothmarkmax ) )
		{
		if( !( i==blocksperscreeny+1 && smoothmark>=smoothmarkmax-blockheight ) )
		{
			for(int j=0; j<blocksperscreenx; j++)
			{
				currentterrain = xlong(levellayers[0][blockmark+i][j]);
				if(currentterrain != -1)
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
	//

	//display player:
	clplayer->display();
	//
	
	//display enemies:
	
	//
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

