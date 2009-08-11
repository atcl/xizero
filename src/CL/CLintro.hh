//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
//#pragma message Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLapi.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLformat.hh"


namespace CLintro
{
	void atcrosslevel();
	void xizero();
}

void CLintro::atcrosslevel()
{
	//load atcrosslevel model
	CLfile *cf = CLsystem::getfile("../dat/cross.y3d");
	CLobject* cross = new CLobject(cf,0);
	CLfile *lf = CLsystem::getfile("../dat/level.y3d");
	CLobject* level0 = new CLobject(lf,0);
	CLobject* level1 = new CLobject(lf,0);
	CLobject* level2 = new CLobject(lf,0);
	CLobject* level3 = new CLobject(lf,0);
	CLobject* level4 = new CLobject(lf,0);
	CLobject* level5 = new CLobject(lf,0);
	CLobject* level6 = new CLobject(lf,0);
	CLobject* level7 = new CLobject(lf,0);
	CLobject* level8 = new CLobject(lf,0);
	CLobject* level9 = new CLobject(lf,0);
	//*
	
	//build scene
	
	//*
	
	//load animation file
	CLfile* aniraw = CLsystem::getfile("../data/other/CLintro.ani");
	xlong*  anicsv = CLformat::loadcsv(aniraw,',');
	//*

	//set animation attributes
	xlong anim_duration = anicsv[2];
	xlong anim_frames = anicsv[3];
	xlong* anim_array = &anicsv[4];
	xlong anim_index = 0;
	xlong anim_curr = 0;
	xlong anim_last = anim_duration / anim_frames;
	xlong anim_start = anim_curr = CLsystem::getmilliseconds();
	xlong anim_stop = anim_duration + anim_start;
	//*

	//run animation
	while( anim_curr <= anim_stop )
	{
		//check time
		anim_curr = CLsystem::getmilliseconds();
		anim_index = long((anim_curr - anim_start) / anim_duration);

		//transform
		
		//atcl->update(ltm);

		//clear buffers
		CLdoublebuffer->clear(0);
		CLzbuffer->clear(zres-1);
		CLstencilbuffer->clear(0);
		//*

		//display
		//atcl->display();
		//*

		//clear transformation matrix
		linearM->unit();
		//*
	}


	//clean up:
	delete cross;
	delete level0;
	delete level1;
	delete level2;
	delete level3;
	delete level4;
	delete level5;
	delete level6;
	delete level7;
	delete level8;
	delete level9;
	//*
}

void CLintro::xizero()
{

}

#endif
