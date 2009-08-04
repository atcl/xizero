//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLTRIBOOL
#define HH_CLTRIBOOL
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"

#define 3true   1
#define 3false -1
#define 3null   0


struct CLtribool
{
	xlong b;
	
	operator=();
	
	operator-();
	operator!();
	
	operator++();
	operator--();
	
	operator+=();
	operator-=();
	operator*=();
	
	operator+();
	operator-();
	operator*();
	
	operator==();
	operator!=();
	operator<=();
	operator>=();
	operator<();
	operator>();
};

#endif
