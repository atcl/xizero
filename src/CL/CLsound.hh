//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#ifdef WIN32
	#include <mmsystem.hh>
#else //ifdef LINUX
	#include <fcntl.h>
	#include <linux/soundcard.h>
	#include <sys/types.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
	#include <signal.h>
#endif

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"

struct CLwav
{
	CLfile* file;
	xlong pcm;
	xlong channel;
	xlong rate;
	xlong bits;
	xlong offset;
	xlong length;
};

class CLsound : public virtual CLcl, public CLsingle<CLsound>
{
	friend class CLsingle<CLsound>;
	
	private:
		CLsystem* system;
		xlong device;
		xlong isloop;
		xlong nosound;
		CLsound();
		~CLsound();
	public:
		bool play(const xchar* f,bool l);
		void stop();
		void exit();
};

CLsound::~CLsound() { }

#ifdef WIN32

CLsound::CLsound() { isloop = -1; device = 0; system = CLsystem::instance(); }

bool CLsound::play(const xchar* f,bool l)
{
	switch(l)
	{
		//play sound once
		case false:
			return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC);
		break;
		//*
		
		//play sound async in loop
		case true:
			isloop = 1;
			return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
		//*
	}
}

void CLsound::stop()
{
	//stop async playing loop sound
	PlaySound(NULL, 0, 0);
	//*
}

void CLsound::exit() { }

#else //ifdef LINUX

CLsound::CLsound()
{
	system = CLsystem::instance();
	
	isloop = -1;
	
	//check if sound device is installed
	if( (device = open("/dev/dsp", O_WRONLY)) == -1)
	{
		system->print("No Soundblaster found");
		nosound = 1;
	}
	//*
	
	//set up sound device
	int i = 0;
	ioctl(device,SNDCTL_DSP_RESET,&i);
	i = 0;
	ioctl(device,SNDCTL_DSP_STEREO,&i);
	i = 44100;
	ioctl(device,SNDCTL_DSP_SPEED,&i);
	i = AFMT_S16_LE;
	ioctl(device,SNDCTL_DSP_SETFMT,&i);
	
	ioctl(device,SNDCTL_DSP_SYNC,0);
	//*
	
	nosound = 0;
}

bool CLsound::play(const xchar* f,bool l)
{
	if(device<0 || nosound==1) return 0;
	
	xlong playid = fork();
	 
	if(playid == 0)
	{
		CLwav current;
		
		current.file = system->getfile(f);
		
		//check if "RIFF"
		if(current.file->data[0] != 'FFIR') { say("wav loading error (RIFF)"); return 0; }
		//*
		
		//check if "WAVE"
		if(current.file->data[2] != 'EVAW') { say("wav loading error (WAVE)"); return 0; }
		//*
		
		//check if "fmt "
		if(current.file->data[3] != ' tmf') { say("wav loading error (fmt )"); return 0; }
		//*
		
		//get length
		current.length = current.file->data[4];
		//*
		
		//get and check if pcm
		current.pcm = (current.file->data[5]>>16);
		if(current.pcm != 1) { say("wav loading error (pcm)"); return 0; }
		//~ say(current.pcm);
		//*
		
		//get and check channels
		current.channel = (current.file->data[5]<<16)>>16;
		if(current.channel != 1) { say("wav loading error (channel)"); return 0; }
		//~ say(current.channel);
		//*
		
		//get bitrate
		current.rate = current.file->data[6];
		if(current.rate != 44100) { say("wav loading error (rate)"); return 0; }
		//~ say(current.rate);
		//*
		
		//get and check bits
		current.bits = (current.file->data[8]>>16);
		if(current.bits != 16) { say("wav loading error (bits)"); return 0; }
		//~ say(current.bits);
		//*
		
		//check if "data"
		if(current.file->data[9] != 'atad') { say("wav loading error (data)"); return 0; }
		//*

		//set device
		//later.. imo in init
		//*

		//write to dsp device
		while(true)
		{
			uxlong till = (current.file->size-44)/1024;
			for(uxlong i=0; i<till; i++)
			{
				write(device,&current.file->text[44+(i*1024)],1024);
			}
			
			if(!l) break;
		}
		//*	
		
		_exit(playid);
	}
	else
	{
		if(l) isloop = playid;
		 
		return 1;
	}
}

void CLsound::stop()
{
	if(device != -1)
	{
		//stop async playing
		kill(isloop,SIGKILL);
		//*
	}
}

void CLsound::exit()
{
	if(device != -1)
	{
		//stop any loop playing
		stop();
		//*
		
		//close the audio device
		close(device);
		//*
	}
}
#endif
//*

#endif

