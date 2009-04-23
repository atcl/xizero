//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
#pragma message "Compiling " __FILE__ " ! TODO: all"
//self-contained


class CLmsgbox
{
	private:
		static xlong version;
		xlong* doublebuffer;
		void*  action;
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c,uxlong color2=0); //also for 3d frames around buttons
		
	public:
		CLmsgbox(xlong* db,void* a);
		~CLmsgbox();
		void draw();
		void onok();
		xlong getversion();
};

xlong CLmsgbox::version = 0x00010000;

CLmsgbox::CLmsgbox(xlong* db,void* a)
{
	doublebuffer = db;
	action = a;
}

CLmsgbox::~CLmsgbox() { }

void CLmsgbox::draw()
{

}

void CLmsgbox::onok()
{
	//action();
}

xlong CLmsgbox::getversion()
{
	return version;
}

#endif
