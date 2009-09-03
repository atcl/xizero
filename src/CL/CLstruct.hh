//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLSTRUCT
#define HH_CLSTRUCT
//#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLvector.hh"


union doubleword
{
	xlong  dd;
	xshort dw[2];
	xchar  db[4];
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

typedef CLfile armember;

struct arfile
{
	armember** members;
	xlong      filecount;
	
	~arfile() { for(int i=0; i<filecount; i++) delete members[i]; }
};

struct screenside
{
	xlong offset;
	float z;
};

struct sprite
{
	xlong  size;
	xlong  width;
	xlong  height;

	xlong* data;
};

struct sprites
{
	xlong  size;
	xlong  width;
	xlong  height;

	xlong  tilesize;
	xlong  tilewidth;
	xlong  tileheight;

	xlong  tilecount;
	xlong* data;
};

typedef sprites CLfont;

struct xmlelement
{
	xchar*  value;
	xchar** attributes;
};

struct every
{
	void(*function)();//function to execute
	xlong interval;  //in milliseconds
	xlong times;
	xlong count;	 //break condition regular (do x-times)
	bool  active;    //break condition extra (if false break)
	xlong id;        //unique id

	xlong last;      //last time done;

	//~ every(void(*f)(),xlong i,xlong t,xlong d)
	//~ {
		//~ function = f;
		//~ interval = i;
		//~ times = t;
		//~ id = d;
//~ 
		//~ last = 0;
		//~ count = 0;
		//~ active = true;
	//~ }
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
