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
	//intro();
	mainmenu();

	for(long i=0;i<LEVELS;++i)
	{
		start(i);
	}

	return 0;
}
///*

