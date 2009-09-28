//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLUTILS
#define HH_CLUTILS
#pragma message: "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLcl.hh"
#include "CLsingle.hh"

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

#define SEED 22695477

class CLutils : public virtual CLcl, public CLsingle<CLutils>
{
	friend class CLsingle<CLutils>;
	
	private:
		xlong seed;
		CLutils();
		~CLutils() { };
	public:
		void   long2char(xlong l,uxchar& r0,uxchar& r1,uxchar& r2,uxchar& r3) const;
		xchar  long2char(xlong l,xlong i) const;
		xlong  chars2long(uxchar i1,uxchar i2,uxchar i3,uxchar i4) const;
		xlong  chararraylength(const xchar* c) const;
		void   copychararray(xchar* dst,const xchar* src,xlong length) const; //for char arrays below 256byte
		xchar* clonechararray(const xchar* src) const;
		xlong  getlinecount(CLfile* sf) const;
		bool   checkextension(xchar* fn,xlong nl,const xchar* fe) const;
		xlong  getrandom(uxlong range);
		xchar* color2string(uxlong c) const;
		xlong  endian(xlong l) const;
		xlong  hatoi(uxchar c) const;
};

CLutils::CLutils() : CLsingle<CLutils>() { seed = SEED; }

void CLutils::long2char(xlong l,uxchar& r0,uxchar& r1,uxchar& r2,uxchar& r3) const
{
	r0 = (l & 0xFF);
	r1 = (l & 0xFF00) >> 8;
	r2 = (l & 0xFF0000) >> 16;
	r3 = (l & 0xFF000000) >> 24;
}

xchar CLutils::long2char(xlong l,xlong i) const
{
	xchar ch = -1;

	switch(i)
	{
		case 0:
			ch = (l & 0xFF);
			break;
		case 1:
			ch = (l & 0xFF00) >> 8;
			break;
		case 2:
			ch = (l & 0xFF0000) >> 16;
			break;
		case 3:
			ch = (l & 0xFF000000) >> 24;
			break;
	}
	return ch;
}

xlong CLutils::chars2long(uxchar i0,uxchar i1,uxchar i2,uxchar i3) const
{
	xlong l;

	l = xlong(i0);
	l += xlong(i1) << 8;
	l += xlong(i2) << 16;
	l += xlong(i3) << 24;

	return l;
}

xlong CLutils::chararraylength(const xchar* c) const
{
	xlong s = 0;
	while (c[s]) { s++; }
	return s;
}

void CLutils::copychararray(xchar* dst,const xchar* src,xlong length) const
{
	for(uxlong i=0; i<length; i++) { dst[i] = src[i]; }
}

xchar* CLutils::clonechararray(const xchar* src) const
{
	xlong l = 0;
	while (src[l]) { l++; }
	xchar* r = new xchar[l+1];
	for(uxlong i=0; i<l; i++) { r[i] = src[i]; }
	r[l] = 0;
	return r;
}

xlong CLutils::getlinecount(CLfile* sf) const
{
	xlong lc = 0;	//line count

	for(uxlong c=0; c<sf->size; c++)
	{
		if( sf->text[c] == '\n' ) lc++;
	}
	//lc holds now the line count of sf

	return lc;
}

bool CLutils::checkextension(xchar* fn,xlong nl,const xchar* fe) const
{
	xlong el = chararraylength(fe);
	xlong es = 0;
	for(uxlong l=0; l<nl; l++) { if(fn[l]==fe[0]) { es = l; } }
	for(uxlong m=0; m<el; m++) { if(fn[es+m]!=fe[m]) return false; }
	return true;
}

xlong CLutils::getrandom(uxlong range)
{
	//xor-shift random number generator
	seed++;
	seed ^= (seed<<15);
	seed ^= (seed>>21);
	seed ^= (seed<<4);
	return seed % range;
	//*
}

xchar* CLutils::color2string(uxlong c) const
{
	doubleword tc;
	tc.dd = c;
	xchar* rc = new xchar[10];

	const xchar* tmp = "0123456789ABCDEF";

	rc[0] = '0';
	rc[9] = 'h';

	rc[1] = tmp[tc.db[0] / 16];
	rc[2] = tmp[tc.db[0] % 16];
	rc[3] = tmp[tc.db[1] / 16];
	rc[4] = tmp[tc.db[1] % 16];
	rc[5] = tmp[tc.db[2] / 16];
	rc[6] = tmp[tc.db[2] % 16];
	rc[7] = tmp[tc.db[3] / 16];
	rc[8] = tmp[tc.db[3] % 16];

	return rc;
}

xlong CLutils::endian(xlong l) const
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

xlong CLutils::hatoi(uxchar c) const
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

#endif

