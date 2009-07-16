#include "CL/CLwindow.hh"

int main(int argc, char *argv[])
{
	xlong* temp = new xlong[800*600*4];

	CLwindow::init(800,600,"atCROSSLEVEL",argc,argv);

	bool i=0;

	while(i==0)
	{
		CLwindow::flush(temp);
	}
	
    return 0;
}

