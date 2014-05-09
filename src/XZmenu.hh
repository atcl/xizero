///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Menu Bar Library ( XZmenu.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZgfx.hh"
#include "XZformat.hh"
#include "XZbuttons.hh"
#include "XZdialog.hh"

#include "XZresource.hh"
///</include>

///<define>
namespace menu
{
	void show();
}

void menu::show()
{
	//enlist buttons
	buttons bl;
	bl.add("Hide",[](){ return 1; },0,16,16,40,15,BLACK,RED,GREY,GREY,1);
	bl.add("About",[](){ dialog::msgbox("XiZero\nby atCROSSLEVEL studios\nVersion: " VERSION ); return 0; },0,60,16,50,15,BLACK,RED,GREY,GREY,1);
	bl.add("Exit",[](){ if(dialog::msgbox("Are you sure?",1)==1) { system::bye(); }; return 0; },0,114,16,50,15,BLACK,RED,GREY,GREY,1);
	
	//*

	//draw menu
	tile ico = format::xpm(resource::icon);
	tile scr = gfx::save();
	xint xit = 0;
	while(screen::run() && xit==0)
	{
		gfx::draw(scr);
		gfx::draw(ico);
		gfx::rect(16,0,XMAX,15,RED,RED,1,0);
		gfx::rect(0,16,XMAX,31,GREY,GREY,1,0);
		font::draw(20,0,"atCROSSLEVEL XiZero",WHITE);

		bl.draw();
		const yint k = screen::key();
		xit = bl.check(k) + (k==ESCAPE);
	}
	//*
}
///</define>

