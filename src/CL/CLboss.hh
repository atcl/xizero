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
		CLboss(CLfile* bossptr,xlong* m,xlong mm,CLlvector* bossp);
		~CLboss();
};

CLboss::CLboss(CLfile* bossptr,xlong* m,xlong mm,CLlvector* bossp) : CLenemy(bossptr,m,mm,bossp)
{ 
	//set entity type
	type = 2;
	//*
	
	//bosses don't troll around
	gear=0;
	setspeed();
	//*

	//set health bar width and height
	hprog->reset(0,0,80,20,health+shield,0,health+shield,0x0000FF00,0,1,0,0x00C0C0C0,0x00C0C0C0);
	//*	
}

CLboss::~CLboss() { }

#endif
