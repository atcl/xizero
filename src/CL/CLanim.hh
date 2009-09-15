//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLANIM
#define HH_CLANIM
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLformat.hh"
#include "CLsystem.hh"

struct CLframe
{
	float duration;
	float units[12];
	float curr[12];
	float comm[12];
};

class CLanim : public virtual CLcl
{
	private:
		CLmatrix* linear;
		CLobject* object;
		xlong*   anicsv;
		bool loop;
		xlong frames;
		xlong currframe;
		CLframe** frame;
		xlong starttime;
		xlong lastupdate;
		CLfvector position;
	public:
		CLanim(CLobject* obj,CLfile* ani,bool l,CLfvector* p=0);
		~CLanim();
		xlong update();
		xlong run();
};

CLanim::CLanim(CLobject* obj,CLfile* ani,bool l,CLfvector* p)
{
	//create linear transformation matrix
	linear = new CLmatrix(1);
	//*
	
	//set up attributes
	object = obj;
	anicsv = CLformat::loadcsv(ani,',');
	loop = l;
	//*
	
	xlong anipointer = 0;
	frames = (anicsv[anipointer])/13;
	anipointer++;
	frame = new CLframe*[frames];
	currframe = 0;
	
	for(uxlong i=0; i<frames; i++)
	{
		frame[i] = new CLframe;
		frame[i]->duration = anicsv[anipointer];
		
		//multiplication is faster than divisions
		float inv_dur = frame[i]->duration;
		//*
		
		//set units for multi frame animation
		if(frame[i]->duration!=0)
		{
			frame[i]->units[0] = anicsv[anipointer+1] * inv_dur;
			frame[i]->units[1] = anicsv[anipointer+2] * inv_dur;
			frame[i]->units[2] = anicsv[anipointer+3] * inv_dur;
			frame[i]->units[3] = anicsv[anipointer+4] * inv_dur;
			frame[i]->units[4] = anicsv[anipointer+5] * inv_dur;
			frame[i]->units[5] = anicsv[anipointer+6] * inv_dur;
			frame[i]->units[6] = anicsv[anipointer+7] * inv_dur;
			frame[i]->units[7] = anicsv[anipointer+8] * inv_dur;
			frame[i]->units[8] = anicsv[anipointer+9] * inv_dur;
			frame[i]->units[9] = anicsv[anipointer+10] * inv_dur;
			frame[i]->units[10] = anicsv[anipointer+11] * inv_dur;
			frame[i]->units[11] = anicsv[anipointer+12] * inv_dur;
		}
		//*
		
		//set units for single frame animation
		else
		{
			frame[i]->units[0] = anicsv[anipointer+1];
			frame[i]->units[1] = anicsv[anipointer+2];
			frame[i]->units[2] = anicsv[anipointer+3];
			frame[i]->units[3] = anicsv[anipointer+4];
			frame[i]->units[4] = anicsv[anipointer+5];
			frame[i]->units[5] = anicsv[anipointer+6];
			frame[i]->units[6] = anicsv[anipointer+7];
			frame[i]->units[7] = anicsv[anipointer+8];
			frame[i]->units[8] = anicsv[anipointer+9];
			frame[i]->units[9] = anicsv[anipointer+10];
			frame[i]->units[10] = anicsv[anipointer+11];
			frame[i]->units[11] = anicsv[anipointer+12];
		}
		//*
		
		//set up animation transformation arrays
		  frame[i]->curr[0] = frame[i]->curr[1] = frame[i]->curr[2]
		= frame[i]->curr[3] = frame[i]->curr[4] = frame[i]->curr[5]
		= frame[i]->curr[6] = frame[i]->curr[7] = frame[i]->curr[8]
		= frame[i]->curr[9] = frame[i]->curr[10] = frame[i]->curr[11] = 0; 
		
		  frame[i]->comm[0] = frame[i]->comm[1] = frame[i]->comm[2]
		= frame[i]->comm[3] = frame[i]->comm[4] = frame[i]->comm[5]
		= frame[i]->comm[6] = frame[i]->comm[7] = frame[i]->comm[8]
		= frame[i]->comm[9] = frame[i]->comm[10] = frame[i]->comm[11] = 0;
		//*
		
		anipointer += 13; 
	}
	
	lastupdate = 0;
	starttime = 0;
}

CLanim::~CLanim()
{
	delete linear;
	delete[] frame;
}

