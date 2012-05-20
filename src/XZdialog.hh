// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZmsgbox.hh
// Dialog Library 

///guard
#ifndef HH_XZDIALOG
#define HH_XZDIALOG
//#pragma message "Compiling " __FILE__ "..." " TODO: yesnobox"
///*

///includes
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZbutton.hh"
#include "XZskms.hh"
///*

///definitions
namespace dialog
{
	inline pure sint ok() { return 1; }
	//inline pure sint no() { return -1; }
	            sint msgbox(const char* m);
	//          sint yesnobox(const char* m);
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
	sint cbrk = 0;
	while(screen::run() && cbrk==0)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(x,y,x+w,y+h,WHITE,WHITE,1,0);
		font::draw(x+20,y+20,m,BLACK,WHITE);
		bok.draw();
		cbrk = button::check(screen::mousex(),screen::mousey(),screen::mouseb());
		gfx::sprite(*screen::cursor(),screen::mousex(),screen::mousey());
	}
	
	button::all(1);
	delete[] scr->data;
	delete scr;
	return 1;
}
///*

#endif

