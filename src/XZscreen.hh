// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZscreen.hh
// Screen Library 

///guard
#ifndef HH_XZSCREEN
#define HH_XZSCREEN
//#pragma message "Compiling " __FILE__ "..." " TODO: Joypad"
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

	namespace
	{
		sint  joy[2] = { 0,0 };
		sint  mouse[4] = { 0,0,0,0 };	
		sint  keys[2] = { 0,0 };
		sint* framebuffer = back.pointer();
		void* mcursor = 0;

		void cb_key(int k,int a)    { keys[1] = keys[0] = math::set(k,a==GLFW_PRESS); }
		void cb_mouseb(int b,int a) { mouse[math::lim(0,b,1)] = (a==GLFW_PRESS); }
		void cb_mousep(int x,int y) { mouse[2] = x; mouse[3] = y; }
	};

	void init(sint x,sint y,const char* t,void* c=0);
	bool run();
	void exit();

	inline sint  key()    { const sint k = keys[1]; keys[1] = 0; return k; }
	inline sint  turbo()  { return keys[0]; }
	inline sint  mousex() { return mouse[2]; }
	inline sint  mousey() { return mouse[3]; }
	inline sint  mousel() { return mouse[0]; }
	inline sint  mouser() { return mouse[1]; }
	inline sint  joya()   { return joy[0]; }
	inline sint  joyb()   { return joy[1]; }
	inline void* cursor() { return mcursor; }
}
///*

///implementation
void screen::init(sint x,sint y,const char* t,void* c)
{
	mcursor = c;
	glfwInit();
	glfwOpenWindow(x,y,8,8,8,8,0,0,GLFW_WINDOW);
	glfwSetWindowTitle(t);

	glfwSetKeyCallback(&screen::cb_key);
	glfwSetMouseButtonCallback(&screen::cb_mouseb);
	glfwSetMousePosCallback(&screen::cb_mousep);
	glfwSwapInterval(0);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,XRES,YRES,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
}

bool screen::run()
{
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,framebuffer);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0,1.0);
		glTexCoord2f(1.0f,0.0f); glVertex2f( 1.0,1.0);
		glTexCoord2f(1.0f,1.0f); glVertex2f( 1.0,-1.0);
		glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0,-1.0);
	glEnd();
	glFlush();
	glfwSwapBuffers();
	glfwGetJoystickPos(0,(float*)&joy[0],2);
	joy[0] = math::set(1,joy[0],joy[0]==1065353216);
	joy[0] = math::set(2,joy[0],joy[0]==-1082130431);
	joy[0] = math::set(3,joy[0],joy[1]==1065353216);
	joy[0] = math::set(4,joy[1]==-1082130431);
	glfwGetJoystickButtons(0,(byte*)&joy[1],4);
	joy[1] = math::set(1,joy[1],(joy[1]>>24)!=0);
	joy[1] = math::set(2,joy[1],(joy[1]>>16)!=0);
	joy[1] = math::set(3,joy[1],(joy[1]>>8)!=0);
	joy[1] = math::set(4,(joy[1])!=0);
	return glfwGetWindowParam(GLFW_OPENED);// && !glfwGetKey(GLFW_KEY_ESC);
}

void screen::exit()
{
	glfwCloseWindow();
	glfwTerminate();
}
///*

#endif

