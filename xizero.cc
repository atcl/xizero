// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// xizero.cc
// XiZero Main Program

///includes
#include "xizero.hh"
///*

///declarations
#define LEVELS 2 
///*

///implementation
int main(int argc,const char* argv[])
{
	init();
	intro();

	for(long i=mainmenu();i<LEVELS;++i)
	{
		start(i);
	}
	
	return 0;
}
///*

