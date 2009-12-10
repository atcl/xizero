///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAPP
#define HH_CLAPP
///*

///includes
#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLresource.hh"
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
class CLapp : public virtual CLcl
{
	private:
		xchar* name;
		xchar* title;
		xchar* icon;
		uxlong size;
		uxlong version;
		
	public:
		CLapp(xlong argc,xchar** argv);
		~CLapp();
		void exit();
		xchar inkey(bool b);
		xchar* getname() const;
		xchar* gettitle() const;
		xchar* geticon() const;
		uxlong getsize() const;
		uxlong getversion() const;
};
///*

///implementation
CLapp::CLapp(xlong argc,xchar** argv) //! noncritical
{
	name = argv[0];
	title = TITLE;
	//icon = &CLicon;
	version = (CLmajor<<24)+(CLminor<<16)+(CLbuild<<8)+CLextra;
}

CLapp::~CLapp() //! noncritical
{

}

void CLapp::exit() //! noncritical
{
	
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

xchar* CLapp::getname() const { return name; } //! noncritical

xchar* CLapp::gettitle() const { return title; } //! noncritical

xchar* CLapp::geticon() const { return icon; } //! noncritical

uxlong CLapp::getsize() const { return size; } //! noncritical

uxlong CLapp::getversion() const { return version; } //! noncritical
///*

#endif
