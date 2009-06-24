//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
#pragma message "Compiling " __FILE__ " ! TODO: ..." 


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

#endif

