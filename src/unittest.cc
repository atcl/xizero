//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include "CLinc.h"

CLar* cldata = 0;

int main(int argc, char** argv)
{
	const xchar* argfile = "dat/other/test.y3d";
	xlong argfileindex =-1;

	if(argc>2) { argfileindex = atoi(argv[2]); }
	else if(argc>1) { argfile = argv[1]; }

	//init sequence
	CLglobal* clglobal = CLsetup();
	cldata = new CLar(BASEDIR"xizero.dat");
	clglobal->clgfx->loadfonts(cldata->findbyname("fonts.a"));
	
	CLmenu* sysmenu = new CLmenu();

	//*
		//test tree:
		CLtree<xlong>* testtree = new CLtree<xlong>();	
	//*
	
	//audio test:
	CLar* clsdata = new CLar(cldata->findbyname("sounds.a"));
	clglobal->clsound->preload(clsdata);
	//clglobal->clsound->play(1,1);
	//*

	CLfile* cube;

	if(argfileindex!=-1)
	{
		CLfile* arch = clglobal->clsystem->getfile(argfile);
		CLar* arar = new CLar(arch);
		cube = arar->getmember(argfileindex);
	}
	else
	{
		CLfile* cubef = clglobal->clsystem->getfile(argfile);
		cube = cubef;
	}

	CLobject* cubus = new CLobject(cube,0);

	CLexplosion* ex = new CLexplosion(cubus);

	CLlvector p(400,300,100);

	bool mode = 1;
	bool shadows = 0;
	short ac = 0;
	bool exp = 0;
	bool mm = 0;

	clglobal->clbench->init(3,COUT_FPS);

	CLlight* testl = new CLlight(50,0x00FF0000);

	sprite* screens;
	CLfile* screenf;

	//~ for(xlong i=0; i<30; i++)
	//~ {
		//~ linearM->unit();
		//~ linearM->translate(0,(14-i)*20,0);
		//~ terrows[i]->update(linearM);
	//~ }

	while(clglobal->clwindow->run())
	{
		if(clglobal->clwindow->getmouselb()!=0) CLbutton::checkclick();
			
		switch(clglobal->clwindow->getturbo())
		{
			case 0:
			break;
	
			//Translate:
			case RIGHT:  linearM->translate(2,0,0);  cubus->update(linearM); break;
			case LEFT:   linearM->translate(-2,0,0); cubus->update(linearM); break;
			case UP:     linearM->translate(0,2,0);  cubus->update(linearM); break;
			case DOWN: 	 linearM->translate(0,-2,0); cubus->update(linearM); break;
			case PGUP:   linearM->translate(0,0,2);	 cubus->update(linearM); break;
			case PGDOWN: linearM->translate(0,0,-2); cubus->update(linearM); break;			

			//Rotate:
			case 'a':    linearM->rotate(0,5,0);     cubus->update(linearM); break;
			case 'd':    linearM->rotate(0,-5,0);    cubus->update(linearM); break;
			case 'w':    linearM->rotate(-5,0,0);    cubus->update(linearM); break;
			case 's':    linearM->rotate(5,0,0);     cubus->update(linearM); break;
			case 'q':    linearM->rotate(0,0,-5);    cubus->update(linearM); break;
			case 'e':    linearM->rotate(0,0,5);     cubus->update(linearM); break;

			//Aspectscale:
			case '7':    linearM->aspectscale(1.1);  cubus->update(linearM); break;
			case '8':    linearM->aspectscale(0.9);  cubus->update(linearM); break;

			//Scale
			case '1':    linearM->scale(1.1,1,1);    cubus->update(linearM); break;
			case '2':    linearM->scale(0.9,1,1);    cubus->update(linearM); break;
			case '3':    linearM->scale(1,1.1,1);    cubus->update(linearM); break;
			case '4':    linearM->scale(1,0.9,1);    cubus->update(linearM); break;
			case '5':    linearM->scale(1,1,1.1);    cubus->update(linearM); break;
			case '6':    linearM->scale(1,1,0.9);    cubus->update(linearM); break;

			//Control:
			//case '^':    mode = !mode; break;
			case 'o':    sysmenu->show();
			case '+':    cubus->reset(); ac = exp = 0; break;
			case '-':    shadows = !shadows; break;
			case '#':    if(exp==0) { exp=1; ex->first(1); } ex->next(); break;
			case '.':    cubus->translatealongnormals(1.1); break;
			case ',':    cubus->translatealongnormals(-1.1); break;
			case '<':    if(ac==0) ac = ANTICY; else ac = 0; break;
			case ' ':    clglobal->clsound->stop(); break;
			case 'r':    clglobal->clsound->play(2); break;
			case 'i':    
				screens = clglobal->clgfx->savescreen();
				screenf = clglobal->clformat->saveras(screens,"screen.im32");
				say(clglobal->clsystem->writefile(screenf,1));
			break;
			//System:
			case '0':    xlong rval = clglobal->clmsgbox->msgbox("hi","bye"); clglobal->clapp->exit(rval,"user : exit"); break;
		}
		
		switch(clglobal->clwindow->getinkey())
		{
			
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
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
			cubus->display(p,CENTER + SHADOW);
			clglobal->clscreen->clstencilbuffer.blendcopy(clglobal->clscreen->cldoublebuffer.getbuffer(),4);
		}

		if(mode==false) cubus->display(p,CENTER + AMBIENT + SHAPE + ac);
		else cubus->display(p,CENTER + AMBIENT + FLAT + ac);

		//clglobal->clgfx->drawsprite(10,10,testlevel);
		
		//~ for(xlong i=0; i<30; i++)
		//~ //for(xlong i=25; i<50; i++)
		//~ //for(xlong i=50; i<75; i++)
		//~ //for(xlong i=75; i<90; i++)
		//~ {
			//~ terrows[i]->display(p,AMBIENT + FLAT + ZLIGHT);
			//~ terrows[i]->display(p,SHAPE);
		//~ }

		linearM->unit();

		clglobal->clbench->inc();
	}

	//exit sequence
	//~ CLsound::exit();
	return 0;
	//***
}
 
