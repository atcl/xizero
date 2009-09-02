//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
//#pragma message Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLsystem.hh"
#include "CLbuffer.hh"
#include "CLobject.hh"
#include "CLmatrix.hh"
#include "CLformat.hh"


namespace CLintro
{
	CLmatrix* linear;
	
	void atcrosslevel();
	void xizero();
}

void CLintro::atcrosslevel()
{
	//make matrix
	linear = new CLmatrix(1);
	//*
	
	//load atcrosslevel model
	CLfile *cf = CLsystem::getfile("../dat/atcl.y3d");
	CLobject* atcl = new CLobject(cf,0);
	//*
	
	//load animation file
	CLfile* aniraw = CLsystem::getfile("../data/other/CLintro.ani");
	xlong*  anicsv = CLformat::loadcsv(aniraw,',');
	//*

	//set animation attributes
	CLlvector anim_pos(0,0,0);
	xlong anim_steps = (anicsv[0])/13;
	xlong anim_pointer = 0;
	float anim_step_dur = 0;
	float anim_div_temp = 1;
	xlong anim_curr_time = 0;
	xlong anim_start_time = 0;
	xlong anim_last_time = 0;
	xlong anim_time_diff = 0;
	float anim_units[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float anim_curr_trans[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//*
	
	//say(anim_steps);
	
	//run animation
	for(uxlong i=0; i<anim_steps; i++)
	{
		anim_step_dur = float(anicsv[anim_pointer]);
		anim_div_temp = 1/anim_step_dur;
		
		if(anim_step_dur!=0)
		{
			//make transformation unit steps
			anim_start_time = CLsystem::getmilliseconds();
			anim_last_time = anim_start_time;
			anim_units[1] = anim_div_temp * anicsv[1];
			anim_units[2] = anim_div_temp * anicsv[2];
			anim_units[3] = anim_div_temp * anicsv[3];
			anim_units[4] = anim_div_temp * anicsv[4];
			anim_units[5] = anim_div_temp * anicsv[5];
			anim_units[6] = anim_div_temp * anicsv[6];
			anim_units[7] = anim_div_temp * anicsv[7];
			anim_units[8] = anim_div_temp * anicsv[8];
			anim_units[9] = anim_div_temp * anicsv[9];
			anim_units[10] = anim_div_temp * anicsv[10];
			anim_units[11] = anim_div_temp * anicsv[11];
			anim_units[12] = anim_div_temp * anicsv[12];
			//*
			
			while(true)
			{
				//clear buffers
				CLdoublebuffer->clear(0);
				CLzbuffer->clear(zres);
				CLstencilbuffer->clear(0);
				//*
				
				anim_curr_time = CLsystem::getmilliseconds();
				anim_time_diff = anim_curr_time - anim_last_time;
				
				//update object
				anim_curr_trans[1] = anim_time_diff * anim_units[1];
				anim_curr_trans[2] = anim_time_diff * anim_units[2];
				anim_curr_trans[3] = anim_time_diff * anim_units[3];
				anim_curr_trans[4] = anim_time_diff * anim_units[4];
				anim_curr_trans[5] = anim_time_diff * anim_units[5];
				anim_curr_trans[6] = anim_time_diff * anim_units[6];
				anim_curr_trans[7] = anim_time_diff * anim_units[7];
				anim_curr_trans[8] = anim_time_diff * anim_units[8];
				anim_curr_trans[9] = anim_time_diff * anim_units[9];
				anim_curr_trans[10] = anim_time_diff * anim_units[10];
				anim_curr_trans[11] = anim_time_diff * anim_units[11];
				anim_curr_trans[12] = anim_time_diff * anim_units[12];
				
				anim_pos.x += anim_curr_trans[1];
				anim_pos.y += anim_curr_trans[2];
				anim_pos.z += anim_curr_trans[3];
				
				if(!( anim_curr_trans[4]==0 && anim_curr_trans[5]==0 && anim_curr_trans[6]==0 ))
				linear->translate(anim_curr_trans[4],anim_curr_trans[5],anim_curr_trans[6]);
				
				if(!( anim_curr_trans[7]==0 && anim_curr_trans[8]==0 && anim_curr_trans[9]==0 ))
				linear->rotate(anim_curr_trans[7],anim_curr_trans[8],anim_curr_trans[9]);
				
				if(!( anim_curr_trans[10]==0 && anim_curr_trans[11]==0 && anim_curr_trans[12]==0 ))
				linear->scale(anim_curr_trans[10],anim_curr_trans[11],anim_curr_trans[12]);
				
				atcl->update(linear);
				//*
				
				//display object
				atcl->display(anim_pos,CENTER + FLAT + AMBIENT + ZLIGHT);
				//*
				
				//unit matrix
				linear->unit();
				//*
				
				anim_last_time = anim_curr_time;
			}
		}
		else
		{
			//clear buffers
			CLdoublebuffer->clear(0);
			CLzbuffer->clear(zres);
			CLstencilbuffer->clear(0);
			//*
			
			//update object
			anim_pos.x += anicsv[anim_pointer+1];
			anim_pos.y += anicsv[anim_pointer+2];
			anim_pos.z += anicsv[anim_pointer+3];
			linear->translate(anicsv[anim_pointer+4],anicsv[anim_pointer+5],anicsv[anim_pointer+6]);
			linear->rotate(anicsv[anim_pointer+7],anicsv[anim_pointer+8],anicsv[anim_pointer+9]);
			linear->scale(anicsv[anim_pointer+10],anicsv[anim_pointer+11],anicsv[anim_pointer+12]);
			atcl->update(linear);
			//*
			
			//display object
			atcl->display(anim_pos,CENTER + FLAT + AMBIENT + ZLIGHT);
			//*
			
			//unit matrix
			linear->unit();
			//*
		}
		
		anim_pointer += 13;
	}	
	//*

	//clean up
	delete linear;
	delete atcl;
	//*
}

void CLintro::xizero()
{

}

#endif
