//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFLTK
#define HH_CLFLTK
#pragma message "Compiling " __FILE__ " ! TODO: show widgets"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <time.h>
#include <string.h>

#include "CLtypes.hh"

#define UPDATE_RATE 0.05


class FLTKwindow : public Fl_Window
{
	doubleword screenbuffer[xres*yres];
	int keystate;
	int keyup;
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

	int mousex;
	int mousey;
	int mousebuttonl;
	int mousebuttonr;

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
				keystate = Fl::event_key();
				return 1;
			case FL_KEYUP:
				keyup = Fl::event_key();
				return 1;
			 case FL_MOVE:
				mousex = Fl::event_x();
				mousey = Fl::event_y();
			case FL_PUSH:
				mousebuttonl = Fl::event_button() && 1;
				mousebuttonr = Fl::event_button() && 2;
		}

	 	return Fl_Window::handle(event);
	}

	int getkeystate()
	{
		if(keyup == keystate)
		{
			keyup = 0;
			return keystate;
		}
		else
		{
			return 0;
		}
	}

	int getturbokeystate()
	{
		if(keyup == keystate)
		{
			keystate = 0;
			keyup = 0;
			return 0;
		}
		else
		{
			return keystate;
		}
	}

	void setwindowtitle();
};


//********

FLTKwindow  *win;
Fl_Menu_Bar *menubar;
Fl_Box      *box;

void Menu_CB(Fl_Widget *w, void*data)
{
	char picked[80];
	menubar->item_pathname(picked, sizeof(picked)-1);
	if ( strcmp(picked, "File/Quit") == 0 ) CLexit_(__func__,"Bye",0);
}

void FLTKsetup()
{
	Fl::visual(FL_RGB);

	win     = new FLTKwindow(xres, yres,"atCROSSLEVEL");
// 	menubar = new Fl_Menu_Bar(0, 0, 600, 25);
// 	box     = new Fl_Box(530,35,590,55,"fps:");

// 	box->labelcolor(FL_WHITE);
// 	box->box(FL_NO_BOX);

/*	menubar->add( "File/Load"  ,0,Menu_CB);
	menubar->add( "File/Quit"  ,0,Menu_CB);
	menubar->add( "Mode/Model"  ,0,Menu_CB);
	menubar->add( "Mode/Model/Wireframe" ,0,Menu_CB);
	menubar->add( "Mode/Model/Flat"      ,0,Menu_CB);
	menubar->add( "Mode/Model/Shadow"    ,0,Menu_CB);
	menubar->add( "Mode/Level" ,0,Menu_CB);
	menubar->add( "Mode/Level/Terrain"   ,0,Menu_CB);
	menubar->add( "Mode/Level/Enemies"   ,0,Menu_CB);
	menubar->add( "Mode/Level/Player"    ,0,Menu_CB);*/	

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

int FLTKgetmousex()
{
	return win->mousex;
}

int FLTKgetmousey()
{
	return win->mousey;
}

int FLTKgetmousebuttonl()
{
	return win->mousebuttonl;
}

int FLTKgetmousebuttonr()
{
	return win->mousebuttonr;
}



#endif
