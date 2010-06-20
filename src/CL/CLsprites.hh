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
	
	private:
		CLscreen& clscreen;
	protected:
		CLcomspr() : clscreen(CLscreen::instance()) { };
		~CLcomspr() { };
	public:
		void drawantimatter(xlong x,xlong y);
		void drawplasma(xlong x,xlong y);
		void drawtracks(xlong x,xlong y,xlong d);
};
///*

///implementation
void CLcomspr::drawantimatter(xlong x,xlong y) //! critical
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) { return; }
	//*
	
	uxlong* db = clscreen.cldoublebuffer.getbuffer();
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw
	db[l1++]=0x00800080; db[l1++]=0x00800080; db[l1++]=0x00800080; db[l1]=0x00800080;
	db[l2++]=0x00800080; db[l2++]=0x00AD00AD; db[l2++]=0x00AD00AD; db[l2++]=0x00AD00AD; db[l2++]=0x00AD00AD; db[l2]=0x00800080;
	db[l3++]=0x00800080; db[l3++]=0x00AD00AD; db[l3++]=0x00FF00FF; db[l3++]=0x00FF00FF; db[l3++]=0x00AD00AD; db[l3]=0x00800080;
	db[l4++]=0x00800080; db[l4++]=0x00AD00AD; db[l4++]=0x00FF00FF; db[l4++]=0x00FF00FF; db[l4++]=0x00AD00AD; db[l4]=0x00800080;
	db[l5++]=0x00800080; db[l5++]=0x00AD00AD; db[l5++]=0x00AD00AD; db[l5++]=0x00AD00AD; db[l5++]=0x00AD00AD; db[l5]=0x00800080;
	db[l6++]=0x00800080; db[l6++]=0x00800080; db[l6++]=0x00800080; db[l6]=0x00800080;
	//*
}

void CLcomspr::drawplasma(xlong x,xlong y) //! critical
{
	//is on screen
	if( (x-2)<0 || (x+3)>=XRES || (y-2)<0 || (y+3)>=YRES) { return; }
	//*
	
	uxlong* db = clscreen.cldoublebuffer.getbuffer();
	
	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES + 1; 
	//*
	
	//draw 
	db[l1++]=0x000000FF; db[l1++]=0x000000FF; db[l1++]=0x000000FF; db[l1]=0x000000FF; 
	db[l2++]=0x000000FF; db[l2++]=0x00000080; db[l2++]=0x00000080; db[l2++]=0x00000080; db[l2++]=0x00000080; db[l2]=0x000000FF;
	db[l3++]=0x00FF0000; db[l3++]=0x00000080; db[l3++]=0x00000080; db[l3++]=0x00000080; db[l3++]=0x00000080; db[l3]=0x00FF0000;
	db[l4++]=0x000000FF; db[l4++]=0x00000080; db[l4++]=0x00000080; db[l4++]=0x00000080; db[l4++]=0x00000080; db[l4]=0x000000FF;
	db[l5++]=0x000000FF; db[l5++]=0x00000080; db[l5++]=0x00000080; db[l5++]=0x00000080; db[l5++]=0x00000080; db[l5]=0x000000FF;
	db[l6++]=0x000000FF; db[l6++]=0x000000FF; db[l6++]=0x000000FF; db[l6]=0x000000FF;
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
	
	uxlong* db = clscreen.cldoublebuffer.getbuffer();

	//calc offsets
	xlong l1 = ((y-2)*XRES)+(x-1);
	xlong l2 = l1 + XRES - 1;
	xlong l3 = l2 + XRES;
	xlong l4 = l3 + XRES;
	xlong l5 = l4 + XRES;
	xlong l6 = l5 + XRES;
	//*

	switch(dd)
	{
		case 0:
		case 12: //draw horizontal
			db[l1++]=0x00804000; db[l1++]=0x00804000; db[l1++]=0x00804000; db[l1]=0x00804000;
			db[l2++]=0x00800000; db[l2++]=0x00800000; db[l2++]=0x00800000; db[l2]=0x00800000;
			db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3]=0x00800000;
			db[l4++]=0x00804000; db[l4++]=0x00804000; db[l4++]=0x00804000; db[l4]=0x00804000;
		break;
		
		case 6:
		case 18: //draw vertical
			db[l1++]=0x00804000; db[l1++]=0x00800000; db[l1++]=0x00800000; db[l1]=0x00804000;
			db[l2++]=0x00804000; db[l2++]=0x00800000; db[l2++]=0x00800000; db[l2]=0x00804000;
			db[l3++]=0x00804000; db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3]=0x00804000;
			db[l4++]=0x00804000; db[l4++]=0x00800000; db[l4++]=0x00800000; db[l4]=0x00804000; 
		break;

		case 1:
		case 7:
		case 19: //draw 
			db[l1++]=0x00804000; db[l1++]=0x00804000;
			db[l2++]=0x00800000; db[l2++]=0x00800000; db[l2++]=0x00804000; db[l2]=0x00804000;
			db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3]=0x00800000;
			db[l4++]=0x00804000; db[l4++]=0x00804000; db[l4++]=0x00800000; db[l4]=0x00800000;
										  l5++; l5++; db[l5++]=0x00804000; db[l5]=0x00804000;
		break;
		
		case 2:
		case 8:
		case 20: //draw
										  l1++; l1++; db[l1++]=0x00804000; db[l1]=0x00804000;
			db[l2++]=0x00800000; db[l2++]=0x00800000; db[l2++]=0x00804000; db[l2]=0x00804000;
			db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3++]=0x00800000; db[l3]=0x00800000;
			db[l4++]=0x00804000; db[l4++]=0x00804000; db[l4++]=0x00800000; db[l4]=0x00800000;
			db[l5++]=0x00804000; db[l5]=0x00804000;
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

