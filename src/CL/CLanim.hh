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
	protected:
		CLmatrix linear;
		CLobject* object;
		xlong*   animcsv;
		
	private:
		bool loop;
		xlong frames;
		xlong currframe;
		CLframe* frame;
		xlong lastupdate
		CLfvector position;
	
	public:
		CLanim(CLobject* obj,CLfile* ani,bool l,CLfvector* p=0);
		~CLanim();
		update();
		display();
};

CLanim::CLanim(CLobject* obj,CLfile* ani,bool l,CLfvector* p)
{
	//create linear transformation matrix
	linear = new CLmatrix(1);
	//*
	
	//set up attributes
	object = obj;
	anicsv = CLformat::loadcsv(aniraw,',');
	loop = l;
	//*
	
	xlong anipointer = 0;
	frames = (anicsv[anipointer])/13;
	anipointer++;
	frame = new CLframe[frames];
	currframe = 0;
	
	for(uxlong i=0; i<frames; i++)
	{
		frame[i]->duration = anicsv[anipointer];
		
		if(duration!=0)
		{
			frame[i]->units[0] = anicsv[anipointer+1] / anicsv[anipointer];
			frame[i]->units[1] = anicsv[anipointer+2] / anicsv[anipointer];
			frame[i]->units[2] = anicsv[anipointer+3] / anicsv[anipointer];
			frame[i]->units[3] = anicsv[anipointer+4] / anicsv[anipointer];
			frame[i]->units[4] = anicsv[anipointer+5] / anicsv[anipointer];
			frame[i]->units[5] = anicsv[anipointer+6] / anicsv[anipointer];
			frame[i]->units[6] = anicsv[anipointer+7] / anicsv[anipointer];
			frame[i]->units[7] = anicsv[anipointer+8] / anicsv[anipointer];
			frame[i]->units[8] = anicsv[anipointer+9] / anicsv[anipointer];
			frame[i]->units[9] = anicsv[anipointer+10] / anicsv[anipointer];
			frame[i]->units[10] = anicsv[anipointer+11] / anicsv[anipointer];
			frame[i]->units[11] = anicsv[anipointer+12] / anicsv[anipointer];
		}
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
		
		frame[i]->curr = { 0,0,0,0,0,0,0,0,0,0,0,0 };
		frame[i]->comm = { 0,0,0,0,0,0,0,0,0,0,0,0 };
		
		anipointer += 13; 
	}
	
	lastupdate = 0;
}

CLanim::~CLanim()
{
	delete linear;
	delete[] frame;
}

CLanim::update()
{
	
}

#endif
