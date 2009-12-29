///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
///*

///sys includes
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
///*

///api includes
#include "CLtypes.hh"
#include "CLresource.hh"
#include "CLformat.hh"
#include "CLapp.hh"
#include "CLbase.hh" 
///*

///header
/* class name:	CLwindow
 * 
 * description:	very basic Xwindow with doublebuffer and msgbox
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLwindow : public CLbase<CLwindow,1>
{
	friend class CLbase<CLwindow,1>;
	
	private:
		static CLformat* clformat;
		static CLapp*    clapp;
		static CLgfx1*   clgfx1;
	protected:
		Display* Xdisplay;
		Screen* Xscreen;
		Visual* Xvisual;
		Window Xwindow;
		GC Xgc;
		Atom Xatom;
		XImage* Ximage;
		XEvent Xevent;
		Cursor Xblank;
		Cursor Xcursor;
		Colormap Xcolormap;
		XColor Xwhite;
		XColor Xblack;
		XColor Xgrey;

		uxlong width;
		uxlong height;
		const xchar* title;
		xlong key;
		xlong keyup;
		xlong turbo;
		xlong mousex;
		xlong mousey;
		xlong mouselb;	
		xlong mouserb;
		bool  displaycursor;
		sprite* cursor;
		uxchar syskey;
		void (*sysmenu)(void* o);
		void* sysobj;
		CLwindow();
		~CLwindow();
		void handle();
	public:
		void draw();
		xlong run();
		void showcursor() { displaycursor = 1; };
		void hidecursor() { displaycursor = 0; };
		void setcursor(sprite* s) { cursor = s; };
		xlong getinkey();
		xlong getturbo();
		xlong getmousex() const { return mousex; };
		xlong getmousey() const { return mousey; };
		xlong getmouselb() const { return mouselb; };
		xlong getmouserb() const { return mouserb; };
		void setsyskey(uxchar k=0,void (*m)(void* o)=0,void* mo=0);
};

CLformat* CLwindow::clformat = CLformat::instance();
CLapp*    CLwindow::clapp    = CLapp::instance();
CLgfx1*   CLwindow::clgfx1   = CLgfx1::instance();
///*

///implementation
CLwindow::CLwindow() //! noncritical
{
	syskey = 0;
	cursor = 0;
	displaycursor = 0;
	width = XRES;
	height = YRES;
	title = TITLE;
	
	//init window
	Xdisplay = XOpenDisplay(0);
	Xscreen = DefaultScreenOfDisplay(Xdisplay);
	Xvisual = DefaultVisualOfScreen(Xscreen);
	Xwindow = XCreateSimpleWindow(Xdisplay,DefaultRootWindow(Xdisplay),0,0,width,height,0,Xblack.pixel,Xblack.pixel);
	Xgc = XCreateGC(Xdisplay,Xwindow,0,0);
	Xcolormap = DefaultColormap(Xdisplay,XDefaultScreen(Xdisplay));
	XColor dummy;
	XAllocNamedColor(Xdisplay,Xcolormap,"white",&Xwhite,&dummy);
	XAllocNamedColor(Xdisplay,Xcolormap,"black",&Xblack,&dummy);
	XAllocNamedColor(Xdisplay,Xcolormap,"grey",&Xgrey,&dummy);
	//init events
	XAutoRepeatOn(Xdisplay);
	XSelectInput(Xdisplay,Xwindow,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|LeaveWindowMask|EnterWindowMask);
	//init window title
	XStoreName(Xdisplay,Xwindow,title);
	//init icon
	sprite* Ticon = clformat->loadxpm(CLicon);
	XImage* Xicon = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,(xchar*)Ticon->data,Ticon->width,Ticon->height,32,(Ticon->width)<<2);
	Pixmap icon = XCreatePixmap(Xdisplay,DefaultRootWindow(Xdisplay),Ticon->width,Ticon->height,24);
	XPutImage(Xdisplay,icon,Xgc,Xicon,0,0,0,0,Ticon->width,Ticon->height);
	XWMHints* Xhints;
	Xhints = XAllocWMHints();
	Xhints->flags = IconPixmapHint;
	Xhints->icon_pixmap = icon;
	XSetWMHints(Xdisplay,Xwindow,Xhints);
	XFree(Xhints);
	//init close button
	Xatom = XInternAtom(Xdisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(Xdisplay,Xwindow,&Xatom,1);
	//show window
	XMapRaised(Xdisplay,Xwindow);
	//init doublebuffer
	xchar* dbuffer = (xchar*)(cldoublebuffer.getbuffer());
	Ximage = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,dbuffer,width,height,32,width<<2);
	//init x-cursor
	xchar data[1] = {0};
	Pixmap blank = XCreateBitmapFromData(Xdisplay,Xwindow,data,1,1);
	Xblank = XCreatePixmapCursor(Xdisplay,blank,blank,&dummy,&dummy,0,0);
	XFreePixmap(Xdisplay,blank);
	XDefineCursor(Xdisplay,Xwindow,Xblank);
}

CLwindow::~CLwindow() //! noncritical
{
	XDestroyImage(Ximage);
	XFreeGC(Xdisplay,Xgc);
	XDestroyWindow(Xdisplay,Xwindow);
	XCloseDisplay(Xdisplay);	
}

void CLwindow::draw() { XPutImage(Xdisplay,Xwindow,Xgc,Ximage,0,0,0,0,width,height); } //! noncritical

void CLwindow::handle() //! critical
{
	if(XPending(Xdisplay)!=0)
	{
		XNextEvent(Xdisplay,&Xevent);
		switch(Xevent.type)
		{
			case Expose:
				draw();
			break;
			
			case KeyPress:
				key = turbo = xchar(XLookupKeysym((XKeyEvent*)&Xevent,0));
				while(XCheckWindowEvent(Xdisplay,Xwindow,KeyPressMask,&Xevent));
				if(key == syskey && syskey!=0) { sysmenu(sysobj); }
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
						mouselb = 0;
					break;
					
					case Button2: 
						mousex = Xevent.xbutton.x;
						mousey = Xevent.xbutton.y;
						mouserb = 0;
					break;
				}
			break;
			
			case MotionNotify:
				mousex = Xevent.xmotion.x;
				mousey = Xevent.xmotion.y;
				while(XCheckWindowEvent(Xdisplay,Xwindow,PointerMotionMask,&Xevent))
				{
					mousex = Xevent.xmotion.x;
					mousey = Xevent.xmotion.y;
				}
			break;
			
			case EnterNotify:
				XDefineCursor(Xdisplay,Xwindow,Xblank);
			break;
			
			case LeaveNotify:
				XUndefineCursor(Xdisplay,Xwindow);
			break;
			
			case ClientMessage:
				clapp->exit(0,"xizero exits: bye");
			break;
		}
	}
}

xlong CLwindow::run() { if(cursor!=0 && displaycursor==1) { clgfx1->drawsprite(mousex,mousey,cursor); } handle(); draw(); return 1; } //! noncritical

xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; } //! noncritical

xlong CLwindow::getturbo() { xlong temp = turbo; turbo = 0; return temp; } //! noncritical

void CLwindow::setsyskey(uxchar k,void (*m)(void* o),void* mo) { syskey = k; sysmenu = m; sysobj = mo; } //! noncritical
///*
	
#endif

