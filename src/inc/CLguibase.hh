//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
#pragma message "Compiling " __FILE__ " ! TODO: all"

class CLguibase
{
	private:
		static xlong version;

		xlong doublebuffer;
		xlong posx;
		xlong posy;
		xlong width;
		xlong height;
		uxlong fcolor;
		uxlong bcolor;
		void drawrectangle();

	public:
		CLguibase();
		~CLguibase();

		void draw(); //virtual!
		void setwidth(xlong w);
		void setheight(xlong h);
		void setx(xlong x);
		void sety(xlong y);
		xlong getwidth();
		xlong getheight();
		xlong getx();
		xlong gety();

		xlong getversion();
};

xlong CLguibase::version = 0x00010000;

CLguibase::CLguibase()
{

}

CLguibase::~CLguibase() { }

// void CLguibase::draw()
// {
// 
// }

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

xlong CLguibase::getversion()
{
	return version;
}


#endif
