#include <fastgl/fastgl.h>

#include "CLtypes.hh"

namespace CLwindow
{
	fgl::FGApp* 		app;
	fgl::FGWindow* 		win;
	fgl::FGDrawBuffer* 	vram;
	
	xlong key;
	xlong tkey;
	xlong mousex;
	xlong mousey;
	xlong mouseb;
	
	void init(xlong w,xlong h,const xchar* t,int argc, char *argv[]);
	void run();
	void flush(xlong* db);
	void exit();
	void update(fgl::FGEvent *e);
	
	xlong getkey();
	xlong turbokey();
	xlong getmousex();
	xlong getmousey();
	xlong getmouseb();
	
	
	
};


void CLwindow::init(xlong w,xlong h,const xchar* t,int argc, char *argv[])
{
	app = new fgl::FGApp(fgl::G800x600,argc,argv,fgl::CBLACK,fgl::APP_ENABLEALTX);
	win = new fgl::FGWindow(0, 0,0,w,h,t,CLwindow::update);
	vram = new fgl::FGDrawBuffer(w,h);	
}

void CLwindow::run()
{
	app->Run();
}

void CLwindow::flush(xlong* db)
{
	win->WindowPutBitmap(0,0,0,0,800,600,vram);
}

void CLwindow::exit()
{
	fgl::FGApp::AppDone();
}

void CLwindow::update(fgl::FGEvent *e)
{
	switch(e->GetType())
	{
		case fgl::INITEVENT:
			//on start
			break;
			
		case fgl::KEYEVENT:

			break;
			
		case fgl::MOVEEVENT:

			break;
			
		case fgl::CLICKLEFTEVENT:

			break;
			
		case fgl::CLICKRIGHTEVENT:
			
			break;
			
		case fgl::TERMINATEEVENT:
			//on exit
			break;
	}
}

