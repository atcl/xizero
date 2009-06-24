
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/keysym.h>

#include <iostream>

#define WIN_TITLE "test"

#define xres 800
#define yres 600
#define zres 400 

//gcc test.c -o test -L/usr/X11R6/lib -lX11 -I/usr/X11R6/include

//! prototypes


Display* d;
Window   w;
Atom     a;
XEvent   e;
Screen*  s;
Visual*  v;
XImage*  i;
GC       g;
int      n;

//current input:
char kp;
char kt;
long mx;
long my;
long mb;
long mt;
//


void XLsetup(char* doublebuffer)
{
// 	if( (d = XOpenDisplay(NULL)) == NULL )
// 	{
// 		//CLexit(__func__,1,"XOpenDisplay is unable to open a Display");
// 	}

	d = XOpenDisplay(NULL);

	s = DefaultScreenOfDisplay(d);
	n = DefaultScreen(d);
	v = DefaultVisualOfScreen(s);

	w = XCreateWindow(d,DefaultRootWindow(d),0,0,xres,yres,0,24,InputOutput,v,0,NULL);

	XStoreName(d,w,WIN_TITLE);

	a = XInternAtom(d,"WM_DELETE_WINDOW",true);
	if(a) XSetWMProtocols(d,w,&a,1);

	XMapWindow(d,w);

	g = XCreateGC(d,w, 0, NULL);

	i = XCreateImage(d,v,24,ZPixmap,0,doublebuffer,xres,yres,32,xres*4);
	
	XSelectInput(d,w, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
}

void XLflush()
{
	XPutImage(d,w,g,i,0,0,0,0,xres,yres);
	XFlush(d);
}

void XLexit()
{
	XDestroyImage(i);
	XFreeGC(d,g);
	XDestroyWindow(d,w);
	XCloseDisplay(d);
}

void XLgetinput()
{
	XNextEvent(d,&e);
	switch(e.type)
	{
		case KeyPress:		kp = XKeycodeToKeysym (d,e.xkey.keycode,0);
							break;

		case KeyRelease:	kt = XKeycodeToKeysym (d,e.xkey.keycode,0);
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

		case ClientMessage:	if(e.xclient.data.l[0] == a) XLexit();
							break;	
	}
}

char XLgetkey()
{
	return kp;
}

char XLgetturbokey()
{
	return kt;
}

long XLgetmousex()
{
	return mx;
}

long XLgetmousey()
{
	return my;
}

long XLgetmousebutton()
{
	return mb;
}

long XLgetturbomousebutton()
{
	return mt;
}

long* XLgetvideobuffer()
{
	
}

int main()
{
	char* vram = new char[xres*yres*4];
	long* buff = reinterpret_cast<long*>(&vram[0]);

	XLsetup(vram);

	bool inf = 1;

	buff[200*xres+200]=0x00FF0000;
	
	long tki = 0;
	long nki = 0;

	while(inf==1)
	{
		XLgetinput();

		tki = XLgetturbokey();
		nki = XLgetkey();

		switch(tki)
		{
			case '0': inf = 0; break; //leave

 
		}

		//XClearWindow(d,w);

		XLflush();
	}

	return 0;
}




