///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLWINDOW
#define HH_CLWINDOW
///*

///sys includes
#include <GL/gl.h>
#include <GL/freeglut.h>
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

///definitions
class CLwindow : public CLbase<CLwindow,1>
{
	friend class CLbase<CLwindow,1>;
	friend class CLmsgbox;
	
	private:
		static CLformat* clformat;
		static CLapp*    clapp;
		static CLgfx*    clgfx;
		static CLscreen* clscreen;
	protected:
		static bool drawcursor;
		static sprite* cursor;
		static uxlong* framebuffer;
		static xlong keydn;
		static xlong turbo;
		static xlong mousex;
		static xlong mousey;
		static xlong mouselb;	
		static xlong mouserb;
		static uxchar syskey;
		static void (*sysmenu)();
		uxlong frame;
		uxlong time;
		uxlong timebase;
		uxlong fps;
		bool printfps;
		CLwindow();
		~CLwindow() { };
		void idle();
		static void setmotion(xlong x,xlong y);
		static void setmouse(xlong button,xlong state,xlong x,xlong y);
		static void setkeydn(uxchar key,xlong x,xlong y);
		static void setkeyup(uxchar key,xlong x,xlong y);
		static void setspecdn(xlong key,xlong x,xlong y);
		static void setspecup(xlong key,xlong x,xlong y);
		static void setgpad(uxlong buttonMask,xlong x,xlong y,xlong z);
		static void draw();
	public:
		bool run();
		void showcursor(bool b) { drawcursor = b; };
		void showfps(bool b) { printfps = b; }
		void setcursor(sprite* s) { cursor = s; };
		xlong getinkey(bool d=0) { if(d) { return keydn; } else { xlong temp = keydn; keydn = 0; return temp; } };
		xlong getturbo() { return turbo; };
		xlong getmousex() const { return mousex; };
		xlong getmousey() const { return mousey; };
		xlong getmouselb() const { return mouselb; };
		xlong getmouserb() const { return mouserb; };
		void setsyskey(uxchar k=0,void (*m)()=0);
		xlong getmilliseconds() const { return glutGet(GLUT_ELAPSED_TIME); };
		void sleep(xlong ms) const;
		xlong getfps();
};

CLformat* CLwindow::clformat = CLformat::instance();
CLapp*    CLwindow::clapp    = CLapp::instance();
CLgfx*    CLwindow::clgfx    = CLgfx::instance();
CLscreen* CLwindow::clscreen = CLscreen::instance();
uxchar CLwindow::syskey = 0;
xlong CLwindow::keydn = 0;
xlong CLwindow::turbo = 0;
xlong CLwindow::mousex = 0;
xlong CLwindow::mousey = 0;
xlong CLwindow::mouselb = 0;	
xlong CLwindow::mouserb = 0;
uxlong* CLwindow::framebuffer = clscreen->cldoublebuffer.getbuffer();
sprite* CLwindow::cursor = 0;
bool CLwindow::drawcursor = 0;
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

void CLwindow::setkeydn(uxchar key,xlong x,xlong y) //! noncritical
{
	turbo = keydn = key;
	//if(syskey!=0 && key==syskey) { sysmenu(); }
}

void CLwindow::setkeyup(uxchar key,xlong x,xlong y) //! noncritical
{
	if(turbo==key) { turbo = 0; }
}

void CLwindow::setspecdn(xlong key,xlong x,xlong y) //! noncritical
{
	switch(key)
	{
		case GLUT_KEY_LEFT: turbo = keydn = LEFT; break;
		case GLUT_KEY_RIGHT: turbo = keydn = RIGHT; break;
		case GLUT_KEY_UP: turbo = keydn = UP; break;
		case GLUT_KEY_DOWN: turbo = keydn = DOWN; break;
		case GLUT_KEY_PAGE_UP: turbo = keydn = PGUP; break;
		case GLUT_KEY_PAGE_DOWN: turbo = keydn = PGDOWN; break;
	}
}

void CLwindow::setspecup(xlong key,xlong x,xlong y) //! noncritical
{
	xlong temp = 0;
	
	switch(key)
	{
		case GLUT_KEY_LEFT: temp = LEFT; break;
		case GLUT_KEY_RIGHT: temp = RIGHT; break;
		case GLUT_KEY_UP: temp = UP; break;
		case GLUT_KEY_DOWN: temp = DOWN; break;
		case GLUT_KEY_PAGE_UP: temp = PGUP; break;
		case GLUT_KEY_PAGE_DOWN: temp = PGDOWN; break;
	}
	
	if(turbo==temp) { turbo = 0; }
}

void CLwindow::setgpad(uxlong buttonMask,xlong x,xlong y,xlong z) //! noncritical
{

}

void CLwindow::idle() //! critical
{
	frame += 1000;
	time = glutGet(GLUT_ELAPSED_TIME);
	uxlong timediff = time-timebase;

	if(timediff>2000)
	{
		fps = frame/timediff;
	 	timebase = time;		
		frame = 0;
		if(printfps) { tty("fps: "); say(fps); }
	}
}

CLwindow::CLwindow() //! noncritical
{	
	frame = 0;
	time = 0;
	timebase = 0;
	fps = 0;
	printfps = 0;
	
	xlong argc = 1;
    xchar *argv[] = { "xizero",NULL };
	glutInit(&argc,argv);
	glutInitWindowPosition(5,5);
	glutInitWindowSize(XRES,YRES);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE); 
	glutCreateWindow(TITLE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(setmouse);
	glutPassiveMotionFunc(setmotion);
	glutKeyboardFunc(setkeydn);
	glutKeyboardUpFunc(setkeyup);
	glutSpecialFunc(setspecdn);
	glutSpecialUpFunc(setspecup);
	glutJoystickFunc(setgpad,500);
	glutDisplayFunc(draw);
	//speed up gldrawpixels:
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferi(GL_RED_SCALE, 1);
	glPixelTransferi(GL_RED_BIAS, 0);
	glPixelTransferi(GL_GREEN_SCALE, 1);
	glPixelTransferi(GL_GREEN_BIAS, 0);
	glPixelTransferi(GL_BLUE_SCALE, 1);
	glPixelTransferi(GL_BLUE_BIAS, 0);
	glPixelTransferi(GL_ALPHA_SCALE, 1);
	glPixelTransferi(GL_ALPHA_BIAS, 0);
	//*
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void CLwindow::draw() //! noncritical
{
	if(cursor!=0 && drawcursor==1) { clgfx->drawsprite(mousex,mousey,cursor); }
	glRasterPos2i(-1,1);
	glPixelZoom(1.0,-1.0);
	glDrawPixels(XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,framebuffer); 
	glFlush();
}

bool CLwindow::run() { glutMainLoopEvent(); draw(); idle(); return 1; } //! noncritical

void CLwindow::setsyskey(uxchar k,void (*m)()) { syskey = k; /*sysmenu = m;*/ } //! noncritical

void CLwindow::sleep(xlong ms) const //! noncritical
{
	xlong starttime = glutGet(GLUT_ELAPSED_TIME);
	xlong stoptime = (starttime + ms);
	xlong nowtime = 0;
	while(nowtime < stoptime) { nowtime = glutGet(GLUT_ELAPSED_TIME); }
}
///*
	
#endif

