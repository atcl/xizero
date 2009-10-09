//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
#pragma message "Compiling " __FILE__ 

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

class CLwindow : public virtual CLcl, public CLsingle<CLwindow>
{
	friend class CLsingle<CLwindow>;
	
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
		void showcursor();
		void hidecursor();
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
	
	xchar* dbuffer = (xchar*)(cldoublebuffer.getbuffer());
	
	//init window
	Xdisplay = XOpenDisplay(0);
	Xscreen = DefaultScreenOfDisplay(Xdisplay);
	Xvisual = DefaultVisualOfScreen(Xscreen);
	int blackcolor = BlackPixel(Xdisplay,DefaultScreen(Xdisplay));
	Xwindow = XCreateSimpleWindow(Xdisplay,DefaultRootWindow(Xdisplay),0,0,width,height,0,blackcolor,blackcolor);
	Xgc = XCreateGC(Xdisplay,Xwindow,0,0);
	//init events
	XSelectInput(Xdisplay,Xwindow,ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|LeaveWindowMask|EnterWindowMask);
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
	Ximage = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,dbuffer,width,height,32,width<<2);
	//init cursors
	XColor dummy;
	xchar data[1] = {0};
	Pixmap blank = XCreateBitmapFromData(Xdisplay,Xwindow,data,1,1);
	Xblank = XCreatePixmapCursor(Xdisplay,blank,blank,&dummy,&dummy,0,0);
	XFreePixmap(Xdisplay,blank);
	//complete:
	sprite* Tcursor = clformat->loadxpm(CLxzcursor);
	XImage* Icursor = XCreateImage(Xdisplay,Xvisual,24,ZPixmap,0,(xchar*)Tcursor->data,Tcursor->width,Tcursor->height,32,(Tcursor->width)<<2);
	Pixmap cursor = XCreatePixmap(Xdisplay,DefaultRootWindow(Xdisplay),Tcursor->width,Tcursor->height,24);
	XPutImage(Xdisplay,cursor,Xgc,Icursor,0,0,0,0,Ticon->width,Ticon->height);
	//Xcursor = XCreatePixmapCursor(Xdisplay,cursor,cursor,&dummy,&dummy,0,0);
	//*
	XDefineCursor(Xdisplay,Xwindow,Xblank);
}

void CLwindow::draw() { XPutImage(Xdisplay,Xwindow,Xgc,Ximage,0,0,0,0,width,height); }

void CLwindow::handle()
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
					turbo = key = XLookupKeysym((XKeyEvent *)&Xevent,0);
					while(XCheckWindowEvent(Xdisplay,Xwindow,KeyPressMask,&Xevent)) turbo = key = XLookupKeysym((XKeyEvent *)&Xevent,0);
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
					clsystem->exit(0,0,"xizero exits","bye");
				break;
			}
		}
}

xlong CLwindow::run() { handle(); draw(); return 1; }

void CLwindow::showcursor() { XDefineCursor(Xdisplay,Xwindow,Xcursor); }

void CLwindow::hidecursor() { XDefineCursor(Xdisplay,Xwindow,Xblank); }

xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; }

xlong CLwindow::getturbo() const { return turbo; }

xlong CLwindow::getmousex() const { return mousex; }

xlong CLwindow::getmousey() const {	return mousey; }

xlong CLwindow::getmouselb() const { return mouselb; }

xlong CLwindow::getmouserb() const { return mouserb; }
	
#endif

