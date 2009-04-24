//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 

#include <iostream>
#include <cstring>

using namespace std;

#include "CLinc.h"


int main(int argc, char** argv)
{
	string argfile = "../dat/cube.y3d";
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
	cout << gg << " " << getchararraysize_(gg) << endl;

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

	xlong *cube;
	
	if(argfileindex!=-1)
	{
		xchar  *arch;
		xlong   arsz;
		arfile *arar;
		arch = CLgetcharfile_(argfile.c_str());
		arsz = CLgetfilesize_(argfile.c_str());
		arar = clformat->loadar(arch,arsz);
		cube = arar->members[argfileindex].filedata;
	}
	else
	{
		cube = CLgetfile_(argfile.c_str());
	}

	//CLobject + CLpolygon

	CLobject* cubus;
	cubus = new CLobject(CLdoublebuffer,CLzbuffer,CLstencilbuffer,cube,400,300,100,clmath,bsm,amb);

	clfps->init();

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
				CLexit_("user","exit",0);
			break;
		}

		//render sequence:
		//1. all non shadow objects (floor,terrain)
		//2. all shadows of all shadow casting objects
		//3. blend stencil to double
		//4. all shadow casting objects

		//ltm->print();
		CLdoublebuffer->clear(0);
		CLzbuffer->clear(zmax);
		CLstencilbuffer->clear(0);

		//cubus->display(1,0,1,0,0);
		//CLdoublebuffer->blendcopy(CLstencilbuffer->getbuffer(),4);
		cubus->display(1,1,1,0,0,1);
		//cubus->setposition(400,300,100);
		//cubus->display(0,1,0,0,0,0);
		//cubus->setposition(300,300,100);
		ltm->unit();

		clfps->increment(); 
	}

	//exit sequence	
	cout << "unittest says: exit" << endl;
	return 0;
	//***
}
 
