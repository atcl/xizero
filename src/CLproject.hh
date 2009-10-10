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

#ifdef REL
	#define BASEDIR "/usr/share/xizero/"
#else //ifded DEV
	#define BASEDIR 
#endif

#define XRES 800
#define YRES 600
#define ZRES 400
#define SCRS XRES*YRES

#define TITLE "atCROSSLEVEL's XiZero"

#endif
