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
	//draw circle with radius r around (x,y)
	//fill surface around circle with black
}

void dissolve(xlong t,CLbuffer<xlong>* db)
{

}

void fadetoblack(xlong t,CLbuffer<xlong>* db)
{

}

#endif
