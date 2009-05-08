//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#pragma message "Compiling " __FILE__ " ! TODO: test on models"

#include "CLtypes.hh"
#include "CLapi.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"


//prototypes:
void atcrosslevel_intro();
void xizero_intro();
//


void atcrosslevel_intro()
{
	//animation variables

		//animation duration in seconds
		xlong anim_duration = 10;

		//framerate (in frames per second)
		xlong anim_frames = 20;

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
	CLfile *atcl_f;
	atcl_f = CLgetfile_("../dat/atcl.y3d");
	CLobject* atcl;
	atcl = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,atcl_f,400,300,100,clmath,bsm,amb,0);

	xlong anim_start = CLgetmilliseconds_();
	anim_stop += anim_start;

	//run animation
	while( anim_current <= anim_stop )
	{
		//check time
		anim_current = CLgetmilliseconds_();
		anim_index = long((anim_current - anim_start) / anim_length);

		//transform
		
		atcl->update(ltm);

		//clear buffers
		CLdoublebuffer->clear(0);
		CLzbuffer->clear(zmax);
		CLstencilbuffer->clear(0);

		//display
		atcl->display(1,0,0,0,0,0);

		//clear transformation matrix
		ltm->unit();
	}
}

void xizero_intro()
{

}

#endif
