///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"
///*

///header
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
///*

///definitions
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
		void setvisible(bool v);
		xlong getwidth() const;
		xlong getheight() const;
		xlong getx() const;
		xlong gety() const;
		uxlong getfcolor() const;
		uxlong getbcolor() const;
		uxlong getrcolor() const;
};
///*

///implementation
CLguibase::CLguibase(xlong px,xlong py,xlong w,xlong h,bool f,uxlong fc,uxlong bc,uxlong rc) //! noncritical
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

void CLguibase::basereset(xlong px,xlong py,xlong w,xlong h,bool f,uxlong fc,uxlong bc,uxlong rc) //! noncritical
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

void CLguibase::draw() { } //virtual!  //! noncritical

void CLguibase::setwidth(xlong w) { width = w; } //! noncritical

void CLguibase::setheight(xlong h) { height = h; } //! noncritical

void CLguibase::setx(xlong x) { posx = x; } //! noncritical

void CLguibase::sety(xlong y) { posy = y; } //! noncritical

void CLguibase::setfcolor(uxlong fc) { fcolor = fc; } //! noncritical

void CLguibase::setbcolor(uxlong bc) { bcolor = bc; } //! noncritical

void CLguibase::setrcolor(uxlong rc) { rcolor = rc; } //! noncritical

void CLguibase::setvisible(bool v) { visible = v; } //! noncritical

xlong CLguibase::getwidth() const { return width; } //! noncritical

xlong CLguibase::getheight() const { return height; } //! noncritical

xlong CLguibase::getx() const { return posx; } //! noncritical

xlong CLguibase::gety() const { return posy; } //! noncritical

uxlong CLguibase::getfcolor() const { return fcolor; } //! noncritical

uxlong CLguibase::getbcolor() const { return bcolor; } //! noncritical

uxlong CLguibase::getrcolor() const { return rcolor; } //! noncritical
///*

#endif

