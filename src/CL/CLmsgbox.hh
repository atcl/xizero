///license
//atCROSSLEVEL studios 2009,2010
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
	friend class CLglobal;
	
	private:
		CLstring& clstring;
		CLwindow& clwindow;
	protected:
		CLmsgbox();
		~CLmsgbox() { };
		static xlong currid;
		static void draw();
	public:
		xlong msgbox(const xchar* title,const xchar* message);
		xlong alertbox(const xchar* title,xlong message);
};
xlong CLmsgbox::currid = 0;
///*

///implementation
CLmsgbox::CLmsgbox() //! noncritical
: clstring(CLstring::instance()), clwindow(CLwindow::instance())
{ }

void CLmsgbox::draw() { } //! noncritical

xlong CLmsgbox::msgbox(const xchar* title,const xchar* message) //! noncritical
{
	//set for 9x15 font
	
	//prepare message
	xlong msglines = clstring.linecount(message);
	xlong msglength = clstring.length(message);
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
	xlong winheight = 40+(msglines*15);
	if(winwidth<200) { winwidth = 200; }
	if(winwidth>600) { winwidth = 600; }
	if(winheight<100) { winheight = 100; }
	if(winheight>250) { winheight = 250; }

	glutInitWindowSize(winwidth,winheight);
	glutInitWindowPosition(10,10);
	currid = glutCreateWindow(title);
	glutMouseFunc(CLwindow::setmouse);
	glutKeyboardFunc(CLwindow::setkeydn);
	//glutDisplayFunc(this->draw);
	
	//glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0,winwidth,0,winheight);
	
	while(true)
	{
		glutSetWindow(currid);
		
		//draw background
		glColor3f(0.7,0.7,0.7);
		glBegin(GL_QUADS);
		glVertex2i(0,0);
		glVertex2i(winwidth,0);
		glVertex2i(winwidth,winheight);
		glVertex2i(0,winheight);
		glEnd();
		//*
		
		//draw text
		glColor3f(0,0,0);
		for(xlong i=0; i<msglines; i++)
		{
			for(xlong j=0; j<msglen[i]; j++)
			{
				glRasterPos2i(10+j*9,winheight-10-(i+1)*15);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15,message[msgpos[i]+j]);
			}
		}
		//*
		
		//draw button
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		glVertex2i((winwidth/2)-50,25);
		glVertex2i((winwidth/2)+50,25);
		glVertex2i((winwidth/2)+50,25);
		glVertex2i((winwidth/2)+50,5);
		glColor3f(0,0,0);
		glVertex2i((winwidth/2)+50,5);
		glVertex2i((winwidth/2)-50,5);
		glVertex2i((winwidth/2)-50,5);
		glVertex2i((winwidth/2)-50,25);
		glEnd();
		
		glRasterPos2i((winwidth/2)-9,10);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'O');
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,'K');
		//*
		
		glFlush();
		
		glutMainLoopEvent();

		if(clwindow.getinkey()==SPACE) { break; };
		if(clwindow.getmouselb()==1)
		{
			if(clwindow.getmousex()>(winwidth/2)-50 &&
			   clwindow.getmousex()<(winwidth/2)+50 &&
			   clwindow.getmousey()>winheight-25 &&
			   clwindow.getmousey()<winheight-5) { break; }
		}
	}
	
	glutDestroyWindow(currid);
	return 1;
}

xlong CLmsgbox::alertbox(const xchar* title,xlong value) { msgbox(title,clstring.toascii(value)); } //! noncritical
///*

#endif
