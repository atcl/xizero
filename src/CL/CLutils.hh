///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLUTILS
#define HH_CLUTILS
///*

///includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLcl.hh"
#include "CLsingle.hh"
///*

///header
/* class name:	CLutils
 * 
 * description:	A rainbow of useful routines
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLutils : public virtual CLcl, public CLsingle<CLutils>
{
	friend class CLsingle<CLutils>;
	
	private:
		CLutils() { };
		~CLutils() { };
	public:
		xlong  endian(xlong l) const;
		xlong  hatoi(uxchar c) const;
		xlong  hatoi(const xchar* c) const;
};
///*

///implementation
xlong CLutils::endian(xlong l) const //! noncritical
{
	//converts/toggles endianess of l

	xchar tc;
	doubleword tl;
	tl.dd = l;

	tc = tl.db[0];
	tl.db[0] = tl.db[3];
	tl.db[3] = tc;

	tc = tl.db[1];
	tl.db[1] = tl.db[2];
	tl.db[2] = tc;

	return tl.dd;
}

xlong CLutils::hatoi(uxchar c) const //! noncritical
{
	switch(c)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default:  return 0;
	}
}

xlong CLutils::hatoi(const xchar* c) const //! noncritical
{
	xchar currchar = 0;
	xlong charcount = 0;
	xlong currvalue = 0;
	bool  hasval = 0;
	xlong r = 0;
	while(charcount<8)
	{
		currchar = c[charcount];
		switch(currchar)
		{
			case '0': currvalue = 0; break;
			case '1': currvalue = 1; break;
			case '2': currvalue = 2; break;
			case '3': currvalue = 3; break;
			case '4': currvalue = 4; break;
			case '5': currvalue = 5; break;
			case '6': currvalue = 6; break;
			case '7': currvalue = 7; break;
			case '8': currvalue = 8; break;
			case '9': currvalue = 9; break;
			case 'A': currvalue = 10; break;
			case 'B': currvalue = 11; break;
			case 'C': currvalue = 12; break;
			case 'D': currvalue = 13; break;
			case 'E': currvalue = 14; break;
			case 'F': currvalue = 15; break;
			case 'a': currvalue = 10; break;
			case 'b': currvalue = 11; break;
			case 'c': currvalue = 12; break;
			case 'd': currvalue = 13; break;
			case 'e': currvalue = 14; break;
			case 'f': currvalue = 15; break;
			default:  currvalue = -1;
		}
		if(currvalue==-1) break;
		if(hasval==0 && currvalue!=0) hasval = 1;
		if(hasval==1)
		{
			r += currvalue;
			r <<= 4;
		}
		charcount++;
	}
	r >>= 4;
	return r;
}
///*

#endif

