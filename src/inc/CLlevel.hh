//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLEVEL
#define HH_CLLEVEL
#pragma message "Compiling " __FILE__ " ! TODO: display crasht!"

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
#include "CLlight.hh"
#include "CLmacros.hh"


class CLlevel : public virtual CLcl
{
	protected:
		CLformat* clformat;
		CLmath*   clmath;

		CLbuffer<xlong>*  cldouble;
		CLbuffer<float>* clzbuffer;
		CLbuffer<xlong>*  clstencil;

		CLmatrix* cllinear;
		CLmatrix* clshadow;
		CLlight*  cllight;

		CLplayer*  clplayer;
		CLenemy**  clenemy;
		CLobject** clterrain;

	private:
		static xlong levelwidth;
		static xlong blockheight;
		static xlong blockwidth;
		static xlong blockdepth;

		xchar*** levellayers;
		xlong levelheight;
		xlong blocksperscreeny;
		xlong blocksperscreenx;
		xlong mark;
		xlong smoothmark;
		xlong smoothlevelheight;

	public:
		CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* playdatlib, xchar* levelcontainer,CLformat* clf,CLmath* clm,CLbuffer<xlong>* cld,CLbuffer<float>* clz,CLbuffer<xlong>* cls);
		~CLlevel();

		void display();
		void subsmark(xlong m);
};

xlong CLlevel::levelwidth = 20; //in blocks
xlong CLlevel::blockheight = 40;
xlong CLlevel::blockwidth = 40;
xlong CLlevel::blockdepth = 40;


CLlevel::CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* playdatlib, xchar* levelcontainer,CLformat* clf,CLmath* clm,CLbuffer<xlong>* cld,CLbuffer<float>* clz,CLbuffer<xlong>* cls)
{
	clformat  = clf;
	clmath    = clm;
	cldouble  = cld;
	clzbuffer = clz;
	clstencil = cls;

	cllinear = new CLmatrix(1,clmath);
	clshadow = new CLmatrix(1,clmath);
	cllight  = new CLlight(1,0,1,1,0x00FFFFFF,clmath);

//terrain:
	//load terrainlib from .ar to array of xlong* to y3d objects
	CLfile* terrainraw = CLgetfile_(terrainlib);
	arfile* terraina = clformat->loadar(terrainraw);
	//*

	//convert array of y3ds to array of CLobjects
	clterrain = new CLobject*[terraina->filecount];
	for(int g=0; g<terraina->filecount; g++)
	{
		clterrain[g] = new CLobject(cldouble,clzbuffer,clstencil,terraina->members[g]->data,400,300,100,clmath,clshadow,cllight,1);
	}
	//*

//level:
	//load levelmaps from .ar
	CLfile* levelraw = CLgetfile_(levelcontainer);
	arfile* levela = clformat->loadar(levelraw);
	//*

	//find terrain map, has to have extension .mapt
	xlong tf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h]->name,16,".mapt",5)==true)
		{
			tf=h;
			break;
		}
	}
	//tf holds index of armember terrain map

	if(tf==-1) CLexit_(1,__func__,"no terrain map found");

	//convert terrain map to 2d xchar array, and remove lineends
	xchar** terrainmap = clformat->loadmap(levela->members[tf],33);
	//now terrain map holds 2d xchar array of terrain objects

	//determine level consts
	levelheight = getlinecount(levela->members[tf]);
	blocksperscreeny = yres / blockheight;
	blocksperscreenx = xres / blockwidth;
	mark = levelheight - blocksperscreeny;
	smoothmark = mark * blockheight;
	smoothlevelheight = mark * blockheight;
	if(mark < 0) CLexit_(1,__func__,"Level too short");
	//*

	//find height map, has to have extension .maph
	xlong hf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h]->name,16,".maph",5)==true)
		{
			hf=h;
			break;
		}
	}
	//tf holds index of armember height map

	if(tf==-1) CLexit_(1,__func__,"no height map found");

	//convert height map to 2d xchar array, and remove lineends
	xchar** heightmap = clformat->loadmap(levela->members[hf],48);
	//now height map holds 2d xchar array of height levels

	//find entity map, has to have extension .mape
	xlong ef = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h]->name,16,".mape",5)==true)
		{
			ef=h;
			break;
		}
	}
	//tf holds index of armember entity map

	if(ef==-1) CLexit_(1,__func__,"no entitiy map found");

	//convert entity map to 2d xchar array, and remove lineends
	xchar** entitymap = clformat->loadmap(levela->members[ef],33);
	//now entity map holds 2d xchar array of entity objects

	//build levellayerscontaining all sub maps
	levellayers = new xchar**[4];
	levellayers[0] = terrainmap;
	levellayers[1] = heightmap;
	levellayers[2] = entitymap;
	//levellayers[3] = specialmap //later when needed.

//***

//player:
CLprint_("player");
	//load playerstuff 
	CLfile* playerraw = CLgetfile_(playerlib);
	arfile* playera = clformat->loadar(playerraw);
// 	//*

