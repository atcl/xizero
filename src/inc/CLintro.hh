//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#warning Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLapi.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"


//prototypes:
void atcrosslevel_intro();
void xizero_intro();
//

//animation file format (csv): [all transformations are scene wide meaning acting on all objects???]
//length,fps
//move-x (length-times values relative)
//move-y (length-times values relative)
//move-z (length-times values relative)
//trans-x (length-times values relative)
//trans-y (length-times values relative)
//trans-z (length-times values relative)
//rot-x (length-times values relative)
//rot-y (length-times values relative)
//rot-z (length-times values relative)
//scl-x (length-times values relative)
//scl-y (length-times values relative)
//scl-z (length-times values relative)
//*

void atcrosslevel_intro()
{
	//animation variables

		//animation duration in seconds
		xlong anim_duration = 10;

		//framerate (in frames per second)
		xlong anim_frames = 15;

		//current frame initialize
		float anim_current = 0;
		xlong  anim_index = 0;

		//animation length
		float anim_length = (anim_duration * anim_frames * 1000);
		float anim_stop = (anim_duration * anim_frames * 1000); //final stop value later

		//arrays for x,y,z translates per step
		xlong *x_trn = new xlong[anim_duration*anim_frames];
		xlong *y_trn = new xlong[anim_duration*anim_frames];
		xlong *z_trn = new xlong[anim_duration*anim_frames];

		//arrays for x,y,z rotations per step
		xlong *x_rot = new xlong[anim_duration*anim_frames];
		xlong *y_rot = new xlong[anim_duration*anim_frames];
		xlong *z_rot = new xlong[anim_duration*anim_frames];

		//arrays for x,y,z scales per step
		xlong *x_scl = new xlong[anim_duration*anim_frames];
		xlong *y_scl = new xlong[anim_duration*anim_frames];
		xlong *z_scl = new xlong[anim_duration*anim_frames];

	//load atcrosslevel model
	CLfile *cf = CLgetfile_("../dat/cross.y3d");
	CLobject* cross = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,cf,400,300,100,clmath,bsm,amb,0);
	CLfile *lf = CLgetfile_("../dat/level.y3d");
	CLobject* level0 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level1 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level2 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level3 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level4 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level5 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level6 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level7 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level8 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);
	CLobject* level9 = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,lf,400,300,100,clmath,bsm,amb,0);

	//build scene

	xlong anim_start = CLgetmilliseconds_();
	anim_stop += anim_start;

	//run animation
	while( anim_current <= anim_stop )
	{
		//check time
		anim_current = CLgetmilliseconds_();
		anim_index = long((anim_current - anim_start) / anim_length);

		//transform
		
		//atcl->update(ltm);

		//clear buffers
		CLdoublebuffer->clear(0);
		CLzbuffer->clear(zres-1);
		CLstencilbuffer->clear(0);

		//display
		//atcl->display(1,0,0,0,0,0);

		//clear transformation matrix
		ltm->unit();
	}


	//clean up:
	delete[] x_trn;
	delete[] y_trn;
	delete[] z_trn;

	delete[] x_rot;
	delete[] y_rot;
	delete[] z_rot;

	delete[] x_scl;
	delete[] y_scl;
	delete[] z_scl;

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
}

void xizero_intro()
{

}

#endif
