//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSCREEN
#define HH_CLSCREEN
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLutils.hh"
#include "CLgfx1.hh"


//gobals:
xchar fade_const;
xlong diss_const;
xlong circ_const;

//prototypes:
void circleblend(xlong x,xlong y,xlong r,xlong t,CLbuffer<xlong>* db);
void dissolve(xlong t,CLbuffer<xlong>* db);
void fadetoblack(xlong t,CLbuffer<xlong>* db);
//


void circleblend(xlong x,xlong y,xlong r,xlong t,CLbuffer<xlong>* db)
{

}

void dissolve(xlong t,CLbuffer<xlong>* db)
{

}

void fadetoblack(xlong t,CLbuffer<xlong>* db)
{
	every* ev = new every;
	//ev->func = ;
	ev->interval = 50;
	ev->count = 256;
	ev->active = true;
	//ev->id = clid->generateid

	CLdoevery_(ev);
}

//everies:

ev_dissolve()
{
	//xlong x = CLrandom_(xres);
	//xlong y = CLrandom_(yres);
	//xlong c = CLrandom_(0x00FFFFFF);

	//clgfx1->drawpixel(x,y,c);
}

ev_fadetoblack()
{
	
}


#endif
