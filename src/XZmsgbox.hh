// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZmsgbox.hh
// Message Box Library 

///guard
#ifndef HH_XZMSGBOX
#define HH_XZMSGBOX
///*

///includes
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZbutton.hh"
#include "XZscreen.hh"
#include "XZicon.hh"
///*

///definitions
namespace dialog
{
	sint msgbox(const char* m);
	sint ok() { return 1; }
}
///*

///implementation
sint dialog::msgbox(const char* m)
{
	const sint w = font::width(m,0)+40;
	const sint h = font::height(m,0)+60;
	const sint x = (XRES-w)>>1;
	const sint y = (YRES-h)>>1;
	button bok("OK",&ok,1,x+(w>>1)-30,y+h-30,60,20,BLACK,SYSCOL,BLACK,1);

	tile* scr = gfx::save();
	tile* cur = format::xpm(cursor);
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())==0)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(x,y,x+w,y+h,WHITE,WHITE,1,0);
		font::draw(x+20,y+20,m,BLACK,WHITE,0);
		bok.draw();
		gfx::sprite(*cur,screen::mousex(),screen::mousey());
	}
	return 0;
}
///*

#endif

