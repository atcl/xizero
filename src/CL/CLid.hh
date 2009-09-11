//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLID
#define HH_CLID
#pagma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


//test cases:
//string: atcrosslevel ; crc: 1734979481
//string: xizero ; crc: 1208686013

class CLid : public virtual CLcl
{
	private: 
		xlong  lastid;
		static uxlong crc32;

	public:
		CLid();
		~CLid();

		xlong generateid();
		xlong generatecrc(xchar* d,xlong l);
		bool checkcrc(uxlong c);
		xlong one();
};

uxlong CLid::crc32 = 0x04C11DB7;

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

uxlong CLid::generatecrc(xchar* d,xlong l)
{
	uxlong r;
	uxchar o;

	if(l<4) return 0;

	r  = *d++ << 24;
	r |= *d++ << 16;
	r |= *d++ << 8;
	r |= *d++ ;
	r  = ~r;
	l -= 4;

	for(uxlong i=0; i<l; i++)
	{
		o = *(d++);

		for(uxlong j=0; j<8; j++)
		{
			if (r & 0x80000000) r = (r << 1) ^ crc32 ^ (o >> 7);

			else r = (r << 1) ^ (o >> 7);

			o <<= 1;
		}
	}

	return r;

}

bool CLid::checkcrc(uxlong c)
{
	uxlong t = c / crc32;

	if(t==0) return true;
	else return false;
}

xlong CLid::one()
{
	return 1;
}

#endif

