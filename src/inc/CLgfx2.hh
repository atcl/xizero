//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license  
#ifndef HH_CLGFX2
#define HH_CLGFX2
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLbuffer.hh"


class CLgfx2 : public virtual CLcl
{
	protected:
		CLbuffer<xlong>* doublebuffer;

	private:

	public:
		CLgfx2(CLbuffer<xlong>* db);
		~CLgfx2();
		void drawfontchar(xlong x,xlong y,xchar a,xlong* f,uxlong c);
		void drawfontchar(xlong p,xchar a,xlong* f,uxlong c);
		void drawfontstring(xlong x,xlong y,xchar* a,xlong* f,uxlong c);
		void drawfontstring(xlong p,xchar* a,xlong* f,uxlong c);
		void drawfontpartstring(xlong x,xlong y,xchar* a,xlong b,xlong e,xlong* f,uxlong c);
		void drawfontpartstring(xlong p,xchar* a,xlong b,xlong e,xlong* f,uxlong c);
		xlong locatemaxx();
		xlong locatemaxy();
		xlong locate(xlong x,xlong y);
		uxlong getQBcolor(xchar c); //get 16 EGA colors
		bool comparecolors(uxlong c1,uxlong c2);
		uxlong blendcolors();
		uxlong blendcolorsalpha();
		uxlong blendcolorsadd();
		uxlong blendcolorsmul();
};

CLgfx2::CLgfx2(CLbuffer<xlong>* db)
{
	doublebuffer = db;
}

CLgfx2::~CLgfx2() { }

void CLgfx2::drawfontchar(xlong x,xlong y,xchar a,xlong* f,uxlong c)
{

}

void CLgfx2::drawfontchar(xlong p,xchar a,xlong* f,uxlong c)
{

}

void CLgfx2::drawfontstring(xlong x,xlong y,xchar* a,xlong* f,uxlong c)
{

}

void CLgfx2::drawfontstring(xlong p,xchar* a,xlong* f,uxlong c)
{

}

xlong CLgfx2::locatemaxx()
{

}

xlong CLgfx2::locatemaxy()
{

}

xlong CLgfx2::locate(xlong x,xlong y)
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

#endif

