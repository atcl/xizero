//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include <iostream>
#include <cstring>

#include "CLinc.h"


int main(int argc, char** argv)
{
	std::string argfile = "../dat/other/test.y3d";
	xlong  argfileindex =-1;

	if(argc>2)
	{
		argfileindex = atoi(argv[2]);
	}
	else if(argc>1)
	{
		argfile = argv[1];
	}

	//init sequence
	CLsetup();
	//***

	//test section:
		CLdoublebuffer->clear(0);
	//
	
		//test tga loading:
			
			CLfile* fonttga = CLsystem::getfile("../dat/other/CLteletype.fnt");
			CLfont* font = CLformat::loadfont(fonttga);
			
			
			//temp
			CLfile* etest = CLsystem::getfile("../dat/enemy/e001.a");
			arfile* ebr   = CLformat::loadar(etest);
			//arfile* ebr   = CLformat::loadar(ear->members[0]);
			xlong in      = CLutils::findarmember(ebr,".ini");
			xmap* et      = CLformat::loadini(ebr->members[1]); //infinite loop!!! because ar returns too big size !?
			say();
			//CLsystem::print((*et)["health"]);
			//*

		//test tree:
			CLtree* testtree = new CLtree();

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

	CLfile* cube;
	
	if(argfileindex!=-1)
	{
		CLfile* arch = CLsystem::getfile(argfile.c_str());
		arfile* arar = CLformat::loadar(arch);
		cube = arar->members[argfileindex];
	}
	else
	{
		CLfile* cubef = CLsystem::getfile(argfile.c_str());
		cube = cubef;
	}

	CLobject* cubus = new CLobject(cube,0);

	CLexplosion* ex = new CLexplosion(cubus);

	clbench->init();

	CLlvector p(400,300,100);

	bool mode=true;

	while(win->run())
	{

		switch(win->getturbo())
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
			case '^':
				if(mode==false) mode=true;
				else mode=false;
			break;
			case '+':
				cubus->reset();
			break;
			case '-':
				//activate shadows
			break;
			case '#':
				ex->next();
			break;

			//System:
			case '0':
				xlong rval = CLsystem::system("Xdialog --msgbox 'bye' 0 0");
				CLsystem::exit(rval,0,"user","exit");
			break;
		}
		
		switch(win->getkey())
		{
			
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		CLdoublebuffer->fastclear(0);
		CLzbuffer->clear(zres-1);
		CLstencilbuffer->fastclear(0);

		//clgfx1->drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
		//clgfx1->putpixel(xlong x,xlong y,uxlong c,xlong m);
		//clgfx1->drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
		//clgfx1->drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		//clgfx1->drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c);
		//clgfx1->drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
		
		CLgfx2::drawfontstring(100,10,"Use w,s,a,d,q,e for rotation",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,30,"Use 1,2,3,4,5,6 for scaling",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,50,"Use arrow keys for translating",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,70,"Use + for reseting",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,90,"Use # for exploding",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,110,"Use ^ for toggling between shading",font,0x00FFFFFF);
		CLgfx2::drawfontstring(100,130,"Use 0 to exit",font,0x00FFFFFF);

		//cubus->display(1,1,1,1,0,0);
		//CLstencilbuffer->blendcopy(CLdoublebuffer->getbuffer(),4);

		if(mode==false) cubus->display(p,CENTER + AMBIENT + FLAT + DEBUG);
		else cubus->display(p,CENTER + AMBIENT + FLAT);

		linearM->unit();

		clbench->inc();
	}

	//exit sequence	
	return 0;
	//***
}
 
