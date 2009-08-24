//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
//#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include "CLtypes.hh"
#include "CLcl.hh"


//basic wav player
namespace CLsound
{
	xlong device;
	
	bool init();
	bool play(const xchar* f,bool l);
	void stop();
	void exit();
};


#ifdef WIN32
#include <mmsystem.hh>

bool CLsound::init()
{
	device = 0;
	return 0;
}

bool CLsound::play(const xchar* f,bool l)
{
	switch(l)
	{
		case false:
			return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC);
		break;
		
		case true:
			return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	}
}

void CLsound::stop()
{
	PlaySound(NULL, 0, 0);
}

void CLsound::exit()
{
	
}

#else //ifdef LINUX
#include <linux/soundcard.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

bool CLsound::init()
{
	device = open("/dev/dsp", O_WRONLY);
	return 0;
}

bool CLsound::play(const xchar* f,bool l)
{
	//load wav (header + data)
	//set bits per sample
	//set number of channels
	//set sample rate
	//start async playing (fork here)
	
	 xlong playid = fork();
	 
	 if(playid == 0)
	 {
		//...
			
		return 0;
	 }
	 else
	 {
		return 1;
	 }
	 
	
}

void CLsound::stop()
{
	//stop async playing
}

void CLsound::exit()
{
	close(device);
}
#endif
//*

#endif

