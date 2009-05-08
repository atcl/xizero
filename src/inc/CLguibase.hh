//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLguibase : public virtual CLcl
{
	protected:
		xlong doublebuffer;
		xlong posx;
		xlong posy;
		xlong width;
		xlong height;
		uxlong fcolor;
		uxlong bcolor;
		uxlong rcolor;

	public:
		CLguibase();
		virtual ~CLguibase();

		virtual void draw();
		void setwidth(xlong w);
		void setheight(xlong h);
		void setx(xlong x);
		void sety(xlong y);
		void setfcolor(uxlong fc);
		void setbcolor(uxlong bc);
		xlong getwidth();
		xlong getheight();
		xlong getx();
		xlong gety();
		uxlong getfcolor();
		uxlong getbcolor();
};

CLguibase::CLguibase()
{

}

CLguibase::~CLguibase() { } //virtual!

void CLguibase::draw() { } //virtual!

void CLguibase::setwidth(xlong w)
{
	width = w;
}

void CLguibase::setheight(xlong h)
{
	height = h;
}

void CLguibase::setx(xlong x)
{
	posx = x;
}

void CLguibase::sety(xlong y)
{
	posy = y;
}

void CLguibase::setfcolor(uxlong fc)
{
	fcolor = fc;
}

void CLguibase::setbcolor(uxlong bc)
{
	bcolor = bc;
}

xlong CLguibase::getwidth()
{
	return width;
}

xlong CLguibase::getheight()
{
	return height;
}

xlong CLguibase::getx()
{
	return posx;
}

xlong CLguibase::gety()
{
	return posy;
}

uxlong CLguibase::getfcolor()
{
	return fcolor;
}

uxlong CLguibase::getbcolor()
{
	return bcolor;
}

#endif

