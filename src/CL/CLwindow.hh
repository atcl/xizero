//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
#pragma message "Compiling " __FILE__ 

//~ #include <FL/Fl.H>
//~ #include <FL/Fl_Single_Window.H>
//~ #include <FL/fl_draw.H>
//~ #include <FL/x.H>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "CLtypes.hh"
#include "CLresource.hh"
#include "CLformat.hh"

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

//~ void timeout(void*)
//~ {
	//~ Fl::redraw();
	//~ Fl::repeat_timeout(0.02,timeout); //change time interval?
//~ }
//~ 
//~ class CLwindow : public Fl_Single_Window, public virtual CLcl, public CLsingle<CLwindow>
//~ {
	//~ friend class CLsingle<CLwindow>;
	//~ 
	//~ private:
		//~ xlong width;
		//~ xlong height;
		//~ xlong hdelta;
		//~ xchar* title;
		//~ uxlong* buffer;
		//~ xlong key;
		//~ xlong keyup;
		//~ xlong turbo;
		//~ xlong mousex;
		//~ xlong mousey;
		//~ xlong mouselb;	
		//~ xlong mouserb;	
		//~ virtual int handle(int event);
		//~ CLwindow();
		//~ ~CLwindow() { };
	//~ public:
		//~ void init(xlong w,xlong h,const xchar* t);
		//~ void draw();
		//~ void redraw();
		//~ static xlong run();
		//~ xlong getinkey();
		//~ xlong getturbo() const;
		//~ xlong getmousex() const;
		//~ xlong getmousey() const;
		//~ xlong getmouselb() const;
		//~ xlong getmouserb() const;
//~ };
//~ 
//~ void CLwindow::draw() { fl_draw_image((const uxchar*)&buffer[0],0,0,width,height,4,hdelta); }
//~ 
//~ int CLwindow::handle(int event)
//~ {
	//~ switch(event)
	//~ {
		//~ case FL_KEYDOWN:
			//~ turbo = Fl::event_key();
			//~ key = Fl::event_key();
		//~ break;
		//~ 
		//~ case FL_KEYUP:
			//~ keyup = Fl::event_key();
			//~ if(keyup==turbo) turbo=0;
		//~ break;
		//~ 
		//~ case FL_MOVE:
			//~ mousex = Fl::event_x();
			//~ mousey = Fl::event_y();
		//~ break;
		//~ 
		//~ case FL_RELEASE:
			//~ mouselb = mouserb = 0;
		//~ break;
		//~ 
		//~ case FL_PUSH:
			//~ mouselb = Fl::event_button() & 1;
			//~ mouserb = Fl::event_button() & 2;
		//~ break;
		//~ 
		//~ case FL_ENTER:
			//~ cursor(FL_CURSOR_NONE);
		//~ break;
		//~ 
		//~ case FL_LEAVE:
			//~ cursor(FL_CURSOR_DEFAULT);
		//~ break;	
	//~ }
	//~ 
	//~ return Fl_Window::handle(event);
//~ }
//~ 
//~ CLwindow::CLwindow() : Fl_Single_Window(0,0,"") { }
//~ 
//~ void CLwindow::init(xlong w,xlong h,const xchar* t)
//~ {
	//~ this->label(t);
	//~ this->size(w,h);
	//~ 
	//~ width = w;
	//~ height = h;
	//~ buffer = cldoublebuffer.getbuffer();
	//~ 
	//~ box(FL_NO_BOX);
	//~ hdelta = 4* width;
	//~ 
	//~ Fl::visual(FL_RGB);
	//~ Fl::add_timeout(0.02,timeout);
	//~ 
	//~ fl_open_display();
	//~ 
	//~ sprite* tempicon = clformat->loadxpm(CLicon);
	//~ 
	//~ #ifdef WIN32
		//~ this->icon((xchar*)LoadIcon(fl_display,tempicon->data));
	//~ #else //ifdef LINUX
		//~ XImage* xiicon = XCreateImage(fl_display,fl_visual->visual,32,ZPixmap,0,(xchar*)tempicon->data,tempicon->width,tempicon->height,0,(tempicon->width)<<2);
		//~ Pixmap p = XCreatePixmap(fl_display,DefaultRootWindow(fl_display),tempicon->width,tempicon->height,24);
		//~ //XPutImage(fl_display,p,fl_gc,xiicon,0,0,0,0,tempicon->width,tempicon->height); //crashes here!
		//~ this->icon((xchar*)p);
	//~ #endif
	//~ 
	//~ this->end();
	//~ this->show();
//~ }
//~ 
//~ void CLwindow::redraw() { Fl::redraw(); }
//~ 
//~ xlong CLwindow::run() { return Fl::wait(); }
//~ 
//~ xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; }
//~ 
//~ xlong CLwindow::getturbo() const { return turbo; }
//~ 
//~ xlong CLwindow::getmousex() const { return mousex; }
//~ 
//~ xlong CLwindow::getmousey() const {	return mousey; }
//~ 
//~ xlong CLwindow::getmouselb() const { return mouselb; }
//~ 
//~ xlong CLwindow::getmouserb() const { return mouserb; }


