//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include <iostream>
#include <cstring>

#include "CLinc.h"


int main(int argc, char** argv)
{
	std::string argfile = "../dat/test.y3d";
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
	FLTKsetup();
	//***

	xchar* gg = "manifolds rule";
	std::cout << gg << " " << CLutils::chararraylength(gg) << std::endl;

//
// 	xlong* fbcx = CLgetfile_("../dat/test.bcx");
// 	xlong **bcx;
// 	xlong lines = CLgetfilelinecount_("../dat/test.bcx");
// 	doubleword tt;
// 	//cout << "linecount: " << lines << endl;
// 	bcx = clformat->loadbcx(fbcx,lines);
// 	//cout << "array size: " << bcx[0][0] << endl;
// 	tt.dd = bcx[0][1];
// 	//cout << "array0[1]: " << tt.db[0] << tt.db[1] <<tt.db[2] <<tt.db[3] << endl;
// 	tt.dd = bcx[1][1];
// 	//cout << "array1[1]: " << tt.db[0] << tt.db[1] <<tt.db[2] <<tt.db[3] << endl;
// 	tt.dd = bcx[0][3];
// 	//cout << "array0[3]: " << tt.db[0] << tt.db[1] <<tt.db[2] <<tt.db[3] << endl;
// 	//cout << "array1[3]: " << bcx[1][3] << endl;
// 


	//test section:
		CLdoublebuffer->clear(0);
	//

		//test bcx loading:
			CLfile* fbcx = CLsystem::CLgetfile_("../dat/test.bcx");
			xlong** bcx;
			doubleword tt;
			bcx = CLformat::loadbcx(fbcx);
		//test tga loading:
			CLfile* ftga = CLsystem::CLgetfile_("../dat/7segment.tga");
			sprite* testsprite = CLformat::loadtga(ftga);

		//test tree:
			CLtree* testtree = new CLtree();

		//while(FLTKwait())
		//{
		//test gfx1:

		//test gfx2:

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
	//
		//CLwaitforkey_();
		//}
	//*

	CLfile* cube;
	
	if(argfileindex!=-1)
	{
		CLfile* arch = CLsystem::CLgetfile_(argfile.c_str());
		arfile* arar = CLformat::loadar(arch);
		cube = arar->members[argfileindex];
	}
	else
	{
		CLfile* cubef = CLsystem::CLgetfile_(argfile.c_str());
		cube = cubef;
	}

	//CLobject + CLpolygon

	CLobject* cubus = new CLobject(cube,400,300,100,0);

	clfps->init();

	FLTKsetfont(8);
	xchar* toprint = u8"hi there";

	bool mode=false;

	while(FLTKwait())
	{

		switch(FLTKgetturbokeystate())
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

			//System:
			case '0':
				xlong reval = CLsystem::CLsystem_("Xdialog --msgbox 'bye' 0 0");
				CLsystem::CLexit_(1,0,"user","exit");
			break;
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
		//clgfx1->copypixel(xlong x1,xlong y1,xlong x2,xlong y2);
		//clgfx1->putpixel(xlong x,xlong y,uxlong c,xlong m);
		//clgfx1->drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
// 		//clgfx1->drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
// 		//clgfx1->drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c);
//		clgfx1->drawcircle(200,200,50,0x0000FF00); //!seg fault!
// 		//clgfx1->drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
// 		//clgfx1->drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c);
//		clgfx1->fill(100,100,0x00000000,0x00FF0000); //!seg fault + overloaded version with no oldcolor and then get per readpixel from provided location
// 		clgfx1->fillframe(xlong x,xlong y,uxlong fc,uxlong c);

		CLgfx1::drawsprite(10,10,testsprite);

		//cubus->display(1,1,1,1,0,0);
		CLstencilbuffer->blendcopy(CLdoublebuffer->getbuffer(),4);

		if(mode==false) cubus->display(CENTER + AMBIENT + FLAT + DEBUG);
		else cubus->display(CENTER + AMBIENT + FLAT);

		linearM->unit();

		clfps->increment(); 
	}

	//exit sequence	
	return 0;
	//***
}
 
