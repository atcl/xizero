//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSETUP
#define HH_CLSETUP
#pragma message "Compiling " __FILE__ " ! TODO: ..."


void CLsetup()
{
	clbuffer        = new CLbuffer();
	CLdoublebuffer	= clbuffer->makelong(xres*yres);
	CLzbuffer	= clbuffer->makefloat(xres*yres);
	CLstencilbuffer	= clbuffer->makelong(xres*yres);

	clfps    = new CLfps(3,true);
	clformat = new CLformat();
	clmath   = new CLmath();

	clgfx1   = new CLgfx1(CLdoublebuffer);
	clgfx2   = new CLgfx2(CLdoublebuffer);

	amb      = new CLlight(1,1,-1,1,0x00FFFFFF,clmath);
	ltm      = new CLmatrix(1,clmath);
	bsm      = new CLmatrix(1,clmath);

	leftside = new screenside[yres];
	rightside = new screenside[yres]; 
}

#endif
