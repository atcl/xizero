///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLBOSS
#define HH_CLBOSS
///*

///includes
#include "CLenemy.hh"
///*

///header
/* class name:	CLboss
 * 
 * description:	The boss (enemy) class.
 * 
 * author:		atcl
 * 
 * notes:		
 * 
 * version: 	0.2
 */
///*

///definitions
class CLboss : public CLenemy
{
	public:
		CLboss(CLfile* bossptr,const xlong& m,xlong mm,CLlvector* bossp);
		~CLboss() { }
};
///*

///implementation
CLboss::CLboss(CLfile* bossptr,const xlong& m,xlong mm,CLlvector* bossp) : CLenemy(bossptr,m,mm,bossp) //! noncritical
{ 
	//set entity type
	type = 2;
	//*
	
	//bosses don't troll around
	gear=0;
	setspeed();
	//*

	//set health bar width and height
	hprog->setwidth(80);
	hprog->setheight(20);
	//*	
}
///*

#endif
