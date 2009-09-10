//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINTRO
#define HH_CLINTRO
#pragma message Compiling __FILE__ ! TODO: test on models

#include "CLtypes.hh"
#include "CLglobal.hh"
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
	CLfile *cf = CLsystem::getfile("../dat/other/atcl.y3d");
	CLobject* atcl = new CLobject(cf,0);
	//*
	
	//load animation file
	CLfile* aniraw = CLsystem::getfile("../dat/other/atcl.ani");
	xlong*  anicsv = CLformat::loadcsv(aniraw,',');
	//*

	//set animation attributes
	CLfvector anim_pos(0,0,0);
	xlong anim_steps = (anicsv[0])/13;
	xlong anim_pointer = 1;
	float anim_step_dur = 0;
	float anim_div_temp = 1;
	xlong anim_curr_time = 0;
	xlong anim_start_time = 0;
	xlong anim_stop_time = 0;
	xlong anim_last_time = 0;
	float anim_time_diff = 0;
	float anim_units[13] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
	float anim_curr_trans[13] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
	float anim_commulative[13] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
	//*
	
	say(anicsv[0]);
	say(anim_steps);
	
	//run animation
	bool breaker = 0;
	for(uxlong i=0; i<anim_steps; i++)
	{
		say(anicsv[anim_pointer]);
		
		anim_step_dur = float(anicsv[anim_pointer]);
		
		if(anim_step_dur!=0)
		{
			anim_div_temp = 1/anim_step_dur;
			
			//make transformation unit steps
			anim_start_time = CLsystem::getmilliseconds();
			anim_last_time = anim_start_time;
			anim_stop_time = anim_start_time + anim_step_dur;
			anim_pointer++;
			anim_units[1] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[2] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[3] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[4] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[5] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[6] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[7] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[8] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[9] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[10] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[11] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			anim_units[12] = anim_div_temp * float(anicsv[anim_pointer]); anim_pointer++;
			//*
			
			while(win->run())
			{	
				//clear buffers
				CLdoublebuffer->clear(0);
				CLzbuffer->clear(zres);
				CLstencilbuffer->clear(0);
				//*

				//determine time
				anim_curr_time = CLsystem::getmilliseconds();
				anim_time_diff = float(anim_curr_time - anim_last_time);
				if(anim_curr_time >= anim_stop_time) breaker = 1;
				//*
				
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
				
				//move
				anim_pos.x += anim_curr_trans[1];
				anim_pos.y += anim_curr_trans[2];
				anim_pos.z += anim_curr_trans[3];
				//*
				
				//translate
				if(!( anim_curr_trans[4]==0 && anim_curr_trans[5]==0 && anim_curr_trans[6]==0 ))
					linear->translate(anim_curr_trans[4],CLmath::round(anim_curr_trans[5]),anim_curr_trans[6]);
				//*
				
				//rotate around x
				if(anim_curr_trans[7] != 0)
				{
					if( anim_curr_trans[7] < 0.5 && anim_curr_trans[7] != 0) anim_commulative[7] += anim_curr_trans[7];
					
					if( CLmath::round(anim_commulative[7]) != 0)
					{
						linear->rotate(CLmath::round(anim_commulative[7]),0,0);
						anim_commulative[7] = 0;
					}
				}
				//*

				//rotate around y
				if(anim_curr_trans[8] != 0)
				{
					if( anim_curr_trans[8] < 0.5 && anim_curr_trans[8] != 0) anim_commulative[8] += anim_curr_trans[8];
					
					if( CLmath::round(anim_commulative[8]) != 0)
					{
						linear->rotate(0,CLmath::round(anim_commulative[8]),0);
						anim_commulative[8] = 0;
					}
				}
				//*
				
				//rotate around z
				if(anim_curr_trans[9] != 0)
				{
					if( anim_curr_trans[9] < 0.5 && anim_curr_trans[9] != 0) anim_commulative[9] += anim_curr_trans[9];
					
					if( CLmath::round(anim_commulative[9]) != 0)
					{
						linear->rotate(0,0,CLmath::round(anim_commulative[9]));
						anim_commulative[9] = 0;
					}
				}
				//*
				
				//scale
				if(!( anim_curr_trans[10]==0 && anim_curr_trans[11]==0 && anim_curr_trans[12]==0 ))
					linear->scale(1+anim_curr_trans[10],1+anim_curr_trans[11],1+anim_curr_trans[12]);
				//*
				
				//apply linear transformations
				atcl->update(linear);
				//*
				
				//display object
				atcl->display(anim_pos,FLAT + AMBIENT + ZLIGHT);
				//*
				
				//unit matrix
				linear->unit();
				//*
				
				anim_last_time = anim_curr_time;
				if(breaker) break;
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
			linear->scale(1+anicsv[anim_pointer+10],1+anicsv[anim_pointer+11],1+anicsv[anim_pointer+12]);
			atcl->update(linear);
			//*
			
			//display object
			atcl->display(anim_pos,CENTER + FLAT + AMBIENT + ZLIGHT);
			//*
			
			//unit matrix
			linear->unit();
			//*
			
			anim_pointer += 13;
		}

		breaker = 0;
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
