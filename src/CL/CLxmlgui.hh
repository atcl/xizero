///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLXMLGUI
#define HH_CLXMLGUI
///*

///includes
#include "CLtypes.hh"
#include "CLstructs.hh"
#include "CLbase.hh"
#include "CLsystem.hh"
#include "CLxml.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
#include "CLprogress.hh"
///*

///header
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
///*

///definition
class CLxmlgui : public CLbase<CLxmlgui,0>
{
	protected:
		CLxml data;
		xchar* title;
	public:
		CLxmlgui(const xchar* filename);
		CLxmlgui(CLfile* fileptr);
		~CLxmlgui();
		
		void draw();
		void handle();
		void show(bool s);
};
///*

///implementation
CLxmlgui::CLxmlgui(const xchar* filename) //! noncritical
{
	
}

CLxmlgui::CLxmlgui(CLfile fileptr) //! noncritical
{
	
}

CLxmlgui::~CLxmlgui() //! noncritical
{
	
}

void CLxmlgui::draw() //! noncritical
{
	
}

void CLxmlgui::handle() //! noncritical
{
	
}

void CLxmlgui::show() //! noncritical
{
	
}
///*

#endif

