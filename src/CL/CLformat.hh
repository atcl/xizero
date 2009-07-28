//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFORMAT
#define HH_CLFORMAT
#pragma message "Compiling " __FILE__ " ! TODO: fix rare loadar crash"

#include <stdlib.h>
#include <map> 

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLutils.hh"
#include "CLmacros.hh"

typedef std::map <xchar*,xchar*> xmap;

typedef CLfile armember;

struct arfile
{
	xlong      filecount;
	armember**  members;
};

namespace CLformat
{
	xlong*  loadcsv(CLfile* sf,xchar sep=',');
	arfile* loadar(CLfile* sf);
	xlong** loadbcx(CLfile* bf);
	xchar** loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv);
	sprite* loadtga(CLfile* sf);
	
	sprites* loadtileset(CLfile* sf,xlong tw,xlong th);
	sprites* loadfont(CLfile* sf);
	xlong**  loadlvl();
	xmap*    loadini(CLfile* bf); //!
}


//add template parameter to change between xlong* and xchar** 
xlong* CLformat::loadcsv(CLfile* sf,xchar sep)
{
	//Works only for integers!
	
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = CLutils::getlinecount(sf);
	//*

	//get comma count per line
	xlong* cc = new xlong[lc];
	xlong tfc = 0;
	xlong tcc = 0;
	xlong tlc = 0;
	while(tlc<lc)
	{
		if(bf[tfc]==sep) tcc++;
		else if(bf[tfc]==CLsystem::eol)
		{
			cc[tlc] = tcc;
			tcc = 0;
			tlc++;
		}
		tfc++;
	}
	//*

	//get value count
	xlong vc = 0;
	
	for(int i=0; i<lc; i++)
	{
		if(cc[i]!=0) vc += cc[i] + 1; 	
	}
	vc++;
	//*
	
	//copy values
	xlong* r = new xlong[vc+1];	
	xlong tvc = 1; tfc = 0;
	r[0] = vc;

	while(tvc<=vc)
	{
		r[tvc] = atoi(&bf[tfc]);
		tvc++;
		while(bf[tfc]!=sep && bf[tfc]!=CLsystem::eol)
		{
			tfc++;
		}
		tfc++;
	}
	//*

	//r is now array of xlongs, r[0] is coutn of values
	return r;
}

