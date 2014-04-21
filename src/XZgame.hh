///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Game Library ( XZgame.hh )
#pragma once 
///</header>

///<include>
#include "XZbasic.hh"
#include "XZsystem.hh"
#include "XZscreen.hh"
///</include>

///<define>
namespace game
{
	void compiled(xint x,xint y,yint r,yint c);
	void benchmark();
}
///</define>

///<code>
void game::compiled(xint x,xint y,yint r,yint c)
{
	//guard(screen::onscreen(x,y)==1);
	yint o = ((y-2)*XRES)+(x-2);

	screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = c; screen::frame[o++] = c; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = c; screen::frame[o++] = c; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o] = r; o += XRES-3;
}

void game::benchmark()
{
	const xint fps = screen::fps();
	ifu(fps>0)
	{
		system::say(string::int2str(polygon::counter*fps)); 			
		system::say("T/s - ");
		system::say(string::int2str(polygon::counter)); 
		system::say("@");
		system::say(string::int2str(fps),1);
	}
}
///</code>

