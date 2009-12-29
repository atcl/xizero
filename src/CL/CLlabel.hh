///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLLABEL
#define HH_CLLABEL
///*

///includes
#include "CLtypes.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"
///*

///header
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
///*

///definitions
class CLlabel : public CLguibase
{
	private:
		static CLstring* clstring;
		static CLgfx2*   clgfx2;
	protected:
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
		xchar* gettext() const { return text; };
};

CLstring* CLlabel::clstring = CLstring::instance();
CLgfx2*   CLlabel::clgfx2   = CLgfx2::instance();
///*

///implementation
CLlabel::CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,const xchar* t,bool f) : CLguibase(px,py,w,h,0,fc,bc,rc) //! noncritical
{
	frame = f;
	if(!f) rcolor = bcolor;
	
	text = clstring->copy(t);
	textwidth = clgfx2->getfontstringwidth(text,0) + 4;
	textheight = clgfx2->getfontstringheight(text,0);
	if(w==-1 || w<textwidth) width = textwidth;
	if(h==-1 || h<textheight) height = textheight;
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}

void CLlabel::draw() const //! noncritical
{
	if(visible==0) return;
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx2->drawfontstring(posx+textx,posy+texty,text,0,fcolor,bcolor);
}

void CLlabel::settext(const xchar* t) //! noncritical
{
	delete text;
	text = clstring->copy(t);
	textwidth = clgfx2->getfontstringwidth(text,0) + 4;
	textheight = clgfx2->getfontstringheight(text,0);
	if(width<textwidth) width = textwidth;
	if(height<textheight) height = textheight;
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}
///*

#endif

