//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSOUND
#define HH_CLSOUND
//#pragma message "Compiling " __FILE__ " ! TODO: ..." 


#include "CLtypes.hh"
#include "CLcl.hh"


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
//~ #include <unistd.h>
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
	//~ //start async playing (fork here)
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