// 	//find player definition, has to have extension .bcx
// 	xlong pd = -1;
// 	for(int h=0; h<playera->filecount; h++)
// 	{
// 		if(checkextension(playera->members[h]->name,16,".bcx",5)==true)
// 		{
// 			pd=h;
// 			break;
// 		}
// 	}
// 	//pd holds index of player definition
// 
// 	if(pd==-1) CLexit_(1,__func__,"no player definition file found");
// 
// 	xlong** playerd = clformat->loadbcx(playera->members[pd]);
// 
// 	//find player model part I, has to have extension .y3d
// 	xlong pm = -1;
// 	for(int h=0; h<playera->filecount; h++)
// 	{
// 		if(checkextension(playera->members[h]->name,16,".y3d",5)==true)
// 		{
// 			pm=h;
// 			break;
// 		}
// 	}
// 	//pm holds index of first model
// 
// 	if(pm==-1) CLexit_(1,__func__,"no player model part I file found");
// 
// 	CLobject* playerm = new CLobject(cldouble,clzbuffer,clstencil,playera->members[pm]->data,400,300,100,clmath,clshadow,cllight,1);
// 	CLobject* playern = NULL; //2nd part of player model as soon as avail.
// 
// 	//!todo: determine player startpos by whatever map holds startpos
// 	clplayer = new CLplayer(playerm,playern,playerd,0,0,0);
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

//player: (test)
// 	//load playerlib from .ar to array of xlong* to y3d objects
// 	xchar *playerraw = CLgetcharfile_(playerlib);
// 	xlong playersize = CLgetfilesize_(playerlib);
// 	arfile *playera = clformat->loadar(playerraw,playersize);
// 	//*
// 
// 	//find player model, has to have extension .y3d
// 	xlong py = -1;
// 	for(int h=0; h<playera->filecount; h++)
// 	{
// 		if(checkextension(playera->members[h].filename,16,".y3d",4)==true)
// 		{
// 			py=h;
// 			break;
// 		}
// 	}
// 	//py holds index of armember player model
// 
// 	if(py==-1) CLexit_(__func__,"no player model found",1);
// 
// 	//find player data, has to have extension .bcx
// 	xlong pd = -1;
// 	for(int h=0; h<playera->filecount; h++)
// 	{
// 		if(checkextension(playera->members[h].filename,16,".bcx",4)==true)
// 		{
// 			py=h;
// 			break;
// 		}
// 	}
// 	//pd holds index of armember player model
// 
// 	if(pd==-1) CLexit_(__func__,"no player data found",1);
// 
// 	//create player object
// 	CLobject* playery3d = new CLobject(cldouble,clzbuffer,clstencil,playera->members[py].filedata,400,300,100,clmath,clshadow,cllight);
// 	xlong** playerbcx = clformat->loadbcx(playera->members[pd].filedata,playera->members[pd].filesizedata);
// 	clplayer = new CLplayer(playery3d,playerbcx);
// 	//*
//***


	//set map start position
//***

}

CLlevel::~CLlevel() { }

void CLlevel::display()
{
	xchar currentterrain = 0;
	xchar currentheight = 0;
	xchar currententity = 0;
	xlong blockoffsetx = blockwidth >> 1;
	xlong blockoffsety = blockheight >> 1; 
	xlong yoffset = smoothmark % blockheight;	//block overhead of possibly not fully displayable block
	xlong currentx = -(xres >> 1) + blockoffsetx;
	xlong currenty = (yres >> 1) - blockoffsety + yoffset  + (blockheight); //source of dispplay errors in height
	xlong currentz = 0;
	xlong tempz = 0;

	cllinear->unit();
	for(int i=-1; i<blocksperscreeny+2; i++)
	{
		if( !( i<0                   && mark<=0 ) )
		{
		if( !( i==blocksperscreeny   && smoothmark>=smoothlevelheight ) )
		{
		if( !( i==blocksperscreeny+1 && smoothmark>=smoothlevelheight-blockheight ) )
		{
			for(int j=0; j<blocksperscreenx; j++)
			{
				currentterrain = levellayers[0][mark+i][j];
				if(currentterrain != -1)
				{
					//hier fehler bei subblocks und höhere leveln.
					currentheight = levellayers[1][mark+i][j];
					currententity = levellayers[2][mark+i][j];
					currentz = -currentheight * (blockdepth >> 2);
					cllinear->translate(currentx,currenty,0);
					clterrain[currentterrain]->update(cllinear);
					if(currentheight!=0)
					{
						//clterrain[0]->addpositionz(currentz);
						if(currentterrain==0) tempz = clterrain[currentterrain]->getpositionz();
						else clterrain[0]->update(cllinear);
						for(int k=1; k<=currentheight; k++)
						{
							clterrain[0]->display(1,1,1,0,0,0);
							clterrain[0]->addpositionz(blockdepth>>2);
						}
						if(currentterrain==0) clterrain[currentterrain]->setpositionz(tempz);
						else clterrain[0]->reset();
					}
					clterrain[currentterrain]->addpositionz(currentz);
					clterrain[currentterrain]->display(1,1,1,0,0,0);
					clterrain[currentterrain]->reset();
					cllinear->unit();
				}
				currentx += blockwidth;
			}
		}
		}
		}
		currentx = -(xres >> 1) + blockoffsetx;
		currenty -= blockheight;
		
	}

	//display player
}

void CLlevel::subsmark(xlong m)
{
	xlong sm = smoothmark - m;

	if( sm<smoothlevelheight && sm>0 )
	{
		smoothmark = sm;
		mark = smoothmark / blockheight;
	}
}

#endif

