//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include "CLinc.h"

CLglobal* clglobal;

inline xlong extractz(sprite* h,xlong y,xlong x) { return (-1) * ((xlong(((xchar*)(&h->data[(y*h->width)+x]))[2]))>>2); }
inline xlong extracte(sprite* h,xlong y,xlong x) { return (xlong(((xchar*)(&h->data[(y*h->width)+x]))[1])); }
inline xlong extracts(sprite* h,xlong y,xlong x) { return (xlong(((xchar*)(&h->data[(y*h->width)+x]))[0])); }
inline xlong extractb(sprite* h,xlong y,xlong x) { return (xlong(((xchar*)(&h->data[(y*h->width)+x]))[3])); }

CLobject** loadheightmap(sprite* h)
{
	xlong width = h->width;
	xlong height = h->height;
	uxlong* data = h->data;
	
	rawpoly* grid = new rawpoly[4*width];
	xlong curr = 0;
	
	CLobject** r = new CLobject*[height];
	
	xlong hordiff[4];
	xlong verdiff[4];
	bool  unbalanced = 0;
	
	for(xlong i=0; i<height-1; i++)
	{
	
		//generate stripe horizontally
		for(xlong j=0; j<width-1; j++)
		{			
			//setup poly
			grid[curr].v[0].x = (j*20)-400;
			grid[curr].v[0].y = 10;
			grid[curr].v[0].z = extractz(h,i,j);
			grid[curr].v[1].x = ((j+1)*20)-400;
			grid[curr].v[1].y = 10;
			grid[curr].v[1].z = extractz(h,i,j+1);
			grid[curr].v[2].x = ((j+1)*20)-400;
			grid[curr].v[2].y = -10;
			grid[curr].v[2].z = extractz(h,i+1,j+1);
			grid[curr].v[3].x = (j*20)-400;
			grid[curr].v[3].y = -10;
			grid[curr].v[3].z = extractz(h,i+1,j);
			//*
			
			//split by balance
			if( ( (grid[curr].v[0].z==grid[curr].v[1].z) && (grid[curr].v[0].z==grid[curr].v[3].z) && (grid[curr].v[0].z!=grid[curr].v[2].z) )
			||  ( (grid[curr].v[2].z==grid[curr].v[1].z) && (grid[curr].v[2].z==grid[curr].v[3].z) && (grid[curr].v[0].z!=grid[curr].v[2].z) ) )
			{
				curr++;
				grid[curr].v[0].x = grid[curr-1].v[1].x;
				grid[curr].v[0].y = grid[curr-1].v[1].y;
				grid[curr].v[0].z = grid[curr-1].v[1].z;
				grid[curr].v[1].x = grid[curr-1].v[2].x;
				grid[curr].v[1].y = grid[curr-1].v[2].y;
				grid[curr].v[1].z = grid[curr-1].v[2].z;
				grid[curr].v[2].x = grid[curr-1].v[3].x;
				grid[curr].v[2].y = grid[curr-1].v[3].y;
				grid[curr].v[2].z = grid[curr-1].v[3].z;
				grid[curr].v[3].x = grid[curr-1].v[3].x;
				grid[curr].v[3].y = grid[curr-1].v[3].y;
				grid[curr].v[3].z = grid[curr-1].v[3].z;
				
				//grid[curr-1].v[0].x = grid[curr-1].v[0].x;
				//grid[curr-1].v[0].y = grid[curr-1].v[0].y;
				//grid[curr-1].v[0].z = grid[curr-1].v[0].z;
				//grid[curr-1].v[1].x = grid[curr-1].v[1].x;
				//grid[curr-1].v[1].y = grid[curr-1].v[1].y;
				//grid[curr-1].v[1].z = grid[curr-1].v[1].z;
				grid[curr-1].v[2].x = grid[curr-1].v[3].x;
				grid[curr-1].v[2].y = grid[curr-1].v[3].y;
				grid[curr-1].v[2].z = grid[curr-1].v[3].z;
				//grid[curr-1].v[3].x = grid[curr-1].v[3].x;
				//grid[curr-1].v[3].y = grid[curr-1].v[3].y;
				//grid[curr-1].v[3].z = grid[curr-1].v[3].z;
				
				unbalanced = 1;
			}
			else if( ( (grid[curr].v[1].z==grid[curr].v[0].z) && (grid[curr].v[1].z==grid[curr].v[2].z) && (grid[curr].v[1].z!=grid[curr].v[3].z) )
			     ||  ( (grid[curr].v[3].z==grid[curr].v[0].z) && (grid[curr].v[3].z==grid[curr].v[2].z) && (grid[curr].v[3].z!=grid[curr].v[1].z) ) )
			{
				curr++;
				grid[curr].v[0].x = grid[curr-1].v[0].x;
				grid[curr].v[0].y = grid[curr-1].v[0].y;
				grid[curr].v[0].z = grid[curr-1].v[0].z;
				grid[curr].v[1].x = grid[curr-1].v[1].x;
				grid[curr].v[1].y = grid[curr-1].v[1].y;
				grid[curr].v[1].z = grid[curr-1].v[1].z;
				grid[curr].v[2].x = grid[curr-1].v[2].x;
				grid[curr].v[2].y = grid[curr-1].v[2].y;
				grid[curr].v[2].z = grid[curr-1].v[2].z;
				grid[curr].v[3].x = grid[curr-1].v[2].x;
				grid[curr].v[3].y = grid[curr-1].v[2].y;
				grid[curr].v[3].z = grid[curr-1].v[2].z;
				
				//grid[curr-1].v[0].x = grid[curr-1].v[0].x;
				//grid[curr-1].v[0].y = grid[curr-1].v[0].y;
				//grid[curr-1].v[0].z = grid[curr-1].v[0].z;
				grid[curr-1].v[1].x = grid[curr-1].v[2].x;
				grid[curr-1].v[1].y = grid[curr-1].v[2].y;
				grid[curr-1].v[1].z = grid[curr-1].v[2].z;
				grid[curr-1].v[2].x = grid[curr-1].v[3].x;
				grid[curr-1].v[2].y = grid[curr-1].v[3].y;
				grid[curr-1].v[2].z = grid[curr-1].v[3].z;
				//grid[curr-1].v[3].x = grid[curr-1].v[3].x;
				//grid[curr-1].v[3].y = grid[curr-1].v[3].y;
				//grid[curr-1].v[3].z = grid[curr-1].v[3].z;
				
				unbalanced = 1;
			}
			//*
			
			//unify horizontally
			else if(curr>0 && unbalanced==0)
			{
				hordiff[0] = grid[curr-1].v[1].z - grid[curr-1].v[0].z;
				hordiff[1] = grid[curr-1].v[2].z - grid[curr-1].v[3].z;
				hordiff[2] = grid[curr].v[1].z   - grid[curr].v[0].z;
				hordiff[3] = grid[curr].v[2].z   - grid[curr].v[3].z;
				
				verdiff[0] = grid[curr-1].v[3].z - grid[curr-1].v[0].z;
				verdiff[1] = grid[curr-1].v[2].z - grid[curr-1].v[1].z;
				verdiff[2] = grid[curr].v[3].z   - grid[curr].v[0].z;
				verdiff[3] = grid[curr].v[2].z   - grid[curr].v[1].z;
				
				if( hordiff[0]==hordiff[2] && hordiff[1]==hordiff[3]
				&&  verdiff[0]==verdiff[2] && verdiff[1]==verdiff[3] )
				{
					grid[curr-1].v[1].x = grid[curr].v[1].x;
					grid[curr-1].v[2].x = grid[curr].v[2].x;
					grid[curr-1].v[1].z = grid[curr].v[1].z;
					grid[curr-1].v[2].z = grid[curr].v[2].z;
					curr--;
				}
			}
			//*
			
			else { unbalanced = 0; }
			
			curr++;
		}
		//*
		
		//generate object
		r[i] = new CLobject(grid,curr,0xFFFFFFFF,0x000000FF);
		//*
		
		//reset for next stripe
		curr = 0;
		//* 			
	}
	
	delete[] grid;
	return r;
}

