///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLBUTTON
#define HH_CLBUTTON
///*

///includes
#include "CLtypes.hh"
#include "CLutils.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"
///*

///header
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
///*

///definitions
class CLbutton : public CLguibase
{
	private:
		static CLstring* clstring;
		static CLgfx2*   clgfx2;
		static CLwindow* clwindow;
	protected:
		void (*action)();
		xchar* caption;
		bool flat;
		xlong captionwidth;
		xlong captionheight;
		xlong captionx;
		xlong captiony;
		static CLlist<CLbutton>* buttonlist;
	public:
		CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),const xchar *c,bool f);
		~CLbutton();
		void draw() const;
		void setaction(void(*a)()) { action = a; };
		void setcaption(xchar* t);
		void setvisible(bool v);
		xchar* getcaption() const { return caption; };
		void click();
		static void checkclick();
};

CLstring* CLbutton::clstring = CLstring::instance();
CLgfx2*   CLbutton::clgfx2   = CLgfx2::instance();
CLwindow* CLbutton::clwindow = CLwindow::instance();
CLlist<CLbutton>* CLbutton::buttonlist = new CLlist<CLbutton>;
///*

///implementation
CLbutton::CLbutton(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,void(*a)(),const xchar *c,bool f) : CLguibase(px,py,w,h,f,fc,bc,rc) //! noncritical
{
	action = a;
	
	caption = clstring->copy(c);
	captionwidth = clgfx2->getfontstringwidth(caption,0) + 4;
	captionheight = clgfx2->getfontstringheight(caption,0);
	if(w==-1 || w<captionwidth) width = captionwidth;
	if(h==-1 || h<captionheight) height = captionheight;
	captionx = (width - captionwidth)>>1;
	captiony = (height - captionheight)>>1;
	flat = f;
	
	buttonlist->append(this);
}

CLbutton::~CLbutton() { delete[] caption; } //! noncritical

void CLbutton::draw() const //! critical
{
	if(visible==0) return;
	clgfx2->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,flat);
	clgfx2->drawfontstring(posx+captionx,posy+captiony,caption,0,fcolor,bcolor);
}

void CLbutton::setcaption(xchar* t) //! noncritical
{
	delete caption;
	caption = clstring->copy(t);
	captionwidth = clgfx2->getfontstringwidth(t,0) + 4;
	captionheight = clgfx2->getfontstringheight(t,0);
	if(width<captionwidth) width = captionwidth;
	if(height<captionheight) height = captionheight;
	captionx = (width - captionwidth)>>1;
	captiony = (height - captionheight)>>1;
}

void CLbutton::setvisible(bool v) //! noncritical
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

void CLbutton::click() { action(); } //! critical

void CLbutton::checkclick() //! critical
{
	if(clwindow->getmouselb()!=0)
	{
		xlong mx = clwindow->getmousex();
		xlong my = clwindow->getmousey();
		CLbutton* curr = 0;
		xlong cx1 = 0;
		xlong cy1 = 0;
		xlong cx2 = 0;
		xlong cy2 = 0;
		
		for(xlong i=buttonlist->setfirst(); i<buttonlist->getlength(); i+=buttonlist->setnext())
		{
			curr = buttonlist->getcurrentdata();
			cx1 = curr->getx();
			cx2 = cx1 + curr->getwidth();
			cy1 = curr->gety();
			cy2 = cy1 + curr->getheight();
			if(mx>cx1 && mx<cx2 && my>cy1 && my<cy2) curr->click();  
		}
	}
}
///*

#endif

