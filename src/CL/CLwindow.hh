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
 * description:	very basic Xwindow with doublebuffer and msgbox
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
		CLwindow();
		~CLwindow();
		void handle();
	public:
		void draw();
		xlong run();
		void showcursor();
		void hidecursor();
		void setcursor(sprite* s);
		xlong getinkey();
		xlong getturbo();
		xlong getmousex() const;
		xlong getmousey() const;
		xlong getmouselb() const;
		xlong getmouserb() const;
		xlong msgbox(const xchar* title,const xchar* message);
};

CLwindow::CLwindow()
{
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

CLwindow::~CLwindow()
{
	XDestroyImage(Ximage);
	XFreeGC(Xdisplay,Xgc);
	XDestroyWindow(Xdisplay,Xwindow);
	XCloseDisplay(Xdisplay);	
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
				key = turbo = xchar(XLookupKeysym((XKeyEvent*)&Xevent,0));
				while(XCheckWindowEvent(Xdisplay,Xwindow,KeyPressMask,&Xevent));
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
				clsystem->exit(0,0,"xizero exits","bye");
			break;
		}
	}
}

xlong CLwindow::run() { if(cursor!=0 && displaycursor==1) { clgfx1->drawsprite(mousex,mousey,cursor); } handle(); draw(); return 1; }

void CLwindow::showcursor() { displaycursor = 1; }

void CLwindow::hidecursor() { displaycursor = 0; }

void CLwindow::setcursor(sprite* s) { cursor = s; }

xlong CLwindow::getinkey() { xlong temp = key; key = 0; return temp; }

xlong CLwindow::getturbo() { xlong temp = turbo; turbo = 0; return temp; }

xlong CLwindow::getmousex() const { return mousex; }

xlong CLwindow::getmousey() const {	return mousey; }

xlong CLwindow::getmouselb() const { return mouselb; }

xlong CLwindow::getmouserb() const { return mouserb; }

xlong CLwindow::msgbox(const xchar* title,const xchar* message)
{
	//prepare message
	xlong msglines = clutils->getlinecount(message);
	xlong msglength = clutils->chararraylength(message);
	xlong* msgpos = new xlong[msglines];
	xlong* msglen = new xlong[msglines];
	xlong npos = 0;
	for(xlong i=0; i<msglines; i++)
	{
		msgpos[i] = npos;
		while(npos<msglength) { if(message[npos]=='\n') { break; } npos++; }
		msglen[i] = npos - msgpos[i];
		npos++;
	}
	xlong maxline = 0;
	for(xlong i=1; i<msglines; i++) { if(msglen[i]>msglen[maxline]) maxline = i; }
	//create window
	XFontStruct *font = XQueryFont(Xdisplay,XGContextFromGC(Xgc));
	xlong winwidth = 20+XTextWidth(font,&message[msgpos[maxline]],msglen[maxline]);
	xlong winheight = 40+msglines*16;
	if(winwidth<200) winwidth = 200;
	if(winwidth>800) winwidth = 400;
	if(winheight<100) winheight = 100;
	if(winheight>600) winheight = 600;
	Window msgbox = XCreateSimpleWindow(Xdisplay,DefaultRootWindow(Xdisplay),0,0,winwidth,winheight,0,Xgrey.pixel,Xgrey.pixel);
	XSelectInput(Xdisplay,msgbox,ExposureMask|KeyPressMask|ButtonPressMask|StructureNotifyMask);
	//set title
	XStoreName(Xdisplay,msgbox,title);
	//show window
	XMapRaised(Xdisplay,msgbox);
	for(;;) { XNextEvent(Xdisplay, &Xevent); if (Xevent.type == MapNotify) break; }
	xlong j= 0;
	
	//wait till press
	bool wait = 0;
	while(wait==0)
	{
		if(XPending(Xdisplay)!=0)
		{			
			XNextEvent(Xdisplay,&Xevent);
			switch(Xevent.type)
			{				
				case Expose:
					XSetForeground(Xdisplay,Xgc,Xblack.pixel);
					XSetBackground(Xdisplay,Xgc,Xgrey.pixel);
					for(j=0; j<msglines; j++) { XDrawImageString(Xdisplay,msgbox,Xgc,10,20+16*j,&message[msgpos[j]],msglen[j]); }
					XDrawLine(Xdisplay,msgbox,Xgc,(winwidth/2)-50,winheight-10,(winwidth/2)+50,winheight-10);
					XDrawLine(Xdisplay,msgbox,Xgc,(winwidth/2)+50,winheight-30,(winwidth/2)+50,winheight-10);	
					XDrawImageString(Xdisplay,msgbox,Xgc,(winwidth/2)-5,winheight-15,"OK",2);
					XSetForeground(Xdisplay,Xgc,Xwhite.pixel);
					XDrawLine(Xdisplay,msgbox,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)-50,winheight-10);
					XDrawLine(Xdisplay,msgbox,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)+50,winheight-30);
				break;
				case KeyPress: if(XLookupKeysym((XKeyEvent*)&Xevent,0)==32) wait = 1; break;
				case ButtonPress: if(Xevent.xbutton.button == Button1 && Xevent.xbutton.x>(winwidth/2)-50 && Xevent.xbutton.y>winheight-30 && Xevent.xbutton.x<(winwidth/2)+50 && Xevent.xbutton.y<winheight-10 ) { wait = 1; } break; 
			}				
		}
		
		draw();
	}
	//destroy window
	XDestroyWindow(Xdisplay,msgbox);
	return 1;
}
	
#endif

