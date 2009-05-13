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

	//CLmatrix
	ltm->unit();
	bsm->shadow(amb->getlight(),plane);

	xchar* gg = "manifolds rule";
	std::cout << gg << " " << getchararraysize_(gg) << std::endl;

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
			CLfile* fbcx = CLgetfile_("../dat/test.bcx");
			xlong** bcx;
			doubleword tt;
			bcx = clformat->loadbcx(fbcx);
		//test tga loading:
			CLfile* ftga = CLgetfile_("../dat/CLteletype.fnt");
			sprite* testsprite = clformat->loadtga(ftga);

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

	xlong* cube;
	
	if(argfileindex!=-1)
	{
		CLfile* arch = CLgetfile_(argfile.c_str());
		arfile* arar = clformat->loadar(arch);
		cube = arar->members[argfileindex]->data;
	}
	else
	{
		CLfile* cubef = CLgetfile_(argfile.c_str());
		cube = cubef->data;
	}

	//CLobject + CLpolygon

	CLobject* cubus = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,cube,400,300,100,clmath,bsm,amb,0);

	clfps->init();

	FLTKsetfont(8);
	xchar* toprint = "hi there";

	while(FLTKwait())
	{

		switch(FLTKgetturbokeystate())
		{
			case 0:
			break;

			//Translate:
			case FL_Right:
				ltm->translate(2,0,0);
				cubus->update(ltm);
			break;
			case FL_Left:
				ltm->translate(-2,0,0);
				cubus->update(ltm);
			break;
			case FL_Up:
				ltm->translate(0,2,0);
				cubus->update(ltm);
			break;
			case FL_Down:
				ltm->translate(0,-2,0);
				cubus->update(ltm);
			break;
			case FL_Page_Up:
				ltm->translate(0,0,2);
				cubus->update(ltm);
			break;
			case FL_Page_Down:
				ltm->translate(0,0,-2);
				cubus->update(ltm);
			break;

			//Rotate:
			case 'a':
				ltm->rotate(0,5,0);
				cubus->update(ltm);
			break;
			case 'd':
				ltm->rotate(0,-5,0);
				cubus->update(ltm);
			break;
			case 'w':
				ltm->rotate(-5,0,0);
				cubus->update(ltm);
			break;
			case 's':
				ltm->rotate(5,0,0);
				cubus->update(ltm);
			break;
			case 'q':
				ltm->rotate(0,0,-5);
				cubus->update(ltm);
			break;
			case 'e':
				ltm->rotate(0,0,5);
				cubus->update(ltm);
			break;

			//Aspectscale:
			case '7':
				ltm->aspectscale(1.1);
				cubus->update(ltm);
			break;
			case '8':
				ltm->aspectscale(0.9);
				cubus->update(ltm);
			break;

			//Scale
			case '1':
				ltm->scale(1.1,1,1);
				cubus->update(ltm);
			break;
			case '2':
				ltm->scale(0.9,1,1);
				cubus->update(ltm);
			break;
			case '3':
				ltm->scale(1,1.1,1);
				cubus->update(ltm);
			break;
			case '4':
				ltm->scale(1,0.9,1);
				cubus->update(ltm);
			break;
			case '5':
				ltm->scale(1,1,1.1);
				cubus->update(ltm);
			break;
			case '6':
				ltm->scale(1,1,0.9);
				cubus->update(ltm);
			break;

			//System:
			case '0':
				xlong reval = CLsystem_("Xdialog --msgbox 'bye' 0 0");
				CLexit_(1,"user","exit");
			break;
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		CLdoublebuffer->fastclear(0);
		CLzbuffer->clear(zmax);
		CLstencilbuffer->fastclear(0);

		//FLTKwritestring(10,10,toprint,0x00ff0000);

		//drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
		//copypixel(xlong x1,xlong y1,xlong x2,xlong y2);
		//putpixel(xlong x,xlong y,uxlong c,xlong m);
		//drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
// 		drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
// 		//drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
// 		//drawarc(xlong xc,xlong yc,xlong r,xlong l,uxlong c);
// 		drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
// 		drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
// 		drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c);
// 		drawcircle(xlong xc,xlong yc,xlong r,uxlong c);
// 		//drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
// 		//drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c);
// 		fill(xlong x,xlong y,uxlong oc,uxlong nc);
// 		fillframe(xlong x,xlong y,uxlong fc,uxlong c);

		clgfx1->drawpixel(20,20,0x000FF0000);
		//clgfx1->drawsprite(10,10,testsprite); //segfault hier! kann aber schon beim laden passieren!

		//cubus->display(1,1,1,1,0,0);
		CLstencilbuffer->blendcopy(CLdoublebuffer->getbuffer(),4);

		cubus->display(1,1,1,0,0,1);
		ltm->unit();

		clfps->increment(); 
	}

	//exit sequence	
	return 0;
	//***
}
 
