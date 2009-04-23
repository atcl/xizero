//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#pragma message "Compiling " __FILE__ " ! TODO: test on models"

#include "CLapi.hh"
#include "CLbuffer"
#include "CLobject.hh"
#include "CLmatrix.hh"


void atcrosslevelintro()
{
	//animation variables

		//animation duration in seconds
		float anim_duration = 10;

		//framerate (in frames per second)
		float anim_frames = 20;

		//current frame initialize
		float anim_current = 0;
		xlong  anim_index = 0;

		//animation length
		float anim_length = (anim_duration * anim_frames * 1000);
		float anim_stop = (anim_duration * anim_frames * 1000); //final stop value later

		//arrays for x,y,z translates per step
		xlong *x_trn = new long[anim_duration*anim_frames];
		xlong *y_trn = new long[anim_duration*anim_frames];
		xlong *z_trn = new long[anim_duration*anim_frames];

		//arrays for x,y,z rotations per step
		xlong *x_rot = new long[anim_duration*anim_frames];
		xlong *y_rot = new long[anim_duration*anim_frames];
		xlong *z_rot = new long[anim_duration*anim_frames];

		//arrays for x,y,z scales per step
		xlong *x_scl = new long[anim_duration*anim_frames];
		xlong *y_scl = new long[anim_duration*anim_frames];
		xlong *z_scl = new long[anim_duration*anim_frames];

	//load atcrosslevel model
	xlong *atcl_f;
	atcl_f = CLgetfile_("../dat/atcl.y3d");
	CLobject* atcl;
	atcl = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,atcl_f,400,300,100,clmath,bsm,amb);

	anim_start = CLgettime();
	anim_stop += anim_start;

	//run animation
	while( anim_current =< anim_stop )
	{
		//check time
		anim_current = CLgettime();
		anim_index = long((anim_current - anim_start) / anim_length)

		//transform
		
		atcl->update(ltm);

		//clear buffers
		clbuffer->clear(0,CLdoublebuffer,CLscreenpixsize);
		clbuffer->clear(zmax,CLzbuffer,CLscreenpixsize);
		clbuffer->clear(0,CLstencilbuffer,CLscreenpixsize);

		//display
		atcl->display(1,0,0,0,0)

		//clear transformation matrix
		ltm->unit();
	}
}

void xizerointro()
{

}

#endif
