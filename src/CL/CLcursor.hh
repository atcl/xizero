//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLCURSOR
#define HH_CLCURSOR
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLcl.hh"
#include "CLsingle.hh"
#include "CLstruct.hh"
#include "CLformat.hh"
#include "CLgfx1.hh"

class CLcursor : public virtual CLcl, public CLsingle<CLcursor>
{
	friend class CLsingle<CLcursor>;
	
	private:
		bool onoff;
		sprite* cursor;
		CLcursor();
		~CLcursor() { };
	public:
		void show();
		void hide();
		void set(sprite* s);
		void set(const xchar* xpm[]);
		void draw(xlong x,xlong y) const;
};

CLcursor::CLcursor() { onoff = 0; }

void CLcursor::show() { onoff = 1; }

void CLcursor::hide() { onoff = 0; }

void CLcursor::set(sprite* s) { cursor = s; }

void CLcursor::set(const xchar* xpm[]) { cursor = clformat->loadxpm(xpm); }

void CLcursor::draw(xlong x,xlong y) const { clgfx1->drawsprite(x,y,cursor); }

#endif
