//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLUTILS
#define HH_CLUTILS
//#pragma message: "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLstruct.hh"


namespace CLutils
{
	xlong seed = 22695477;
	xlong multiplicator = 1;
	xlong incrementor = 1;

	xchar* long2char(xlong l);
	xchar long2char(xlong l,xlong i);
	xlong chars2long(uxchar upper,uxchar midup,uxchar midlow, uxchar lower);
	xlong chararraylength(const xchar* c);
	void copychararray(xchar* dst,xchar* src,xlong length); //for char arrays below 256byte
	xlong comparechararrays(const xchar* t1,const xchar* t2,xlong tl=0);
	xlong getlinecount(CLfile* sf);
	bool checkextension(xchar* fn,xlong nl,const xchar* fe);
	xlong getrandom(xlong range);
	xchar* color2string(uxlong c);
	xlong endian(xlong l);
	xlong findarmember(arfile* a,const xchar* e);
};


xchar* CLutils::long2char(xlong l)
{
	xchar* ch = new xchar[4];

	ch[0] = (l & 0xFF);
	ch[1] = (l & 0xFF00) >> 8;
	ch[2] = (l & 0xFF0000) >> 16;
	ch[3] = (l & 0xFF000000) >> 24;

	return ch;
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

xlong CLutils::chars2long(uxchar upper,uxchar midup,uxchar midlow, uxchar lower)
{
	xlong l;

	l = xlong(lower);
	l += xlong(midlow) << 8;
	l += xlong(midup) << 16;
	l += xlong(upper) << 24;


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

void CLutils::copychararray(xchar* dst,xchar* src,xlong length)
{
	for(int i=0; i<length; i++)
	{
		dst[i] = src[i];
	}
}

xlong CLutils::comparechararrays(const xchar* t1,const xchar* t2,xlong tl)
{
	if(tl==0) tl = chararraylength(t1);
	
	for(int i=0; i<tl; i++)
	{
		if(t1[i]!=t2[i]) return 1;
	}

	return 0;
}

xlong CLutils::getlinecount(CLfile* sf)
{
	xlong lc = 0;	//line count

	for(int c=0; c<sf->size; c++)
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
	for(int l=0; l<nl; l++)
	{
		if(fn[l]==fe[0])
		{
			es = l;
		}
	}

	for(int m=0; m<el; m++)
	{
		if(fn[es+m]!=fe[m]) return false;
	}

	return true;
}

xlong CLutils::getrandom(xlong range)
{
	seed *= multiplicator;
	seed += incrementor;
	//rotation bitwise?
	return seed % range;
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
		
		for(int h=0; h<a->filecount; h++)
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

