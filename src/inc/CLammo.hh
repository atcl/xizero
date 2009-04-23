//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLAMMO
#define HH_CLAMMO
#pragma message "Compiling " __FILE__ " ! TODO: update,draw"



class CLammo
{
	private:
		static xlong version;
		xlong* sprite;
		xlong xcoord;
		xlong ycoord;
		xlong xspeed;
		xlong yspeed;
		xlong xdirection;
		xlong ydirection;

	public:
		CLammo();
		~CLammo();
		update();
		draw();

		xlong getversion();
};

xlong CLammo::version = 0x00010000;

CLammo::CLammo()
{

}

CLammo::~Clammo() { }

void CLammo::update()
{

} 

void CLammo::draw()
{

}

xlong CLammo::getversion()
{
	return version;
}

#endif
