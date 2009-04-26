//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLAMMO
#define HH_CLAMMO
#pragma message "Compiling " __FILE__ " ! TODO: update,draw"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLammo : public virtual CLcl
{
	private:
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
};

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

#endif

