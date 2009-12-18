///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSOUND
#define HH_CLSOUND
///*

///sys includes
#include <AL/al.h>
#include <AL/alut.h>
///*

///idp includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsingle.hh"
#include "CLutils.hh"
///*

///api includes
#include "CLar.hh"
///*

///header
/* class name:	CLsound
 * 
 * description:	Plays wave files
 * 
 * author:	atcl
 * 
 * notes:	make more flexible.
 * 
 * version: 0.1
 */
///*

///declarations
#define NUMBUF 8
#define NUMSRC 8
///*

///definitions
class CLsound : public virtual CLcl, public CLsingle<CLsound>
{
	friend class CLsingle<CLsound>;
	
	private:
		ALfloat	alpos[3];
		ALfloat	alvel[3];
		ALfloat	alori[6];
		xlong numbuf;
		xlong numsrc;
		xchar** names;
		ALuint* alsources;
		ALuint* alwavs;
		CLsound();
		~CLsound();
	public:
		bool preload(CLar* aa);
		bool play(xlong i,bool l,bool o);
		void stop();
};
///*

///implementation
CLsound::CLsound() //! noncritical
{
	numbuf = 0;
	numsrc = 0;
	names = 0;
	
	ALuint alwavs = 0;
	
	//Init
	alutInit(0,0);
	alGetError();
	//*
	
	//set up 3d sound
	alpos[0] = 0; alpos[1] = 0; alpos[2] = 0;
	alvel[0] = 0; alvel[1] = 0; alvel[2] = 0;
	alori[0] = 0; alori[1] = 0; alori[2] = -1;
	alori[3] = 0; alori[4] = 1; alori[5] = 0;
	
	alListenerfv(AL_POSITION,alpos);
	alListenerfv(AL_VELOCITY,alvel);
	alListenerfv(AL_ORIENTATION,alori);
	//*
}

CLsound::~CLsound()
{
	//release buffers and sources
	if(numsrc!=0) { alDeleteSources(numsrc,alsources); }
	if(numbuf!=0) { alDeleteBuffers(numbuf,alwavs); }
	//*

	//exit
	alutExit();
	//*
}

bool CLsound::preload(CLar* aa) //! noncritical
{
	//obtain sound file count
	numbuf = numsrc = aa->getfilecount();
	alsources = new ALuint[numsrc];
	names = new xchar*[numbuf];
	//*
	
	//Create Sources
	alGenSources(numsrc,alsources);
	if(alGetError()!=AL_NO_ERROR) { err(__func__,"OpenAL error: alGenSources"); return 0; }
	//*
	
	//Load Wavs
	ALvoid* aldata = 0;
	CLfile* curr = 0;
	alwavs = new ALuint[numbuf];
	for(xlong i=0; i<numbuf; i++)
	{
		curr = aa->getmember(i);
		names[i] = clstring->copy(curr->name);
		
		aldata = static_cast<void*>(curr->data);
		alwavs[i] = alutCreateBufferFromFileImage(aldata,curr->size);
		if(alGetError()!=AL_NO_ERROR) { err(__func__,"OpenAL error: alutCreateBufferFromFileImage"); return 0; }
		
		alSourcei(alsources[i],AL_BUFFER,alwavs[i]); 
		if(alGetError()!=AL_NO_ERROR) { err(__func__,"OpenAL error: alSourcei"); return 0; }
	}
	//*
	
	return 1;
}

bool CLsound::play(xlong i,bool l,bool o) //! noncritical
{
	//set up 3d sound for source??? alSourcefv ?
	
	alSourcePlay(alsources[i]);
	
	return 1;
}

void CLsound::stop() //! noncritical
{

}
///*

#endif

