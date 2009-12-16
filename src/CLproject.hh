///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLPROJECT
#define HH_CLPROJECT
///*

///declarations
#define FNAME __func__" "

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
///*

#endif
