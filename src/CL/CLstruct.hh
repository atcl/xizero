///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSTRUCT
#define HH_CLSTRUCT
///*

///includes
#include "CLtypes.hh"
///*

///definitions
union doubleword
{
	uxlong  dd;
	uxshort dw[2];
	uxchar  db[4];
	
	inline operator uxlong() const { return this->dd; }
	
	//byteadd
	//bytesub
};

union quadword //:doubleword 
{
	uxhyper dq;
	uxlong  dd[2];
	uxshort dw[4];
	uxchar  db[8];
};

union CLcolor
{
	uxlong rgba;
	uxchar r;
	uxchar b;
	uxchar g;
	uxchar a;
};

struct CLfile
{
	xchar* name;
	xchar* text;
	xlong* data;
	xlong  size;
	xlong  lsize;
	
	//~CLfile() { delete text; delete name; }
};

struct screenside
{
	xlong offset;
	float z;
};

struct sprite
{
	uxlong  size;
	uxlong  width;
	uxlong  height;
	uxlong* data;
	uxlong  index; //used for tilesets ONLY!
	uxlong  count; //used for tilesets ONLY!
};

struct CLgamepadstate
{
	xlong axis[2];
	bool  button[10];
	bool  tbutton[10];
};

struct CLexe
{
	xchar* name;
	xchar* title;
	xlong  size;
	xlong  version;
};
///*

#endif
