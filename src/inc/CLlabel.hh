//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLLABEL
#define HH_CLLABEL
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLguibase.hh"


class CLlabel : public CLguibase
{
	private:
		static xlong version;
		xchar* text;

	public:
		CLlabel();
		~CLlabel();

		void draw();
		void settext(const xchar* t);
		xchar* gettext();

		xlong getversion();
};

xlong CLlabel::version = 0x00010000;

CLlabel::CLlabel()
{

}

CLlabel::~CLlabel() { }

void CLlabel::draw()
{

}

void CLlabel::settext(const xchar* t)
{

}

xchar* CLlabel::gettext()
{
	return text;
}

xlong CLlabel::getversion()
{
	return version;
}

#endif