//************************************************************

class CLwindow : public virtual CLcl, public CLsingle<CLwindow>
{
	friend class CLsingle<CLwindow>;
	
	protected:
		Display* Xdisplay;
		Screen* Xscreen;
		Visual* Xvisual;
		Window Xwindow;
		GC Xgc;
		XImage* Ximage;
		XEvent Xevent;
		XImage* Xicon;
		Pixmap Xpixmap;
	
		uxlong width;
		uxlong height;
		const xchar* title;
		xlong key;
		xlong keyup;
		xlong turbo;
		xlong mousex;
		xlong mousey;
		xlong mouselb;	
		xlong mouserb;	;
		CLwindow() { };
		~CLwindow();
		void handle();
	public:
		void init(uxlong w,uxlong h,const xchar* t);
		void draw();
		xlong run();
		xlong getinkey();
		xlong getturbo() const;
		xlong getmousex() const;
		xlong getmousey() const;
		xlong getmouselb() const;
		xlong getmouserb() const;
};

CLwindow::~CLwindow()
{
	XDestroyImage(Ximage);
	XFreeGC(Xdisplay,Xgc);
	XDestroyWindow(Xdisplay,Xwindow);
	XCloseDisplay(Xdisplay);	
}

void CLwindow::init(uxlong w,uxlong h,const xchar* t)
{
	width = w;
	height = h;
	title = t;
	
	Xdisplay = XOpenDisplay(0);
	Xscreen = DefaultScreenOfDisplay(Xdisplay);
	Xvisual = DefaultVisualOfScreen(Xscreen);
	int blackcolor = BlackPixel(Xdisplay,DefaultScreen(Xdisplay));
	Xwindow = XCreateSimpleWindow(Xdisplay,DefaultRootWindow(Xdisplay),0,0,width,height,0,blackcolor,blackcolor);
	Xgc = XCreateGC(Xdisplay,Xwindow,0,0);
	XStoreName(Xdisplay,Xwindow,title);
	Ximage = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,(xchar*)(cldoublebuffer.getbuffer()),width,height,32,width<<2);
	//~ sprite* Ticon = clformat->loadxpm(CLicon);
	//~ Xicon = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,(xchar*)Ticon->data,Ticon->width,Ticon->height,0,(Ticon->width)<<2);
	//~ Xpixmap = XCreatePixmap(Xdisplay,DefaultRootWindow(Xdisplay),Ticon->width,Ticon->height,24);
	//~ XPutImage(Xdisplay,Xpixmap,Xgc,Xicon,0,0,0,0,Ticon->width,Ticon->height);
	//~ XWMHints* Xhints;
	//~ Xhints->flags = IconPixmapHint;
	//~ Xhints->icon_pixmap = Xpixmap;
	//~ XSetWMHints(Xdisplay,Xwindow,Xhints);
	//~ XFree(Xhints);
	XMapRaised(Xdisplay,Xwindow);
}

void CLwindow::draw()
{
	XPutImage(Xdisplay,Xwindow,Xgc,Ximage,0,0,0,0,width,height);
	XFlush(Xdisplay); //necessary?
}

void CLwindow::handle()
{
		XNextEvent(Xdisplay,&Xevent);
		switch(Xevent.type)
		{
			case Expose:
				draw();
			break;
			
			case KeyPress:
				turbo = key = XLookupKeysym((XKeyEvent *)&Xevent,0);
// 				ClearKBuf(xw);
			break;
			
			case KeyRelease:
				keyup = XLookupKeysym((XKeyEvent *)&Xevent,0);
				if(keyup==turbo) turbo=0;
			break;
			
			case ButtonPress:
				switch(Xevent.xbutton.button)
				{
					case Button1:
						mousex = Xevent.xbutton.x;
						mousey = Xevent.xbutton.y;
						mouselb = 1;
					break;
					
					case Button2: 
						mousex = Xevent.xbutton.x;
						mousey = Xevent.xbutton.y;
						mouserb = 1;
					break;
				}
			break;
			
			case ButtonRelease:
				switch(Xevent.xbutton.button)
				{
					case Button1:
						mousex = Xevent.xbutton.x;
						mousey = Xevent.xbutton.y;
						mouselb = 1;
					break;
					
					case Button2: 
						mousex = Xevent.xbutton.x;
						mousey = Xevent.xbutton.y;
						mouserb = 1;
					break;
				}
			break;
			
			case MotionNotify:
				mousex = Xevent.xmotion.x;
				mousey = Xevent.xmotion.y;
			break;
			
			case EnterNotify:
				XDefineCursor(Xdisplay,Xwindow,None);
			break;
			
			case LeaveNotify:
				XUndefineCursor(Xdisplay,Xwindow);
			break;
		}
}

xlong CLwindow::run() { handle(); draw(); return 1; }

xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; }

xlong CLwindow::getturbo() const { return turbo; }

xlong CLwindow::getmousex() const { return mousex; }

xlong CLwindow::getmousey() const {	return mousey; }

xlong CLwindow::getmouselb() const { return mouselb; }

xlong CLwindow::getmouserb() const { return mouserb; }
	
#endif

