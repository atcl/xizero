//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLID
#define HH_CLID
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLid : public virtual CLcl
{
	private: 
		xlong lastid;

	public:
		CLid();
		~CLid();

		xlong generateid();
		xlong generatecrc();
		bool checkcrc();
		xlong one();
};

CLid::CLid()
{
	lastid = 0;
}

CLid::~Clid() { }

xlong CLid::generateid()
{
	lastid++;
	return lastid;
}

xlong CLid::generatecrc()
{

}

bool CLid::checkcrc()
{

}

xlong CLid::one()
{
	return 1;
}

#endif

