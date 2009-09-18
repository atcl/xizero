//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include "CLinc.h"

int main(int argc, char** argv)
{
	std::string argfile = "../dat/other/test.y3d";
	xlong  argfileindex =-1;

	if(argc>2) { argfileindex = atoi(argv[2]); }
	else if(argc>1) { argfile = argv[1]; }

	//init sequence
	CLglobal* clglobal = CLsetup();
	//*

	//test section:
		clglobal->cldoublebuffer.clear(0);
	//

		//test tga loading:
			
		CLfile* fonttga = clglobal->clsystem->getfile("../dat/other/CLteletype.fnt");
		CLfont* font = clglobal->clformat->loadfont(fonttga);

		//test tree:
		CLtree<xlong>* testtree = new CLtree<xlong>();

		//test button:
			//CLbutton testbutton = new CLbutton();
		//test label:
			//CLlabel testlabel = new CLlabel();
		//test progress:
			//CLprogress testprogress = new CLprogress();
		// //test msgbox:
			//CLmsgbox testmsgbox = new CLmsgbox();
		// //test menu:
			//CLmenu testmenu = new CLmenu();
			
	//*
	
	//audio test:
	clglobal->clsound->play("../../toxic.wav",1);
	//*

	CLfile* cube;
	
	if(argfileindex!=-1)
	{
		CLfile* arch = clglobal->clsystem->getfile(argfile.c_str());
		arfile* arar = clglobal->clformat->loadar(arch);
		cube = arar->members[argfileindex];
	}
	else
	{
		CLfile* cubef = clglobal->clsystem->getfile(argfile.c_str());
		cube = cubef;
	}

	CLobject* cubus = new CLobject(cube,0);

	CLexplosion* ex = new CLexplosion(cubus);

	CLlvector p(400,300,100);

	bool mode = 1;
	bool shadows = 0;
	short ac = 0;
	bool exp = 0;

	while(clglobal->clwindow->run())
	{
		switch(clglobal->clwindow->getturbo())
		{
			case 0:
			break;

			//Translate:
			case FL_Right:
				linearM->translate(2,0,0);
				cubus->update(linearM);
			break;
			case FL_Left:
				linearM->translate(-2,0,0);
				cubus->update(linearM);
			break;
			case FL_Up:
				linearM->translate(0,2,0);
				cubus->update(linearM);
			break;
			case FL_Down:
				linearM->translate(0,-2,0);
				cubus->update(linearM);
			break;
			case FL_Page_Up:
				linearM->translate(0,0,2);
				cubus->update(linearM);
			break;
			case FL_Page_Down:
				linearM->translate(0,0,-2);
				cubus->update(linearM);
			break;

			//Rotate:
			case 'a':
				linearM->rotate(0,5,0);
				cubus->update(linearM);
			break;
			case 'd':
				linearM->rotate(0,-5,0);
				cubus->update(linearM);
			break;
			case 'w':
				linearM->rotate(-5,0,0);
				cubus->update(linearM);
			break;
			case 's':
				linearM->rotate(5,0,0);
				cubus->update(linearM);
			break;
			case 'q':
				linearM->rotate(0,0,-5);
				cubus->update(linearM);
			break;
			case 'e':
				linearM->rotate(0,0,5);
				cubus->update(linearM);
			break;

			//Aspectscale:
			case '7':
				linearM->aspectscale(1.1);
				cubus->update(linearM);
			break;
			case '8':
				linearM->aspectscale(0.9);
				cubus->update(linearM);
			break;

			//Scale
			case '1':
				linearM->scale(1.1,1,1);
				cubus->update(linearM);
			break;
			case '2':
				linearM->scale(0.9,1,1);
				cubus->update(linearM);
			break;
			case '3':
				linearM->scale(1,1.1,1);
				cubus->update(linearM);
			break;
			case '4':
				linearM->scale(1,0.9,1);
				cubus->update(linearM);
			break;
			case '5':
				linearM->scale(1,1,1.1);
				cubus->update(linearM);
			break;
			case '6':
				linearM->scale(1,1,0.9);
				cubus->update(linearM);
			break;
			case 'y':
				linearM->aspectscale(1.1);
				cubus->update(linearM);
			break;
			case 'x':
				linearM->aspectscale(0.9);
				cubus->update(linearM);
			break;
			case '^':
				mode = !mode;
			break;
			case '+':
				cubus->reset();
				ac = 0;
				exp = 0;				
			break;
			case '-':
				shadows = !shadows;
			break;
			case '#':
				if(exp==0)
				{
					exp=1;
					ex->first(1);
				}
				ex->next();
			break;
			case '.':
				cubus->translatealongnormals(1.1);
			break;
			case ',':
				cubus->translatealongnormals(-1.1);
			break;
			case '<':
				if(ac==0) ac = ANTICY;
				else ac = 0;
			break;
			case ' ':
				clglobal->clsound->stop();
			break;
			case 'r':
				clglobal->clsound->play("../../cannon.wav",0);
			break;
			case 'k':
				clglobal->clintro->atcrosslevel();
			break;
			
			//System:
			case '0':
				xlong rval = clglobal->clsystem->msgbox("bye");
				clglobal->clsystem->exit(rval,0,"user","exit");
			break;
		}
		
		switch(clglobal->clwindow->getinkey())
		{
			
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		clglobal->cldoublebuffer.fastclear(0);
		clglobal->clzbuffer.fastclear(zres-1);
		clglobal->clstencilbuffer.fastclear(0);

		//clgfx1->drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
		//clgfx1->putpixel(xlong x,xlong y,uxlong c,xlong m);
		//clgfx1->drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
		//clgfx1->drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		//clgfx1->drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c);
		
		clglobal->clgfx2->drawfontstring(100,10,"Use w,s,a,d,q,e for rotation",font,0x00FFFFFF,0x00FF0000);
		clglobal->clgfx2->drawfontstring(100,30,"Use 1,2,3,4,5,6 for scaling",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,50,"Use x,y for aspect-scaling",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,70,"Use arrow keys and scroll-up/down for translating",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,90,"Use + for reseting",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,110,"Use # for exploding",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,130,"Use ^ for toggling between shading",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,150,"Use - for toggling between shadowing",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,170,"Use . and  , to translate along normals",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,190,"Use < anti-cyclicly rotate vertices",font,0x00FFFFFF);
		clglobal->clgfx2->drawfontstring(100,210,"Use 0 to exit",font,0x00FFFFFF);

		if(shadows==1)
		{
			cubus->display(p,CENTER + SHADOW);
			clglobal->clstencilbuffer.blendcopy(clglobal->cldoublebuffer.getbuffer(),4);
		}

		if(mode==false) cubus->display(p,CENTER + AMBIENT + FLAT + DEBUG + ac);
		else cubus->display(p,CENTER + AMBIENT + FLAT + ac);

		linearM->unit();

		clglobal->clbench->inc();
	}

	//exit sequence
	//~ CLsound::exit();
	return 0;
	//***
}
 
