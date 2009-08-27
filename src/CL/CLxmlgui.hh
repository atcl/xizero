//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLXMLGUI
#define HH_CLXMLGUI
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstructs.hh"
#include "CLsystem.hh"
#include "CLxml.hh"
#include "CLlabel.hh"
#include "CLbutton.hh"
#include "CLprogress.hh"


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

#endif

