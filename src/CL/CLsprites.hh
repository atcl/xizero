///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSPRITES
#define HH_CLSPRITES
///*

///includes
#include "CLtypes.hh"
#include "CLbuffer.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLcomspr
 * 
 * description:	Draws compiled sprites
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLcomspr : public CLbase<CLcomspr,1>
{
	friend class CLbase<CLcomspr,1>;
	friend class CLglobal;
	
	protected:
		CLcomspr() { };
		~CLcomspr() { };
	public:
		static void drawantimatter(xlong x,xlong y);
		static void drawplasma(xlong x,xlong y);
		static void drawtracks(xlong x,xlong y,xlong d);
};
///*

///implementation
void CLcomspr::drawantimatter(xlong x,xlong y) //! critical
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) { return; }
	//*
	
	CLubuffer& db = CLscreen::instance().cldoublebuffer;
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw frst row
	db[l1] = 0x00800080; l1++;
	db[l1] = 0x00800080; l1++;
	db[l1] = 0x00800080; l1++;
	db[l1] = 0x00800080;
	//*
	
	//draw second row
	db[l2] = 0x00800080; l2++;
	db[l2] = 0x00AD00AD; l2++;
	db[l2] = 0x00AD00AD; l2++;
	db[l2] = 0x00AD00AD; l2++;
	db[l2] = 0x00AD00AD; l2++;
	db[l2] = 0x00800080;
	//*
	
	//draw third row
	db[l3] = 0x00800080; l3++;
	db[l3] = 0x00AD00AD; l3++;
	db[l3] = 0x00FF00FF; l3++;
	db[l3] = 0x00FF00FF; l3++;
	db[l3] = 0x00AD00AD; l3++;
	db[l3] = 0x00800080;
	//*
	
	//draw fourth row
	db[l4] = 0x00800080; l4++;
	db[l4] = 0x00AD00AD; l4++;
	db[l4] = 0x00FF00FF; l4++;
	db[l4] = 0x00FF00FF; l4++;
	db[l4] = 0x00AD00AD; l4++;
	db[l4] = 0x00800080;
	//*
	
	//draw fifth row
	db[l5] = 0x00800080; l5++;
	db[l5] = 0x00AD00AD; l5++;
	db[l5] = 0x00AD00AD; l5++;
	db[l5] = 0x00AD00AD; l5++;
	db[l5] = 0x00AD00AD; l5++;
	db[l5] = 0x00800080;
	//*
	
	//draw sixth row
	db[l6] = 0x00800080; l6++;
	db[l6] = 0x00800080; l6++;
	db[l6] = 0x00800080; l6++;
	db[l6] = 0x00800080;
	//*
}

void CLcomspr::drawplasma(xlong x,xlong y) //! critical
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) { return; }
	//*
	
	CLubuffer& db = CLscreen::instance().cldoublebuffer;
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw first row
	db[l1] = 0x000000FF; l1++;
	db[l1] = 0x000000FF; l1++;
	db[l1] = 0x000000FF; l1++;
	db[l1] = 0x000000FF;
	//*
	
	//draw second row
	db[l2] = 0x000000FF; l2++;
	db[l2] = 0x00000080; l2++;
	db[l2] = 0x00000080; l2++;
	db[l2] = 0x00000080; l2++;
	db[l2] = 0x00000080; l2++;
	db[l2] = 0x000000FF;
	//*
	
	//draw third row
	db[l3] = 0x00FF0000; l3++;
	db[l3] = 0x00000080; l3++;
	db[l3] = 0x00000080; l3++;
	db[l3] = 0x00000080; l3++;
	db[l3] = 0x00000080; l3++;
	db[l3] = 0x00FF0000;
	//*
	
	//draw fourth row
	db[l4] = 0x000000FF; l4++;
	db[l4] = 0x00000080; l4++;
	db[l4] = 0x00000080; l4++;
	db[l4] = 0x00000080; l4++;
	db[l4] = 0x00000080; l4++;
	db[l4] = 0x000000FF;
	//*
	
	//draw fifth row
	db[l5] = 0x000000FF; l5++;
	db[l5] = 0x00000080; l5++;
	db[l5] = 0x00000080; l5++;
	db[l5] = 0x00000080; l5++;
	db[l5] = 0x00000080; l5++;
	db[l5] = 0x000000FF;
	//*
	
	//draw sixth row
	db[l6] = 0x000000FF; l6++;
	db[l6] = 0x000000FF; l6++;
	db[l6] = 0x000000FF; l6++;
	db[l6] = 0x000000FF;
	//*
}

void CLcomspr::drawtracks(xlong x,xlong y,xlong d) //! critical
{
	//mit liste die alle sichtbaren behält?
	
	//is on screen
	if( (x-2)<0 || (x+2)>=XRES || (y-2)<0 || (y+2)>=YRES) { return; }
	//*
	
	xlong dd = ((d-7)/15)%24;
	if(dd<0) { dd*=-1; }
	
	CLubuffer& db = CLscreen::instance().cldoublebuffer;

	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	//*

	switch(dd)
	{
		case 0:
		case 12:
			//draw first row
			db[l1] = 0x00804000; l1++;
			db[l1] = 0x00804000; l1++;
			db[l1] = 0x00804000; l1++;
			db[l1] = 0x00804000;
			//*
			
			//draw second row
			db[l2] = 0x00800000; l2++;
			db[l2] = 0x00800000; l2++;
			db[l2] = 0x00800000; l2++;
			db[l2] = 0x00800000;
			//*
			
			//draw third row
			db[l3] = 0x00800000; l3++;
			db[l3] = 0x00800000; l3++;
			db[l3] = 0x00800000; l3++;
			db[l3] = 0x00800000;
			//*
			
			//draw fourth row
			db[l4] = 0x00804000; l4++;
			db[l4] = 0x00804000; l4++;
			db[l4] = 0x00804000; l4++;
			db[l4] = 0x00804000;
			//*
		break;
		
		case 6:
		case 18:
			//draw first row
			db[l1] = 0x00804000; l1++;
			db[l1] = 0x00800000; l1++;
			db[l1] = 0x00800000; l1++;
			db[l1] = 0x00804000;
			//*
			
			//draw second row
			db[l2] = 0x00804000; l2++;
			db[l2] = 0x00800000; l2++;
			db[l2] = 0x00800000; l2++;
			db[l2] = 0x00804000;
			//*
			
			//draw third row
			db[l3] = 0x00804000; l3++;
			db[l3] = 0x00800000; l3++;
			db[l3] = 0x00800000; l3++;
			db[l3] = 0x00804000;
			//*
			
			//draw fourth row
			db[l4] = 0x00804000; l4++;
			db[l4] = 0x00800000; l4++;
			db[l4] = 0x00800000; l4++;
			db[l4] = 0x00804000;
			//* 
		break;

		case 1:
		case 7:
		case 19:
		break;
		
		case 2:
		case 8:
		case 20:
		break;
		
		case 3:
		case 9:
		case 21:
		break;
		
		case 4:
		case 10:
		case 22:
		break;
		
		case 5:
		case 11:
		case 23:
		break;
	}
}
///*

#endif

