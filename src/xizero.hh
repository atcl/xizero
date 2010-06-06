//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
#ifndef HH_XIZERO
#define HH_XIZERO

#include "CLinc.h"

CLar* cldata = 0;

CLglobal* global = 0;

void exitgame() { global->clapp.exit(0,"xizero says: bye"); }

void newgame()
{
	global->clwindow.showcursor(0);
	
	//open full screen images archive
	CLar* screens = new CLar(cldata->findbyname("screens.a"));
	//*
	
	//fullscreen loading bar
	global->clfsprog.reset();
	//*
	
	CLar* lvls = new CLar(cldata->findbyname("levels.a"));
	xlong gamestate = 0;
	xlong currlevel = 0;
	xlong maxlevel = lvls->getfilecount();

	while(gamestate!=-1 && currlevel<maxlevel)
	{
		//display intro
		sprite* introscreen = 0;
		xchar* extras = global->clstring.concat(global->clstring.toascii(currlevel),".ras");
		xchar* exttxt = global->clstring.concat(global->clstring.toascii(currlevel),".txt");
		introscreen = global->clformat.loadras(screens->findbyextension(extras));
		xchar* introtext = screens->findbyextension(exttxt)->text;
		xlong introlength = screens->findbyextension(exttxt)->size;
		global->clgfx.drawscreen(introscreen);
		global->clgfx.drawfontstring(0,12,introtext,1,0x00FFFFFF,0,introlength);
		global->clwindow.run();
		global->clwindow.sleep(1000);
		global->clfsprog.set(10);
		global->clfsprog.draw();
		global->clwindow.run();
		//*

		//load and init level
		xchar** lfn = global->clformat.loadlvl(lvls->getmember(currlevel)); //crash in here
		CLlevel* testlevel = new CLlevel(cldata->findbyname(lfn[1]),cldata->findbyname(lfn[2]),cldata->findbyname(lfn[3]),cldata->findbyname(lfn[4]),0,currlevel);
		global->clfsprog.set(100);
		global->clfsprog.draw();
		global->clwindow.run();
		global->clwindow.sleep(1000);
		//*
		
		//game loop variables
		gamestate = 1;
		xlong mode = 1;
		xlong displacement = 0;
		uxchar input = 0;
		uxchar turbo = 0;
		//*
		
		//display key bindings and init level
		const xchar* startmsg = "Use these keys:\n"
								"Arrow Left/Right: Turn Tank\n"
								"Arrow Forw/Backw: Drive Forw/Backw\n"
								"a,d:              Turn Tower\n"
								"w:                Reset Tower\n"
								"0:                Fire Chassis Weapon\n"
								"Space:            Fire Tower Weapon\n"
								"p:                Pause game\n"
								"^:                Exit to main menu\n"
								"Esc:              Quit XiZero\n"
								"#:                Activate/Deactivate Anti-Aliasing (2xFSAA RGMS)\n"
								"After pressing OK the game will start immediately!";
								
		if(global->clmsgbox.msgbox("Info",startmsg)==1) { xlong tempo = 0; }
		//*
		
		//game loop
		bool aa = 0;
		
		testlevel->start();
		global->clwindow.showfps(1);
		while(global->clwindow.run() && gamestate>0) 
		{
			//check input
			input = global->clwindow.getinkey(1);
			//*

			//grab system keys and update level
			switch(input)
			{
				case ESC: exitgame(); break;
				case 'p': testlevel->pause(); break;
				case '#': aa = !aa;
				default: gamestate = testlevel->update(); break;
			}
			//*

			global->clscreen.cldoublebuffer.clear();
			global->clscreen.clstencilbuffer.clear();
			global->clscreen.clzbuffer.clear(ZRES);
			testlevel->display();

			if(aa)
			{
				global->clscreen.cldoublebuffer.copy(&(global->clscreen.clframebuffer),2);
				global->clscreen.clframebuffer.copy(&(global->clscreen.cldoublebuffer),1);
			}
		}
		global->clwindow.showfps(0);
		//*
		
		//crush level
		//delete testlevel;
		//*
		
		currlevel++;
		global->cltrans.fadetoblack();
	}
	
	sprite* overscreen = 0;
	xchar* overtext = 0;
	xlong overlength = 0;
		
	switch(gamestate)
	{
		//draw winner screen
		case 0:
			overscreen = global->clformat.loadras(screens->findbyname("gamewon.ras"));
			overtext = screens->findbyname("gamewon.txt")->text;
			overlength = screens->findbyname("gamewon.txt")->size;
			global->clgfx.drawscreen(overscreen);
			global->clgfx.drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			global->clwindow.run();
			global->clwindow.sleep(11000);
		break;
		//*
		
		//draw looser screen
		default:
			overscreen = global->clformat.loadras(screens->findbyname("gameover.ras"));
			overtext = screens->findbyname("gameover.txt")->text;
			overlength = screens->findbyname("gameover.txt")->size;
			global->clgfx.drawscreen(overscreen);
			global->clgfx.drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			global->clwindow.run();
			global->clwindow.sleep(11000);
		break;
		//*
	}
	
	global->cltrans.dissolve();
	
	global->clwindow.showcursor(0);
	global->clwindow.showcursor(1);
	
	//~ delete screens;
	//~ delete lvls;
	//~ delete testlevel; 
	
	return;
}


void loadgame() { say("not available yet"); }
void options() { say("not available yet"); }

#endif
