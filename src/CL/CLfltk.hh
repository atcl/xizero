//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFLTK
#define HH_CLFLTK
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <time.h>
#include <string.h>

#include "CLtypes.hh"

#define UPDATE_RATE 0.05


class FLTKwindow : public Fl_Window
{
	doubleword screenbuffer[xres*yres];
	int keystate;
	int keyup;
	int tkeystate;
	int width;
	int height;

	void draw()
	{
		fl_draw_image((const uxchar*)&screenbuffer, 0, 0, width, height, 4, xres*4);
	}

	static void RenderImage_CB(void *userdata)
	{
		FLTKwindow *win = (FLTKwindow*)userdata;
		win->RenderImage();
		Fl::repeat_timeout(UPDATE_RATE, RenderImage_CB, userdata);
	}

	public:

	xlong mousex;
	xlong mousey;
	xlong mousebutton;

	FLTKwindow(int w, int h, const char *name=0) : Fl_Window(w,h,name)
	{
		RenderImage();
		Fl::add_timeout(UPDATE_RATE, RenderImage_CB, (void*)this);
		width = w;
		height = h;
	}

	void RenderImage()
	{
		memcpy((uxlong*)&screenbuffer,CLdoublebuffer->getbuffer(),(xres*yres*4));
		redraw();
	}

	virtual int handle(int event)
	{
		switch (event)
		{
			case FL_KEYBOARD:
				tkeystate = Fl::event_key();
				keystate = Fl::event_key();
				return 1;
			case FL_KEYUP:
				keyup = Fl::event_key();
				return 1;
// 			case: FL_KEYDOWN:
// 				keydown = Fl::event_key();
// 				return 1;
			case FL_MOVE:
				mousex = Fl::event_x();
				mousey = Fl::event_y();
			case FL_RELEASE:
				mousebutton = 0;
				break;
			case FL_PUSH:
				mousebutton = Fl::event_button() & 3;
		}

	 	return Fl_Window::handle(event);
	}

	int getkeystate()
	{
		
		int temp = keystate;
		keystate = 0;
		return temp;
	}

	int getturbokeystate()
	{
		if(keyup == tkeystate)
		{
			tkeystate = 0;
			keyup = 0;
			return 0;
		}
		else
		{
			return tkeystate;
		}
	}

	void setwindowtitle();
};


//********

FLTKwindow  *win;

void FLTKsetup()
{
	Fl::visual(FL_RGB);

	win = new FLTKwindow(xres, yres,"atCROSSLEVEL");
	win->end();
	win->show();
}

int FLTKwaitforesc()
{
	return(Fl::run());
}

int FLTKgetkeystate()
{
	return(win->getkeystate());
}

int FLTKgetturbokeystate()
{
	return(win->getturbokeystate());
}

int FLTKwait(int t=0)
{
	if(t!=0)
	{
		return int(Fl::wait(t));
	}
	else
	{
		return Fl::wait();
	}
}

xlong FLTKgetmousex()
{
	return win->mousex;
}

xlong FLTKgetmousey()
{
	return win->mousey;
}

xlong FLTKgetmousebutton()
{
	return win->mousebutton;
}

#endif
