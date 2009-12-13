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
		virtual void draw() { };
		void setwidth(xlong w) { width = w; };
		void setheight(xlong h) { height = h; };
		void setx(xlong x) { posx = x; };
		void sety(xlong y) { posy = y; };
		void setfcolor(uxlong fc) { fcolor = fc; };
		void setbcolor(uxlong bc) { bcolor = bc; };
		void setrcolor(uxlong rc) { rcolor = rc; };
		void setvisible(bool v) { visible = v; };
		xlong getwidth() const { return width; };
		xlong getheight() const { return height; };
		xlong getx() const { return posx; };
		xlong gety() const { return posy; };
		uxlong getfcolor() const { return fcolor; };
		uxlong getbcolor() const { return bcolor; };
		uxlong getrcolor() const { return rcolor; };
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
///*

#endif

