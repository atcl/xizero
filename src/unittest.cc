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
	xlong polyz = 0;
	
	CLobject** terrows = new CLobject*[(rows-1)/2];
	
	//check each row
	xlong jj = 0; 
	xlong j = -1;
	xlong k = 0;
	xlong l = 0;
	for(xlong i=0; i<2; i++) //2 -> rows
	{
		while(polycount==0 || polys[polycount-1].v[1].x<400)
		{
			//set first vertex
			j++; jj = j;
			currz.dd = map[i*cols+j];
			polyz = xlong(currz.db[2]);
			if(polycount==0) polys[polycount].v[0].x = -400;
			else             polys[polycount].v[0].x = polys[polycount-1].v[1].x;				
			polys[polycount].v[0].y = 10;
			polys[polycount].v[0].z = 100-(polyz/4);
			//*
			
			//find second vertex in same row
			while(true)
			{
				currz.dd = map[(i*cols)+j];
				if(currz.db[2]!=polyz) { j--; break; }
				if(j>=cols) { j=0; break; } //? correct?
				j++;
			}
			//*
			
			//set second vertex
			polys[polycount].v[1].x = j*20 - 400;
			polys[polycount].v[1].y = 10;
			polys[polycount].v[1].z = polys[polycount].v[0].z;
			//*
			
			//find third vertex in next row
			k = jj;
			currz.dd = map[((i+1)*cols)+k];
			if(currz.db[2]!=polyz)
			{
				while(true)
				{
					currz.dd = map[((i+1)*cols)+k];
					if(currz.db[2]!=polyz) { k--; break; }
					if(k>=cols) { k=-1; break; }
					k++;
				}
			}
			else
			{
				while(true)
				{
					currz.dd = map[((i+1)*cols)+k];
					if(currz.db[2]!=polyz) { break; }
					k--;
					if(k<0) { k=0; break; }
				}
			}		
			//*

			//set third vertex
			if(k==-1)
			{
				polys[polycount].v[3].x = polys[polycount].v[0].x;
				polys[polycount].v[3].y = polys[polycount].v[0].y;
				polys[polycount].v[3].z = polys[polycount].v[0].z;
			}
			else
			{
				polys[polycount].v[3].x = k*20 -400;
				polys[polycount].v[3].y = -10;
				polys[polycount].v[3].z = polys[polycount].v[0].z;
			}
			//*
			
			//find fourth vertex
			if(k<jj) k = jj;
			l = 0;
			if(k!=-1)
			{
				while(true)
				{
					currz.dd = map[((i+1)*cols)+k+l];
					if(currz.db[2]!=polyz) { l--; break; }
					l++;
					if((k+l)>cols) { l-=2; break; }
				}
			}
			//*
			
			//set fourth vertex
			polys[polycount].v[2].x = (k+l)*20 -400;
			polys[polycount].v[2].y = -10;
			polys[polycount].v[2].z = polys[polycount].v[0].z;
			//*

			//create object
			std::cout << "poly nr: " << polycount << std::endl;
			std::cout << polys[polycount].v[0].x << ' ' << polys[polycount].v[0].y << ' ' << polys[polycount].v[0].z << std::endl;
			std::cout << polys[polycount].v[1].x << ' ' << polys[polycount].v[1].y << ' ' << polys[polycount].v[1].z << std::endl;
			std::cout << polys[polycount].v[2].x << ' ' << polys[polycount].v[2].y << ' ' << polys[polycount].v[2].z << std::endl;
			std::cout << polys[polycount].v[3].x << ' ' << polys[polycount].v[3].y << ' ' << polys[polycount].v[3].z << std::endl;
			//*
			
			polycount++;
		}
		terrows[i] = new CLobject(polys,polycount,0x00FF0000,0);
		polycount = 0;
		jj = 0;
		j = 0;
		k = 0;
		l = 0;
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
	CLlvector q(400,350,0);

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
			case 'b':    clglobal->clwindow->msgbox("hi","bye"); break;
				
			//System:
			case '0':    xlong rval = clglobal->clwindow->msgbox("hi","This is line 1!!! \n This is line 2!!!!  \n This is 3XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n and finally Line 4"); clglobal->clsystem->exit(rval,0,"user","exit"); break;
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

		clglobal->clgfx1->drawsprite(10,10,testlevel);
		q.y = 350;
		for(xlong i=0; i<2; i++)
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
 
