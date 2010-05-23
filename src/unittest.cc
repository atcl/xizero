//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license 

#include "CLinc.h"

CLglobal* clglobal;

int main(int argc, char** argv)
{
	//init API
	clglobal = new CLglobal(argv[0]); 
	//*
	
	//set command line arguments
	const xchar* argfile = "dat/other/test.y3d";
	xlong argfileindex =-1;
	if(argc>1) { argfile = argv[1]; }
	if(argc>2) { argfileindex = atoi(argv[2]); }
	//*
	
	//load argument file
	CLfile* objfile;
	if(argfileindex==-1)
	{
		objfile = clglobal->clsystem->getfile(argfile);
	}
	else
	{
		CLfile* objarch = clglobal->clsystem->getfile(argfile);
		CLar* objar = new CLar(objarch);
		objfile = objar->getmember(argfileindex);
	}
	CLobject* test = new CLobject(objfile,0);
	//*

	//load datafile
	CLar* cldata = new CLar(BASEDIR"xizero.dat");
	clglobal->clgfx->loadfonts(cldata->findbyname("fonts.a"));
	//*

	//cl test section
	
		//test tree:
		CLtree<xlong>* testtree = new CLtree<xlong>();	
		//*
		
		//test audio:
		CLar* clsdata = new CLar(cldata->findbyname("sounds.a"));
		clglobal->clsound->preload(clsdata);
		//clglobal->clsound->play(1,1);
		//*
		
	//*
	
	//main loop variables
	CLexplosion* ex = new CLexplosion(test,1);
	bool mode = 1;
	bool shadows = 0;
	short ac = 0;
	bool exp = 0;
	bool mm = 0;
	CLlvector p(400,300,100);
	sprite* screens;
	CLfile* screenf;
	//*
	
	//main loop
	while(clglobal->clwindow->run())
	{
		uxchar k = clglobal->clwindow->getinkey();
		
		switch(k)
		{
			case 0:
			break;

			//Translate:
			case RIGHT:  test->getmatrix()->translate(2,0,0);  test->update(); break;
			case LEFT:   test->getmatrix()->translate(-2,0,0); test->update(); break;
			case UP:     test->getmatrix()->translate(0,2,0);  test->update(); break;
			case DOWN: 	 test->getmatrix()->translate(0,-2,0); test->update(); break;
			case PGUP:   test->getmatrix()->translate(0,0,2);	test->update(); break;
			case PGDOWN: test->getmatrix()->translate(0,0,-2); test->update(); break;			

			//Rotate:
			case 'a':    test->getmatrix()->rotate(0,5,0);  test->update(); break;
			case 'd':    test->getmatrix()->rotate(0,-5,0); test->update(); break;
			case 'w':    test->getmatrix()->rotate(-5,0,0); test->update(); break;
			case 's':    test->getmatrix()->rotate(5,0,0);  test->update(); break;
			case 'q':    test->getmatrix()->rotate(0,0,-5); test->update(); break;
			case 'e':    test->getmatrix()->rotate(0,0,5);  test->update(); break;

			//Aspectscale:
			case '7':    test->getmatrix()->aspectscale(1.1); test->update(); break;
			case '8':    test->getmatrix()->aspectscale(0.9); test->update(); break;

			//Scale
			case '1':    test->getmatrix()->scale(1.1,1,1);    test->update(); break;
			case '2':    test->getmatrix()->scale(0.9,1,1);    test->update(); break;
			case '3':    test->getmatrix()->scale(1,1.1,1);    test->update(); break;
			case '4':    test->getmatrix()->scale(1,0.9,1);    test->update(); break;
			case '5':    test->getmatrix()->scale(1,1,1.1);    test->update(); break;
			case '6':    test->getmatrix()->scale(1,1,0.9);    test->update(); break;

			//Control:
			//case '^':    mode = !mode; break;
			case 'o':    clglobal->clmenu->show();
			case '+':    test->reset(); ac = exp = 0; break;
			case '-':    shadows = !shadows; break;
			case '#':    if(exp==0) { exp=1; ex->first(); } ex->next(); break;
			case '.':    test->translatealongnormals(1.1); break;
			case ',':    test->translatealongnormals(-1.1); break;
			case '<':    if(ac==0) ac = ANTICY; else ac = 0; break;
			case ' ':    clglobal->clsound->stop(); break;
			case 'r':    clglobal->clsound->play(2); break;
			case 'y':    clglobal->cltransitions->circleblend(200,300,50); break;
			case 'i':    
				screens = clglobal->clgfx->savescreen();
				screenf = clglobal->clformat->saveras(screens,"screen.im32");
				say(clglobal->clsystem->writefile(screenf,1));
			break;
			//System:
			case '0':    xlong rval = clglobal->clmsgbox->msgbox("hi","bye"); clglobal->clapp->exit(rval,"user : exit"); break;
		}

		//render sequence:
		//1. all non shadow objects (terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		clglobal->clscreen->cldoublebuffer.clear(0);
		clglobal->clscreen->clzbuffer.clear(ZRES);
		clglobal->clscreen->clstencilbuffer.clear(0);
		
		clglobal->clgfx->drawfontstring(100,10,"Use w,s,a,d,q,e for rotation",2,0x00FFFFFF,0x00FF0000);
		clglobal->clgfx->drawfontstring(100,30,"Use 1,2,3,4,5,6 for scaling",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,50,"Use 7,8 for aspect-scaling",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,70,"Use arrow keys and scroll-up/down for translating",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,90,"Use + for reseting",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,110,"Use # for exploding",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,130,"Use ^ for toggling between shading",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,150,"Use - for toggling between shadowing",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,170,"Use . and  , to translate along normals",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,190,"Use < anti-cyclicly rotate vertices",2,0x00FFFFFF);
		clglobal->clgfx->drawfontstring(100,210,"Use 0 to exit",2,0x00FFFFFF);

		if(shadows==1)
		{
			test->display(p,CENTER + SHADOW);
			clglobal->clscreen->clstencilbuffer.copy(&clglobal->clscreen->cldoublebuffer,12);
		}

		if(mode==false) test->display(p,CENTER + AMBIENT + SHAPE + ac);
		else test->display(p,CENTER + AMBIENT + FLAT + ac);

		test->getmatrix()->unit();
	}
	//*

	//exit sequence
	//~ CLsound::exit();
	return 0;
	//***
}
 
