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
#include "CLbase.hh"
#include "CLstring.hh"
#include "CLwindow.hh"
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
		static CLwindow* clwindow;
	protected:
		static xlong mousex;
		static xlong mousey;
		static xlong mouselb;
		static xlong keydn;
		static void setkeys(uxchar key,xlong x,xlong y);
		static void setmouse(xlong button,xlong state,xlong x,xlong y);
		CLmsgbox() { };
		~CLmsgbox() { };
	public:
		xlong msgbox(const xchar* title,const xchar* message);
		xlong alertbox(const xchar* title,xlong message);
};

CLstring* CLmsgbox::clstring = CLstring::instance();
CLwindow* CLmsgbox::clwindow = CLwindow::instance();

xlong CLmsgbox::mousex = 0;
xlong CLmsgbox::mousey = 0;
xlong CLmsgbox::mouselb = 0;
xlong CLmsgbox::keydn = 0;
///*

///implementation
xlong CLmsgbox::msgbox(const xchar* title,const xchar* message) //! noncritical
{
	//set for 9x15 font
	
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
	xlong winwidth = 20+(9*msglen[maxline]);
	xlong winheight = 40+msglines*15;
	if(winwidth<200) { winwidth = 200; }
	if(winwidth>800) { winwidth = 400; }
	if(winheight<100) { winheight = 100; }
	if(winheight>600) { winheight = 600; }
	
	xlong mainid = glutGetWindow(); 
	glutInitWindowPosition(5,5);
	glutInitWindowSize(winwidth,winheight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	xlong currid = glutCreateWindow(title);
	glutSetWindow(currid);
	
	while(true)
	{
		glutMainLoopEvent();
		
		//get input
		if(clwindow->getinkey()==SPACE) { break; };
		if(clwindow->getmouselb()==1)
		{
			if(clwindow->getmousex()>(winwidth/2)-50 &&
			   clwindow->getmousex()<(winwidth/2)+50 &&
			   clwindow->getmousey()>winheight-25 &&
			   clwindow->getmousey()<winheight-5) { break; }
		}
		//*
		
		//~ glRasterPos2f(-1,1);
		
		//draw background
		glColor3f(0.5,0.5,0.5);
		glBegin(GL_QUADS);
		glVertex2f(-1,-1);
		glVertex2f(-1,1);
		glVertex2f(1,1);
		glVertex2f(1,-1);
		glEnd();
		//*
		
		//draw text
		//~ glColor3f(0,0,0);
		//~ for(xlong i=0; i<msglines; i++)
		//~ {
			//~ for(xlong j=0; j<msglen[i]; j++)
			//~ {
				//~ glRasterPos2i(10+j*9,10+i*15);
				//~ glutBitmapCharacter(GLUT_BITMAP_9_BY_15,message[msgpos[i]+j]);
			//~ }
		//~ }
		//*
		
		//draw button
		//~ glColor3f(0,0,0);
		//~ glBegin(GL_LINES);
		//~ glVertex2i((winwidth/2)-50,winheight-25);
		//~ glVertex2i((winwidth/2)+50,winheight-25);
		//~ glVertex2i((winwidth/2)+50,winheight-25);
		//~ glVertex2i((winwidth/2)+50,winheight-5);
		//~ glEnd();
		//~ 
		//~ glColor3f(1,1,1);
		//~ glBegin(GL_LINES);
		//~ glVertex2i((winwidth/2)+50,winheight-5);
		//~ glVertex2i((winwidth/2)-50,winheight-5);
		//~ glVertex2i((winwidth/2)-50,winheight-5);
		//~ glVertex2i((winwidth/2)-50,winheight-25);
		//~ glEnd();
		//*
		
		glFlush();
	}
	
	
	glutSetWindow(mainid);
	glutDestroyWindow(currid);
	return 1;
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
