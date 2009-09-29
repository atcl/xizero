//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_XIZERO
#define HH_XIZERO
//#pragma message "Compiling " __FILE__ " ! TODO: complete garbage collector"

#include "CLinc.h"

void xizero_cleanup() { CLgarbage(); }

CLglobal* clglobal = 0;
CLar* cldata = 0;

void exitgame()
{
	clglobal->clsound->exit();
	clglobal->clsystem->exit(0,0,"xizero says","bye");
}

void newgame()
{
	clglobal->clcursor->hide();
	
	//open full screen images archive
	CLar* screens = new CLar(cldata->findbyname("screens.a"));
	//*
	
	//fullscreen loading bar
	clglobal->clfsprogress->reset();
	//*
	
	//display intro
	sprite* introscreen = 0;
	introscreen = clglobal->clformat->loadtga(screens->findbyname("introtext.tga"));
	clglobal->clgfx1->drawscreen(introscreen);
	clglobal->clwindow->draw();
	clglobal->clsystem->wait(1000);
	clglobal->clfsprogress->set(10);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->draw();
	//*
	
	//load and init level
	CLar* lvls = new CLar(cldata->findbyname("levels.a"));
	xchar** lfn = clglobal->clformat->loadlvl(lvls->getmember(0));
	CLlevel* testlevel = new CLlevel(cldata->findbyname(lfn[0]),cldata->findbyname(lfn[1]),cldata->findbyname(lfn[2]),cldata->findbyname(lfn[3]),cldata->findbyname(lfn[4]));
	clglobal->clfsprogress->set(90);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->draw();
	//*
	
	//init level floor
	clglobal->clfloor->init(100,670,0x0000b0b0,1);
	clglobal->clfsprogress->set(100);
	clglobal->clfsprogress->draw();
	clglobal->clwindow->draw();
	clglobal->clsystem->wait(1000);
	//*
	
	//game loop variables
	xlong gamestate = 1;
	xlong mode = 1;
	xlong displacement = 0;
	xchar input = 0;
	xchar turbo = 0;
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
							
	if(clglobal->clsystem->msgbox(startmsg)==0) testlevel->start();
	//*
	
	//game loop
	while(gamestate>0 && clglobal->clwindow->run()) 
	{
		//check input
		turbo = clglobal->clwindow->getturbo();
		input = clglobal->clwindow->getinkey();
		clglobal->clgamepad->handle();
		//*

		//grab system keys and update level
		switch(input)
		{
			case 27: exitgame(); break;
			
			case '^': gamestate = -2; break;
			
			case 'p': testlevel->pause(); break;
			
			#ifdef DEV
			case '1': mode = 1; break;
			case '2': mode = 2; break;
			case '3': mode = 3; break;
			#endif
			
			default: gamestate = testlevel->update(input,turbo,clglobal->clgamepad->getstate()); break;
		}
		//*

		//clear buffers
		clglobal->cldoublebuffer.ultraclear(0);
		clglobal->clzbuffer.clear(ZRES);
		clglobal->clstencilbuffer.ultraclear(0);
		//*

		//render level depending on mode (modes just for dev)
		switch(mode)
		{
			//default render path
			case 1: 
				clglobal->clfloor->draw();
				testlevel->display();
			break;
			//*

			//render levellandscape
			case 2:
				displacement = ( testlevel->getmark() ) * XRES;
				clglobal->clmisc3d->drawzbuffer(testlevel->getlandscape(),displacement);
				testlevel->getplayer()->showbox();
			break;
			//*

			//render zbuffer
			case 3:
				clglobal->clfloor->draw();
				testlevel->display();
				clglobal->clmisc3d->drawzbuffer();
			break;
			//*
		}
		
		//increment fps count
		clglobal->clbench->inc();		
		//*
	}
	//*
	
	//crush level
	//delete testlevel;
	//*
	
	sprite* overscreen = 0;
	switch(gamestate)
	{
		//draw winner screen
		case 0:
			overscreen = clglobal->clformat->loadtga(screens->findbyname("gamewon.tga"));
			clglobal->clgfx1->drawscreen(overscreen);
			clglobal->clwindow->draw();
			clglobal->clsystem->wait(11000);
		break;
		//*
		
		//draw looser screen
		case -1:
			overscreen = clglobal->clformat->loadtga(screens->findbyname("gameover.tga"));
			clglobal->clgfx1->drawscreen(overscreen);
			clglobal->clwindow->draw();
			clglobal->clsystem->wait(11000);
		break;
		//*
	}
	
	clglobal->clcursor->show();
}


void loadgame() { say("not available yet"); }
void options() { say("not available yet"); }

#endif
