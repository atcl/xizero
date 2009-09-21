//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLMAINMENU
#define HH_CLMAINMENU
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLgfx1.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"

/* class name:	CLmainmenu
 * 
 * description:	This class creates a typical game main menu
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
 
class CLmainmenu : public virtual CLcl
{
	private:
		sprite* background;
		CLbutton** button;
		xlong buttoncount;
		CLlabel* header;
		CLlabel* footer;
		CLmainmenu(CLfile* bg, xlong bc, const xchar** bt, const xchar* hl, const xchar* fl);
		~CLmainmenu();
	public:
		draw();
		update();
};

CLmainmenu::CLmainmenu(CLfile* bg, xlong bc, const xchar** bt, const xchar* hl, const xchar* fl)
{
	background = clformat->loadtga(bg);
	
	button = new CLbutton*[bc];
	for(uxlong i=0; i<bc; i++)
	{
		//button[i] = new CLbutton( ... );
	}
	
	//header = new CLlabel( ... );
	//footer = new CLlabel( ... );
}
		
CLmainmenu::~CLmainmenu()
{
	delete background;
	for(uxlong i=0; i<bc; i++) { delete button[i]; }
	delete header;
	delete footer;
}

CLmainmenu::draw()
{
	
}

CLmainmenu::update()
{
	
}
 
 #endif
