//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPROGRESS
#define HH_CLPROGRESS
#pragma message "Compiling " __FILE__ " ! TODO: constructor"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"


class CLprogress : public CLguibase , public virtual CLcl
{
	private:
		xlong framethickness;
		uxlong barcolor;
		uxlong framecolor;
		xlong progress;
		xlong start;
		xlong end;
		
	public:
		CLprogress();
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

CLprogress::CLprogress()
{

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

