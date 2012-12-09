///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZfrmbuf.hh
// Direct Framebuffer Access and Input Handling Library
#pragma once
///</header>

///<include>
#include <time.h>	//clock,CLOCKS_PER_SEC
#include <fcntl.h>	//open,close,O_RDWR
#include <unistd.h>	//STDIN_FILENO
#include <termios.h>	//tcgetattr,tcsetattr,ICANON,ECHO,TCSANOW
#include <sys/mman.h>	//mmap,munmap,PROT_READ,PROT_WRITE,MAP_SHARED,MAP_FIXED,MAP_FAILED
#include <sys/ioctl.h>	//ioctl,FIONREAD
#include <linux/fb.h>	//FBIOGET_FSCREENINFO,FBIOGET_VSCREENINFO,FBIOPUT_VSCREENINFO,FBIOPAN_DISPLAY,FB_ACTIVATE_NOW

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
#define JS_DEV "/dev/input/js0"

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
	buffer frame(XRES*YRES);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		void* cs = 0;					//cursor image
		uint  tk = 0;					//turbo key
		uint  kk = 0;					//keyboard key
		uint  ms = uint((XRES/2)<<16)+uint(YRES/2);	//compressend mouse data
		uint  ls = 0;					//.
		char* nu = 0;					//.

		termios nc;					//new terminal config
		termios oc;					//old terminal config

		sint  fd;					//framebuffer device handle
		sint  gd;					//gamepad device handle

		struct fb_fix_screeninfo finfo;
		struct fb_var_screeninfo oinfo;
		struct fb_var_screeninfo vinfo{XRES,YRES,XRES,YRES,0,0,32,0,{16,8,0},{8,8,0},{0,8,0},{0,0,0},0,0,0xFFFFFFFF,0xFFFFFFFF,1};//,25000,88,40,23,1,128,4,0,0,0,0,0,0,0,0}; 
	}

	uint kbhit();
	void init(void* c);
	void set();
	void _flush()		{ frame.copy(back); }
	void flush()		{ frame.copy(back); }
	void event();
	void close();

	inline uint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ while(k!=kk) { event(); } }
	void sleep(sint t)	{ const sint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=ls&&o==1) { ls=t+FPS; t=f>>2; f=0; return t; } return -1; } 

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
	nc.c_lflag &= ~(ICANON|ECHO);
	//nc.c_cc[VMIN] = 4;
	//nc.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&nc);
	ls = time()+4000;
	system::ifx(close);
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

	//TODO: gamepad 

	uint mx =  math::lim(0,MOUSEY(ms)+((kk==DOWN)<<3)-((kk==UP)<<3),YRES);		//set bottom word to mouse y
	mx += math::lim(0,MOUSEX(ms)+((kk==LEFT)<<3)-((kk==RIGHT)<<3),XRES)<<16;	//set top word to mouse x
	mx += (kk==SPACE)<<31;								//set top bit to mouse button
	ms = mx;
	ifu(kk==CLOSE) { system::bye(); }
}

void screen::set()
{
        system::err((fd=open(FB_DEV,O_RDWR))<=0,"ERROR: Could not open framebuffer device");
	system::err(ioctl(fd,FBIOGET_FSCREENINFO,&finfo)<0,"ERROR: Could not read fixed framebuffer info");
	system::err(ioctl(fd,FBIOGET_VSCREENINFO,&oinfo)<0,"ERROR: Could not read variable framebuffer info");
	system::err(ioctl(fd,FBIOPUT_VSCREENINFO,&vinfo)<0,"ERROR: Could not write variable framebuffer info");
	system::err(mmap(frame.pointer(),XRES*YRES*4,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FIXED,fd,0)==MAP_FAILED,"ERROR: Could not map buffer object!");
	system::err(ioctl(fd,FBIOPAN_DISPLAY,&vinfo)<0,"ERROR: Could not report changes to kernel");
alert(finfo.smem_len);
}

void screen::close()
{
	back.clear();
	//"press CTRL ALT F1\n then CTRL ALT F7"
	_flush();
	system::say("XiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
	munmap(frame.pointer(),XRES*YRES*4);
	oinfo.activate = FB_ACTIVATE_NOW;
	ioctl(fd,FBIOPUT_VSCREENINFO,&oinfo);
	::close(fd);
	tcsetattr(STDIN_FILENO,TCSANOW,&oc);
	//delete nu; 
}
///</code>

