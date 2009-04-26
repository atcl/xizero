//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include "CLtypes.hh"
#include "CLcl.hh"


class CLsound : public virtual CLcl
{
	private:
		xlong volume;

		void fadein();
		void fadeout();
	public:
		CLsound();
		~CLsound();
		void playvoc(xlong* bf,xlong eff);
		void loopplayvoc(xlong* bf,xlong eff);
}

#endif

