//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
#pragma message "Compiling " __FILE__ 

#include <FL/Fl.H>
#include <FL/Fl_Single_Window.H>
#include <FL/fl_draw.H>
#include <FL/x.H>

#include "CLtypes.hh"
#include "CLresource.hh"
#include "CLpixmap.hh"

/* class name:	CLwindow
 * 
 * description:	The fltk interface class handles the main window
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

void timeout(void*)
{
	Fl::redraw();
	Fl::repeat_timeout(0.02,timeout); //change time interval?
}

class CLwindow : public Fl_Single_Window, public virtual CLcl, public CLsingle<CLwindow>
{
	friend class CLsingle<CLwindow>;
	
	private:
		xlong width;
		xlong height;
		xlong hdelta;
		xchar* title;
		xlong* buffer;
		xlong key;
		xlong turbo;
		xlong mousex;
		xlong mousey;
		xlong mouseb;
		xlong keyup;
		void draw();
		virtual int handle(int event);
		CLwindow();
		~CLwindow() { };
	public:
		void init(xlong w,xlong h,const xchar* t);
		void redraw();
		static xlong run();
		xlong getinkey();
		xlong getturbo() const;
		xlong getmousex() const;
		xlong getmousey() const;
		xlong getmouseb() const;
};

void CLwindow::draw() { fl_draw_image((const uxchar*)&buffer[0],0,0,width,height,4,hdelta); }

int CLwindow::handle(int event)
{
	switch(event)
	{
		case FL_KEYDOWN:
			turbo = Fl::event_key();
			key = Fl::event_key();
			break;
		case FL_KEYUP:
			keyup = Fl::event_key();
			if(keyup==turbo) turbo=0;
			break;
		case FL_MOVE:
			mousex = Fl::event_x();
			mousey = Fl::event_y();
			break;
		case FL_RELEASE:
			mouseb = 0;
			break;
		case FL_PUSH:
			mouseb = Fl::event_button() & 3;
			break;		
	}
	
	return Fl_Window::handle(event);
}

CLwindow::CLwindow() : Fl_Single_Window(0,0,"") { }

void CLwindow::init(xlong w,xlong h,const xchar* t)
{
	this->label(t);
	this->size(w,h);
	
	width = w;
	height = h;
	buffer = cldoublebuffer.getbuffer();
	
	box(FL_NO_BOX);
	hdelta = 4* width;
	
	Fl::visual(FL_RGB);
	Fl::add_timeout(0.02,timeout);
	
	fl_open_display();
	
	#ifdef WIN32
		uxchar* temp = xpm2ico(CLicon);
		this->icon((xchar*)LoadIcon(fl_display,temp));
	#else //ifdef LINUX
		Pixmap p = XCreatePixmap(fl_display,DefaultRootWindow(fl_display),16,16,32);
		//set pixmap p to CLicon
		this->icon((xchar*)p);
	#endif
	
	this->end();
	this->show();
}

void CLwindow::redraw() { Fl::redraw(); }

xlong CLwindow::run() { return Fl::wait(); }

xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; }

xlong CLwindow::getturbo() const { return turbo; }

xlong CLwindow::getmousex() const { return mousex; }

xlong CLwindow::getmousey() const {	return mousey; }

xlong CLwindow::getmouseb() const {	return mouseb; }

#endif

