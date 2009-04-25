//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license  
#ifndef HH_CLGFX2
#define HH_CLGFX2
#pragma message "Compiling " __FILE__ " ! TODO: all"


class CLgfx2
{
	private:
		static xlong version;
		xlong* doublebuffer;
		
	public:
		CLgfx2(xlong* db);
		~CLgfx2();
		void drawfontchar(xlong x,xlong y,xchar a,xlong* f,uxlong c);
		void drawfontchar(xlong p,xchar a,xlong* f,uxlong c);
		void drawfontstring(xlong x,xlong y,xchar* a,xlong* f,uxlong c);
		void drawfontstring(xlong p,xchar* a,xlong* f,uxlong c);
		xlong locatemaxx();
		xlong locatemaxy();
		uxlong locate(xlong x,xlong y);

		xlong getversion();
};

xlong CLgfx2::version = 0x00010000;

CLgfx2::CLgfx2(xlong* db)
{
	doublebuffer = db;
}

CLgfx2::~CLgfx2() { }



xlong CLgfx2::getversion()
{
	return version;
}

#endif
