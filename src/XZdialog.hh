// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZmsgbox.hh
// Dialog Library 

///guard
#ifndef HH_XZDIALOG
#define HH_XZDIALOG
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZbutton.hh"
#include "XZscreen.hh"
///*

///definitions
namespace dialog
{
	sint msgbox(const char* m);
	inline sint ok() { return 1; }
}
///*

///implementation
sint dialog::msgbox(const char* m)
{
	const sint w = font::width(m)+40;
	const sint h = font::height(m)+60;
	const sint x = (XRES-w)>>1;
	const sint y = (YRES-h)>>1;
	button::all(0);
	const button bok("OK",&ok,1,x+(w>>1)-30,y+h-30,60,20,BLACK,SYSCOL,BLACK,1);

	tile* scr = gfx::save();
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())==0)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(x,y,x+w,y+h,WHITE,WHITE,1,0);
		font::draw(x+20,y+20,m,BLACK,WHITE);
		bok.draw();
		gfx::sprite(*(tile*)screen::cursor(),screen::mousex(),screen::mousey());
	}
	
	button::all(1);
	delete scr;
	return 1;
}
///*

#endif

