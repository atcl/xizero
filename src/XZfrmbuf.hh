///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZfrmbuf.hh
// Direct Framebuffer Access and Input Handling Library
///</header>

///<include>
#pragma once
#include <time.h>		//clock,CLOCKS_PER_SEC
#include <SDL/SDL.h>

#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define MOUSEX(x) ((x&0x7FFFFFFF)>>16)
#define MOUSEY(x)  (x&0x0000FFFF)
#define MOUSEB(x) ((x&0x80000000)!=0)

#define BPP 32
#define FPS 4000

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
	buffer frame(XRES*YRES,0);	//Framebuffer
	buffer depth(XRES*YRES);	//Z-Buffer
	byte zs = 0;			//Z State

	namespace
	{
		void* cs = 0;					//cursor image
		yint  kk = 0;					//keyboard key
		yint  ms = yint((XRES/2)<<16)+yint(YRES/2);	//compressend mouse data
		yint  ls = 0;					//last fps update

		SDL_Surface* video;
	}

	void init(void* c);
	void event();
	void close();

	inline yint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ kk=0; while(k!=kk) { event(); } kk=0; }
	void sleep(xint t)	{ const xint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	yint fps(bool o=1)	{ static yint f=0; yint t=time(); f+=o; if(t>=ls&&o==1) { ls=t+FPS; t=f>>2; f=0; return t; } return -1; } 

	inline void  flush()	{ SDL_Flip(video); ++zs; }
	inline void  aaflush()	{ frame.fsaa(frame); SDL_Flip(video); ++zs; }
	inline bool  run()	{ flush(); event(); return 1; }
	inline yint  key()	{ const yint r=kk; kk=0; return r; }
	inline yint  turbo()    { return kk; }
	inline yint  mouse()    { return ms; }
	inline void* cursor()	{ return cs; }
	inline void  smouse(yint x=XRES/2,yint y=YRES/2) { ms = (x<<16)+y; }
}
///</define>

///<code>
void screen::init(void* c)
{
	cs = c;

	video = SDL_SetVideoMode(XRES, YRES, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL); 
	frame.pointer(video->pixels);

	system::ifx(close);
}

void screen::event()
{/*
	ms &= 0x7FFFFFFF;

	kk = getchar(); //read(0, &kk, 1);
	ifu(kk==ESCAPE||kk==0) { kk = getchar(); }

	const xint r = read(jd,&joyst,JS_RETURN);
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

	const yint mk = math::set(1,3,((joyst.y!=128)||(joyst.x!=128))&&(r>0));
	yint mx =  math::lim(0,MOUSEY(ms)+((kk==DOWN)<<mk)-((kk==UP)<<mk),YRES);	//set bottom word to mouse y
	     mx += math::lim(0,MOUSEX(ms)+((kk==LEFT)<<mk)-((kk==RIGHT)<<mk),XRES)<<16;	//set top word to mouse x
	     mx += (kk==SPACE)<<31;							//set top bit to mouse button
	ms = mx;
	ifu(kk==CLOSE) { system::bye(); }
*/}

void screen::close()
{
	frame.clear();
	flush();

	SDL_Quit();
	system::say("\nXiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
	
}
///</code>

