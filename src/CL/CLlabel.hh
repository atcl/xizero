//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLLABEL
#define HH_CLLABEL
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"

/* class name:	CLlabel
 * 
 * description:	A standard gui element label
 * 
 * author:	atcl
 * 
 * notes:	finish implementing.
 * 
 * version: 0.1
 */

class CLlabel : public CLguibase
{
	private:
		xchar* text;
		bool   frame;
	public:
		CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,xchar* t,bool f);
		~CLlabel() { };
		void draw() const;
		void settext(const xchar* t);
		xchar* gettext() const;
};

CLlabel::CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,xchar* t,bool f) : CLguibase(px,py,w,h,fc,bc,rc)
{
	frame = f;
	if(!f) rcolor = bcolor;
	
	text = clutils->clonechararray(t);
	xlong nw = clgfx2->getfontstringwidth(t,0) + 4;
	xlong nh = clgfx2->getfontstringheight(t,0) + 4;
	if(w==-1 || w<nw) width = nw;
	if(h==-1 || h<nh) height = nh;
}

void CLlabel::draw() const
{
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx2->drawfontstring(posx+2,posy+2,text,0,fcolor,bcolor);
}

void CLlabel::settext(const xchar* t)
{
	delete text;
	text = clutils->clonechararray(t);
	xlong nw = clgfx2->getfontstringwidth(text,0) + 4;
	xlong nh = clgfx2->getfontstringheight(text,0) + 4;
	if(width<nw) width = nw;
	if(height<nh) height = nh;
}

xchar* CLlabel::gettext() const { return text; }

#endif