void displaymap(CLobject** l)
{
	
}

int main(int argc, char** argv)
{
	//init API
	clglobal = CLglobal::instance(); 
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
	
	//init system menu
	CLmenu* sysmenu = new CLmenu();
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
	
	//test heightmap
	CLfile* testim24 = clglobal->clsystem->getfile("dat/maps/level000.im24");
	sprite* testlevel = clglobal->clformat->loadras(testim24);
	CLobject** hlev = loadheightmap(testlevel);
	//*
	
	//main loop variables
	CLexplosion* ex = new CLexplosion(test);
	bool mode = 1;
	bool shadows = 0;
	short ac = 0;
	bool exp = 0;
	bool mm = 0;
	CLlvector p(400,300,100);
	sprite* screens;
	CLfile* screenf;
	//*
	
	for(xlong i=10; i<40; i++)
	{
		hlev[i]->getmatrix()->translate(0,((i-10)-15)*-20,0);
		hlev[i]->update();
	}
	
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
			case 'o':    sysmenu->show();
			case '+':    test->reset(); ac = exp = 0; break;
			case '-':    shadows = !shadows; break;
			case '#':    if(exp==0) { exp=1; ex->first(1); } ex->next(); break;
			case '.':    test->translatealongnormals(1.1); break;
			case ',':    test->translatealongnormals(-1.1); break;
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

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		clglobal->clscreen->cldoublebuffer.clear(0x00FF0000);
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

		//if(mode==false) test->display(p,CENTER + AMBIENT + SHAPE + ac);
		//else test->display(p,CENTER + AMBIENT + FLAT + ac);
		
		for(xlong i=10; i<40; i++)
		{
			hlev[i]->display(p,AMBIENT + FLAT + ZLIGHT);
			hlev[i]->display(p,SHAPE);
		}

		test->getmatrix()->unit();
	}
	//*

	//exit sequence
	//~ CLsound::exit();
	return 0;
	//***
}
 
