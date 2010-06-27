///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLLABEL
#define HH_CLLABEL
///*

///includes
#include "CLtypes.hh"
#include "CLguibase.hh"
#include "CLgfx.hh"
///*

///header
/* class name:	CLlabel
 * 
 * description:	A standard gui element label
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLlabel : public CLguibase
{
	private:
		CLstring& clstring;
		CLgfx&    clgfx;
	protected:
		xchar* text;
		bool  frame;
		xlong textwidth;
		xlong textheight;
		xlong textx;
		xlong texty;
	public:
		CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,const xchar* t,bool f);
		~CLlabel() { delete text; };
		void draw() const;
		void settext(const xchar* t);
		xchar* gettext() const { return text; };
};
///*

///implementation
CLlabel::CLlabel(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,const xchar* t,bool f) //! noncritical
: clstring(CLstring::instance()), clgfx(CLgfx::instance()), CLguibase(px,py,w,h,0,fc,bc,rc), frame(f)
{
	if(!f) { rcolor = bcolor; }
	text = clstring.copy(t);
	textwidth = clgfx.getfontstringwidth(text,0) + 4;
	textheight = clgfx.getfontstringheight(text,0);
	if(w==-1 || w<textwidth) { width = textwidth; }
	if(h==-1 || h<textheight) { height = textheight; }
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}

void CLlabel::draw() const //! noncritical
{
	if(visible==0) { return; }
	clgfx.drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx.drawfontstring(posx+textx,posy+texty,text,0,fcolor,bcolor);
}

void CLlabel::settext(const xchar* t) //! noncritical
{
	text = clstring.copy(t);
	textwidth = clgfx.getfontstringwidth(text,0) + 4;
	textheight = clgfx.getfontstringheight(text,0);
	if(width<textwidth) { width = textwidth; }
	if(height<textheight) { height = textheight; }
	textx = (width - textwidth)>>1;
	texty = (height - textheight)>>1;
}
///*

#endif

