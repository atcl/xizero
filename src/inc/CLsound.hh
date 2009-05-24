//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
#pragma message "Compiling " __FILE__ " ! TODO: ..." 


#include "CLtypes.hh"
#include "CLcl.hh"

//#include <canberra.h>


class CLsound : public virtual CLcl
{
	private:
		xlong volume;
		
		void fadein();
		void fadeout();
	public:
		CLsound(CLfile* bf);
		~CLsound();
		void playvoc(xlong eff);
		void loopplayvoc(xlong eff);
		void setvolume(xlong v);
		xlong getvolume();
		xlong addvolume(xlong v);
};

CLsound::CLsound()
{

}

CLsound::~CLsound()
{

}

void CLsound::playvoc(xlong eff)
{

}

void CLsound::loopplayback(xlong eff)
{

}

void CLsound::setvolume(xlong v)
{
	volume = v;
}

xlong CLsound::getvolume()
{
	return volume;
}

void CLsound::addvolume(xlong v)
{
	volume += v;
}

#endif

