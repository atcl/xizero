//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLEVEL
#define HH_CLLEVEL
#pragma message "Compiling " __FILE__ " ! TODO: display crasht!"

#include "CLapi.hh"
#include "CLmath.hh"
#include "CLformat.hh"
#include "CLutils.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLplayer.hh"
#include "CLenemy.hh"
#include "CLlight.hh"


class CLlevel
{
	protected:
		CLformat* clformat;
		CLmath*   clmath;

		xlong*  cldouble;
		float* clzbuffer;
		xlong*  clstencil;

	private:
		static xlong version;
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


		CLmatrix* cllinear;
		CLmatrix* clshadow;
		CLlight*  cllight;

		CLplayer*  clplayer;
		CLenemy**  clenemy;
		CLobject** clterrain;

	public:
		CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* playdatlib, xchar* levelcontainer,CLformat* clf,CLmath* clm,xlong* cld,float* clz,xlong* cls);
		~CLlevel();

		void display();
		void subsmark(xlong m);
		xlong getversion();
};

xlong CLlevel::version = 0x00010000;

xlong CLlevel::levelwidth = 20;
xlong CLlevel::blockheight = 40;
xlong CLlevel::blockwidth = 40;
xlong CLlevel::blockdepth = 40;


CLlevel::CLlevel(xchar* terrainlib, xchar* enemylib, xchar* enedatlib, xchar* playerlib, xchar* playdatlib, xchar* levelcontainer,CLformat* clf,CLmath* clm,xlong* cld,float* clz,xlong* cls)
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
	xchar *terrainraw = CLgetcharfile_(terrainlib);
	xlong terrainsize = CLgetfilesize_(terrainlib);
	arfile *terraina = clformat->loadar(terrainraw,terrainsize);

	//convert array of y3ds to array of CLobjects
	clterrain = new CLobject*[terraina->filecount];
	for(int g=0; g<terraina->filecount; g++)
	{
		clterrain[g] = new CLobject(cldouble,clzbuffer,clstencil,terraina->members[g].filedata,400,300,100,clmath,clshadow,cllight);
	}

//level:
	//load levelmaps from .ar
	xchar *levelraw = CLgetcharfile_(levelcontainer);
	xlong levelsize = CLgetfilesize_(levelcontainer);
	arfile *levela = clformat->loadar(levelraw,levelsize);

	// //find terrain map, has to have extension .mapt
	xlong tf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h].filename,16,".mapt",5)==true)
		{
			tf=h;
			break;
		}
	}
	//tf holds index of armember terrain map
	if(tf==-1) CLexit_(__func__,"no terrain map found",1);
	//convert terrain map to 2d xchar array, and remove lineends
	xchar** terrainmap = map2array(levela->members[tf].filetext,levela->members[tf].filesizetext,levelwidth,33);
	//now terrain map holds 2d xchar array of terrain objects

	//determine level consts
	levelheight = getlinecount(levela->members[tf].filetext,levela->members[tf].filesizetext);
	blocksperscreeny = yres / blockheight;
	blocksperscreenx = xres / blockwidth;
	mark = levelheight - blocksperscreeny;
	smoothmark = mark * blockheight;
	smoothlevelheight = mark * blockheight;
	if(mark < 0) CLexit_(__func__,"Level too short",1);

	// //find height map, has to have extension .maph
	xlong hf = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h].filename,16,".maph",5)==true)
		{
			hf=h;
			break;
		}
	}
	//tf holds index of armember height map
	if(tf==-1) CLexit_(__func__,"no height map found",1);
	//convert height map to 2d xchar array, and remove lineends
	xchar** heightmap = map2array(levela->members[hf].filetext,levela->members[hf].filesizetext,levelwidth,48);
	//now height map holds 2d xchar array of height levels

	// //find entity map, has to have extension .mape
	xlong ef = -1;
	for(int h=0; h<levela->filecount; h++)
	{
		if(checkextension(levela->members[h].filename,16,".mape",5)==true)
		{
			ef=h;
			break;
		}
	}
	//tf holds index of armember entity map
	if(tf==-1) CLexit_(__func__,"no entitiy map found",1);
	//convert entity map to 2d xchar array, and remove lineends
	xchar** entitymap = map2array(levela->members[ef].filetext,levela->members[ef].filesizetext,levelwidth);
	//now entity map holds 2d xchar array of entity objects

	//build levellayerscontaining all sub maps
	levellayers = new xchar**[4];
	levellayers[0] = terrainmap;
	levellayers[1] = heightmap;
	levellayers[2] = entitymap;
	//levellayers[3] = specialmap //later when needed.
//***

// 	for(int s=0; s<levelheight ;s++)
// 	{
// 		for(int t=0; t<levelwidth ;t++)
// 		{
// 			cout << xchar(levellayers[0][s][t]+20) << " ";
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
CLprint_("basic loading done");

//enemies:
// 	//load enemylib from .ar to array of xlong* to .y3d files
// 	temp1 = CLgetfile_(enemylib);
// 	temp0 = CLgetfilesize_(enemylib);
// 	temp2 = clformat->loadar(temp1,temp0);
// 	temp5 = new CLobject*[temp2[0][0]];
// 	for(int h=0; h<temp2[0][0] ; h++)
// 	{
// 		temp5[h] = new CLobject(cldouble,clzbuffer,clstencil,temp2[h+1],0,0,0,clmath,clshadow,cllight);
// 	}
// 
// 
// 	//load enemydat from .ar to array of xlong* to .bcx files
// 	temp1 = CLgetfile_(enedatlib);
// 	temp0 = CLgetfilesize_(enedatlib);
// 	temp3 = clformat->loadar(temp1,temp0);
// 	temp4 = new xlong**[temp3[0][0]];
// 	for(int i=0; i<temp3[0][0]; i++)
// 	{
// 		temp0    = clformat->getlinecount(&temp3[i+1][1],temp3[i+1][0]);
// 		temp4[i] = clformat->loadbcx(&temp3[i+1][1],temp0);
// 	}
// 
// 	//convert arrays of y3d and bcx to array of clenemy
// 	clenemy = new CLenemy*[temp2[0][0]];
// 	for(int j=0; j<temp2[0][0]; j++)
// 	{
// 		clenemy[j] = new CLenemy(temp5[j],temp4[j]);
// 	}
//***	

//player:
// 	//load playerlib to xlong* to y3d objects
// 	temp1 = CLgetfile_(playerlib);
// 	temp0 = CLgetfilesize_(playerlib);
// 	temp2 = clformat->loadar(temp1,temp0);
// 	temp6 = new CLobject(cldouble,clzbuffer,clstencil,temp2[1],0,0,0,clmath,clshadow,cllight);
// 	
// 	//load playdat to xlong* to bcx
// 	temp1 = CLgetfile_(playdatlib);
// 	temp0 = CLgetfilesize_(playdatlib);
// 	temp3 = clformat->loadar(temp1,temp0);
// 	temp0 = clformat->getlinecount(&temp3[1][1],temp3[1][0]);
// 	temp2 = clformat->loadbcx(&temp3[1][1],temp0);
// 
// 	//convert y3d and bcx to clplayer
// 	clplayer = new CLplayer(temp6,temp2);

//***

	//hieghtmap element * 40 + terrain.z

	//set map start position
//***

}

CLlevel::~CLlevel() { }

void CLlevel::display()
{
	//berücksichtigen das m auch nicht auf ganzen block grenzen liegen muss.

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

xlong CLlevel::getversion()
{
	return version;
}

#endif
