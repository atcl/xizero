///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZfrmbuf.hh
// Direct Framebuffer Access and Input Handling Library
///</header>

///<include>
#pragma once
#include <time.h>		//clock,CLOCKS_PER_SEC
#include <SDL/SDL.h>		//SDL_Surface,SDL_SetVideoMode,SDL_Flip,SDL_Quit,SDL_GetKeyState

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

#define ESCAPE	SDLK_ESCAPE
#define ENTER	SDLK_RETURN
#define SPACE	SDLK_SPACE
#define PGUP	SDLK_PAGEUP
#define PGDOWN	SDLK_PAGEDOWN
#define UP	SDLK_UP
#define DOWN	SDLK_DOWN
#define LEFT	SDLK_LEFT
#define RIGHT	SDLK_RIGHT
#define CTRL    SDLK_LCTRL
#define KEYW    SDLK_w
#define KEYA    SDLK_a
#define KEYS    SDLK_s
#define KEYD    SDLK_d
#define KEYX    SDLK_x
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
		Uint8* keys;
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

	keys = SDL_GetKeyState(0);
	video = SDL_SetVideoMode(XRES, YRES, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL); 
	frame.pointer(video->pixels);
	SDL_WM_SetCaption(TITLE " " VERSION,TITLE);

	system::ifx(close);
}

void screen::event()
{
	SDL_PumpEvents();
	ifu(keys[CTRL]&&keys[KEYX]) { system::bye(); }

	kk = math::set(UP,kk,keys[UP]);
	kk = math::set(DOWN,kk,keys[DOWN]);
	kk = math::set(LEFT,kk,keys[LEFT]);
	kk = math::set(RIGHT,kk,keys[RIGHT]);
	kk = math::set(SPACE,kk,keys[SPACE]);
	kk = math::set(ENTER,kk,keys[ENTER]);
	kk = math::set(KEYW,kk,keys[KEYW]);
	kk = math::set(KEYA,kk,keys[KEYA]);
	kk = math::set(KEYS,kk,keys[KEYS]);
	kk = math::set(KEYD,kk,keys[KEYD]);
	kk = math::set(PGUP,kk,keys[PGUP]);
	kk = math::set(PGDOWN,kk,keys[PGDOWN]);

	yint mx =  math::lim(0,MOUSEY(ms)+(kk==DOWN)-(kk==UP),YRES);		//set bottom word to mouse y
	     mx += math::lim(0,MOUSEX(ms)+(kk==RIGHT)-(kk==LEFT),XRES)<<16;	//set top word to mouse x
	     mx += (kk==SPACE)<<31;						//set top bit to mouse button
	ms = mx;
}

void screen::close()
{
	frame.clear();
	flush();

	SDL_Quit();
	system::say("\nXiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
}
///</code>

