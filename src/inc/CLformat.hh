//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFORMAT
#define HH_CLFORMAT
#pragma message "Compiling " __FILE__ " ! TODO: fix loadar crash"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLutils.hh"
#include "CLmacros.hh"


class CLformat : public virtual CLcl
{
	private:

	public:
		CLformat();
		~CLformat();
		xchar** loadcsv(CLfile* sf);
		arfile* loadar(CLfile* sf);
		xlong** loadbcx(CLfile* bf);
		xchar** loadmap(CLfile* sf,xlong subconst);
		sprite* loadtga(CLfile* sf);

		sprite* loadfont(CLfile* sf);
		xlong** loadlvl();
		xlong** loadini();
};

CLformat::CLformat()
{

}

CLformat::~CLformat() { }

xchar** CLformat::loadcsv(CLfile* sf)
{

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
	if( comparechararrays(bf,"!<arch>",6) == 1 )
	{
		xlong bc = 8;
		xlong fc = 0;
		xchar fn[16];
		xlong fs = 0;
		xlong ts[10];

		do
		{
			//read member header
			copychararray(&fn[0],&bf[bc],16);	//member filename
			bc += 48;				//no necessary information here, so skip
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
				ts[9]  = (bf[bc] - 0x30) * 1000000000;
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
				ts[9]  = (bf[bc] - 0x30) * 100000000;
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
				ts[9]  = (bf[bc] - 0x30) * 10000000;
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
				ts[9]  = (bf[bc] - 0x30) * 1000000;
				fs = ts[3] + ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+5] != 0x20 )
			{
				ts[4]  = (bf[bc+5] - 0x30);
				ts[5]  = (bf[bc+4] - 0x30) * 10;
				ts[6]  = (bf[bc+3] - 0x30) * 100;
				ts[7]  = (bf[bc+2] - 0x30) * 1000;
				ts[8]  = (bf[bc+1] - 0x30) * 10000;
				ts[9]  = (bf[bc] - 0x30) * 100000;
				fs = ts[4] + ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+4] != 0x20 )
			{
				ts[5]  = (bf[bc+4] - 0x30);
				ts[6]  = (bf[bc+3] - 0x30) * 10;
				ts[7]  = (bf[bc+2] - 0x30) * 100;
				ts[8]  = (bf[bc+1] - 0x30) * 1000;
				ts[9]  = (bf[bc] - 0x30) * 10000;
				fs = ts[5] + ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+3] != 0x20 )
			{
				ts[6]  = (bf[bc+3] - 0x30);
				ts[7]  = (bf[bc+2] - 0x30) * 10;
				ts[8]  = (bf[bc+1] - 0x30) * 100;
				ts[9]  = (bf[bc] - 0x30) * 1000;
				fs = ts[6] + ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+2] != 0x20 )
			{
				ts[7]  = (bf[bc+2] - 0x30);
				ts[8]  = (bf[bc+1] - 0x30) * 10;
				ts[9]  = (bf[bc] - 0x30) * 100;
				fs = ts[7] + ts[8] + ts[9];
			}
			else if( bf[bc+1] != 0x20 )
			{
				ts[8]  = (bf[bc+1] - 0x30);
				ts[9]  = (bf[bc] - 0x30) * 10;
				fs = ts[8] + ts[9];
			}
			else if( bf[bc] != 0x20 )
			{
				ts[9]  = (bf[bc] - 0x30);
				fs = ts[9];
			}
			//fs contains filesize

			bc+=12; //goto end of header

			//build array for current ar member
			xlong fs2 = fs>>2;
			xlong* tb = new xlong[fs2+1];
			xlong* bf2 = reinterpret_cast<xlong*>(&bf[bc]);
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
			tindex[fc]->name = new xchar[16]; copychararray(tindex[fc]->name,&fn[0],16);
			tindex[fc]->data = tb;
			tindex[fc]->text = reinterpret_cast<xchar *>(&tb[0]);
			//tindex[fc] contains complete armember

			tsize -= (fs+60); //subtract readin size from global size

			fc++; //increment filecount

		} while( tsize > 0 );	//wo kommen die 3 her in level.a ???

		//create return value
		arfile* af = new arfile;
		af->filecount = fc;
		af->members = new armember*[fc];
		//return value created

		//place file contents in return value
		for(int j=0; j<fc; j++)
		{ 
			af->members[j] = tindex[j];
		}
		//af is now complete

		return af;
	}

	return 0;
}

xlong** CLformat::loadbcx(CLfile* bf)
{
	xlong lc = getlinecount(bf->text,bf->size);

	doubleword nl;
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

xchar** CLformat::loadmap(CLfile* sf,xlong subconst)
{
	xchar* bf = sf->text;
	xlong bs = sf->size;
	xlong lc = getlinecount(bf,bs);

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
			if(bf[li]!='\n') rev[j][k] = bf[li] - subconst;
			else CLexit_(1,__func__,"Map not conform with given width");
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

	xchar	imageid		= bf[0];

	xchar	colormap	= bf[1];
	xchar	imagetype	= bf[2];

	if(imagetype > 2) return 0; //no TGA!

	xshort	colormaporigin	= bf[3] + (xshort(bf[4])<<16);
	xshort	colormaplength	= bf[5] + (xshort(bf[6])<<16);
	xchar	colormaprentry	= bf[7];

	xshort	imageoriginx	= bf[8] + (xshort(bf[9])<<16);
	xshort	imageoriginy	= bf[10] + (xshort(bf[11])<<16);
	xshort	imagewidth	= bf[12] + (xshort(bf[13])<<16);
	xshort	imageheight	= bf[14] + (xshort(bf[15])<<16);
	xchar	imagepixelsize	= bf[16];

	//if(imagepixelsize != 32 || imagepixelsize != 24) return 0; //nor 32bit or 24bit //fix here!

	xchar	imagedescriptor	= bf[17];

	//if( (imagedescriptor && 0x00010000) == 0x00010000) return 0; //not upper left = origin //test

	xshort imageoffset = imageid + colormaplength;

	sprite* r = new sprite;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->data = reinterpret_cast<xlong*>(&bf[18]); // + imageoffset);

	return r;
}

#endif
