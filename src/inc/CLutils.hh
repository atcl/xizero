//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLUTILS
#define HH_CLUTILS
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"


xchar* long2char(xlong l)
{
	xchar* ch = new xchar[4];

	ch[0] = (l & 0xFF);
	ch[1] = (l & 0xFF00) >> 8;
	ch[2] = (l & 0xFF0000) >> 16;
	ch[3] = (l & 0xFF000000) >> 24;

	return ch;
}

xchar long2char(xlong l,xlong i)
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

xlong chars2long(uxchar upper,uxchar midup,uxchar midlow, uxchar lower)
{
	xlong l;

	l = xlong(lower);
	l += xlong(midlow) << 8;
	l += xlong(midup) << 16;
	l += xlong(upper) << 24;


	return l;
}

void copychararray(xchar* dst,xchar* src,xlong length)
{
	for(int i=0; i<length; i++)
	{
		dst[i] = src[i];
	}
}

bool comparechararrays(xchar* t1, xchar* t2,xlong tl)
{
	for(int i=0; i<tl; i++)
	{
		if(t1[i]!=t2[i]) return false;
	}

	return true;
}

xlong getlinecount(xlong* bf,xlong bs)
{
	xlong lc = 0;	//line count
	doubleword cc;	//current chars

	//check for linebreaks a xlong at a time
	for(int c=0; c<bs; c++)
	{
		cc.dd = bf[c];
		if( cc.db[0] == '\n' ) lc++;
		if( cc.db[1] == '\n' ) lc++;
		if( cc.db[2] == '\n' ) lc++;
		if( cc.db[3] == '\n' ) lc++;
	}
	//lc holds now the line count of bf

	return lc;
}

xlong getlinecount(xchar* bf,xlong bs)
{
	xlong lc = 0;	//line count

	for(int c=0; c<bs; c++)
	{
		if( bf[c] == '\n' && bf[c-1] != '\n' ) lc++;
	}
	//lc holds now the line count of bf

	return lc;
}

bool checkextension(xchar* fn,xlong nl,const xchar* fe,xlong el)
{
	xlong es;
	for(int l=0; l<nl; l++)
	{
		if(fn[l]=='.')
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

xlong getrandom()
{

}

#endif
