//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license  
#ifndef HH_CLGFX2
#define HH_CLGFX2
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLformat.hh"


#define teletype 0;
#define monotype 1;
#define linetype 2;
#define segmtype 3;


namespace CLgfx2
{
	sprites* tele;
	sprites* mono;
	sprites* line;
	sprites* segm;
	
	void drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f);
	void drawfontchar(xlong x,xlong y,xchar a,xlong f,uxlong c);
	void drawfontchar(xlong p,xchar a,xlong f,uxlong c);
	void drawfontstring(xlong x,xlong y,xchar* a,xlong f,uxlong c);
	void drawfontstring(xlong p,xchar* a,xlong f,uxlong c);
	void drawfontpartstring(xlong x,xlong y,xchar* a,xlong b,xlong e,xlong f,uxlong c);
	void drawfontpartstring(xlong p,xchar* a,xlong b,xlong e,xlong f,uxlong c);
	uxlong getQBcolor(xchar c); //get 16 EGA colors
	bool comparecolors(uxlong c1,uxlong c2);
	uxlong blendcolors();
	void savescreenshot(const xchar*);
}


void CLgfx2::drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f)
{
	xlong offset1 = (y1*xres)+x1;
	xlong offset2 = offset1;
	xlong doffset = ((y2-y1)*xres);
	xlong diff    = x2-x1;
	uxlong c3     = c2;
	if(f) c3 = 0x000FFFFFF - c2;

	for(int i=y1; i<=y2; i++)
	{
		(*CLdoublebuffer)[offset1] = c2;
		offset1++;
		for(int j=x1+1; j<x2; j++)
		{
			(*CLdoublebuffer)[offset1] = c1;
			offset1++;
		}
		(*CLdoublebuffer)[offset1] = c3;
		offset1 += xres - diff;
	}

	for(int k=x1; k<x2; k++)
	{
		(*CLdoublebuffer)[offset2] = c2;
		(*CLdoublebuffer)[offset2+doffset] = c3;
		offset2++;
	}
}

void CLgfx2::drawfontchar(xlong x,xlong y,xchar a,xlong f,uxlong c)
{

}

void CLgfx2::drawfontchar(xlong p,xchar a,xlong f,uxlong c)
{

}

void CLgfx2::drawfontstring(xlong x,xlong y,xchar* a,xlong f,uxlong c)
{

}

void CLgfx2::drawfontstring(xlong p,xchar* a,xlong f,uxlong c)
{

}

uxlong CLgfx2::getQBcolor(xchar c)
{
	switch(c)
	{
		case 0:
			return 0x00000000; //black
		break;
		case 1:
			return 0x000000FF; //blue
		break;
		case 2:
			return 0x0000FF00; //green
		break;
		case 3:
			return 0x0000FFFF; //cyan
		break;
		case 4:
			return 0x00FF0000; //red
		break;
		case 5:
			return 0x00FF00FF; //magenta
		break;
		case 6:
			return 0x00FFFF00; //yellow
		break;
		case 7:
			return 0x00808080; //dark white?
		break;
		case 8:
			return 0x00404040; //gray
		break;
		case 9:
			return 0x008080FF; //light blue
		break;
		case 10:
			return 0x0080FF80; //light green
		break;
		case 11:
			return 0x0080FFFF; //light cyan
		break;
		case 12:
			return 0x00FF8080; //light red
		break;
		case 13:
			return 0x00FF80FF; //light magenta
		break;
		case 14:
			return 0x00FFFF80; //light yellow
		break;
		case 15:
			return 0x00FFFFFF; //white
		break;
		default:
			return 0;
	}
}

bool CLgfx2::comparecolors(uxlong c1,uxlong c2)
{

}

uxlong CLgfx2::blendcolors()
{
	//add,mul,alpha,and,or,nand,nor,not,xor
}

void CLgfx2::savescreenshot(const xchar*)
{

}

#endif