arfile* CLformat::loadar(CLfile* sf)
{
	//all .ar members must be aligned on 4byte (long) borders
	//so the filesize is fully dividable by 4
	//test with: if(fs%4==0) ...

	xchar* bf = sf->text;
	xlong cfs = sf->size;

	//ar can contain max 127 files!!!
	armember* tindex[128];
	xlong tsize = cfs - 8;

	//check for "magic-string"
	if( CLutils::comparechararrays(bf,"!<arch>",6) == 1 )
	{
		xlong bc = 8;
		xlong fc = 0;
		xchar fn[16];
		xlong fs = 0;
		xlong ts[10];

		do
		{
			//read member header
			CLutils::copychararray(&fn[0],&bf[bc],16);	//member filename
			bc += 48;					//no necessary information here, so skip
			//*

			//decode filesize of current ar member
			if( bf[bc+9] != 0x20 )
			{
				ts[0]  = (bf[bc+9] - 0x30);
				ts[1]  = (bf[bc+8] - 0x30) * 10;
				ts[2]  = (bf[bc+7] - 0x30) * 100;
				ts[3]  = (bf[bc+6] - 0x30) * 1000;
				ts[4]  = (bf[bc+5] - 0x30) * 10000;
				ts[5]  = (bf[bc+4] - 0x30) * 100000;
				ts[6]  = (bf[bc+3] - 0x30) * 1000000;
				ts[7]  = (bf[bc+2] - 0x30) * 10000000;
				ts[8]  = (bf[bc+1] - 0x30) * 100000000;
				ts[9]  = (bf[bc] -   0x30) * 1000000000;
				fs = ts[0] + ts[1] + ts[2] + ts[3] + ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+8] != 0x20 )
			{
				ts[1]  = (bf[bc+8] - 0x30);
				ts[2]  = (bf[bc+7] - 0x30) * 10;
				ts[3]  = (bf[bc+6] - 0x30) * 100;
				ts[4]  = (bf[bc+5] - 0x30) * 1000;
				ts[5]  = (bf[bc+4] - 0x30) * 10000;
				ts[6]  = (bf[bc+3] - 0x30) * 100000;
				ts[7]  = (bf[bc+2] - 0x30) * 1000000;
				ts[8]  = (bf[bc+1] - 0x30) * 10000000;
				ts[9]  = (bf[bc] -   0x30) * 100000000;
				fs = ts[1] + ts[2] + ts[3] + ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+7] != 0x20 )
			{
				ts[2]  = (bf[bc+7] - 0x30);
				ts[3]  = (bf[bc+6] - 0x30) * 10;
				ts[4]  = (bf[bc+5] - 0x30) * 100;
				ts[5]  = (bf[bc+4] - 0x30) * 1000;
				ts[6]  = (bf[bc+3] - 0x30) * 10000;
				ts[7]  = (bf[bc+2] - 0x30) * 100000;
				ts[8]  = (bf[bc+1] - 0x30) * 1000000;
				ts[9]  = (bf[bc] -   0x30) * 10000000;
				fs = ts[2] + ts[3] + ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+6] != 0x20 )
			{
				ts[3]  = (bf[bc+6] - 0x30);
				ts[4]  = (bf[bc+5] - 0x30) * 10;
				ts[5]  = (bf[bc+4] - 0x30) * 100;
				ts[6]  = (bf[bc+3] - 0x30) * 1000;
				ts[7]  = (bf[bc+2] - 0x30) * 10000;
				ts[8]  = (bf[bc+1] - 0x30) * 100000;
				ts[9]  = (bf[bc] -   0x30) * 1000000;
				fs = ts[3] + ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+5] != 0x20 )
			{
				ts[4]  = (bf[bc+5] - 0x30);
				ts[5]  = (bf[bc+4] - 0x30) * 10;
				ts[6]  = (bf[bc+3] - 0x30) * 100;
				ts[7]  = (bf[bc+2] - 0x30) * 1000;
				ts[8]  = (bf[bc+1] - 0x30) * 10000;
				ts[9]  = (bf[bc] -   0x30) * 100000;
				fs = ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+4] != 0x20 )
			{
				ts[5]  = (bf[bc+4] - 0x30);
				ts[6]  = (bf[bc+3] - 0x30) * 10;
				ts[7]  = (bf[bc+2] - 0x30) * 100;
				ts[8]  = (bf[bc+1] - 0x30) * 1000;
				ts[9]  = (bf[bc] -   0x30) * 10000;
				fs = ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+3] != 0x20 )
			{
				ts[6]  = (bf[bc+3] - 0x30);
				ts[7]  = (bf[bc+2] - 0x30) * 10;
				ts[8]  = (bf[bc+1] - 0x30) * 100;
				ts[9]  = (bf[bc] -   0x30) * 1000;
				fs = ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+2] != 0x20 )
			{
				ts[7]  = (bf[bc+2] - 0x30);
				ts[8]  = (bf[bc+1] - 0x30) * 10;
				ts[9]  = (bf[bc] -   0x30) * 100;
				fs = ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+1] != 0x20 )
			{
				ts[8]  = (bf[bc+1] - 0x30);
				ts[9]  = (bf[bc] -   0x30) * 10;
				fs = ts[8] + ts[9];
			}
			else if( bf[bc] != 0x20 )
			{
				ts[9]  = (bf[bc] -   0x30);
				fs = ts[9];
			}
			//fs contains filesize

			bc+=12; //goto end of header

			//build array for current ar member
			xlong fs2 = fs>>2;

			xlong* tb = new xlong[fs2+1];
			xlong* bf2 = reinterpret_cast<xlong*>(&bf[bc]); //!

			for(int i=0; i<fs2+1; i++)
			{
				tb[i] = bf2[i];
			}
			bc += fs;
			//tb contains file contents

			//make new armember
			tindex[fc] = new armember;
			tindex[fc]->size = fs;
			tindex[fc]->lsize = fs2;
			tindex[fc]->name = new xchar[16]; CLutils::copychararray(tindex[fc]->name,&fn[0],16);
			tindex[fc]->data = tb;
			tindex[fc]->text = reinterpret_cast<xchar *>(&tb[0]); //!
			//tindex[fc] contains complete armember

			if(fs%4!=0) tsize -= (4 - (fs % 4)); //if filesize % 4 != 0 adjust

			tsize -= (fs+60); //subtract readin size from global size

			fc++; //increment filecount

		} while( tsize > 0 );	//wo kommen die 3 her in level.a ???

		//create return value
		arfile* arf = new arfile;
		arf->filecount = fc;
		arf->members = new armember*[fc];
		//return value created

		//place file contents in return value
		for(int j=0; j<fc; j++)
		{ 
			arf->members[j] = tindex[j];
		}
		//arf is now complete

		return arf;
	}

	return 0;
}

