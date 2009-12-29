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
	clglobal->clgfx2->loadfonts(cldata->findbyname("fonts.a"));
	
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
	xlong opolycount = 0;
	xlong oldpolycount = 0;
	xlong ooldpolycount = 0;
	rawpoly* polys = new rawpoly[cols*3];
	rawpoly* opolys = new rawpoly[cols*3];
	
	CLobject** terrows = new CLobject*[rows-1];
	
	xlong jj = 0;
	xlong kk = 0;
	xlong j = 0;
	xlong k = 0;
	xlong l = 0;
	
	xlong x0 = 0;
	xlong x1 = 0;
	xlong x2 = 0;
	xlong x3 = 0;
	xlong xz = 0;
	xlong o0 = 0;
	xlong o1 = 0;
	xlong o2 = 0;
	xlong o3 = 0;
	xlong oz = 0;
	
	bool resetx2 = 0;
	bool resetx3 = 0;
	bool resetx22 = 0;
	xlong y1 = 0;
	xlong y2 = 0;

	//check each row
	for(xlong i=0; i<rows-1; i++)
	{
		while(o1<cols && o2<cols)
		{
			//set first vertex
			x0 = o1;
			currz.dd = map[i*cols+x0];
			xz = currz.db[2];
			polys[polycount].v[0].x = (x0*20)-400;
			polys[polycount].v[0].y = 10;
			polys[polycount].v[0].z = -(xz/4);
			//*
			
			//find second vertex
			if(x0<cols)
			{
				for(x1=x0; x1<cols; x1++) //->cols-1 
				{
					currz.dd = map[i*cols+x1];
					if(currz.db[2]!=xz) break;
				}
				
				//inter gap
				for(y1=x1; y1<cols; y1++)
				{
					currz.dd = map[(i*cols)+y1];
					if(currz.db[2]==xz)
					{
						resetx22 = 1;
						break;
					}
				}
				//*
			}
			else
			{
				x1 = x0;
			}
			//*
			
			//set second vertex
			polys[polycount].v[1].x = (x1*20)-400;
			polys[polycount].v[1].y = 10;
			polys[polycount].v[1].z = -(xz/4);
			//*
			
			//find third vertex
			currz.dd = map[((i+1)*cols)+x0];
			if(currz.db[2]==xz)
			{
				for(x3=x0; x3>o2; x3--)
				{
					currz.dd = map[((i+1)*cols)+x3];
					if(currz.db[2]!=xz) break;
				}
			}
			else
			{
				for(x3=x0; x3<cols; x3++)
				{
					currz.dd = map[((i+1)*cols)+x3];
					if(currz.db[2]==xz) break;
				}
				if(x3>x1)
				{
					x3 = x0;
					currz.dd = map[((i+1)*cols)+x3];
					xz = currz.db[2];
					polys[polycount].v[0].z = -(xz/4);
					polys[polycount].v[1].z = -(xz/4);
					resetx2 = 1;
				}
			}
			//*

			//set third vertex
			polys[polycount].v[3].x = (x3*20)-400;
			polys[polycount].v[3].y = -10;
			polys[polycount].v[3].z = -(xz/4);
			//*
			
			//find fourth vertex
			if(resetx2==0)
			{
				for(x2=x3; x2<cols; x2++)
				{
					currz.dd = map[((i+1)*cols)+x2];
					if(currz.db[2]!=xz) break;
				}
				
				//inter gap
				if(resetx3!=0)
				{
					x3 = x0;
					polys[polycount].v[3].x = (x3*20)-400;
					resetx3 = 0;
				}
				
				for(y2=x2; y2<cols; y2++)
				{
					currz.dd = map[((i+1)*cols)+y2];
					if(currz.db[2]==xz && y2<x1)
					{
						x1 = x2;
						polys[polycount].v[1].x = (x1*20)-400;
						resetx3 = 1;
						break;
					}
				}
				
				if(resetx22==1 && y1<x2)
				{
					x2 = x1;
					resetx22 = 0;
				}
				//*
			}
			else
			{
				x2 = x1;
			}
			//*
			
			//set fourth vertex
			polys[polycount].v[2].x = (x2*20)-400;
			polys[polycount].v[2].y = -10;
			polys[polycount].v[2].z = -(xz/4);
			//*
			
			//fix start and end of rows
			if(x0==0)
			{
				polys[polycount].v[3].x = -400;
			}
			//~ else if(x1==cols-1)
			//~ {
				//~ polys[polycount].v[2].x = (x1*20)-400;
			//~ }
			//~ else if(x2==cols-1)
			//~ {
				//~ polys[polycount].v[1].x = (x2*20)-400;
			//~ }
			//*
			
			//merge if mergeable with previous polygon and prepare for next polygon
			if(oz==xz && o1==x0 && o2==x3)
			{
				polys[polycount-1].v[1].x = polys[polycount].v[1].x;
				polys[polycount-1].v[2].x = polys[polycount].v[2].x;
				o1 = x1;
				o2 = x2;
			}
			else
			{
				polycount++;
				o0 = x0;
				o1 = x1;
				o2 = x2;
				o3 = x3;
				oz = xz;
			}
				
			resetx2 = 0;
			//*
		}
		
		//insert horizontal connecting polygons
		oldpolycount = polycount;
		for(xlong j=1; j<oldpolycount; j++)
		{
			polys[polycount].v[0].x = polys[j-1].v[1].x;
			polys[polycount].v[1].x = polys[j].v[0].x;
			polys[polycount].v[2].x = polys[j].v[3].x;
			polys[polycount].v[3].x = polys[j-1].v[2].x;
			
			polys[polycount].v[0].y = 10;
			polys[polycount].v[1].y = 10;
			polys[polycount].v[2].y = -10;
			polys[polycount].v[3].y = -10;
			
			polys[polycount].v[0].z = polys[j-1].v[0].z;
			polys[polycount].v[1].z = polys[j].v[0].z;
			polys[polycount].v[2].z = polys[j].v[0].z;
			polys[polycount].v[3].z = polys[j-1].v[0].z;
			
			polycount++;
		}
		//*
		
		//insert lower vertical connecting polygons
		if(i>0 && i<(rows-1))
		{
			xlong cp0 = 0;
			xlong cp1 = ooldpolycount - 1;
			xlong cp = 0;
			
			for(xlong k=0; k<ooldpolycount; k++)
			{
				//~ while(cp0 < ooldpolycount) { if(polys[cp0].v[0].x <= opolys[k].v[3].x && polys[cp0+1].v[0].x > opolys[k].v[3].x) { break; } cp0++; }
				//~ while(cp1 >= 0) { if(polys[cp1].v[1].x >= opolys[k].v[2].x && polys[cp1-1].v[1].x < opolys[k].v[3].x) { break; } cp1--; }
				//~ 
				//~ if(opolys[k].v[0].z != polys[cp0].v[0].z || opolys[k].v[0].z != polys[cp1].v[0].z)
				//~ {
					//~ cp = cp0;
					//~ if( polys[cp1].v[0].z < polys[cp0].v[0].z) cp = cp1;
					//~ 
					//~ polys[polycount].v[0].x = opolys[k].v[3].x;
					//~ polys[polycount].v[1].x = opolys[k].v[2].x;
					//~ polys[polycount].v[2].x = opolys[k].v[2].x;
					//~ polys[polycount].v[3].x = opolys[k].v[3].x;
					//~ 
					//~ polys[polycount].v[0].y = 10;
					//~ polys[polycount].v[1].y = 10;
					//~ polys[polycount].v[2].y = 10;
					//~ polys[polycount].v[3].y = 10;
					//~ 
					//~ polys[polycount].v[0].z = opolys[k].v[0].z;
					//~ polys[polycount].v[1].z = opolys[k].v[0].z;
					//~ polys[polycount].v[2].z = polys[cp].v[0].z;
					//~ polys[polycount].v[3].z = polys[cp].v[0].z;
					//~ 
					//~ polycount++;
				//~ }
				//~ 
				//~ cp0 = 0;
				//~ cp1 = ooldpolycount - 1;
			//~ }
			
			//~ for(xlong k=0; k<oldpolycount; k++)
			//~ {
				//~ while(cp0 < oldpolycount) { if(polys[cp0].v[0].x <= opolys[k].v[3].x && polys[cp0+1].v[0].x > opolys[k].v[3].x) { break; } cp0++; }
				//~ while(cp1 >= 0) { if(polys[cp1].v[1].x >= opolys[k].v[2].x && polys[cp1-1].v[1].x < opolys[k].v[3].x) { break; } cp1--; }
				//~ 
				//~ if(opolys[k].v[0].z != polys[cp0].v[0].z || opolys[k].v[0].z != polys[cp1].v[0].z)
				//~ {
					//~ cp = cp0;
					//~ if( polys[cp1].v[0].z < polys[cp0].v[0].z) cp = cp1;
					//~ 
					//~ polys[polycount].v[0].x = polys[k].v[0].x;
					//~ polys[polycount].v[1].x = polys[k].v[1].x;
					//~ polys[polycount].v[2].x = polys[k].v[1].x;
					//~ polys[polycount].v[3].x = polys[k].v[0].x;
					//~ 
					//~ polys[polycount].v[0].y = 10;
					//~ polys[polycount].v[1].y = 10;
					//~ polys[polycount].v[2].y = 10;
					//~ polys[polycount].v[3].y = 10;
					//~ 
					//~ polys[polycount].v[0].z = polys[k].v[0].z;
					//~ polys[polycount].v[1].z = ppolys[k].v[0].z;
					//~ polys[polycount].v[2].z = opolys[cp].v[0].z;
					//~ polys[polycount].v[3].z = opolys[cp].v[0].z;
			//~ 
					//~ polycount++;
				//~ }
				//~ 
				//~ cp0 = 0;
				//~ cp1 = oldpolycount - 1;
			}
		}
		//*
		
		terrows[i] = new CLobject(polys,polycount,0x000000FF,0);
		for(xlong l=0; l<polycount; l++) { opolys[l] = polys[l]; }
		opolycount = polycount;
		ooldpolycount = oldpolycount;
		polycount = x0 = x1 = x2 = x3 = xz = o0 = o1 = o2 = o3 = oz = y1 = y2 = 0;
	}
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
	CLlvector q(400,0,100);

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

		clglobal->cldoublebuffer.clear(0);
		clglobal->clzbuffer.clear(ZRES);
		clglobal->clstencilbuffer.clear(0);

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
		for(xlong i=0; i<25; i++)
		//for(xlong i=25; i<50; i++)
		//for(xlong i=50; i<75; i++)
		//for(xlong i=75; i<90; i++)
		{
			terrows[i]->display(q,AMBIENT + FLAT + ZLIGHT);
			terrows[i]->display(q,SHAPE);
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
 
