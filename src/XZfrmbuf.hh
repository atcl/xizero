///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZfrmbuf.hh
// Direct Framebuffer Access and Input Handling Library
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: mouse"
///</header>

///<include>
#include <cstdio>	//atexit
#include <time.h>	//clock,CLOCKS_PER_SEC
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
#include "XZstring.hh" //temp
///</include>

///<declare>
#define BPP 32
#define FPS 4000

#define FB_DEV "/dev/fb0"
#define JS_DEV "/dev/js0"

#define ESCAPE	27
#define ENTER	10
#define SPACE	32
#define UP	65
#define PGUP	53
#define LEFT	67
#define RIGHT	68
#define DOWN	66
#define PGDOWN	54

#define CLOSE	24	//CTRL+X
///</declare>

///<define>
namespace screen
{
	buffer frame(XRES*YRES,1);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		void* cs = 0;				//cursor image
		uint tk = 0;				//turbo key
		uint kk = 0;				//keyboard key
		uint ms = uint((XRES/2)<<16)+uint(YRES/2);//compressend mouse data

		uint last = 0;
		char* nu = 0;

		termios nc;			//new terminal config
		termios oc;			//old terminal config

		uint fd;			//framebuffer device handle
		void* fp;			//framebuffer pointer

		struct fb_fix_screeninfo finfo;
		struct fb_var_screeninfo oinfo;
		struct fb_var_screeninfo vinfo{XRES,YRES,XRES,YRES,0,0,32,0,{16,8,0},{8,8,0},{0,8,0},{0,0,0},0,0,0xFFFFFFFF,0xFFFFFFFF,1};//,25000,88,40,23,1,128,4,0,0,0,0,0,0,0,0}; 
	}

	uint kbhit();
	void init(void* c);
	void set();
	void _flush()		{ frame.copy(back); ioctl(fd,FBIOPAN_DISPLAY,&vinfo); }
	void flush()		{ /*frame.swap(back);*/ frame.copy(back); ioctl(fd,FBIOPAN_DISPLAY,&vinfo); }
	void event();
	void close();
	void error(bool c,const char* m) { if(c) { system::say(m,1); screen::close(); system::bye(1); } }

	inline uint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ while(k!=kk) { event(); } }
	void sleep(sint t)	{ const sint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=last&&o==1) { last=t+FPS; t=f>>2; f=0; return t; } return -1; } 

	inline bool  run()	{ flush(); event(); return 1; }
	inline uint  key()	{ const uint r=kk; kk=0; return r; }
	inline uint  turbo()	{ return tk; }
	inline uint  mouse()    { return ms; }
	inline void* cursor()	{ return cs; }
	inline void  smouse(uint x=XRES/2,uint y=YRES/2) { ms = (x<<16)+y; }
}
///</define>

///<code>
uint screen::kbhit()
{
	int c = 0;
	ioctl(0,FIONREAD,&c);
	struct timeval tv = { 0,0 };
	select(1,0,0,0,&tv);
	return c;
}

void screen::init(void* c)
{
	cs = c;
	nu = new char[256];
	tcgetattr(STDIN_FILENO,&oc);
	nc = oc;
	nc.c_lflag &= ~(ICANON|ECHO|ISIG);
	//nc.c_cc[VMIN] = 4;
	//nc.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&nc);
	last = time()+4000;
	atexit(close);
}

void screen::event()
{
	ms &= 0x7FFFFFFF;
	const int r = kbhit();
	guard(r==0);

	const int s = 3-(r==1)-(r==2);
	packed t;
	read(0,&t,s);
	tk = kk = math::set(t.b[2],t.b[0],t.b[1]==91);
	read(0,&nu,math::min(r-s,256));

	//gamepad 

	uint mx =  math::lim(0,MOUSEY(ms)+((kk==DOWN)<<3)-((kk==UP)<<3),YRES);		//set bottom word to mouse y
	mx += math::lim(0,MOUSEX(ms)+((kk==LEFT)<<3)-((kk==RIGHT)<<3),XRES)<<16;	//set top word to mouse x
	mx += (kk==SPACE)<<31;								//set top bit to mouse button
	ms = mx;
	ifu(kk==CLOSE) { system::bye(); }
}

void screen::set()
{
        fd = open(FB_DEV,O_RDWR);
	error(fd<=0,"Error: Could not open framebuffer device");

	const sint e = ioctl(fd,FBIOGET_FSCREENINFO,&finfo);
	error(e<0,"Error: Could not read fixed framebuffer info");

	const sint f = ioctl(fd,FBIOGET_VSCREENINFO,&oinfo);
	error(f<0,"Error: Could not read variable framebuffer info");

	const sint g = ioctl(fd,FBIOPUT_VSCREENINFO,&vinfo);
	error(g<0,"Error: Could not write variable framebuffer info");

	fp = mmap(0,XRES*YRES*4,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
	error(fp==MAP_FAILED,"Error: Could not map buffer object!");

	frame.pointer(fp);
}

void screen::close()
{
	back.clear();
	_flush();
	system::say("XiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
	munmap(fp,XRES*YRES*4);
	oinfo.activate = FB_ACTIVATE_NOW;
	ioctl(fd,FBIOPUT_VSCREENINFO,&oinfo);
	::close(fd);
	tcsetattr(STDIN_FILENO,TCSANOW,&oc);
	//delete nu; 
}
///</code>

