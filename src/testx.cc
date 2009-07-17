#include "CL/CLwindow.hh"

int main(int argc, char *argv[])
{
	xlong* temp = new xlong[800*600];
	
	for(int i=0; i<800;i++)
	{
		for(int j=0; j<600; j++)
		{
			temp[j*600+i] = i*j;
		}
	}

	CLwindow win(800,600,"atCROSSLEVEL",temp);

	while(win.run()!=0)
	{
		win.redraw();
	}
	
    return 0;
}

