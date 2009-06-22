//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLLABEL
#define HH_CLLABEL
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"


class CLlabel : public CLguibase
{
	private:
		xchar* text;

	public:
		CLlabel();
		~CLlabel();

		void draw();
		void settext(const xchar* t);
		xchar* gettext();
};

CLlabel::CLlabel()
{

}

CLlabel::~CLlabel() { }

void CLlabel::draw()
{
	//drawflatrectangle(posx,posy,posx+width,posy+height,bcolor,rcolor);
	//drawtext
}

void CLlabel::settext(const xchar* t)
{
	text = (xchar*)t;
}

xchar* CLlabel::gettext()
{
	return text;
}

#endif

