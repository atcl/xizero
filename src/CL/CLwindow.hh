//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
#pragma message: "Compiling " __FILE__ " ! TODO: "

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "CLtypes.hh"


namespace CLwindow
{
	//
	Display* d;
	Window   w;
	Atom     a;
	XEvent   e;
	Screen*  s;
	Visual*  v;
	XImage*  i;
	GC       g;
	int      n;
	
	//
	xchar  kp;
	xchar  kt;
	xlong  mx;
	xlong  my;
	xlong  mb;
	xlong  mt;
	xchar* vm;
	xlong* vb;
	
	//
	void   init();
	void   flush();
	void   close();
	void   getinput();
	xchar  getkey();
	xchar  getturbokey();
	xlong  getmousex();
	xlong  getmousey();
	xlong  getmouseb();
	xlong* getvideobuffer();
	void   settitle(const xchar* c);
	void   drawtext(xlong x,xlong y,const xchar* s,xlong c);
}

void CLwindow::init()
{
	d = XOpenDisplay(NULL);

	s = DefaultScreenOfDisplay(d);
	n = DefaultScreen(d);
	v = DefaultVisualOfScreen(s);

	w = XCreateWindow(d,DefaultRootWindow(d),0,0,xres,yres,0,24,InputOutput,v,0,NULL);

	a = XInternAtom(d,"WM_DELETE_WINDOW",true);
	if(a) XSetWMProtocols(d,w,&a,1);

	XMapWindow(d,w);

	g = XCreateGC(d,w, 0, NULL);

	vm = new xchar[xres*yres*4];
	vb = reinterpret_cast<xlong*>(&vm[0]);

	i = XCreateImage(d,v,24,ZPixmap,0,vm,xres,yres,32,xres*4);
	
	XSelectInput(d,w, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
}

void CLwindow::flush()
{
	XPutImage(d,w,g,i,0,0,0,0,xres,yres);
	XFlush(d);
}

void CLwindow::close()
{
	XDestroyImage(i);
	XFreeGC(d,g);
	XDestroyWindow(d,w);
	XCloseDisplay(d);
}

void CLwindow::getinput()
{
	XNextEvent(d,&e);
	switch(e.type)
	{
		case KeyPress:		kp = XKeycodeToKeysym(d,e.xkey.keycode,0);
							break;

		case KeyRelease:	kt = XKeycodeToKeysym(d,e.xkey.keycode,0);
							break;

		case ButtonPress:	mb = e.xbutton.button;
							mx = e.xbutton.x;
							my = e.xbutton.y;
							break;

		case ButtonRelease:	mt = e.xbutton.button;
							mx = e.xbutton.x;
							my = e.xbutton.y;
							break;

		case MotionNotify:	mx = e.xbutton.x;
							my = e.xbutton.y;
							break;

//		case ClientMessage:	if(e.xclient.data.l[0] == a) close();
//							break;	
	}
}

xchar CLwindow::getkey()
{
	return kp;
}

xchar CLwindow::getturbokey()
{
	return kt;
}

xlong CLwindow::getmousex()
{
	return mx;
}

xlong CLwindow::getmousey()
{
	return my;
}

xlong CLwindow::getmouseb()
{
	return mb;
}

xlong* CLwindow::getvideobuffer()
{
	return vb;
}

void CLwindow::settitle(const xchar* c)
{
	XStoreName(d,w,c);
}

void CLwindow::drawtext(xlong x,xlong y,const xchar* s,xlong c)
{
	//XDrawString(d,d,g,x,y,c,length)
}

#endif

