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
		void* action;
		xchar* caption;
		bool flat;
		
	public:
		CLbutton();
		~CLbutton();

		void draw();
		void setcaption(const xchar* t);
		xchar* getcaption();
};

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

#endif

