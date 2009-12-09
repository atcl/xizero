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
};

struct tileset
{
	uxlong  tilesize;
	uxlong  tilewidth;
	uxlong  tileheight;
	uxlong  tilecount;
	uxlong** tiledata;
	
	//sprite* getsprite(xlong i);
};

typedef tileset CLfont;

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
