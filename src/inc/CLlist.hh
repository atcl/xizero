//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"


class CLlist : public virtual CLcl
{
	private:

		void setstart();
		void setlast();
		void setprev();
		void setnext();
	public:
		CLlist();
		~CLlist();

		void append();
		void getcurrent();
		void delcurrent();
		xlong* getdata();
		xlong getlength();
		void settoindex();
		void clear();		
};

void CLlist::setstart()
{

}

void CLlist::setlast()
{

}

void CLlist setnext()
{

}

void CLlist::setprev()
{

}

CLlist::CLlist()
{

}

CLlist::~CLlist() { }

void CLlist::append()
{

}

void CLlist::getcurrent()
{

}

void CLlist::delcurrent()
{

}

xlong* CLlist::getdata()
{

}

xlong CLlist::getlength()
{

}

void CLlist::settoindex()
{

}

void CLlist::clear()
{

}

#endif

