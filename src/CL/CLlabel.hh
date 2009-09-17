//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLLABEL
#define HH_CLLABEL
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"

class CLlabel : public CLguibase
{
	private:
		xchar* text;
		bool   frame;
	public:
		CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,xchar* t,bool f);
		~CLlabel();
		void draw();
		void settext(const xchar* t);
		xchar* gettext();
};

CLlabel::CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,xchar* t,bool f) : CLguibase(px,py,w,h,fc,bc,rc)
{
	text = t;
	frame = f;
}

CLlabel::~CLlabel() { }

void CLlabel::draw()
{
	CLgfx2::drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	//CLgfx2::drawfontstring(
}

void CLlabel::settext(const xchar* t) { text = (xchar*)t; }

xchar* CLlabel::gettext() { return text; }

#endif

