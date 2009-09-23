//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLBOSS
#define HH_CLBOSS
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLcl.hh"
#include "CLentity.hh"

/* class name:	CLboss
 * 
 * description:	The boss (enemy) class.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

class CLboss : public CLenemy
{
	public:
		CLboss(CLfile* bossptr,xlong* m,xlong mm);
		~CLboss() { };
};

CLboss::CLboss(CLfile* bossptr,xlong* m,xlong mm) : CLenemy(bossptr,m,mm)
{ 
	//set entity type
	type = 2;
	//*
	
	//set health bar width and height
	hprog->setwidth(80);
	hprog->setheight(20);
	//*	
}

#endif
