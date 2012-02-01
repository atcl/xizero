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

#include "XZbasic.hh"
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

	void init(sint x,long y,const char* t,sint* b=0);
	bool run();
	void exit();

	class input
	{
		static sint joy[2];
		static sint mouse[4];	
		static sint key[2];
		static sint* framebuffer;
		static sint* counter;

		friend void cb_key(int k,int a);
		friend void cb_mouseb(int b,int a);
		friend void cb_mousep(int x,int y);

		friend sint key();
		friend sint turbo();
		friend sint mousex();
		friend sint mousey();
		friend sint mousel();
		friend sint mouser();
		friend sint joya();
		friend sint joyb();

		friend bool run();
		friend void init(sint x,sint y,const char* t,sint* b);
	};

	void cb_key(int k,int a)    { input::key[1] = input::key[0] = math::set(k,a==GLFW_PRESS); }
	void cb_mouseb(int b,int a) { input::mouse[math::lim(0,b,1)] = (a==GLFW_PRESS); }
	void cb_mousep(int x,int y) { input::mouse[2] = x; input::mouse[3] = y; }

	inline sint key()    { const sint k = input::key[1]; input::key[1] = 0; return k; }
	inline sint turbo()  { return input::key[0]; }
	inline sint mousex() { return input::mouse[2]; }
	inline sint mousey() { return input::mouse[3]; }
	inline sint mousel() { return input::mouse[0]; }
	inline sint mouser() { return input::mouse[1]; }
	inline sint joya()   { return input::joy[0]; }
	inline sint joyb()   { return input::joy[1]; }
}
///*

///implementation
sint  screen::input::joy[2] = { 0,0 };
sint  screen::input::mouse[4] = { 0,0,0,0 };
sint  screen::input::key[2] = { 0,0 };
sint* screen::input::framebuffer = back.pointer();
sint* screen::input::counter = 0;

void screen::init(sint x,sint y,const char* t,sint* b)
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
	glfwGetJoystickPos(0,(float*)&input::joy[0],2);
	input::joy[0] = math::set(1,input::joy[0],input::joy[0]==1065353216);
	input::joy[0] = math::set(2,input::joy[0],input::joy[0]==-1082130431);
	input::joy[0] = math::set(3,input::joy[0],input::joy[1]==1065353216);
	input::joy[0] = math::set(4,input::joy[1]==-1082130431);
	glfwGetJoystickButtons(0,(byte*)&input::joy[1],4);
	input::joy[1] = math::set(1,input::joy[1],(input::joy[1]>>24)!=0);
	input::joy[1] = math::set(2,input::joy[1],(input::joy[1]>>16)!=0);
	input::joy[1] = math::set(3,input::joy[1],(input::joy[1]>>8)!=0);
	input::joy[1] = math::set(4,(input::joy[1])!=0);
	return glfwGetWindowParam(GLFW_OPENED);// && !glfwGetKey(GLFW_KEY_ESC);
}

void screen::exit()
{
	glfwCloseWindow();
	glfwTerminate();
}
///*

#endif

