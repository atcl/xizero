///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAPP
#define HH_CLAPP
///*

///includes
#include <stdlib.h>

#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLresource.hh"
#include "CLutils.hh"
///*

///header
/* class name:	CLapp
 * 
 * description:	This class manages the application itself.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLapp : public virtual CLcl, public CLsingle<CLapp>
{
	private:
		xchar* name;
		xchar* title;
		xchar* icon;
		uxlong size;
		uxlong version;
		
	public:
		CLapp();
		~CLapp() { };
		void exit(xlong r=0,const xchar* m="");
		xchar inkey(bool b);
		xchar* getname() const { return name; };
		xchar* gettitle() const { return title; };
		xchar* geticon() const { return icon; };
		uxlong getsize() const { return size; };
		uxlong getversion() const { return version; };
};
///*

///implementation
CLapp::CLapp() //! noncritical
{
	//name = argv[0];
	//size = argv[-1];
	title = TITLE;
	//icon = ICON;
	version = uxchar(MAJOR<<24)+uxchar(MINOR<<16)+uxchar(BUILD<<8)+uxchar(EXTRA);
}

void CLapp::exit(xlong r,const xchar* m) //! noncritical
{
	say(m);
	::exit(r);
}

xchar inkey(bool b) //! noncritical
{
	xchar r = 0;
	xlong s = 0;
	
	if(b)
	{
		while(true)
		{
			//usleep(20000);
			s = read(0,&r,1);
			if(s>0) { break; }
		}
	}
	else
	{
		s = read(0,&r,1);
		if(s<=0) { r=0; }
	}
	
	return r;
}
///*

#endif
