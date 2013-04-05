///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZfrmbuf.hh
// Direct Framebuffer Access and Input Handling Library
///</header>

///<include>
#pragma once
#include <time.h>		//clock,CLOCKS_PER_SEC
#include <fcntl.h>		//open,read,close,O_RDWR,O_RDONLY,O_NONBLOCK
#include <unistd.h>		//STDIN_FILENO
#include <termios.h>		//tcgetattr,tcsetattr,ICANON,ECHO,TCSANOW
#include <sys/mman.h>		//mmap,munmap,PROT_READ,PROT_WRITE,MAP_SHARED,MAP_FIXED,MAP_FAILED
#include <sys/ioctl.h>		//ioctl
#include <linux/fb.h>		//FBIOGET_FSCREENINFO,FBIOGET_VSCREENINFO,FBIOPUT_VSCREENINFO,FBIOPAN_DISPLAY,FB_ACTIVATE_NOW
#include <linux/joystick.h>	//JS_DATA_TYPE

#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define BPP 32
#define FPS 4000

#define FB_DEV "/dev/fb0"
#define JS_DEV "/dev/input/js0"

#define ESCAPE	27
#define ENTER	10
#define SPACE	32
#define PGUP	53
#define PGDOWN	54
#define UP	65
#define DOWN	66
#define LEFT	67
#define RIGHT	68

#define CLOSE	24	//CTRL+X
///</declare>

///<define>
namespace screen
{
	buffer frame(XRES*YRES);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer

	byte zs = 0;			//Z State

	namespace
	{
		void* cs = 0;					//cursor image
		uint  tk = 0;					//turbo key
		uint  kk = 0;					//keyboard key
		uint  ms = uint((XRES/2)<<16)+uint(YRES/2);	//compressend mouse data
		uint  ls = 0;					//.

		termios nc;					//new terminal config
		termios oc;					//old terminal config

		sint  fd;					//framebuffer device handle
		sint  jd;					//gamepad device handle
		struct JS_DATA_TYPE      joyst;
		struct fb_fix_screeninfo finfo;
		struct fb_var_screeninfo oinfo;
		struct fb_var_screeninfo vinfo{XRES,YRES,XRES,YRES,0,0,32,0,{16,8,0},{8,8,0},{0,8,0},{0,0,0},0,FB_ACTIVATE_NOW,0xFFFFFFFF,0xFFFFFFFF};//,25000,88,40,23,1,128,4,0,0,0,0,0,0,0,0}; 
	}

	void init(void* c);
	void set();
	void event();
	void close();

	inline uint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ kk=0; while(k!=kk) { event(); } kk=0; }
	void sleep(sint t)	{ const sint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=ls&&o==1) { ls=t+FPS; t=f>>2; f=0; return t; } return -1; } 

	inline void  flush()	{ frame.copy(back); ++zs; }
	inline bool  run()	{ flush(); event(); return 1; }
	inline uint  key()	{ const uint r=kk; kk=0; return r; }
	inline uint  turbo()	{ return tk; }
	inline uint  mouse()    { return ms; }
	inline void* cursor()	{ return cs; }
	inline void  smouse(uint x=XRES/2,uint y=YRES/2) { ms = (x<<16)+y; }
}
///</define>

///<code>
void screen::init(void* c)
{
	cs = c;
	tcgetattr(STDIN_FILENO,&oc);
	nc = oc;
	nc.c_lflag &= ~(ICANON|ECHO);
	nc.c_cc[VMIN] = 0;
	tcsetattr(STDIN_FILENO,TCSANOW,&nc);
	ls = time()+4000;
	jd = open(JS_DEV,O_RDONLY|O_NONBLOCK|O_NOCTTY|O_NDELAY);
	system::ifx(close);
}

void screen::event()
{
	ms &= 0x7FFFFFFF;
	const uint k1 = getchar();
	const uint k2 = getchar();
	tk = kk = math::set(k2,k1,k1==ESCAPE);
	
	read(jd,&joyst,JS_RETURN);
	{
		kk = math::set(UP,   kk, joyst.y==1   && (joyst.buttons&1)==0);
		kk = math::set(DOWN, kk, joyst.y==255 && (joyst.buttons&1)==0);
		kk = math::set(LEFT, kk, joyst.x==255 && (joyst.buttons&1)==0);
		kk = math::set(RIGHT,kk, joyst.x==1   && (joyst.buttons&1)==0);

		kk = math::set('a',  kk, joyst.x==1   && (joyst.buttons&1)!=0);
		kk = math::set('d',  kk, joyst.x==255 && (joyst.buttons&1)!=0);
		kk = math::set('w',  kk, joyst.y==1   && (joyst.buttons&1)!=0);
		kk = math::set(SPACE,kk,(joyst.buttons&2)!=0);

		kk = math::set(ENTER,kk,(joyst.buttons&8)!=0);
	}

	const uint mk = math::set(1,3,(joyst.y!=128)||(joyst.x!=128));
	uint mx =  math::lim(0,MOUSEY(ms)+((kk==DOWN)<<mk)-((kk==UP)<<mk),YRES);	//set bottom word to mouse y
	     mx += math::lim(0,MOUSEX(ms)+((kk==LEFT)<<mk)-((kk==RIGHT)<<mk),XRES)<<16;	//set top word to mouse x
	     mx += (kk==SPACE)<<31;							//set top bit to mouse button
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
}

void screen::close()
{
	back.clear();
	flush();
	system::say("\nXiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
	munmap(frame.pointer(),XRES*YRES*4);
	oinfo.activate = FB_ACTIVATE_NOW;
	ioctl(fd,FBIOPUT_VSCREENINFO,&oinfo);
	::close(fd);
	::close(jd);
	tcsetattr(STDIN_FILENO,TCSANOW,&oc);
	//::system("sudo chvt 1 && sudo chvt 7");
/*#define VT_ACTIVATE   0x5606
#define VT_WAITACTIVE 0x5607
uint tty = open("/dev/tty",O_RDWR);
ioctl(tty,VT_ACTIVATE,7);
ioctl(tty,VT_WAITACTIVE,7);
::close(tty);*/
}
///</code>

