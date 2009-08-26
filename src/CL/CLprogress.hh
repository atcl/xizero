//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPROGRESS
#define HH_CLPROGRESS
//#pragma message "Compiling " __FILE__ " ! TODO: constructor"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"
#include "CLgfx2.hh"


class CLprogress : public CLguibase
{
	private:
		uxlong barcolor;
		xlong progress;
		xlong start;
		xlong end;
		bool  flat;
		bool  horver;
		
	public:
		CLprogress(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,uxlong ba,xlong p,xlong s,xlong e,bool f);
		~CLprogress();

		void draw();
		void setstart(xlong s);
		void setend(xlong e);
		void setprogress(xlong p);
		void addprogress(xlong a);
		xlong getstart();
		xlong getend();
		xlong getprogress();
};

CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,uxlong fc,uxlong bc,uxlong rc,uxlong ba,xlong p,xlong s,xlong e,bool f) : CLguibase(px,py,w,h,fc,bc,rc)
{
	barcolor = ba;
	progress = p;
	start = s;
	end  = e;
	flat = f;
}

CLprogress::~CLprogress() { }

void CLprogress::draw()
{

}

void CLprogress::setstart(xlong s)
{
	start = s;
}

void CLprogress::setend(xlong e)
{
	end = e;
}

void CLprogress::setprogress(xlong p)
{
	progress = p;
}

void CLprogress::addprogress(xlong a)
{
	progress += a;
}

xlong CLprogress::getstart()
{
	return start;
}

xlong CLprogress::getend()
{
	return end;
}

xlong CLprogress::getprogress()
{
	return progress;
}

#endif

