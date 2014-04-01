///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZmsgbox.hh
// Dialog Library
#pragma once
///</header>

///<include>
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZbuttons.hh"
#include "XZfrmbuf.hh"
///</include>

///<define>
namespace dialog
{
	xint msgbox(const char* m,bool q=0);
}
///</define>

///<code>
xint dialog::msgbox(const char* m,bool q)
{
	const xint w = font::width(m)+80;
	const xint h = font::height(m)+60;
	const xint x = (XRES-w)/2;
	const xint y = (YRES-h)/2;
	buttons bl;
	bl.add("OK",[](){ return  xint(1); },1,x+(w/2)-20-(30*q),y+h-30,40,20,BLACK,RED,SYSCOL,BLACK,1); 
	if(q) { bl.add("NO",[](){ return xint(-1); },1,x+(w/2)+10,y+h-30,40,20,BLACK,RED,SYSCOL,BLACK,q); }

	xint xit = 0;
	while(xit==0 && screen::run())
	{
		gfx::rect(x,y,x+w,y+h,WHITE,WHITE,1,0);
		font::draw(x+20,y+20,m,BLACK,WHITE);

		bl.draw();
		xit = bl.check(screen::key());
	}

	return xit;
}
///</code>

