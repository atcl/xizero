///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAPP
#define HH_CLAPP
///*

///sys includes
#include <cstdlib>
///*

///api includes
#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLresource.hh"
#include "CLutils.hh"
#include "CLbase.hh"
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
class CLapp : public CLbase<CLapp,1>
{
	friend class CLbase<CLapp,1>;
	
	protected:
		const xchar* name;
		const xchar* title;
		xchar* icon;
		uxlong size;
		uxlong version;
		CLapp();
		~CLapp() { };
	public:
		void exit(xlong r=0,const xchar* m="");
		xchar inkey(bool b);
		const xchar* getname() const { return name; };
		const xchar* gettitle() const { return title; };
		xchar* geticon() const { return icon; };
		uxlong getsize() const { return size; };
		uxlong getversion() const { return version; };
};
///*

///implementation
CLapp::CLapp() : name(TITLE), title(TITLE) //! noncritical
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
