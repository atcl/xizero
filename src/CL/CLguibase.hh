//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"

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
		void setwidth(xlong w);
		void setheight(xlong h);
		void setx(xlong x);
		void sety(xlong y);
		void setfcolor(uxlong fc);
		void setbcolor(uxlong bc);
		void setrcolor(uxlong rc);
		xlong getwidth() const;
		xlong getheight() const;
		xlong getx() const;
		xlong gety() const;
		uxlong getfcolor() const;
		uxlong getbcolor() const;
		uxlong getrcolor() const;
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

xlong CLguibase::getwidth() const { return width; }

xlong CLguibase::getheight() const { return height; }

xlong CLguibase::getx() const { return posx; }

xlong CLguibase::gety() const { return posy; }

uxlong CLguibase::getfcolor() const { return fcolor; }

uxlong CLguibase::getbcolor() const { return bcolor; }

uxlong CLguibase::getrcolor() const { return rcolor; }

#endif

