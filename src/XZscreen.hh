// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
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

struct tile;
///*

///definitions
namespace screen
{
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		sint  mouse[4] = { 0,0,0,0 };	
		sint  keys[2] = { 0,0 };
		sint  joys = 0;
		tile* mcursor = 0;

		void cb_key(int k,int a)    { keys[1] = keys[0] = math::set(k,a==GLFW_PRESS); }
		void cb_mouseb(int b,int a) { mouse[math::lim(0,b,1)] = (a==GLFW_PRESS); }
		void cb_mousep(int x,int y) { mouse[2] = x; mouse[3] = y; }
	}

	void init(sint x,sint y,const char* t,tile* c=0);
	bool run();
	void close()       { glfwCloseWindow(); glfwTerminate(); }
	void wait(sint k)  { while(glfwGetKey(k)!=GLFW_PRESS) { glfwWaitEvents(); } }
	void sleep(sint t) { glfwSleep(double(t)/1000.0); }
	sint time()        { return (sint)(1000.0*glfwGetTime()); }
	sint fps(bool o=1) { static sint f=0; static sint l=time()+4000; sint t=time(); f+=o; if(t>=l&&o==1) { l=t+4000; t=f>>2; f=0; return t; } return -1; } 

	inline sint  key()    { const sint k = keys[1]; keys[1] = 0; return k; }
	inline sint  turbo()  { return keys[0]; }
	inline sint  mousex() { return mouse[2]; }
	inline sint  mousey() { return mouse[3]; }
	inline sint  mouseb() { const sint b = mouse[0]; mouse[0] = 0; return b; }
	inline sint  joy()    { return joys; }
	inline tile* cursor() { return mcursor; }
}
///*

///implementation
void screen::init(sint x,sint y,const char* t,tile* c)
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
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,XRES,YRES,GL_RGBA,GL_UNSIGNED_BYTE,back.pointer());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex2f(-1.0,1.0);
		glTexCoord2f(1.0f,0.0f); glVertex2f( 1.0,1.0);
		glTexCoord2f(1.0f,1.0f); glVertex2f( 1.0,-1.0);
		glTexCoord2f(0.0f,1.0f); glVertex2f(-1.0,-1.0);
	glEnd();
	glFlush();
	glfwSwapBuffers();
	sint temp = 0;
	glfwGetJoystickPos(0,(float*)&temp,2);
	joys  = (temp==1065353216) | ((temp==-1082130431)<<1) | ((temp== 1065353216)<<2) | ((temp==-1082130431)<<3);
	glfwGetJoystickButtons(0,(byte*)&temp,4);
	joys |= (((temp>>24)!=0)<<4) | (((temp>>16)!=0)<<5) | (((temp>>8)!=0)<<6) | (((temp)!=0)<<7);
	//back.swap(accum);
	return glfwGetWindowParam(GLFW_OPENED);// && !glfwGetKey(GLFW_KEY_ESC);
}
///*

#endif

