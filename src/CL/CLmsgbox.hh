///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLMSGBOX
#define HH_CLMSGBOX
///*

///includes
#include "CLtypes.hh"
///*

///header
/* class name:	CLmsgbox
 * 
 * description:	
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLmsgbox : public virtual CLcl
{
	private:
		
	public:
		CLmsgbox();
		~CLmsgbox();
		xlong msgbox();
};
///*

///implementation
CLmsgbox::CLmsgbox() //! noncritical
{

}

CLmsgbox::~CLmsgbox() //! noncritical
{

}


///*

#endif
