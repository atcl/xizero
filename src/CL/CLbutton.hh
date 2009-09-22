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
		static CLlist<CLbutton>* buttonlist;
	public:
		CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),xchar *c,bool f);
		~CLbutton();
		void draw() const;
		void setaction(void(*a)());
		void setcaption(xchar* t);
		void setvisible(bool v);
		xchar* getcaption() const;
		void click();
		static void checkclick();
};

CLlist<CLbutton>* CLbutton::buttonlist = new CLlist<CLbutton>;

CLbutton::CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),xchar *c,bool f) : CLguibase(px,py,w,h,fc,bc,rc)
{
	action = a;
	
	caption = clutils->clonechararray(c);
	xlong nw = clgfx2->getfontstringwidth(caption,0) + 4;
	xlong nh = clgfx2->getfontstringheight(caption,0) + 4;
	if(w==-1 || w<nw) width = nw;
	if(h==-1 || h<nh) height = nh;

	buttonlist->append(this);
}

CLbutton::~CLbutton() { delete[] caption; }

void CLbutton::draw() const
{
	if(visible==0) return;
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx2->drawfontstring(posx+2,posy+2,caption,0,fcolor,bcolor);
}

void CLbutton::setaction(void(*a)()) { action = a; }

void CLbutton::setcaption(xchar* t)
{
	delete caption;
	caption = clutils->clonechararray(t);
	xlong nw = clgfx2->getfontstringwidth(t,0) + 4;
	xlong nh = clgfx2->getfontstringheight(t,0) + 4;
	if(width<nw) width = nw;
	if(height<nh) height = nh;
}

void CLbutton::setvisible(bool v)
{
	if(visible==0 && v==1)
	{
		visible = 1;
		buttonlist->append(this);
	}
	else if(visible==1 && v==0)
	{
		visible = 0;
		buttonlist->seekdata(this);
		buttonlist->delcurrent();
	}
}

xchar* CLbutton::getcaption() const { return caption; }

void CLbutton::click() { action(); }

void CLbutton::checkclick()
{
	xlong mx = clwindow->getmousex();
	xlong my = clwindow->getmousey();
	
	for(xlong i=buttonlist->setfirst(); i<buttonlist->getlength(); i+=buttonlist->setnext())
	{
		CLbutton* curr = buttonlist->getcurrentdata();
		if(mx>curr->getx() && mx<curr->getx()+curr->getwidth() && my>curr->gety() && my<curr->gety()+curr->getheight()) curr->click();  
	}
}

#endif

