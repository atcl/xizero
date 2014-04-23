///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// SDL Interface Library ( XZscreen.hh )
#pragma once
///</header>

///<include>
#include <SDL/SDL.h>		// SDL_Surface,SDL_SetVideoMode,SDL_Flip,SDL_Quit,SDL_GetKeyState,SDL_PumpEvents,SDL_WM_SetCaption

#include "XZbasic.hh"
#include "XZtile.hh"
#include "XZbuffer.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define XRES 640
#define YRES 480
#define BPP  32
#define FPS  4000

#define ZRES 480

#define XMIN 0
#define XMAX XRES-1
#define YMIN 0
#define YMAX YRES-1

#define ESCAPE	SDLK_ESCAPE
#define ENTER	SDLK_RETURN
#define SPACE	SDLK_SPACE
#define UP	SDLK_UP
#define DOWN	SDLK_DOWN
#define LEFT	SDLK_LEFT
#define RIGHT	SDLK_RIGHT
#define KEYW    SDLK_w
#define KEYA    SDLK_a
#define KEYS    SDLK_s
#define KEYD    SDLK_d

#define PGUP    SDLK_PAGEUP
#define PGDOWN  SDLK_PAGEDOWN
#define CTRL    SDLK_LCTRL
#define KEYX    SDLK_x
///</declare>

///<define>
namespace screen
{
	buffer frame(XRES*YRES,0);	// framebuffer
	buffer depth(XRES*YRES);	// z-buffer
	bool zs = 0;			// z state

	namespace
	{
		yint  kk = 0;		// keyboard key
		yint  tk = 0;		// turbo key

		SDL_Surface* video;
		Uint8*       keys; 	//tmp
		SDL_Event    event;
	}

	inline yint key()         { const yint r = kk; kk = 0; return r; }
	inline yint turbo()       { return tk; }
	inline yint time()        { return SDL_GetTicks(); }
	inline void sleep(xint t) { SDL_Delay(t); }

	void init(const tile& c);
	bool run();
	void close();
	yint fps(bool o=1);
	void wait(yint k)	{ while(key()!=k) run(); }

	pure inline bool onscreen(xint x,xint y) { return (x>=0) && (y>=0) && (x<XRES) && (y<YRES); }
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
}

bool screen::run()
{
	//if(aa) frame.fsaa(frame);
	SDL_Flip(video);
	zs = !zs;

	SDL_PollEvent(&event);
	const yint ek = event.key.keysym.sym;

	switch(event.type)
	{
		case SDL_KEYDOWN:
			kk = math::set(ek,kk,tk!=ek);
			tk = ek;
			break;

		case SDL_KEYUP:
			tk = math::set(tk,tk!=ek);
			break;

		case SDL_QUIT:
			system::bye();
	}

	ifu( (keys[CTRL]&&keys[KEYX]) ) { system::bye(); } //tmp

	return 1;
}

void screen::close()
{
	frame.clear();
	SDL_Flip(video);
	SDL_Quit();
}

yint screen::fps(bool o)
{
	static yint last = 0;
	static yint frames = 0;

	yint curr = SDL_GetTicks();
	++frames;
	if(curr >= last && o==1)
	{
		last = curr + FPS;
		curr = frames>>2;
		frames = 0;
		return curr;
	}

	return -1;
} 
///</code>

