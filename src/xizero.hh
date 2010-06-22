///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_XIZERO
#define HH_XIZERO
///*

///includes
#include "CL/CLglobal.hh"
///*

///definitions
void exitgame(void* c,void* d);
void newgame(void* c,void* d);
void loadgame(void* c,void* d);
void options(void* c,void* d);
///*

///implementation
void exitgame(void* c,void* d) { CLglobal& clglobal = *(static_cast<CLglobal*>(c)); clglobal.clapp.exit(0,"xizero says: bye"); }

void newgame(void* c,void* d)
{
	CLglobal& clglobal = *(static_cast<CLglobal*>(c));
	CLar* cldata = static_cast<CLar*>(d);
	
	clglobal.clwindow.showcursor(0);
	
	//open full screen images archive
	CLar* screens = new CLar(cldata->findbyname("screens.a"));
	//*
	
	//fullscreen loading bar
	clglobal.clfsprog.reset();
	//*
	
	CLar* lvls = new CLar(cldata->findbyname("levels.a"));
	xlong gamestate = 0;
	xlong currlevel = 0;
	xlong maxlevel = lvls->getfilecount();

	while(gamestate!=-1 && currlevel<maxlevel)
	{
		//display intro
		sprite* introscreen = 0;
		xchar* extras = clglobal.clstring.concat(clglobal.clstring.toascii(currlevel),".ras");
		xchar* exttxt = clglobal.clstring.concat(clglobal.clstring.toascii(currlevel),".txt");
		introscreen = clglobal.clformat.loadras(screens->findbyextension(extras));
		xchar* introtext = screens->findbyextension(exttxt)->text;
		xlong introlength = screens->findbyextension(exttxt)->size;
		clglobal.clgfx.drawscreen(introscreen);
		clglobal.clgfx.drawfontstring(0,12,introtext,1,0x00FFFFFF,0,introlength);
		clglobal.clwindow.run();
		clglobal.clwindow.sleep(1000);
		clglobal.clfsprog.set(10);
		clglobal.clfsprog.draw();
		clglobal.clwindow.run();
		//*

		//load and init level
		xchar** lfn = clglobal.clformat.loadlvl(lvls->getmember(currlevel)); //crash in here
		CLlevel* level = new CLlevel(cldata->findbyname(lfn[1]),cldata->findbyname(lfn[2]),cldata->findbyname(lfn[3]),cldata->findbyname(lfn[4]),0,currlevel);
		clglobal.clfsprog.set(100);
		clglobal.clfsprog.draw();
		clglobal.clwindow.run();
		clglobal.clwindow.sleep(1000);
		//*
		
		//game loop variables
		uxchar input = 0;
		bool aa = 0;
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
								
		gamestate = clglobal.clmsgbox.msgbox("Info",startmsg); //gamestate initialized with 1! Just a freaky way of doing it.
		//*
		
		//game loop
		level->start();
		clglobal.clwindow.showfps(1);
		while(clglobal.clwindow.run() && gamestate>0) 
		{
			//check input
			input = clglobal.clwindow.getinkey(1);
			//*

			//grab system keys and update level
			switch(input)
			{
				case ESC: exitgame(c,d); break;
				case '^': level->pause(); CLmenu::wrapper(); level->pause(); break; //temp!!
				case 'p': level->pause(); break;
				case '$': gamestate = 0; break;
				case '#': aa = !aa;
				default: gamestate = level->update(); break;
			}
			//*

			clglobal.clscreen.cldoublebuffer.clear();
			clglobal.clscreen.clstencilbuffer.clear();
			clglobal.clscreen.clzbuffer.clear(ZRES);
			level->display();

			if(aa)
			{
				clglobal.clscreen.cldoublebuffer.copy(&(clglobal.clscreen.clframebuffer),2);
				clglobal.clscreen.clframebuffer.copy(&(clglobal.clscreen.cldoublebuffer),1);
			}
		}
		clglobal.clwindow.showfps(0);
		//*
		
		clglobal.clwindow.sleep(1000);
		currlevel++;
		CLfvector* p = level->getplayer()->getposition();
		clglobal.cltrans.circleblend(p->x,p->y-level->getmark(),80);
		clglobal.cltrans.fadetoblack();
	}
	
	sprite* overscreen = 0;
	xchar* overtext = 0;
	xlong overlength = 0;
		
	switch(gamestate)
	{
		//draw winner screen
		case 0:
			overscreen = clglobal.clformat.loadras(screens->findbyname("gamewon.ras"));
			overtext = screens->findbyname("gamewon.txt")->text;
			overlength = screens->findbyname("gamewon.txt")->size;
			clglobal.clgfx.drawscreen(overscreen);
			clglobal.clgfx.drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			clglobal.clwindow.run();
			clglobal.clwindow.sleep(7000);
		break;
		//*
		
		//draw looser screen
		default:
			overscreen = clglobal.clformat.loadras(screens->findbyname("gameover.ras"));
			overtext = screens->findbyname("gameover.txt")->text;
			overlength = screens->findbyname("gameover.txt")->size;
			clglobal.clgfx.drawscreen(overscreen);
			clglobal.clgfx.drawfontstring(0,12,overtext,1,0x00FFFFFF,0,overlength);
			clglobal.clwindow.run();
			clglobal.clwindow.sleep(7000);
		break;
		//*
	}
	
	clglobal.cltrans.dissolve();
	
	clglobal.clwindow.showcursor(0);
	clglobal.clwindow.showcursor(1);
	
	//~ delete screens;
	//~ screens = 0;
	//~ delete lvls;
	//~ lvls = 0;
	
	return;
}

void loadgame(void* c,void* d) { say("not available yet"); }
void options(void* c,void* d) { say("not available yet"); }
///*

#endif
