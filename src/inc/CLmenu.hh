//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMENU
#define HH_CLMENU
#pragma message "Compiling " __FILE__ " ! TODO: all"


class CLmenu
{
	private:
		static xlong version;
		xlong* doublebuffer;
		char* title;
		xlong* icon;
		void* actionptr;
		
	public:
		CLmenu(xlong* db,void* a);
		~CLmenu();
		void draw();
		void action();
		xlong getversion();
};

xlong CLmenu::version = 0x00010000;

CLmenu::CLmenu(xlong* db,void* a)
{
	doublebuffer = db;
	actionptr = a;
}

CLmenu::~CLmenu() { }

void CLmenu::draw()
{

}

void CLmenu::action()
{

}

xlong CLmenu::getversion()
{
	return version;
}

#endif
