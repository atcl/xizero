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
		CLapp();
		~CLapp();
		void exit();
		xlong waitforkey();
		xchar* getname() const;
		xchar* gettitle() const;
		xchar* geticon() const;
		uxlong getsize() const;
		uxlong getversion() const;
};
///*

///implementation
CLapp::CLapp()
{

}

CLapp::~CLapp()
{

}

void CLapp::exit()
{
	
}

xlong CLapp::waitforkey()
{
	
}

xchar* CLapp::getname() const //! noncritical
{
	
}

xchar* CLapp::gettitle() const //! noncritical
{
	
}

xchar* CLapp::geticon() const //! noncritical
{
	
}

uxlong CLapp::getsize() const //! noncritical
{
	
}

uxlong CLapp::getversion() const //! noncritical
{
	
}
///*

#endif
