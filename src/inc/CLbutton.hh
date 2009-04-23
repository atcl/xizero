//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUTTON
#define HH_CLBUTTON
#pragma message "Compiling " __FILE__ " ! TODO: constructor,action"

#include "CLguibase.hh"


class CLbutton : public CLguibase
{
	private:
		static xlong version;

		void* action;
		xchar* caption;
		bool flat;
		
	public:
		CLbutton();
		~CLbutton();

		void draw();
		void setcaption(const xchar* t);
		xchar* getcaption();

		xlong getversion();
};

xlong CLbutton::version = 0x00010000;

CLbutton::CLbutton()
{

}

CLbutton::~CLbutton() { }

void CLbutton::draw()
{

}

void CLbutton::setcaption(const xchar* t)
{
	//delete caption;
	xlong s = getchararraysize_(t);
	caption = new xchar[s]; //wie bestimme ich die Größe eines char arrays?
	for(int i=0; i<s ;i++)
	{
		caption[i] = t[i];
	}
}

xchar* CLbutton::getcaption()
{
	return caption;
}

xlong CLbutton::getversion()
{
	return version;
}

#endif
