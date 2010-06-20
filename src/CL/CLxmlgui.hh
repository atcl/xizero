///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLXMLGUI
#define HH_CLXMLGUI
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
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
 * author:		atcl
 * 
 * notes:		implement
 * 
 * version: 	0.1
 */
///*

///definition
class CLxmlgui : public CLbase<CLxmlgui,0>
{
	protected:
		CLxml data;
	public:
		CLxmlgui(CLfile* f);
		~CLxmlgui();
		void draw();
		void handle();
};
///*

///implementation
CLxmlgui::CLxmlgui(CLfile* f) : data(f) //! noncritical
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
///*

#endif

