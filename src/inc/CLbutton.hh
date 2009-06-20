//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUTTON
#define HH_CLBUTTON
#pragma message "Compiling " __FILE__ " ! TODO: constructor,action"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLutils.hh"
#include "CLguibase.hh"

class CLbutton : public CLguibase
{
	private:
		void (*action)();
		xchar* caption;
		bool flat;
		
	public:
		CLbutton(void(*a)(),xchar *c);
		~CLbutton();

		void draw();
		void setaction(void(*a)());
		void setcaption(xchar* t);
		void setcaption(const xchar* t);
		xchar* getcaption();

};

CLbutton::CLbutton(void(*a)(),xchar *c)
{
	action = a;
	caption = c;
}

CLbutton::~CLbutton()
{
	delete[] caption;
}

void CLbutton::draw()
{
	if(flat==0)
	{
		//drawflatrectangle(posx,posy,posx+width,posy+height,bcolor,rcolor);
	}
	else
	{
		//drawflatrectangle(posx,posy,posx+width,posy+height,bcolor,rcolor);
	}
	//drawtext
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
	xlong s = CLutils::chararraylength(t);
	caption = new xchar[s];
	for(int i=0; i<s ;i++)
	{
		caption[i] = t[i];
	}
}

xchar* CLbutton::getcaption()
{
	return caption;
}

#endif