xlong CLanim::update()
{
	if(currframe>=frames) return 0;
	
	if(frame[currframe]->duration==0)
	{
		//update object
		position.x += frame[currframe]->units[0];
		position.y += frame[currframe]->units[1];
		position.z += frame[currframe]->units[2];
		linear->translate(frame[currframe]->units[3],frame[currframe]->units[4],frame[currframe]->units[5]);
		linear->rotate(frame[currframe]->units[6],frame[currframe]->units[7],frame[currframe]->units[8]);
		linear->scale(1+frame[currframe]->units[9],1+frame[currframe]->units[10],1+frame[currframe]->units[11]);
		object->update(linear);
		//*
		
		//unit matrix
		linear->unit();
		//*
		
		//increase frame counter
		currframe++;
		//*
	}
	else
	{
		//is first run in this frame
		if(starttime==0) lastupdate = starttime = CLsystem::getmilliseconds();
		//*
		
		//determine time
		xlong curr_time = CLsystem::getmilliseconds();
		xlong time_diff = float(curr_time - lastupdate);
		//*
		
		//is last run in this frame
		bool lastrun = 0;
		if(curr_time > curr_time + starttime) lastrun = 1;
		//*
				
		//update object
		frame[currframe]->curr[0] = time_diff * frame[currframe]->units[1];
		frame[currframe]->curr[1] = time_diff * frame[currframe]->units[2];
		frame[currframe]->curr[2] = time_diff * frame[currframe]->units[3];
		frame[currframe]->curr[3] = time_diff * frame[currframe]->units[4];
		frame[currframe]->curr[4] = time_diff * frame[currframe]->units[5];
		frame[currframe]->curr[5] = time_diff * frame[currframe]->units[6];
		frame[currframe]->curr[6] = time_diff * frame[currframe]->units[7];
		frame[currframe]->curr[7] = time_diff * frame[currframe]->units[8];
		frame[currframe]->curr[8] = time_diff * frame[currframe]->units[9];
		frame[currframe]->curr[9] = time_diff * frame[currframe]->units[10];
		frame[currframe]->curr[10] = time_diff * frame[currframe]->units[11];
		frame[currframe]->curr[11] = time_diff * frame[currframe]->units[12];
		//*
				
		//move
		position.x += frame[currframe]->curr[0];
		position.y += frame[currframe]->curr[1];
		position.z += frame[currframe]->curr[2];
		//*
				
		//translate
		if(!(frame[currframe]->curr[3]==0 && frame[currframe]->curr[4]==0 && frame[currframe]->curr[5]==0 ))
			linear->translate(CLmath::round(frame[currframe]->curr[3]),CLmath::round(frame[currframe]->curr[4]),CLmath::round(frame[currframe]->curr[5]));
		//*
				
		//rotate around x
		if(frame[currframe]->curr[6]!=0)
		{
			if(frame[currframe]->curr[6]< 0.5) frame[currframe]->comm[6] += frame[currframe]->curr[6];
			
			if( CLmath::round(frame[currframe]->comm[6])!= 0)
			{
				linear->rotate(CLmath::round(frame[currframe]->comm[6]),0,0);
				frame[currframe]->comm[6] = 0;
			}
		}
		//*

		//rotate around y
		if(frame[currframe]->curr[7]!= 0)
		{
			if(frame[currframe]->curr[7]< 0.5) frame[currframe]->comm[7] += frame[currframe]->curr[7];
			
			if( CLmath::round(frame[currframe]->comm[7]) != 0)
			{
				linear->rotate(0,CLmath::round(frame[currframe]->comm[7]),0);
				frame[currframe]->comm[7] = 0;
			}
		}
		//*
				
		//rotate around z
		if(frame[currframe]->curr[8]!= 0)
		{
			if(frame[currframe]->curr[8]< 0.5) frame[currframe]->comm[8] += frame[currframe]->curr[8];
			
			if( CLmath::round(frame[currframe]->comm[8])!=0)
			{
				linear->rotate(0,0,CLmath::round(frame[currframe]->comm[8]));
				frame[currframe]->comm[8] = 0;
			}
		}
		//*
				
		//scale
		if(!(frame[currframe]->curr[9]==0 && frame[currframe]->curr[10] && frame[currframe]->curr[11]==0 ))
			linear->scale(1+frame[currframe]->curr[9],1+frame[currframe]->curr[10],1+frame[currframe]->curr[11]);
		//*
				
		//apply linear transformations
		object->update(linear);
		//*
		
		//unit matrix
		linear->unit();
		//*
		
		//save time
		lastupdate = curr_time;
		//*
		
		//increase frame counter if last run of frame
		if(lastrun)
		{
			if(loop && currframe==frames+1)
			{
				currframe=0;
			}
			else
			{
				currframe++;
				starttime=0;
			}
		}
		//*
	}
	
	if(!loop && currframe==frames) return 0;
	else return 1;
}

xlong CLanim::run()
{
	//use this if the instantiated animation is supposed to run autonomus from other program parts
	//in example intro animations
	
	//clear buffers
	CLdoublebuffer->clear(0);
	CLzbuffer->clear(zres);
	CLstencilbuffer->clear(0);
	//*
	
	//update animation
	xlong r = update();
	//*
	
	//display object
	object->display(position,FLAT + AMBIENT + ZLIGHT);
	//*
	
	//return if animation running or over
	return r;
	//*
}

#endif
