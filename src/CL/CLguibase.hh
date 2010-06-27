///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGUIBASE
#define HH_CLGUIBASE
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLgfx.hh"
///*

///header
/* class name:	CLguibase
 * 
 * description:	base class for all gui elements.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLguibase : public CLbase<CLguibase,0>
{
	protected:
		xlong  posx;
		xlong  posy;
		xlong  width;
		xlong  height;
		uxlong fcolor;
		uxlong bcolor;
		uxlong rcolor;
		bool   flat;
		bool   visible;
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
CLguibase::CLguibase(xlong px,xlong py,xlong w,xlong h,bool f,uxlong fc,uxlong bc,uxlong rc) 
: posx(px),posy(py),width(w),height(h),fcolor(fc),bcolor(bc),rcolor(rc),flat(f),visible(1) { } //! noncritical
///*

#endif

