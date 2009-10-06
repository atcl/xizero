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
		bool  frame;
		xlong textwidth;
		xlong textheight;
		xlong textx;
		xlong texty;
	public:
		CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,const xchar* t,bool f);
		~CLlabel() { };
		void draw() const;
		void settext(const xchar* t);
		inline xchar* gettext() const;
};

CLlabel::CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,const xchar* t,bool f) : CLguibase(px,py,w,h,0,fc,bc,rc)
{
	frame = f;
	if(!f) rcolor = bcolor;
	
	text = clutils->clonechararray(t);
	textwidth = clgfx2->getfontstringwidth(text,0) + 4;
	textheight = clgfx2->getfontstringheight(text,0) + 4;
	if(w==-1 || w<textwidth) width = textwidth;
	if(h==-1 || h<textheight) height = textheight;
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}

void CLlabel::draw() const
{
	if(visible==0) return;
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx2->drawfontstring(posx+textx,posy+texty,text,0,fcolor,bcolor);
}

void CLlabel::settext(const xchar* t)
{
	delete text;
	text = clutils->clonechararray(t);
	textwidth = clgfx2->getfontstringwidth(text,0) + 4;
	textheight = clgfx2->getfontstringheight(text,0) + 4;
	if(width<textwidth) width = textwidth;
	if(height<textheight) height = textheight;
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}

xchar* CLlabel::gettext() const { return text; }

#endif

