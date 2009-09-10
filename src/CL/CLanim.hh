//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLANIM
#define HH_CLANIM
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLmatrix.hh"
#include "CLobject.hh"
#include "CLformat.hh"
#include "CLsystem.hh"


class CLanim : public virtual CLcl
{
	protected:
		CLmatrix linear;
		CLobject* object;
		xlong*   animcsv;
		
	private:
		bool loop;
	
	public:
		CLanim(CLobject* obj,CLfile* ani,bool l);
		~CLanim();
		update();
};

CLanim::CLanim(CLobject* obj,CLfile* ani,bool l)
{
	
}

CLanim::~CLanim()
{
	
}

CLanim::update()
{
	
}

#endif
