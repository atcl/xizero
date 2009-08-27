//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPROJECT
#define HH_CLPROJECT

#if __GNUC__ <= 4 && __GNUC_MINOR__ < 4
	#define u8 
#endif

#ifdef WIN32 
	#define INTEL
#else //ifdef LINUX
	#define ATT
#endif 

#define xres 800
#define yres 600
#define zres 400 

#define CLtitle "atCROSSLEVEL's XiZero"

#endif
