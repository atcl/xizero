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
#include "CLwindow.hh"

/* class name:	CLcursor
 * 
 * description:	This class handles the mouse cursor
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

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
		void draw() const;
};

CLcursor::CLcursor() { onoff = 0; cursor = 0; }

void CLcursor::show() { onoff = 1; }

void CLcursor::hide() { onoff = 0; }

void CLcursor::set(sprite* s) { cursor = s; }

void CLcursor::set(const xchar* xpm[]) { cursor = clformat->loadxpm(xpm); }

void CLcursor::draw() const
{
	if(onoff==1 && cursor!=0)
	{
		clgfx1->drawsprite(clwindow->getmousex(),clwindow->getmousey(),cursor); 
		//~ clgfx1->drawfilledrectangle(clwindow->getmousex(),clwindow->getmousey(),clwindow->getmousex()+32,clwindow->getmousey()+32,0xFFFFFFFF);
	}
}

#endif