xlong** CLformat::loadbcx(CLfile* bf)
{
	xlong lc = CLutils::getlinecount(bf);

	doubleword nl = { 0 };
	xlong bc = 0;	

	xlong *arr0 = new xlong[lc];
	xlong *arr1 = new xlong[lc];

	arr0[0] = lc-2;
	arr1[0] = lc-2;

	if( bf->data[bc] == 'BLC<' )
	{
		bc += 4;

		for(int i=1; i < lc; i++)
		{
			if( bf->data[bc] == 'LUN<' ) break;

			arr0[i] = bf->data[bc]; bc++;
			arr1[i] = bf->data[bc]; bc++;
			nl.dd   = bf->data[bc]; bc++;

			if( nl.dw[0] == '##' )
			{
				//CLprint_(&nl.db[0]);
				nl.dd = bf->data[bc-2];
				arr1[i]  = (nl.db[0] - 0x30) * 1000;
				arr1[i] += (nl.db[1] - 0x30) * 100; 
				arr1[i] += (nl.db[2] - 0x30) * 10; 
				arr1[i] += (nl.db[3] - 0x30);
			}
			bc++;
		}
	}

	xlong **re = new xlong*[2];
	re[0] = arr0;
	re[1] = arr1;
	return re;
}

xchar** CLformat::loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv)
{
	xchar* bf = sf->text;
	//xlong bs = sf->size;
	xlong lc = CLutils::getlinecount(sf);

	//determine line length
	xlong lw = 0;
	xlong cc = 0;
	while(cc < lc)
	{
		if(bf[cc]=='\n') break;
		else lw++;
		cc++;
	}
	//lw contains line length

	xchar** rev = new xchar*[lc];
	for(int i=0; i<lc; i++)
	{
		rev[i] = new xchar[lw];
	}

	xlong li = 0;
	for(int j=0; j<lc; j++)
	{
		for(int k=0; k<lw; k++)
		{
			if(bf[li]!='\n')
			{
				if(bf[li]==rc) rev[j][k] = rv;
				else rev[j][k] = bf[li] - subconst;
			}
			else CLsystem::exit(1,0,__func__,"Map not conform with given width");
			
			li++;
		}
		li++;
	}

	return rev;
}

sprite* CLformat::loadtga(CLfile* sf)
{
//loads only TGA's with datatype=1,2, origin in upper left, and 32bit color depth.

	xchar* bf = sf->text;

	//xchar	imageid		= bf[0];

	//xchar	colormap	= bf[1];
	xchar	imagetype	= bf[2];

	if(imagetype > 2) return 0; //no TGA!

	//xshort	colormaporigin	= bf[3] + (xshort(bf[4])<<16);
	//xshort	colormaplength	= bf[5] + (xshort(bf[6])<<16);
	//xchar	colormaprentry	= bf[7];

	//xshort	imageoriginx	= bf[8] + (xshort(bf[9])<<16);
	//xshort	imageoriginy	= bf[10] + (xshort(bf[11])<<16);
	xshort	imagewidth	= bf[12] + (xshort(bf[13])<<8);
	xshort	imageheight	= bf[14] + (xshort(bf[15])<<8);
	//xchar	imagepixelsize	= bf[16];

	//if(imagepixelsize != 32 || imagepixelsize != 24) return 0; //nor 32bit or 24bit //fix here!

	//xchar	imagedescriptor	= bf[17];

	//if( (imagedescriptor && 0x00010000) == 0x00010000) return 0; //not upper left = origin //test

	//xshort imageoffset = imageid + colormaplength;

	sprite* r = new sprite;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->data = reinterpret_cast<xlong*>(&bf[18]); // + imageoffset); //!

	return r;
}

sprites* CLformat::loadtileset(CLfile* sf,xlong tw,xlong th)
{
//loads only TGA's with datatype=1,2, origin in upper left, and 32bit color depth.

	xchar* bf = sf->text;

	//xchar	imageid		= bf[0];

	//xchar	colormap	= bf[1];
	xchar	imagetype	= bf[2];

	if(imagetype > 2) return 0; //no TGA!

	//xshort	colormaporigin	= bf[3] + (xshort(bf[4])<<16);
	//xshort	colormaplength	= bf[5] + (xshort(bf[6])<<16);
	//xchar	colormaprentry	= bf[7];

	//xshort	imageoriginx	= bf[8] + (xshort(bf[9])<<16);
	//xshort	imageoriginy	= bf[10] + (xshort(bf[11])<<16);
	xshort	imagewidth	= bf[12] + (xshort(bf[13])<<8);
	xshort	imageheight	= bf[14] + (xshort(bf[15])<<8);
	//xchar	imagepixelsize	= bf[16];

	//if(imagepixelsize != 32 || imagepixelsize != 24) return 0; //nor 32bit or 24bit //fix here!

	//xchar	imagedescriptor	= bf[17];

	//if( (imagedescriptor && 0x00010000) == 0x00010000) return 0; //not upper left = origin //test

	//xshort imageoffset = imageid + colormaplength;

	sprites* r = new sprites;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->tilesize = tw * th;
	r->tilewidth = tw;
	r->tileheight = th;
	r->data = reinterpret_cast<xlong*>(&bf[18]); // + imageoffset); //!
	
	if( (r->width%r->tilewidth!=0) || (r->height%r->tileheight!=0) ) CLsystem::exit(1,0,__func__,"tile dimensions do not match image dimensions");
	r->tilecount = (r->width/r->tilewidth) * (r->height*r->tileheight);

	return r;
}

