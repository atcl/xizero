///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZmsgbox.hh
// Dialog Library 
///</header>

///<include>
#pragma once
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZbutton.hh"
#include "XZfrmbuf.hh"
///</include>

///<define>
namespace dialog
{
	sint msgbox(const char* m,bool q=0);
}
///</define>

///<code>
sint dialog::msgbox(const char* m,bool q)
{
	const sint w = font::width(m)+80;
	const sint h = font::height(m)+60;
	const sint x = (XRES-w)>>1;
	const sint y = (YRES-h)>>1;
	button::all(0,-1);
	const button bok("OK",[](){ return  1; },1,x+(w>>1)-20-(30*q),y+h-30,40,20,BLACK,SYSCOL,BLACK,1);
	const button bno("NO",[](){ return -1; },1,x+(w>>1)+10,y+h-30,40,20,BLACK,SYSCOL,BLACK,q);

	tile* scr = gfx::save();
	sint cbrk = 0;
	screen::smouse();
	while(screen::run() && cbrk==0)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(x,y,x+w,y+h,WHITE,WHITE,1,0);
		font::draw(x+20,y+20,m,BLACK,WHITE);
		bok.draw();
		if(q) bno.draw();
		const uint ms = screen::mouse();
		cbrk = button::check(ms);
		gfx::sprite(*(tile*)screen::cursor(),MOUSEX(ms),MOUSEY(ms));
	}
	
	button::all(1,-1);
	delete[] scr->data;
	delete scr;
	return cbrk;
}
///</code>

