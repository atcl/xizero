//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLWINDOW
#define HH_CLWINDOW

#include <FL/Fl.H>
#include <FL/Fl_Single_Window.H>
#include <FL/fl_draw.H>

#include "CLtypes.hh"

void timeout(void*)
{
	Fl::redraw();
	Fl::repeat_timeout(0.02,timeout);
}

class CLwindow : public Fl_Single_Window
{
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
		
		bool  havegamepad;
		
		void draw();
		virtual int handle(int event);
		
	public:
		CLwindow(xlong w,xlong h,const xchar* t,xlong* b);
		~CLwindow();
		void redraw();
		static xlong run();
		xlong getkey();
		xlong getturbo();
		xlong getmousex();
		xlong getmousey();
		xlong getmouseb();
};

void CLwindow::draw()
{
	fl_draw_image((const uxchar*)&buffer[0],0,0,width,height,4,hdelta);
}

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

CLwindow::CLwindow(xlong w,xlong h,const xchar* t,xlong* b) : Fl_Single_Window(w,h,t)
{
	width = w;
	height = h;
	//title = t;
	buffer = b;
	
	box(FL_NO_BOX);
	hdelta = 4* width;
	
	Fl::visual(FL_RGB);
	Fl::add_timeout(0.02,timeout);
	
	this->end();
	this->show();
}

CLwindow::~CLwindow() {}

void CLwindow::redraw()
{
	Fl::redraw();
}

xlong CLwindow::run()
{
	return Fl::wait();
}

xlong CLwindow::getkey()
{
	xlong temp = key;
	key = 0;
	return temp;
}

xlong CLwindow::getturbo()
{
	return turbo;
}

xlong CLwindow::getmousex()
{
	return mousex;
}

xlong CLwindow::getmousey()
{
	return mousey;
}

xlong CLwindow::getmouseb()
{
	return mouseb;
}

#endif

