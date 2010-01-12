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
		static bool  glut;
		static xlong keydn;
		static xlong keyup;
		static xlong turbo;
		static xlong mousex;
		static xlong mousey;
		static xlong mouselb;	
		static xlong mouserb;
		//~ static uxchar syskey;
		//~ static void (*sysmenu)(void* o);
		//~ static void* sysobj;
		static float frame;
		static float time;
		static float timebase;
		static float fps;
		CLwindow();
		~CLwindow() { };
		static void setmouse(xlong button,xlong state,xlong x,xlong y);
		static void setkeys(uxchar key,xlong x,xlong y);
		static void idle();
	public:
		static void draw();
		xlong run();
		void showcursor() { displaycursor = 1; };
		void hidecursor() { displaycursor = 0; };
		void setcursor(sprite* s) { cursor = s; };
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
		static bool isglut() { return glut; };
};

CLformat* CLwindow::clformat = CLformat::instance();
CLapp*    CLwindow::clapp    = CLapp::instance();
CLgfx*    CLwindow::clgfx    = CLgfx::instance();
CLscreen* CLwindow::clscreen = CLscreen::instance();
bool  CLwindow::glut = 0;
xlong CLwindow::keydn = 0;
xlong CLwindow::keyup = 0;
xlong CLwindow::turbo = 0;
xlong CLwindow::mousex = 0;
xlong CLwindow::mousey = 0;
xlong CLwindow::mouselb = 0;	
xlong CLwindow::mouserb = 0;
//~ uxchar CLwindow::syskey = 0;
//~ void (*sysmenu)(void* o) = empty;
//~ void* CLwindow::sysobj = 0;
float CLwindow::frame = 0;
float CLwindow::time = 0;
float CLwindow::timebase = 0;
float CLwindow::fps = 0;
///*

///implementation
void CLwindow::setmouse(xlong button,xlong state,xlong x,xlong y) //! noncritical
{
	mousex = x;
	mousey = y;
	mouselb = (state==GLUT_DOWN) && (button==GLUT_LEFT_BUTTON);
	mouserb = (state==GLUT_DOWN) && (button==GLUT_RIGHT_BUTTON);
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
	
	if(time-timebase>1000)
	{
		fps = frame*1000.0/(time-timebase);
	 	timebase = time;		
		frame = 0;
	}
	
	tty("fps: "); say(fps);
}

CLwindow::CLwindow() //! noncritical
{
	cursor = 0;
	displaycursor = 0;
	
	xlong argc = 1;
    xchar *argv[] = { "xizero",NULL };
	glutInit(&argc,argv);
	glutInitWindowPosition(5,5);
	glutInitWindowSize(XRES,YRES);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // GLUT_SINGLE, GLUT_STENCIL, GLUT_DEPTH, GLUT_ACCUM
	glutCreateWindow(TITLE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(setmouse);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutMainLoop();
	glut = 1;
}

void CLwindow::draw() { glDrawPixels(XRES,YRES,GL_RGBA,GL_UNSIGNED_INT,clscreen->cldoublebuffer.getbuffer()); } //! noncritical

xlong CLwindow::run() { if(cursor!=0 && displaycursor==1) { clgfx->drawsprite(mousex,mousey,cursor); } draw(); return 1; } //! noncritical

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

