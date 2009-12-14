///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
///*

///includes
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "CLtypes.hh"
///*

///header
/* class name:	CLmsgbox
 * 
 * description:	
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLmsgbox : public virtual CLcl, public CLsingle<CLmsgbox>
{
	private:
		Display* Xdisplay;
		Window Xwindow;
		GC Xgc;
		XEvent Xevent;
		Colormap Xcolormap;
		XColor Xwhite;
		XColor Xblack;
		XColor Xgrey;
		XFontStruct* Xfont;
		
	public:
		CLmsgbox();
		~CLmsgbox();
		xlong msgbox(const xchar* title,const xchar* message);
};
///*

///implementation
CLmsgbox::CLmsgbox() //! noncritical
{
	Xdisplay = XOpenDisplay(0);
	Xcolormap = DefaultColormap(Xdisplay,XDefaultScreen(Xdisplay));
	XColor dummy;
	XAllocNamedColor(Xdisplay,Xcolormap,"white",&Xwhite,&dummy);
	XAllocNamedColor(Xdisplay,Xcolormap,"black",&Xblack,&dummy);
	XAllocNamedColor(Xdisplay,Xcolormap,"grey",&Xgrey,&dummy);
}

CLmsgbox::~CLmsgbox() //! noncritical
{
	XCloseDisplay(Xdisplay);	
}

xlong CLmsgbox::msgbox(const xchar* title,const xchar* message) //! noncritical
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
	Xwindow = XCreateSimpleWindow(Xdisplay,DefaultRootWindow(Xdisplay),0,0,1,1,0,Xgrey.pixel,Xgrey.pixel);
	Xgc = XCreateGC(Xdisplay,Xwindow,0,0);
	Xfont = XQueryFont(Xdisplay,XGContextFromGC(Xgc));
	xlong winwidth = 20+XTextWidth(Xfont,&message[msgpos[maxline]],msglen[maxline]);
	xlong winheight = 40+msglines*16;
	if(winwidth<200) winwidth = 200;
	if(winwidth>800) winwidth = 400;
	if(winheight<100) winheight = 100;
	if(winheight>600) winheight = 600;
	XResizeWindow(Xdisplay,Xwindow,winwidth,winheight);
	XSelectInput(Xdisplay,Xwindow,ExposureMask|KeyPressMask|ButtonPressMask|StructureNotifyMask);
	//set title
	XStoreName(Xdisplay,Xwindow,title);
	//show window
	XMapRaised(Xdisplay,Xwindow);
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
					for(j=0; j<msglines; j++) { XDrawImageString(Xdisplay,Xwindow,Xgc,10,20+16*j,&message[msgpos[j]],msglen[j]); }
					XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-10,(winwidth/2)+50,winheight-10);
					XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)+50,winheight-30,(winwidth/2)+50,winheight-10);	
					XDrawImageString(Xdisplay,Xwindow,Xgc,(winwidth/2)-5,winheight-15,u8"OK",2);
					XSetForeground(Xdisplay,Xgc,Xwhite.pixel);
					XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)-50,winheight-10);
					XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)+50,winheight-30);
				break;
				case KeyPress: if(XLookupKeysym((XKeyEvent*)&Xevent,0)==32) wait = 1; break;
				case ButtonPress: if(Xevent.xbutton.button == Button1 && Xevent.xbutton.x>(winwidth/2)-50 && Xevent.xbutton.y>winheight-30 && Xevent.xbutton.x<(winwidth/2)+50 && Xevent.xbutton.y<winheight-10 ) { wait = 1; } break; 
			}				
		}
	}
	//destroy window
	XFreeGC(Xdisplay,Xgc);
	XDestroyWindow(Xdisplay,Xwindow);
	XFlush(Xdisplay);
	return 1;
}
///*

#endif
