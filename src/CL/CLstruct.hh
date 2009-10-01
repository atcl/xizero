//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSTRUCT
#define HH_CLSTRUCT
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"

union doubleword
{
	xlong  dd;
	xshort dw[2];
	xchar  db[4];
};

struct packedbytes
{
	uxchar hh;
	uxchar hl;
	uxchar lh;
	uxchar ll;
	
	operator uxlong() const { return ( ((uxlong(hh))<<24)+((uxlong(hl))<<16)+((uxlong(lh))<<8)+(uxlong(ll)) ); }
};

union quadbytes
{
	uxlong      ul;
	packedbytes qb;
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

struct sprites
{
	uxlong  size;
	uxlong  width;
	uxlong  height;

	uxlong  tilesize;
	uxlong  tilewidth;
	uxlong  tileheight;

	uxlong  tilecount;
	uxlong* data;
};

typedef sprites CLfont;

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
	xchar* icon;
};

#endif
