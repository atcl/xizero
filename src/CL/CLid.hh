//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLID
#define HH_CLID
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"

/* class name:	CLid
 * 
 * description:	This class checks ids and crcs.
 * 
 * author:	atcl
 * 
 * notes:	test.
 * 
 * version: 0.1
 */

//test cases:
//string: atcrosslevel ; crc: 1734979481
//string: xizero ; crc: 1208686013

class CLid : public virtual CLcl, public CLsingle<CLid>
{
	friend class CLsingle<CLid>;
	
	private: 
		xlong  lastid;
		static uxlong crc32;
		CLid();
		~CLid() { };
	public:
		xlong generateid();
		uxlong generatecrc(xchar* d,xlong l);
		bool checkcrc(uxlong c);
		xlong one() const;
};

uxlong CLid::crc32 = 0x04C11DB7;

CLid::CLid() { lastid = 0; }

xlong CLid::generateid()
{
	lastid++;
	return lastid;
}

uxlong CLid::generatecrc(xchar* d,xlong l)
{
	uxlong r = 0;
	uxchar o = 0;

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

xlong CLid::one() const { return 1; }

#endif

