//
//

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/rg.h>


namespace CLwindow
{
	AG_Window*  win;
	AG_Surface* vram;

	open();
	close();
	flush();
	
}

void CLwindow::setup()
{
	if(AG_InitCore("xizero",AG_CREATE_DATADIR)==-1) return 1;
	if(AG_InitVideo(800, 600, 32, AG_VIDEO_HWSURFACE)==-1) return 1;

	win = AG_WindowNew(0);
	vram = AG_SurfaceStdRGBA(800,600);

	//set exitfunc: void AG_AtExitFunc (void (*fn)(void))
	//AG_AtExitFunc()

	AG_WindowSetCaption(win,"XiZero");

	AG_WindowShow(win);

	AG_EventLoop(); //start here once, or call every frame?
} 

void CLwindow::close()
{
	AG_Destroy();
	AG_Quit();
} 

void CLwindow::flush()
{
	AG_WidgetBlit(win,vram,0,0);
} 

int main()
{
	long temp[800*600*4];

	CLwindow::setup();

	bool i=0;

	while(i==0)
	{
		CLwindow::flush();
	}

	return (0);
}

