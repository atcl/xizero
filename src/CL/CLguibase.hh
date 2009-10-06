//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"

/* class name:	CLguibase
 * 
 * description:	base class for all gui elements.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

class CLguibase : public virtual CLcl
{
	protected:
		xlong posx;
		xlong posy;
		xlong width;
		xlong height;
		uxlong fcolor;
		uxlong bcolor;
		uxlong rcolor;
		bool flat;
		bool visible;
		void basereset(xlong px,xlong py,xlong w,xlong h,bool f=0,uxlong fc=CLguifront,uxlong bc=CLguiback,uxlong rc=CLguiframe);
	public:
		CLguibase(xlong px,xlong py,xlong w,xlong h,bool f=0,uxlong fc=CLguifront,uxlong bc=CLguiback,uxlong rc=CLguiframe);
		virtual ~CLguibase() { };
		virtual void draw();
		inline void setwidth(xlong w);
		inline void setheight(xlong h);
		inline void setx(xlong x);
		inline void sety(xlong y);
		inline void setfcolor(uxlong fc);
		inline void setbcolor(uxlong bc);
		inline void setrcolor(uxlong rc);
		inline void setvisible(bool v);
		inline xlong getwidth() const;
		inline xlong getheight() const;
		inline xlong getx() const;
		inline xlong gety() const;
		inline uxlong getfcolor() const;
		inline uxlong getbcolor() const;
		inline uxlong getrcolor() const;
};

CLguibase::CLguibase(xlong px,xlong py,xlong w,xlong h,bool f,uxlong fc,uxlong bc,uxlong rc)
{
	//set up attributes
	posx = px;
	posy = py;
	width = w;
	height = h;
	fcolor = fc;
	bcolor = bc;
	rcolor = rc;
	flat = f;
	visible = 1;
	//*
}

void CLguibase::basereset(xlong px,xlong py,xlong w,xlong h,bool f,uxlong fc,uxlong bc,uxlong rc)
{
	posx = px;
	posy = py;
	width = w;
	height = h;
	fcolor = fc;
	bcolor = bc;
	rcolor = rc;
	flat = f;
}

void CLguibase::draw() { } //virtual!

void CLguibase::setwidth(xlong w) { width = w; }

void CLguibase::setheight(xlong h) { height = h; }

void CLguibase::setx(xlong x) { posx = x; }

void CLguibase::sety(xlong y) { posy = y; }

void CLguibase::setfcolor(uxlong fc) { fcolor = fc; }

void CLguibase::setbcolor(uxlong bc) { bcolor = bc; }

void CLguibase::setrcolor(uxlong rc) { rcolor = rc; }

void CLguibase::setvisible(bool v) { visible = v; }

xlong CLguibase::getwidth() const { return width; }

xlong CLguibase::getheight() const { return height; }

xlong CLguibase::getx() const { return posx; }

xlong CLguibase::gety() const { return posy; }

uxlong CLguibase::getfcolor() const { return fcolor; }

uxlong CLguibase::getbcolor() const { return bcolor; }

uxlong CLguibase::getrcolor() const { return rcolor; }

#endif

