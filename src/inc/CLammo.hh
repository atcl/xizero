//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLAMMO
#define HH_CLAMMO
#pragma message "Compiling " __FILE__ " ! TODO: ctor,update,draw"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLammo : public virtual CLcl
{
	protected:
		CLbuffer<xlong>* doublebuffer;

	private:
		sprite* sprite;
		vector pos;
		vector spd;
		vector dir;
		void* comspr(CLbuffer<xlong>* db,xlong x,xlong y);

	public:
		CLammo(sprite* s,vector p,vector d);
		~CLammo();
		update();
		draw();
};

CLammo::CLammo(sprite* s,vector p,vector d)
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

