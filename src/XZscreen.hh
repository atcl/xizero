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
class screen
{
	private:
		static screen* _instance;
		long   _mouse[4];
		long   _key[2];
		long*  _framebuffer;
		
		static void cb_key(int k,int a);
		static void cb_mouseb(int b,int a);
		static void cb_mousep(int x,int y);
	public:
		screen();
		~screen();
		static screen* instance();
		INLINE bool run();
		INLINE bool key(long x) const { return _key[1]==x; }
		INLINE long key();
		INLINE long turbo()  const { return _key[0]; }
		INLINE long mousex() const { return _mouse[2]; }
		INLINE long mousey() const { return _mouse[3]; }
		INLINE bool mousel() const { return _mouse[0]; }
		INLINE bool mouser() const { return _mouse[1]; }

		static buffer back;
		static buffer depth;
		static buffer stencil;
		static buffer accum;	//for FSAA and FSMB
		static buffer inter;	//for user interfaces
};

screen* screen::_instance = 0;
buffer  screen::back      = buffer(XRES*YRES);
buffer  screen::depth     = buffer(XRES*YRES);
buffer  screen::stencil   = buffer(XRES*YRES);
buffer  screen::accum     = buffer(XRES*YRES);
buffer  screen::inter     = buffer(XRES*YRES);
///*

///implementation
void screen::cb_key(int k,int a)
{
	_instance->_key[1] = _instance->_key[0] = math::set(k,a==GLFW_PRESS);
}

void screen::cb_mouseb(int b,int a)
{
	_instance->_mouse[(b==0||b==1)+(b==1)] = (a==GLFW_PRESS);
}

void screen::cb_mousep(int x,int y)
{
	//_instance->_mouse[2] = x;
	//_instance->_mouse[3] = y;
}

screen::screen() : _framebuffer(back.pointer())
{
	glfwInit();
	glfwOpenWindow(XRES,YRES,8,8,8,8,0,0,GLFW_WINDOW);
	glfwSetWindowTitle(TITLE" "VERSION);

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

screen::~screen()
{
	glfwCloseWindow();
	glfwTerminate();
}

screen* screen::instance()
{
	if(_instance==0)
	{
		_instance = new screen();		
	}
	return _instance;
}

bool screen::run()
{
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,_framebuffer);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0,1.0);
		glTexCoord2f(1.0f,0.0f); glVertex2f( 1.0,1.0);
		glTexCoord2f(1.0f,1.0f); glVertex2f( 1.0,-1.0);
		glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0,-1.0);
	glEnd();
	glFlush();
	glfwSwapBuffers();
	return glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC);
}

long screen::key()
{
	long k = _key[1];
	_key[1] = 0;
	return k;
}
///*

#endif

