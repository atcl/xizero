//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUTTON
#define HH_CLBUTTON
#pragma message "Compiling " __FILE__ " ! TODO: constructor,action"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"


class CLbutton : public CLguibase , public virtual CLcl
{
	private:
		void (*action)();
		xchar* caption;
		xlong x;
		xlong y;
		xlong width;
		xlong height;
		uxlong fcolor;
		uxlong bcolor;
		bool flat;
		
	public:
		CLbutton(void(*a)(),xchar *c,xlong xx, xlong yy, xlong w, xlong h,bool f,uxlong fc,uxlong bc);
		~CLbutton();

		void draw();
		void setaction(void(*a)());
		void setcaption(xchar* t);
		void setcaption(const xchar* t);
		void setx(xlong xx);
		void sety(xlong yy);
		void setwidth(xlong w);
		void setheight(xlong h);
		void setfcolor(uxlong fc);
		void setbcolor(uxlong bc);
		xchar* getcaption();
		xlong getx();
		xlong gety();
		xlong getwidth();
		xlong getheight();
		uxlong getfcolor();
		uxlong getbcolor();
};

CLbutton::CLbutton(void(*a)(),xchar *c,xlong xx, xlong yy, xlong w, xlong h, bool f,uxlong fc,uxlong bc)
{
	action = a;
	caption = c;

	x = xx;
	y = yy;
	width = w;
	height = h;
	flat = f;
	fcolor = fc;
	bcolor = bc;
}

CLbutton::~CLbutton() { }

void CLbutton::draw()
{

}

void CLbutton::setaction(void(*a)())
{
	action = a;
}

void CLbutton::setcaption(xchar* t)
{
	caption = t;
}

void CLbutton::setcaption(const xchar* t)
{
	delete caption;
	xlong s = getchararraysize_(t);
	caption = new xchar[s];
	for(int i=0; i<s ;i++)
	{
		caption[i] = t[i];
	}
}

void CLbutton::setx(xlong xx)
{
	x = xx;
}

void CLbutton::sety(xlong yy)
{
	y = yy;
}

void CLbutton::setwidth(xlong w)
{
	width = w;
}

void CLbutton::setheight(xlong h)
{
	height = h;
}

void CLbutton::setfcolor(uxlong fc)
{
	fcolor = fc;
}

void CLbutton::setbcolor(uxlong bc)
{
	bcolor = bc;
}

xchar* CLbutton::getcaption()
{
	return caption;
}

xlong CLbutton::getx()
{
	return x;
}

xlong CLbutton::gety()
{
	return y;
}

xlong CLbutton::getwidth()
{
	return width;
}

xlong CLbutton::getheight()
{
	return height;
}

uxlong CLbutton::getfcolor()
{
	return fcolor;
}

uxlong CLbutton::getbcolor()
{
	return bcolor;
}

#endif

