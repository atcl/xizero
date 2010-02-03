//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_XIZERO
#define HH_XIZERO
//#pragma message "Compiling " __FILE__ " ! TODO: complete garbage collector"

#include "CLinc.h"

CLglobal* clglobal = 0;
CLar* cldata = 0;

void exitgame() { clglobal->clapp->exit(0,"xizero says: bye"); }

void newgame()
{
	clglobal->clwindow->showcursor(0);
	
	//open full screen images archive
	CLar* screens = new CLar(cldata->findbyname("screens.a"));
	//*
	
	//fullscreen loading bar
	clglobal->clfsprogress->reset();
	//*
	
	CLar* lvls = new CLar(cldata->findbyname("levels.a"));
	xlong gamestate = 0;
	xlong currlevel = 0;
	xlong maxlevel = lvls->getfilecount();

	while(gamestate!=-1 && currlevel<maxlevel)
	{
		//display intro
		sprite* introscreen = 0;
		xchar* extras = clglobal->clstring->concat(clglobal->clstring->toascii(currlevel),".ras");
		xchar* exttxt = clglobal->clstring->concat(clglobal->clstring->toascii(currlevel),".txt");
		introscreen = clglobal->clformat->loadras(screens->findbyextension(extras));
		xchar* introtext = screens->findbyextension(exttxt)->text;
		xlong introlength = screens->findbyextension(exttxt)->size;
		clglobal->clgfx->drawscreen(introscreen);
		clglobal->clgfx->drawfontstring(0,12,introtext,1,0x00FFFFFF,0,introlength);
		clglobal->clwindow->run();
		clglobal->clwindow->sleep(1000);
		clglobal->clfsprogress->set(10);
		clglobal->clfsprogress->draw();
		clglobal->clwindow->run();
		//*
		
		//load and init level
		xchar** lfn = clglobal->clformat->loadlvl(lvls->getmember(currlevel));
		CLlevel* testlevel = new CLlevel(cldata->findbyname(lfn[1]),cldata->findbyname(lfn[2]),cldata->findbyname(lfn[3]),cldata->findbyname(lfn[4]),0,currlevel);
		clglobal->clfsprogress->set(100);
		clglobal->clfsprogress->draw();
		clglobal->clwindow->run();
		clglobal->clwindow->sleep(1000);
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
								"Left Ctrl:        Fire Chassis Weapon\n"
								"Space:            Fire Tower Weapon\n"
								"p:                Pause game\n"
								"^:                Exit to main menu\n"
								"Esc:              Quit XiZero\n"
								"After pressing OK the game will start immediately!";
								
		if(clglobal->clmsgbox->msgbox("Info",startmsg)==1) { xlong tempo = 0; } //testlevel->start();
		//*
		
		//game loop
		bool aa = 0;
		
		testlevel->start();
		clglobal->clwindow->showfps(1);
		while(clglobal->clwindow->run() && gamestate>0) 
		{
			//check input
			input = clglobal->clwindow->getinkey(1);
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

			clglobal->clscreen->cldoublebuffer.clear();
			clglobal->clscreen->clstencilbuffer.clear();
			clglobal->clscreen->clzbuffer.clear(ZRES);
			testlevel->display();

			if(aa)
			{
				clglobal->clscreen->cldoublebuffer.copy(&clglobal->clscreen->clframebuffer,2);
				clglobal->clscreen->clframebuffer.copy(&clglobal->clscreen->cldoublebuffer,1);
			}
		}
		clglobal->clwindow->showfps(0);
		//*
		
		//crush level
		//delete testlevel;
		//*
		
		currlevel++;
		clglobal->cltransitions->fadetoblack();
	}
	
	sprite* overscreen = 0;
	xchar* overtext = 0;
	xlong overlength = 0;
		
	switch(gamestate)
	{
		//draw winner screen
		case 0:
			overscreen = clglobal->clformat->loadras(screens->findbyname("gamewon.ras"));
			overtext = screens->findbyname("gamewon.txt")->text;
			overlength = screens->findbyname("gamewon.txt")->size;
			clglobal->clgfx->drawscreen(overscreen);
			clglobal->clgfx->drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			clglobal->clwindow->run();
			clglobal->clwindow->sleep(11000);
		break;
		//*
		
		//draw looser screen
		default:
			overscreen = clglobal->clformat->loadras(screens->findbyname("gameover.ras"));
			overtext = screens->findbyname("gameover.txt")->text;
			overlength = screens->findbyname("gameover.txt")->size;
			clglobal->clgfx->drawscreen(overscreen);
			clglobal->clgfx->drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			clglobal->clwindow->run();
			clglobal->clwindow->sleep(11000);
		break;
		//*
	}
	
	clglobal->cltransitions->dissolve();
	
	clglobal->clwindow->showcursor(0);
	clglobal->clwindow->showcursor(1);
	
	//~ delete screens;
	//~ delete lvls;
	//~ delete testlevel; 
}


void loadgame() { say("not available yet"); }
void options() { say("not available yet"); }

#endif
