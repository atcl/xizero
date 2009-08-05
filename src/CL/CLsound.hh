//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
//#pragma message "Compiling " __FILE__ " ! TODO: ..." 


#include <audiere.h>

#include "CLtypes.hh"
#include "CLcl.hh"


namespace CLaudio
{
	audiere::AudioDevicePtr device = 0;
	
	void init();
}

void CLaudio::init()
{
	audiere::AudioDevicePtr temp(audiere::OpenDevice());
	device = temp;
}

//

class CLsound : public virtual CLcl
{
	private:
		audiere::OutputStreamPtr sound;
		float volume;
		
	public:
		CLsound(const xchar* c);
		~CLsound();
		void play(bool r);
		void stop();
		void setvolume(float v);
		void addvolume(float v);
		float getvolume();
};

CLsound::CLsound(const xchar* c)
{
	if(CLaudio::device!=0) { audiere::OutputStreamPtr temp(audiere::OpenSound(CLaudio::device,c,false)); sound = temp; }
}

CLsound::~CLsound() { }

void CLsound::play(bool r)
{
	sound->setRepeat(r);
	sound->play();
}

void CLsound::stop()
{
	sound->stop();
}

void CLsound::setvolume(float v)
{
	volume = v;
}

void CLsound::addvolume(float v)
{
	volume += v;
}

float CLsound::getvolume()
{
	return volume;
}

//~ //basic wav player
//~ namespace CLxsound
//~ {
	//~ xlong device;
	//~ 
	//~ bool init();
	//~ bool play();
	//~ void stop();
	//~ void exit();
//~ };
//~ 
//~ 
//~ #ifdef WIN32
//~ #include <mmsystem.hh>
//~ 
//~ bool init()
//~ {
	//~ device=0;
	//~ return 0;
//~ }
//~ 
//~ bool play(const xchar* f,bool l)
//~ {
	//~ switch(l)
	//~ {
		//~ case false:
			//~ return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC);
		//~ break;
		//~ 
		//~ case true:
			//~ return PlaySound(TEXT(f), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//~ break;
	//~ }
//~ }
//~ 
//~ void stop()
//~ {
	//~ PlaySound(NULL, 0, 0);
//~ }
//~ 
//~ void exit()
//~ {
	//~ 
//~ }
//~ #else //ifdef LINUX
//~ #include <linux/soundcard.h>
//~ #include <sys/ioctl.h>
//~ 
//~ bool init()
//~ {
	//~ device = open("/dev/dsp", O_WRONLY);
//~ }
//~ 
//~ bool play(const xchar f,bool l)
//~ {
	//~ //load wav (header + data)
	//~ //set bits per sample
	//~ //set number of channels
	//~ //set sample rate
	//~ //start async playing (how?, maybe fork!)
	//~ 
//~ }
//~ 
//~ void stop()
//~ {
	//~ //stop async playing
//~ }
//~ 
//~ void exit()
//~ {
	//~ close(device);
//~ }
//~ #endif
//~ //*

#endif

