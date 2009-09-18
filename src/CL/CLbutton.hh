//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUTTON
#define HH_CLBUTTON
#pragma message "Compiling " __FILE__ " ! TODO: constructor,action"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLutils.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"

/* class name:	CLbutton
 * 
 * description:	A standard gui element button
 * 
 * author:	atcl
 * 
 * notes:	finish implementing.
 * 
 * version: 0.1
 */

class CLbutton : public CLguibase
{
	private:
		void (*action)();
		xchar* caption;
		bool flat;
	public:
		CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),xchar *c,bool f);
		~CLbutton();
		void draw();
		void setaction(void(*a)());
		void setcaption(xchar* t);
		void setflat(bool f);
		xchar* getcaption();
};

CLbutton::CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),xchar *c,bool f) : CLguibase(px,py,w,h,fc,bc,rc)
{
	//set up attributes
	action = a;
	caption = c;
	flat = f;
	//*
}

CLbutton::~CLbutton() { delete[] caption; }

void CLbutton::draw()
{
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	
	//drawtext
}

void CLbutton::setaction(void(*a)())
{
	action = a;
}

void CLbutton::setcaption(xchar* t)
{
	delete caption;
	xlong s = clutils->chararraylength(t);
	caption = new xchar[s];
	for(uxlong i=0; i<s ;i++)
	{
		caption[i] = t[i];
	}
}

void CLbutton::setflat(bool f) { flat = f; }

xchar* CLbutton::getcaption() { return caption; }

#endif

