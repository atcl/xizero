//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLXMLGUI
#define HH_CLXMLGUI
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstructs.hh"
#include "CLsystem.hh"
#include "CLxml.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
#include "CLprogress.hh"

/* class name:	CLxmlgui
 * 
 * description:	
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

class CLxmlgui : public virtual CLcl
{
	protected:
		CLxml data;
		
	private:
		xchar* title;
		
	public:
		CLxmlgui(const xchar* filename);
		CLxmlgui(CLfile* fileptr);
		~CLxmlgui();
		
		void draw();
		void handle();
		void show(bool s);
};

CLxmlgui::CLxmlgui(const xchar* filename)
{
	
}

CLxmlgui::CLxmlgui(CLfile fileptr)
{
	
}

CLxmlgui::~CLxmlgui()
{
	
}

void CLxmlgui::draw()
{
	
}

void CLxmlgui::handle()
{
	
}

void CLxmlgui::show()
{
	
}

#endif

