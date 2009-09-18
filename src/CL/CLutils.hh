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
		void   long2char(xlong l,uxchar& r0,uxchar& r1,uxchar& r2,uxchar& r3);
		xchar  long2char(xlong l,xlong i);
		xlong  chars2long(uxchar i1,uxchar i2,uxchar i3,uxchar i4);
		xlong  chararraylength(const xchar* c);
		void   copychararray(xchar* dst,const xchar* src,xlong length); //for char arrays below 256byte
		xlong  getlinecount(CLfile* sf);
		bool   checkextension(xchar* fn,xlong nl,const xchar* fe);
		xlong  getrandom(uxlong range);
		xchar* color2string(uxlong c);
		xlong  endian(xlong l);
		xlong  findarmember(arfile* a,const xchar* e);
};

CLutils::CLutils() : CLsingle<CLutils>() { seed = SEED; }

void CLutils::long2char(xlong l,uxchar& r0,uxchar& r1,uxchar& r2,uxchar& r3)
{
	r0 = (l & 0xFF);
	r1 = (l & 0xFF00) >> 8;
	r2 = (l & 0xFF0000) >> 16;
	r3 = (l & 0xFF000000) >> 24;
}

xchar CLutils::long2char(xlong l,xlong i)
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

xlong CLutils::chars2long(uxchar i0,uxchar i1,uxchar i2,uxchar i3)
{
	xlong l;

	l = xlong(i0);
	l += xlong(i1) << 8;
	l += xlong(i2) << 16;
	l += xlong(i3) << 24;

	return l;
}

xlong CLutils::chararraylength(const xchar* c)
{
	xlong s = 0;
	while (c[s])
	{
		s++;
	}
	return s;
}

void CLutils::copychararray(xchar* dst,const xchar* src,xlong length)
{
	for(uxlong i=0; i<length; i++)
	{
		dst[i] = src[i];
	}
}

xlong CLutils::getlinecount(CLfile* sf)
{
	xlong lc = 0;	//line count

	for(uxlong c=0; c<sf->size; c++)
	{
		if( sf->text[c] == '\n' ) lc++;
	}
	//lc holds now the line count of sf

	return lc;
}

bool CLutils::checkextension(xchar* fn,xlong nl,const xchar* fe)
{
	xlong el = chararraylength(fe);
	xlong es = 0;
	for(uxlong l=0; l<nl; l++)
	{
		if(fn[l]==fe[0])
		{
			es = l;
		}
	}

	for(uxlong m=0; m<el; m++)
	{
		if(fn[es+m]!=fe[m]) return false;
	}

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

xchar* CLutils::color2string(uxlong c)
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

xlong CLutils::endian(xlong l)
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

xlong CLutils::findarmember(arfile* a,const xchar* e)
{
		xlong r = -1;
		
		for(uxlong h=0; h<a->filecount; h++)
		{
			if(CLutils::checkextension(a->members[h]->name,16,e)==true)
			{
				r=h;
				break;
			}
		}
		
		return r;
}

#endif

