// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZscreen.hh
// Screen Library 

///guard
#ifndef HH_XZSCREEN
#define HH_XZSCREEN
///*

///includes
#include <GL/gl.h>
#include <GL/glfw.h>

#include "XZbuffer.hh"
#include "XZmath.hh"
///* 

///declaration
#define LEFT	GLFW_KEY_LEFT
#define RIGHT	GLFW_KEY_RIGHT
#define UP	GLFW_KEY_UP
#define DOWN	GLFW_KEY_DOWN
#define ESCAPE	GLFW_KEY_ESC
#define LCTRL	GLFW_KEY_LCTRL
#define RCTRL	GLFW_KEY_RCTRL
#define ENTER	GLFW_KEY_ENTER
#define PGUP	GLFW_KEY_PAGEUP
#define PGDOWN	GLFW_KEY_PAGEDOWN
#define SPACE   GLFW_KEY_SPACE

#define AXES    2
#define BUTTONS 4
///*

///definitions
namespace screen
{
	buffer back     = buffer(XRES*YRES);	//System Memory Double Buffer
	buffer depth    = buffer(XRES*YRES);	//Z-Buffer
	buffer stencil  = buffer(XRES*YRES);	//Stencil (Shadow) Buffer
	buffer accum    = buffer(XRES*YRES);	//Accumulation Buffer for FSAA/FXAA and FSMB
	buffer inter    = buffer(XRES*YRES);	//Mask Buffer for user interfaces

	void init(long x,long y,const char* t,long* b=0);
	bool run();
	void exit();

	class input
	{
		static uchar joy[8];
		static long mouse[4];	
		static long key[2];
		static long* framebuffer;
		static long* counter;

		friend void cb_key(int k,int a);
		friend void cb_mouseb(int b,int a);
		friend void cb_mousep(int x,int y);

		friend long key();
		friend long turbo();
		friend long mousex();
		friend long mousey();
		friend long mousel();
		friend long mouser();
		friend long joyb(long i);

		friend bool run();
		friend void init(long x,long y,const char* t,long* b);
	};

	void cb_key(int k,int a)    { input::key[1] = input::key[0] = math::set(k,a==GLFW_PRESS); }
	void cb_mouseb(int b,int a) { input::mouse[math::lim(0,b,1)] = (a==GLFW_PRESS); }
	void cb_mousep(int x,int y) { input::mouse[2] = x; input::mouse[3] = y; }

	INLINE long key()    { const long k = input::key[1]; input::key[1] = 0; return k; }
	INLINE long turbo()  { return input::key[0]; }
	INLINE long mousex() { return input::mouse[2]; }
	INLINE long mousey() { return input::mouse[3]; }
	INLINE long mousel() { return input::mouse[0]; }
	INLINE long mouser() { return input::mouse[1]; }
	INLINE long joyb(long i) { return input::joy[i]; }
}
///*

///implementation
uchar screen::input::joy[8] = { 0,0,0,0,0,0,0,0 };
long  screen::input::mouse[4] = { 0,0,0,0 };
long  screen::input::key[2] = { 0,0 };
long* screen::input::framebuffer = back.pointer();
long* screen::input::counter = 0;

void screen::init(long x,long y,const char* t,long* b)
{
	input::counter = b;
	glfwInit();
	glfwOpenWindow(x,y,8,8,8,8,0,0,GLFW_WINDOW);
	glfwSetWindowTitle(t);

	glfwSetKeyCallback(&screen::cb_key);
	glfwSetMouseButtonCallback(&screen::cb_mouseb);
	glfwSetMousePosCallback(&screen::cb_mousep);
	
	glfwSwapInterval(0);
	//glfwDisable(GLFW_MOUSE_CURSOR);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,XRES,YRES,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
}

bool screen::run()
{
	*input::counter = 0;
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,input::framebuffer);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0,1.0);
		glTexCoord2f(1.0f,0.0f); glVertex2f( 1.0,1.0);
		glTexCoord2f(1.0f,1.0f); glVertex2f( 1.0,-1.0);
		glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0,-1.0);
	glEnd();
	glFlush();
	glfwSwapBuffers();
	glfwGetJoystickButtons(1,&input::joy[0],8);
	return glfwGetWindowParam(GLFW_OPENED);// && !glfwGetKey(GLFW_KEY_ESC);
}

void screen::exit()
{
	glfwCloseWindow();
	glfwTerminate();
}
///*

#endif