sprites* CLformat::loadfont(CLfile* sf)
{
//loads only TGA's with datatype=1,2, origin in upper left, and 32bit color depth.

	xchar* bf = sf->text;

	//xchar	imageid		= bf[0];

	//xchar	colormap	= bf[1];
	xchar	imagetype	= bf[2];

	if(imagetype > 2) return 0; //no TGA!

	//xshort	colormaporigin	= bf[3] + (xshort(bf[4])<<16);
	//xshort	colormaplength	= bf[5] + (xshort(bf[6])<<16);
	//xchar	colormaprentry	= bf[7];

	//xshort	imageoriginx	= bf[8] + (xshort(bf[9])<<16);
	//xshort	imageoriginy	= bf[10] + (xshort(bf[11])<<16);
	xshort	imagewidth	= bf[12] + (xshort(bf[13])<<8);
	xshort	imageheight	= bf[14] + (xshort(bf[15])<<8);
	//xchar	imagepixelsize	= bf[16];

	//if(imagepixelsize != 32 || imagepixelsize != 24) return 0; //nor 32bit or 24bit //fix here!

	//xchar	imagedescriptor	= bf[17];

	//if( (imagedescriptor && 0x00010000) == 0x00010000) return 0; //not upper left = origin //test

	//xshort imageoffset = imageid + colormaplength;

	if( (imagewidth%256)!=0 ) CLsystem::exit(1,0,__func__,"Not 256 font tiles in one row!");

	CLfont* r = new CLfont;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->tilewidth = imagewidth / 256;
	r->tileheight = imageheight;
	r->tilesize = (imagewidth / 256) * imageheight;
	r->data = reinterpret_cast<xlong*>(&bf[18]); // + imageoffset); //!
	r->tilecount = 256;

	return r;
}

xlong** CLformat::loadlvl()
{
	return 0;
}

xmap* CLformat::loadini(CLfile* sf)
{
	//steckt in irgendeiner whileschleife fest...
	xmap* r = new xmap;
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = CLutils::getlinecount(sf);
	//*
	CLsystem::print(lc);
	
	xlong cc=0;
	xlong tc0=0;
	xlong tc1=0;
	xchar* tp0=0;
	xchar* tp1=0;
	bool apos=0;
	xlong aps=0;
	for(int i=0; i<lc; i++)
	{
		while(bf[cc]==' ') cc++;
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!=CLsystem::eol)
		{
			//pre equal sign
			tc0 = cc;
			tc1=0;
			while(bf[cc]!='=')
			{
				if(bf[cc]!=' ') tc1++;
				cc++;
			}

			cc ^= tc0 ^= cc ^= tc0; //xor swap trick
			
			tp0 = new xchar[tc1+1];
			tp0[tc1]=0;
			for(int j=0; j<tc1; j++)
			{
				tp0[j] = bf[cc+j];
			}
			cc = tc0;
			cc++;
			//*
			
			//post equal sign
			while(bf[cc]==' ') cc++;

			tc0 = cc;
			tc1=0;
			while(bf[cc] != CLsystem::eol)
			{
				     if( (bf[cc]=='"' || bf[cc]=='\'') && apos==0) { apos=1; cc++; aps=cc; }
				else if( (bf[cc]=='"' || bf[cc]=='\'') && apos==1) { apos=0; break; }
				else if(bf[cc]!=' ' && apos==0) { tc1++; cc++; }
				else if(apos==1) {tc1++; cc++; }
				else if( bf[cc]=='#' || bf[cc]==';') break;
			}

			cc ^= tc0 ^= cc ^= tc0; //xor swap trick
			
			if(aps==0)
			{
				tp1 = new xchar[tc1+1];
				tp1[tc1]=0;
				for(int j=0; j<tc1; j++)
				{
					tp1[j] = bf[cc+j];
				}
			}
			else 
			{
				tp1 = new xchar[tc1+1];
				tp1[tc1]=0;
				for(int j=0; j<tc1; j++)
				{
					tp1[j] = bf[aps+j];
				}
			}
			cc = tc0;
			//*
			
			//map values in xmap
			(*r)[tp0] = tp1;
			//*
		}
			
		//reset fopr next line
		while(bf[cc]!=CLsystem::eol) cc++;
		cc++;
		aps=0;
		//*

	}
	
	return r;
}

#endif

