
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>

#include <stdio.h>

#define WIN_TITLE "test"
#define xres 800
#define yres 600


//gcc test.c -o test -L/usr/X11R6/lib -lX11 -I/usr/X11R6/include

Display* d;
Window   w;
Atom     a;
XEvent   e;
Visual*  v;
Ximage   i;
GC       g;

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
	if( (d=XOpenDisplay(NULL) == NULL )
	{
		//CLexit(__func__,1,"XOpenDisplay is unable to open a Display");
	}

	w = XCreateSimpleWindow(d,DefaultRootWindow(d),0,0,xres,yres,0,0,BlackPixel(d,DefaultScreen(d)));

	v =DefaultVisualOfScreen(d);

	XStoreName(d,w,WIN_TITLE);

	a = XInternAtom(d,"WM_DELETE_WINDOW",true);
	if(a) XSetWMProtocols(d,w,&a,1);

	XMapWindow(d,w);

	XGCValues values = CapButt | JoinBevel;
	unsigned long valuemask = GCCapStyle | GCJoinStyle;
	g = XCreateGC(d,w, valuemask, &values);

	i = XCreateImage(d,v,32,ZPixmap,0,doublebuffer,xres,yres,32,xres*4);
	
	XSelectInput(d,w, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
}

void XLflush()
{
	XPutImage(d,w,g,i,0,0,0,0,xres,yres);
	XFlush(d);
}

void XLgetinput()
{
	XNextEvent(d,e);
	switch(e.type)
	{
		case KeyPress:		kp = event.xkey.keycode;
					break;

		case KeyRelease:	kt = event.xkey.keycode;
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

		case ClientMessage:	if (event.xclient.data.l[0] == delete_atom);
					CloseAll();
					break;	
	}
}

char XLgetkey()
{
	return kp;
}

char XLgetkeyascii()
{
	
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

void XLexit()
{
	XDestroyImage(i);
	XFreeGC(d,g);
	XDestroyWindow(d,w);
	XCloseDisplay(d);
}

int main()
{
	char* vram = new char[xres*yres*4];

	XLsetup(vram);

	bool inf = 1;

	while(inf==1)
	{
		XLgetinput();

		switch(XLgetkey())
		{
			case 0: inf = 0; //leave

			case 1: ; //output pixel

			case 2: ; //output event data to console

		}

		XLflush();
	}
}




