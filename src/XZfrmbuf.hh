// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZskms.hh
// Super KMS library

///guard
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: mouse"
///*

///includes
#include <time.h>
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
///*
#include "XZstring.hh"
///declarations
#define BPP 32
#define FPS 4000

#define ESCAPE	27
#define ENTER	10
#define SPACE	32
#define UP	65
#define PGUP	53
#define LEFT	67
#define RIGHT	68
#define DOWN	66
#define PGDOWN	54

#define CLOSE		//CTRL+ESC

struct tile;
///*

///definitions
namespace screen
{
	buffer frame(XRES*YRES,1);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		tile* cs = 0;			//cursor image
		uint tk = 0;			//turbo key
		uint kk = 0;			//keyboard key
		uint mx = XRES/2;		//mouse horizontal position
		uint my = YRES/2;		//mouse vertical position
		bool mb = 0;			//mouse button pressed

		uint last = 0;
		char* nu = 0;

		termios nc;			//new terminal config
		termios oc;			//old terminal config

		uint fd;			//framebuffer device handle
		uint xs;			//x stride

		struct fb_fix_screeninfo finfo;
		struct fb_var_screeninfo oinfo;
		struct fb_var_screeninfo vinfo{XRES,YRES,XRES,YRES/**2*/,0,0,32,0,{16,8,0},{8,8,0},{0,8,0},{0,0,0},0,0,-1,-1,1};//,25000,88,40,23,1,128,4,0,0,0,0,0,0,0,0}; 
	}

	uint kbhit();
	void init(tile* c);
	void set();
	void _flush()		{ frame.copy(back); ioctl(fd,FBIOPAN_DISPLAY,&vinfo); }
	void flush()		{ back.swap(accum); frame.copy(back); ioctl(fd,FBIOPAN_DISPLAY,&vinfo); }
	bool event();
	void close();
	void error(bool c,const char* m) { if(c) { system::say(m,1); screen::close(); system::bye(1); } }

	inline uint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ while(k!=kk) { event(); } }
	void sleep(sint t)	{ const sint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=last&&o==1) { last=t+FPS; t=f>>2; f=0; return t; } return -1; } 

	inline bool  run()	{ flush(); return event(); }
	inline uint  key()	{ const uint r=kk; kk=0; return r; }
	inline uint  turbo()	{ return tk; }
	inline uint  mousex()	{ return mx; }
	inline uint  mousey()	{ return my; }
	inline uint  mouseb()	{ return mb; }
	inline tile* cursor()	{ return cs; }
	//inline void  smouse(uint x=XRES>>1,uint y=YRES>>1)    { mx=x; my=y; }
}
///*

uint screen::kbhit()
{
	int c = 0;
	ioctl(0,FIONREAD,&c);
	struct timeval tv = { 0,0 };
	select(1,0,0,0,&tv);
	return c;
}

///implementation
void screen::init(tile* c)
{
	cs = c;
	nu = new char[256];
	tcgetattr(STDIN_FILENO,&oc);
	nc = oc;
	nc.c_lflag &= ~(ICANON|ECHO); //ISIG
	//nc.c_cc[VMIN] = 4;
	//nc.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&nc);
	last = time()+4000;
}

bool screen::event()
{
	const int r = kbhit();
	guard(r==0,(mb=0)==0); 

	const int s = 3-(r==1)-(r==2);
	packed t;
	read(0,&t,s);
	tk = kk = math::set(t.b[2],t.b[0],t.b[1]==91);
	read(0,&nu,math::min(r-s,256));

	mb = kk==SPACE;
	mx = math::lim(0,mx+((kk==LEFT)<<3)-((kk==RIGHT)<<3),XRES);
	my = math::lim(0,my+((kk==DOWN)<<3)-((kk==UP)<<3),YRES);

	return 1; //kk!=CLOSE;
}

void screen::set()
{
        fd = open("/dev/fb0",O_RDWR);
	error(fd<=0,"Error: Could not open /dev/fb0");

	const sint e = ioctl(fd,FBIOGET_FSCREENINFO,&finfo);
	error(e<0,"Error: Could not read fixed framebuffer info");

	const sint f = ioctl(fd,FBIOGET_VSCREENINFO,&oinfo);
	error(f<0,"Error: Could not read variable framebuffer info");

	const sint g = ioctl(fd,FBIOPUT_VSCREENINFO,&vinfo);
	error(g<0,"Error: Could not write variable framebuffer info");

	void* ptr = mmap(0,XRES*YRES*4,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
	error(ptr==MAP_FAILED,"Error: Could not map buffer object!");

	frame.pointer(ptr);
}

void screen::close()
{
	//delete accum;
	//delete depth;
	//delete back;
	//delete frame; 
	munmap(frame.pointer(),XRES*YRES*4);
	ioctl(fd,FBIOPUT_VSCREENINFO,&oinfo);
	::close(fd);
	tcsetattr(STDIN_FILENO,TCSANOW,&oc);
	delete nu;
}
///*

