//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_XIZERO
#define HH_XIZERO
//#pragma message "Compiling " __FILE__ " ! TODO: complete garbage collector"

#include "CLinc.h"

void xizero_cleanup() { CLgarbage(); }

CLglobal* clglobal = 0;
CLar* cldata = 0;
xlong currlevel = 0;

void exitgame() { clglobal->clapp->exit(0,"xizero says: bye"); }

void newgame()
{
	clglobal->clwindow->hidecursor();
	
	//open full screen images archive
	CLar* screens = new CLar(cldata->findbyname("screens.a"));
	//*
	
	//fullscreen loading bar
	clglobal->clfsprogress->reset();
	//*
	
	//display intro
	sprite* introscreen = 0;
	introscreen = clglobal->clformat->loadras(screens->findbyname("intro000.ras"));
	clglobal->clgfx->drawscreen(introscreen);
	clglobal->clwindow->run();
	clglobal->clwindow->sleep(1000);
	clglobal->clfsprogress->set(10);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->run();
	//*
	
	//load and init level
	CLar* lvls = new CLar(cldata->findbyname("levels.a"));
	xchar** lfn = clglobal->clformat->loadlvl(lvls->getmember(currlevel));
	CLlevel* testlevel = new CLlevel(cldata->findbyname(lfn[1]),cldata->findbyname(lfn[2]),cldata->findbyname(lfn[3]),cldata->findbyname(lfn[4]),cldata->findbyname(lfn[5]),currlevel);
	clglobal->clfsprogress->set(90);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->run();
	//*
	
	//init level floor
	uxlong floorcolor = clglobal->clstring->hex(lfn[0]);
	clglobal->clfloor->init(100,670,floorcolor,1);
	clglobal->clfsprogress->set(100);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->run();
	clglobal->clwindow->sleep(1000);
	//*
	
	//game loop variables
	xlong gamestate = 1;
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
							
	//if(clglobal->clmsgbox->msgbox("Info",startmsg)==1) testlevel->start();
	//*
	
	//game loop
	bool aa = 0;
	
	CLmenu* sysmenu = new CLmenu();
	testlevel->start();
	while(clglobal->clwindow->run() && gamestate>0) 
	{
		//check input
		turbo = clglobal->clwindow->getturbo();
		input = clglobal->clwindow->getinkey();
		clglobal->clgamepad->handle();
		//*

		//grab system keys and update level
		switch(input)
		{
			case ESC: exitgame(); break;
			
			case 'p': testlevel->pause(); break;
			
			case '#': aa = !aa;
			
			default: gamestate = testlevel->update(input,turbo,clglobal->clgamepad->getstate()); break;
		}
		//*

		clglobal->clfloor->draw();
		testlevel->display();

		if(aa)
		{
			clglobal->clscreen->cldoublebuffer.copy(&clglobal->clscreen->clframebuffer,2);
			clglobal->clscreen->clframebuffer.copy(&clglobal->clscreen->cldoublebuffer,1);
		}

		//~ //render level depending on mode (modes just for dev)
		//~ switch(mode)
		//~ {
			//~ //render levellandscape
			//~ case 2:
				//~ clglobal->cldoublebuffer.clear(0);
				//~ clglobal->clzbuffer.clear(ZRES);
				//~ clglobal->clstencilbuffer.clear(0);
				//~ displacement = ( testlevel->getmark() ) * XRES;
				//~ clglobal->clmisc3d->drawzbuffer(testlevel->getlandscape(),displacement);
				//~ testlevel->getplayer()->showbox();
			//~ break;
			//~ //*
//~ 
			//~ //render zbuffer
			//~ case 3:
				//~ clglobal->clfloor->draw();
				//~ testlevel->display();
				//~ clglobal->clmisc3d->drawzbuffer();
			//~ break;
			//~ //*
		//~ }
	}
	//*
	
	//crush level
	//delete testlevel;
	delete sysmenu;
	//*
	
	clglobal->cltransitions->fadetoblack();
	
	sprite* overscreen = 0;
	switch(gamestate)
	{
		//draw winner screen
		case 0:
			overscreen = clglobal->clformat->loadras(screens->findbyname("gamewon.ras"));
			clglobal->clgfx->drawscreen(overscreen);
			clglobal->clwindow->run();
			clglobal->clwindow->sleep(11000);
		break;
		//*
		
		//draw looser screen
		case -1:
			overscreen = clglobal->clformat->loadras(screens->findbyname("gameover.ras"));
			clglobal->clgfx->drawscreen(overscreen);
			clglobal->clwindow->run();
			clglobal->clwindow->sleep(11000);
		break;
		//*
	}
	
	clglobal->cltransitions->dissolve();
	
	clglobal->clwindow->showcursor();
	
	//~ delete screens;
	//~ delete lvls;
	//~ delete testlevel; 
}


void loadgame() { say("not available yet"); }
void options() { say("not available yet"); }

#endif
