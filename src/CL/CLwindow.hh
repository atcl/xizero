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
//#include <X11/xpm.h>
///*

///api includes
#include "CLtypes.hh"
#include "CLresource.hh"
#include "CLformat.hh"
#include "CLbase.hh"
#include "CLscreen.hh"
///*

///header
/* class name:	CLwindow
 * 
 * description:	very basic window with doublebuffering
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLwindow : public CLbase<CLwindow,1>
{
	friend class CLbase<CLwindow,1>;
	friend class CLglobal;
	friend class CLmsgbox;
	
	private:
		CLapp&    clapp;
		CLformat& clformat;
		CLgfx&    clgfx;
		CLscreen& clscreen;
	protected:
		static bool drawcursor;
		static sprite* cursor;
		static uxlong* framebuffer;
		static CLgfx* gfx;
		static xlong keydn;
		static xlong turbo;
		static xlong mousex;
		static xlong mousey;
		static xlong mouselb;	
		static xlong mouserb;
		xlong  winid;
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
		xlong getmilliseconds() const { return glutGet(GLUT_ELAPSED_TIME); };
		void sleep(xlong ms) const;
};

xlong CLwindow::keydn = 0;
xlong CLwindow::turbo = 0;
xlong CLwindow::mousex = 0;
xlong CLwindow::mousey = 0;
xlong CLwindow::mouselb = 0;	
xlong CLwindow::mouserb = 0;
uxlong* CLwindow::framebuffer = 0;
CLgfx*  CLwindow::gfx = 0;
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

void CLwindow::setkeydn(uxchar key,xlong x,xlong y) { turbo = keydn = key; } //! noncritical

void CLwindow::setkeyup(uxchar key,xlong x,xlong y) { if(turbo==key) { turbo = 0; } } //! noncritical

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
: clapp(CLapp::instance()), clformat(CLformat::instance()), clgfx(CLgfx::instance()), clscreen(CLscreen::instance())
{	
	framebuffer = clscreen.cldoublebuffer.getbuffer();
	gfx = &clgfx;
	
	printfps = frame = time = timebase = fps = 0;
	
	xlong argc = 0;
    xchar *argv[] = { const_cast<xchar*>(clapp.getexename()),NULL };
	glutInit(&argc,argv);
	glutInitWindowPosition(5,5);
	glutInitWindowSize(XRES,YRES);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); 
	winid = glutCreateWindow(TITLE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(setmouse);
	glutPassiveMotionFunc(setmotion);
	glutKeyboardFunc(setkeydn);
	glutKeyboardUpFunc(setkeyup);
	glutSpecialFunc(setspecdn);
	glutSpecialUpFunc(setspecup);
	glutJoystickFunc(setgpad,500);
	glutDisplayFunc(draw);
	glutIdleFunc(0);
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
	
	//icon
	//~ Pixmap image;
	//~ Pixmap shape;
	//~ XWMHints* win_hints;
	//~ Display* display = XOpenDisplay(":0.0");
	//~ XpmCreatePixmapFromData(display,DefaultRootWindow(display),CLicon,&image,&shape,0);
	//~ win_hints = XAllocWMHints();
	//~ win_hints->flags = IconPixmapHint|IconMaskHint;
	//~ win_hints->icon_pixmap = image;
	//~ win_hints->icon_mask = shape;
	//~ XSetWMHints(display,DefaultRootWindow(display),win_hints);
	//*
	
	//*
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void CLwindow::draw() //! noncritical
{
	if(cursor!=0 && drawcursor==1) { gfx->drawsprite(mousex,mousey,cursor); }
	glRasterPos2i(-1,1);
	glPixelZoom(1.0,-1.0);
	glDrawPixels(XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,framebuffer); 
	glutSwapBuffers();
}

bool CLwindow::run() { glutMainLoopEvent(); draw(); idle(); return 1; } //! noncritical

void CLwindow::sleep(xlong ms) const //! noncritical
{
	xlong starttime = glutGet(GLUT_ELAPSED_TIME);
	xlong stoptime = (starttime + ms);
	xlong nowtime = 0;
	while(nowtime < stoptime) { nowtime = glutGet(GLUT_ELAPSED_TIME); }
}
///*
	
#endif

