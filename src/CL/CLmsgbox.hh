///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
///*

///sys includes
#include <GL/gl.h>
#include <GL/glut.h>
///*

///api includes
#include "CLtypes.hh"
#include "CLstring.hh"
#include "CLbase.hh"
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
class CLmsgbox : public CLbase<CLmsgbox,1>
{
	friend class CLbase<CLmsgbox,1>;
	
	private:
		static CLstring* clstring;
	protected:

		CLmsgbox();
		~CLmsgbox();
	public:
		xlong msgbox(const xchar* title,const xchar* message);
		xlong yesnobox(const xchar* title,const xchar* message);
		xlong alertbox(const xchar* title,xlong message);
};

CLstring* CLmsgbox::clstring = CLstring::instance();
///*

///implementation
CLmsgbox::CLmsgbox() //! noncritical
{
	
}

CLmsgbox::~CLmsgbox() //! noncritical
{

}

xlong CLmsgbox::msgbox(const xchar* title,const xchar* message) //! noncritical
{
	//prepare message
	xlong msglines = clstring->linecount(message);
	xlong msglength = clstring->length(message);
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
	xlong winwidth = 20; //+XTextWidth(Xfont,&message[msgpos[maxline]],msglen[maxline]);
	xlong winheight = 40+msglines*16;
	if(winwidth<200) winwidth = 200;
	if(winwidth>800) winwidth = 400;
	if(winheight<100) winheight = 100;
	if(winheight>600) winheight = 600;
	
	if(CLwindow::isglut()==0)
	{
		xlong argc = 1;
		xchar *argv[] = { "xizero",NULL };
		glutInit(&argc,argv);
	}
	
	glutInitWindowPosition(5,5);
	glutInitWindowSize(winwidth,winheight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	xlong currid = glutCreateWindow(title);
	
	//~ //wait till press
	//~ bool wait = 0;
	//~ while(wait==0)
	//~ {
		//~ if(XPending(Xdisplay)!=0)
		//~ {			
			//~ XNextEvent(Xdisplay,&Xevent);
			//~ switch(Xevent.type)
			//~ {				
				//~ case Expose:
					//~ XSetForeground(Xdisplay,Xgc,Xblack.pixel);
					//~ XSetBackground(Xdisplay,Xgc,Xgrey.pixel);
					//~ for(j=0; j<msglines; j++) { XDrawImageString(Xdisplay,Xwindow,Xgc,10,20+16*j,&message[msgpos[j]],msglen[j]); }
					//~ XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-10,(winwidth/2)+50,winheight-10);
					//~ XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)+50,winheight-30,(winwidth/2)+50,winheight-10);	
					//~ XDrawImageString(Xdisplay,Xwindow,Xgc,(winwidth/2)-5,winheight-15,u8"OK",2);
					//~ XSetForeground(Xdisplay,Xgc,Xwhite.pixel);
					//~ XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)-50,winheight-10);
					//~ XDrawLine(Xdisplay,Xwindow,Xgc,(winwidth/2)-50,winheight-30,(winwidth/2)+50,winheight-30);
				//~ break;
				//~ case KeyPress: if(XLookupKeysym((XKeyEvent*)&Xevent,0)==32) wait = 1; break;
				//~ case ButtonPress: if(Xevent.xbutton.button == Button1 && Xevent.xbutton.x>(winwidth/2)-50 && Xevent.xbutton.y>winheight-30 && Xevent.xbutton.x<(winwidth/2)+50 && Xevent.xbutton.y<winheight-10 ) { wait = 1; } break; 
			//~ }				
		//~ }
	//~ }
	//~ //destroy window
	//~ XFreeGC(Xdisplay,Xgc);
	//~ XDestroyWindow(Xdisplay,Xwindow);
	//~ XFlush(Xdisplay);
	return 1;
}

xlong CLmsgbox::yesnobox(const xchar* title,const xchar* message) //! noncritical
{

}

xlong CLmsgbox::alertbox(const xchar* title,xlong value) //! noncritical
{
	//prepare message
	xchar* message = clstring->toascii(value);
	xlong msglen = clstring->length(message);
	
	
	delete message;
	return 1;
}
///*

#endif
