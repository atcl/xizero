//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSTRUCT
#define HH_CLSTRUCT
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"


struct vertex
{
	xlong x;
	xlong y;
	xlong z;
};

struct fvertex
{
	float x;
	float y;
	float z;
};

union doubleword
{
	xlong  dd;
	xshort dw[2];
	xchar  db[4];
};

struct screenside
{
	xlong offset;
	float z;
};

struct sprite
{
	xlong size;
	xlong width;
	xlong height;

	xlong* data;
};

struct sprites
{
	xlong size;
	xlong width;
	xlong height;

	xlong perrow;
	xlong tilesize;
	xlong tilewidth;
	xlong tileheight;

	xlong* data;
};

struct listmember
{
	void* data;
	listmember* next;
	listmember* prev;
};

struct node
{
	node* parent;
	node* next;
	node* prev;
	node** child;
	xlong childcount;
	void* data;
};

struct armember
{
	xlong filesizedata;
	xlong filesizetext;
	xchar *filename;
	xlong *filedata;
	xchar *filetext;
};

struct arfile
{
	xlong     filecount;
	armember* members;
};

struct xmlelement
{
	xchar* value;
	xchar** attributes;
};

struct exe
{
	xchar*  name;
	xchar*  title;
	xlong   size;
	xlong   version;
	sprite* splash;
};

#endif
