//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include "CLinc.h"

CLar* cldata = 0;

int main(int argc, char** argv)
{
	std::string argfile = "dat/other/test.y3d";
	xlong  argfileindex =-1;

	if(argc>2) { argfileindex = atoi(argv[2]); }
	else if(argc>1) { argfile = argv[1]; }

	//init sequence
	CLglobal* clglobal = CLsetup();
	cldata = new CLar(BASEDIR"xizero.dat");
	clglobal->clgfx2->loadfonts(cldata->findbyname("fonts.a"));
	//*

		//test tree:
		CLtree<xlong>* testtree = new CLtree<xlong>();

		// //test menu:
			//CLmenu testmenu = new CLmenu();
			
	//*
	
	//audio test:
	clglobal->clsound->play("../../toxic.wav",1);
	//*
	
	//load height
	CLfile* testim24 = clglobal->clsystem->getfile("dat/maps/test.im24");
	sprite* testlevel = clglobal->clformat->loadras(testim24);
	
	xlong rows = testlevel->height;
	xlong cols = testlevel->width;
	uxlong* map = testlevel->data;
	doubleword currz = { 0 };
	doubleword lastz = { 0 };
	xlong vertcount = 0;
	xlong polycount = 0;
	rawpoly* polys = new rawpoly[cols*2];
	xchar polyz = 0;
	
	CLobject** terrows = new CLobject*[(rows-1)/2];
	
	xlong jj = 0;
	xlong kk = 0;
	xlong j = 0;
	xlong k = 0;
	xlong l = 0;
	
	xlong x0 = 0;
	xlong x1 = 0;
	xlong x2 = 0;
	xlong x3 = 0;
	xlong o0 = 0;
	xlong o1 = 0;
	xlong o2 = 0;
	xlong o3 = 0;
	
	//check each row
	for(xlong i=0; i<20; i++)
	{
		while(o1<cols && o2<cols)
		{
			//set first vertex
			x0 = o1;
			currz.dd = map[i*cols+x0];
			polyz = currz.db[2];
			polys[polycount].v[0].x = (x0*20)-400;
			polys[polycount].v[0].y = 10;
			polys[polycount].v[0].z = 100-(polyz/4);
			//*
			
			//find second vertex
			for(x1=x0; x1<cols; x1++)
			{
				currz.dd = map[i*cols+x1];
				if(currz.db[2]!=polyz) break;
			}
			//*
			
			//set second vertex
			polys[polycount].v[1].x = (x1*20)-400;
			polys[polycount].v[1].y = 10;
			polys[polycount].v[1].z = 100-(polyz/4);
			//*
			
			//find third vertex
			currz.dd = map[((i+1)*cols)+x0];
			if(currz.db[2]==polyz)
			{
				for(x3=x0; x3>o2; x3--)
				{
					currz.dd = map[((i+1)*cols)+x3];
					if(currz.db[2]!=polyz) break;
				}
			}
			else
			{
				for(x3=x0; x3<cols; x3++)
				{
					currz.dd = map[((i+1)*cols)+x3];
					if(currz.db[2]==polyz) break;
				}
			}
			//*
			
			//set third vertex
			polys[polycount].v[3].x = (x3*20)-400;
			polys[polycount].v[3].y = -10;
			polys[polycount].v[3].z = 100-(polyz/4);
			//*
			
			//find fourth vertex
			for(x2=x3; x2<cols-1; x2++)
			{
				currz.dd = map[((i+1)*cols)+x2];
				if(currz.db[2]!=polyz) break;
			}
			//*
			
			//set fourth vertex
			polys[polycount].v[2].x = (x2*20)-400;
			polys[polycount].v[2].y = -10;
			polys[polycount].v[2].z = 100-(polyz/4);
			//*
			
			//prepare for net polygon
			polycount++;
			o0 = x0;
			o1 = x1;
			o2 = x2;
			o3 = x3;
			//*
		}
		terrows[i] = new CLobject(polys,polycount,0x00FF0000,0);
		polycount = x0 = x1 = x2 = x3 = o0 = o1 = o2 = o3 = 0;
	}
	//*

	CLfile* cube;

	if(argfileindex!=-1)
	{
		CLfile* arch = clglobal->clsystem->getfile(argfile.c_str());
		CLar* arar = new CLar(arch);
		cube = arar->getmember(argfileindex);
	}
	else
	{
		CLfile* cubef = clglobal->clsystem->getfile(argfile.c_str());
		cube = cubef;
	}

	CLobject* cubus = new CLobject(cube,0);

	CLexplosion* ex = new CLexplosion(cubus);

	CLlvector p(400,300,100);
	CLlvector q(400,20,0);

	bool mode = 1;
	bool shadows = 0;
	short ac = 0;
	bool exp = 0;
	bool mm = 0;

	clglobal->clbench->init(3,COUT_FPS);

	CLlight* testl = new CLlight(50,0x00FF0000);

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
			case '^':    mode = !mode; break;
			case '+':    cubus->reset(); ac = exp = 0; break;
			case '-':    shadows = !shadows; break;
			case '#':    if(exp==0) { exp=1; ex->first(1); } ex->next(); break;
			case '.':    cubus->translatealongnormals(1.1); break;
			case ',':    cubus->translatealongnormals(-1.1); break;
			case '<':    if(ac==0) ac = ANTICY; else ac = 0; break;
			case ' ':    clglobal->clsound->stop(); break;
			case 'r':    clglobal->clsound->play("../../cannon.wav",0); break;
				
			//System:
			case '0':    xlong rval = clglobal->clwindow->msgbox("hi","bye"); clglobal->clsystem->exit(rval,0,"user","exit"); break;
		}
		
		switch(clglobal->clwindow->getinkey())
		{
			
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		clglobal->cldoublebuffer.clear(0);
		clglobal->clzbuffer.clear(ZRES);
		clglobal->clstencilbuffer.clear(0);

		//clgfx1->drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);

		/*
		clglobal->clgfx2->drawfontstring(100,10,"Use w,s,a,d,q,e for rotation",2,0x00FFFFFF,0x00FF0000);
		clglobal->clgfx2->drawfontstring(100,30,"Use 1,2,3,4,5,6 for scaling",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,50,"Use 7,8 for aspect-scaling",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,70,"Use arrow keys and scroll-up/down for translating",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,90,"Use + for reseting",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,110,"Use # for exploding",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,130,"Use ^ for toggling between shading",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,150,"Use - for toggling between shadowing",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,170,"Use . and  , to translate along normals",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,190,"Use < anti-cyclicly rotate vertices",2,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,210,"Use 0 to exit",2,0x00FFFFFF);

		if(shadows==1)
		{
			cubus->display(p,CENTER + SHADOW);
			clglobal->clstencilbuffer.blendcopy(clglobal->cldoublebuffer.getbuffer(),4);
		}

		if(mode==false) cubus->display(p,CENTER + AMBIENT + SHAPE + ac);
		else cubus->display(p,CENTER + AMBIENT + FLAT + ac);

		clglobal->clgfx1->drawsprite(10,10,testlevel);*/
		
		q.y = 20;
		for(xlong i=0; i<20; i++)
		{
			terrows[i]->display(q,AMBIENT + SHAPE);
			q.y += 20;
		}
	
		linearM->unit();

		clglobal->clbench->inc();
	}

	//exit sequence
	//~ CLsound::exit();
	return 0;
	//***
}
 
