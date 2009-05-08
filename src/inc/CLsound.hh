//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

//#include <libsndfile.h>
#include "CLtypes.hh"
#include "CLcl.hh"


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
};



#endif

