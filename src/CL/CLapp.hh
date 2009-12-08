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
		CLapp(xlong argc,xchar** argv,const xchar* t,const xchar* i,uxlong v);
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
CLapp::CLapp(xlong argc,xchar** argv,const xchar* t,const xchar* i,uxlong v) //! noncritical
{
	name = argv[0];
	//title = t;
	//icon = i;
	version = v;
}

CLapp::~CLapp() //! noncritical
{

}

void CLapp::exit() //! noncritical
{
	
}

xlong CLapp::waitforkey() //! noncritical
{
	
}

xchar* CLapp::getname() const { return name; } //! noncritical

xchar* CLapp::gettitle() const { return title; } //! noncritical

xchar* CLapp::geticon() const { return icon; } //! noncritical

uxlong CLapp::getsize() const { return size; } //! noncritical

uxlong CLapp::getversion() const { return version; } //! noncritical
///*

#endif
