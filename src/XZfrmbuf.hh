///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZfrmbuf.hh
// SDL Interface
///</header>

///<include>
#pragma once
#include <time.h>		//clock,CLOCKS_PER_SEC
#include <SDL/SDL.h>		//SDL_Surface,SDL_SetVideoMode,SDL_Flip,SDL_Quit,SDL_GetKeyState

#include "XZbasic.hh"
#include "XZtile.hh"
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
		yint  kk = 0;					//keyboard key
		yint  tk = 0;					//turbo key
		yint  ms = yint((XRES/2)<<16)+yint(YRES/2);	//compressend mouse data
		yint  ls = 0;					//last fps update

		SDL_Surface* video;
		Uint8* keys;
	}

	void init(const tile& c);
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
	inline yint  turbo()    { return tk; }
}
///</define>

///<code>
void screen::init(const tile& c)
{
	//SDL_WM_SetIcon(c,0);
	keys = SDL_GetKeyState(0);
	video = SDL_SetVideoMode(XRES,YRES,BPP,SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL); 
	frame.pointer(video->pixels);
	SDL_WM_SetCaption(TITLE " " VERSION,TITLE);

	system::ifx(close);
}

void screen::event()
{
	SDL_PumpEvents();
	ifu(keys[CTRL]&&keys[KEYX]) { system::bye(); }

	yint tt = 0;
	tt = math::set(UP,tt,keys[UP]);
	tt = math::set(DOWN,tt,keys[DOWN]);
	tt = math::set(LEFT,tt,keys[LEFT]);
	tt = math::set(RIGHT,tt,keys[RIGHT]);
	tt = math::set(SPACE,tt,keys[SPACE]);
	tt = math::set(ENTER,tt,keys[ENTER]);
	tt = math::set(ESCAPE,tt,keys[ESCAPE]);
	tt = math::set(KEYW,tt,keys[KEYW]);
	tt = math::set(KEYA,tt,keys[KEYA]);
	tt = math::set(KEYS,tt,keys[KEYS]);
	tt = math::set(KEYD,tt,keys[KEYD]);
	tt = math::set(PGUP,tt,keys[PGUP]);
	tt = math::set(PGDOWN,tt,keys[PGDOWN]);

	kk = math::set(tk,tt!=tk);
	tk = tt;
}

void screen::close()
{
	frame.clear();
	flush();

	SDL_Quit();
	system::say("\nXiZero " VERSION " by atCROSSLEVEL. Thanks for playing!",1);
}
///</code>

