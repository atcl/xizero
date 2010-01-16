///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
///*

///sys includes
#include <GL/gl.h>
#include <GL/glut.h>
///*

///api includes
#include "CLtypes.hh"
#include "CLresource.hh"
#include "CLformat.hh"
#include "CLapp.hh"
#include "CLbase.hh"
#include "CLscreen.hh"
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

///*
void empty(void* o) { };
///*

///definitions
class CLwindow : public CLbase<CLwindow,1>
{
	friend class CLbase<CLwindow,1>;
	
	private:
		static CLformat* clformat;
		static CLapp*    clapp;
		static CLgfx*    clgfx;
		static CLscreen* clscreen;
	protected:
		bool  displaycursor;
		sprite* cursor;
		uxlong* framebuffer;
		bool  glut;
		xlong keydn;
		xlong keyup;
		xlong turbo;
		xlong mousex;
		xlong mousey;
		xlong mouselb;	
		xlong mouserb;
		//~ static uxchar syskey;
		//~ static void (*sysmenu)(void* o);
		//~ static void* sysobj;
		float frame;
		float time;
		float timebase;
		float fps;
		CLwindow();
		~CLwindow() { };
		void setmouse(xlong button,xlong state,xlong x,xlong y);
		void setmotion(xlong x,xlong y);
		void setkeys(uxchar key,xlong x,xlong y);
		void idle();
	public:
		void draw();
		void run();
		void showcursor() { displaycursor = 1; };
		void hidecursor() { displaycursor = 0; };
		void setcursor(sprite* s) { cursor = s; };
		void setdisplay(void (*f)(void)) { glutDisplayFunc(f); };
		xlong getinkey() { xlong temp = keydn; keydn = 0; return temp; };
		xlong getturbo() { return turbo; };
		xlong getmousex() const { return mousex; };
		xlong getmousey() const { return mousey; };
		xlong getmouselb() const { return mouselb; };
		xlong getmouserb() const { return mouserb; };
		//void setsyskey(uxchar k=0,void (*m)(void* o)=0,void* mo=0);
		xlong getmilliseconds() const { return glutGet(GLUT_ELAPSED_TIME); };
		void sleep(xlong ms) const;
		xlong getfps();
		bool isglut() { return glut; };
};

CLformat* CLwindow::clformat = CLformat::instance();
CLapp*    CLwindow::clapp    = CLapp::instance();
CLgfx*    CLwindow::clgfx    = CLgfx::instance();
CLscreen* CLwindow::clscreen = CLscreen::instance();
//~ uxchar CLwindow::syskey = 0;
//~ void (*sysmenu)(void* o) = empty;
//~ void* CLwindow::sysobj = 0;
///*

///implementation
void CLwindow::setmouse(xlong button,xlong state,xlong x,xlong y) //! noncritical
{
	mousex = x;
	mousey = y;
	mouselb = (state==GLUT_DOWN) && (button==GLUT_LEFT_BUTTON);
	mouserb = (state==GLUT_DOWN) && (button==GLUT_RIGHT_BUTTON);
}

void CLwindow::setmotion(xlong x,xlong y) //! noncritical
{
	mousex = x;
	mousey = y;
}

void CLwindow::setkeys(uxchar key,xlong x,xlong y) //! noncritical
{
	keydn = key;
	if(turbo==key) { turbo = 0; }
	//if(key==syskey) { sysmenu(sysobj); }
}

void CLwindow::idle()
{
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	
	if(time-timebase>2000)
	{
		fps = frame*2000.0/(time-timebase);
	 	timebase = time;		
		frame = 0;
		tty("fps: "); say(xlong(fps));
	}
}

CLwindow::CLwindow() //! noncritical
{
	cursor = 0;
	displaycursor = 0;
	framebuffer = clscreen->cldoublebuffer.getbuffer();
	
	xlong argc = 1;
    xchar *argv[] = { "xizero",NULL };
	glutInit(&argc,argv);
	glutInitWindowPosition(5,5);
	glutInitWindowSize(XRES,YRES);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // GLUT_SINGLE, GLUT_STENCIL, GLUT_DEPTH, GLUT_ACCUM
	glutCreateWindow(TITLE);
	//~ glutSetCursor(GLUT_CURSOR_NONE);
	//~ glutMouseFunc(setmouse);
	//~ glutMotionFunc(setmotion);
	//~ glutKeyboardFunc(setkeys);
	//~ glutIdleFunc(idle);
	glut = 1;
}

void CLwindow::draw() //! noncritical
{
	if(cursor!=0 && displaycursor==1) { clgfx->drawsprite(mousex,mousey,cursor); }
	//glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,framebuffer); 
	glFlush();
}

void CLwindow::run() { glutMainLoop(); } //! noncritical

//void CLwindow::setsyskey(uxchar k,void (*m)(void* o),void* mo) { syskey = k; sysmenu = m; sysobj = mo; } //! noncritical

void CLwindow::sleep(xlong ms) const //! noncritical
{
	xlong starttime = glutGet(GLUT_ELAPSED_TIME);
	xlong stoptime = (starttime + ms);
	xlong nowtime = 0;
	while(nowtime < stoptime) { nowtime = glutGet(GLUT_ELAPSED_TIME); }
}
///*
	
#endif

