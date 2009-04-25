//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSTRUCT
#define HH_CLSTRUCT
#pragma message "Compiling " __FILE__ " ! TODO: ..."


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
	xlong dd;
	xshort dw[2];
	xchar db[4];
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
	xlong filecount;
	armember* members;
};

#endif
