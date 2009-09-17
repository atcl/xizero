//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFORMAT
#define HH_CLFORMAT
#pragma message "Compiling " __FILE__ " ! TODO: fix rare loadar crash"

#include <map>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLutils.hh"
#include "CLmacros.hh"
#include "CLsystem.hh"

struct cmpstr
{
	bool operator()(const xchar* a,const xchar* b) { return CLsystem::instance()->cmpcstr(a,b) < 0; }
};

typedef std::map <const xchar*,const xchar*,cmpstr> xmap;

namespace CLformat
{
	CLutils* utils = CLutils::instance();
	CLsystem* system = CLsystem::instance();
	
	xlong*   loadcsv(CLfile* sf,xchar sep=',');
	arfile*  loadar(CLfile* sf);
	xchar**  loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv);
	sprite*  loadtga(CLfile* sf);
	sprites* loadtileset(CLfile* sf,xlong tw,xlong th);
	sprites* loadfont(CLfile* sf);
	xlong**  loadlvl();
	xmap*    loadini(CLfile* bf);
}


xlong* CLformat::loadcsv(CLfile* sf,xchar sep)
{
	//add template parameter to change between xlong*,xchar**,xfixed* and float*
	//Works only for integers!
	
	//! bug with new line at end of file !
	//! add comma at end of line !
	//! no comma after value means end of data !
	//! crashes in line 84 (with new line at end of file) !
	
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = utils->getlinecount(sf);
	//*

	//get comma count per line
	xlong* cc = new xlong[lc];
	xlong tfc = 0;
	xlong tcc = 0;
	xlong tlc = 0;
	while(tlc<lc)
	{
		if(bf[tfc]==sep) tcc++;
		else if(bf[tfc]==system->eol())
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
	
	for(uxlong i=0; i<lc; i++)
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
		r[tvc] = system->ato(&bf[tfc]);
		tvc++;
		while(bf[tfc]!=sep && bf[tfc]!=system->eol())
		{
			tfc++;
		}
		tfc++;
	}
	//*

	//r is now array of xlongs, r[0] is coutn of values
	return r;
	//*
}

arfile* CLformat::loadar(CLfile* sf)
{
	xchar* bf = sf->text;
	xlong cfs = sf->size;

	//ar can contain max 127 files!!!
	armember* tindex[128];
	xlong tsize = cfs - 8;
	//*

	//check for "magic-string"
	if( system->cmpcstr(bf,"!<arch>",6) == 0 )
	{
		//init variables
		xlong bc = 8;
		xlong fc = 0;
		xchar fn[16];
		xlong fs = 0;
		xlong ts[10];
		//*

		//for each member do
		do
		{
			//read member header
			utils->copychararray(&fn[0],&bf[bc],16);	//member filename
			bc += 48;					//no necessary information here, so skip
			//*
			
			//decode filesize of current ar member
			fs = system->ato(&bf[bc]);
			bc+=12; //goto end of header
			//*

			//create xlong array for current ar member
			xlong fs2 = fs>>2;
			if(fs%4!=0) fs2++;
			fs2++;
			xlong* tb = new xlong[fs2];
			xlong* bf2 = static_cast<xlong*>(static_cast<void*>(&bf[bc]));
			//*

			//fill array
			for(uxlong i=0; i<fs2; i++)
			{
				tb[i] = bf2[i];
			}
			bc += fs;
			//*

			//make new armember
			tindex[fc] = new armember;
			tindex[fc]->size = fs;
			tindex[fc]->lsize = fs2;
			tindex[fc]->name = new xchar[16]; utils->copychararray(tindex[fc]->name,&fn[0],16);
			tindex[fc]->data = tb;
			tindex[fc]->text = static_cast<xchar*>(static_cast<void*>(&tb[0]));
			//*

			//adjust global ar variables
			if(fs%2!=0) { bc++; tsize--; }
			tsize -= (fs+60); //subtract reading size from global size
			fc++; //increment filecount
			//*

		} while( tsize > 0 );
		//*

		//create arfile
		arfile* arf = new arfile;
		arf->filecount = fc;
		arf->members = new armember*[fc];
		//*

		//copy armembers to arfile
		for(uxlong j=0; j<fc; j++)
		{ 
			arf->members[j] = tindex[j];
		}
		//*

		return arf;
	}

	return 0;
}

xchar** CLformat::loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv)
{
	//calc subconst yourself by rc and rv
	
	xchar* bf = sf->text;
	//xlong bs = sf->size;
	xlong lc = utils->getlinecount(sf);

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

	//for each row create xchar array of line length
	xchar** rev = new xchar*[lc];
	for(uxlong i=0; i<lc; i++)
	{
		rev[i] = new xchar[lw];
	}
	//*

	//copy and manipulate contents depending on subconst,rc,rv
	xlong li = 0;
	for(uxlong j=0; j<lc; j++)
	{
		for(uxlong k=0; k<lw; k++)
		{
			if(bf[li]!='\n')
			{
				if(bf[li]==rc) rev[j][k] = rv;
				else rev[j][k] = bf[li] - subconst;
			}
			else system->exit(1,0,__func__,"Map not conform with given width");
			
			li++;
		}
		li++;
	}
	//*

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

	//fill sprite struct
	sprite* r = new sprite;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->data = static_cast<xlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	//*

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

	//fill sprites struct
	sprites* r = new sprites;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->tilesize = tw * th;
	r->tilewidth = tw;
	r->tileheight = th;
	r->data = static_cast<xlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	//*
	
	if( (r->width%r->tilewidth!=0) || (r->height%r->tileheight!=0) ) system->exit(1,0,__func__,"tile dimensions do not match image dimensions");
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

	if( (imagewidth%256)!=0 ) system->exit(1,0,__func__,"Not 256 font tiles in one row!");

	//fill CLfont struct
	CLfont* r = new CLfont;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->tilewidth = imagewidth / 256;
	r->tileheight = imageheight;
	r->tilesize = (imagewidth / 256) * imageheight;
	r->data = static_cast<xlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	r->tilecount = 256;
	//*

	return r;
}

xlong** CLformat::loadlvl()
{
	return 0;
}

xmap* CLformat::loadini(CLfile* sf)
{
	//inis need newline at end of file!!
	xmap* r = new xmap;
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = utils->getlinecount(sf);
	//*
	
	xlong cc=0;
	xlong tc0=0;
	xlong tc1=0;
	xchar* tp0=0;
	xchar* tp1=0;
	bool apos=0;
	xlong aps=0;
	for(uxlong i=0; i<lc; i++)
	{
		while(bf[cc]==' ') cc++;
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!=system->eol())
		{
			//pre equal sign
			tc0 = cc;
			tc1=0;
			bool noinfo = 0;
			bool noequal = 0;
			while(bf[cc]!='=')
			{
				if(bf[cc]!=' ') noinfo = 1;
				if(bf[cc]==system->eol() && noinfo==1) noequal = 1;
				if(bf[cc]!=' ') tc1++;
				cc++;
			}
			
			if(noequal==0)
			{
				cc ^= tc0 ^= cc ^= tc0; //xor swap trick
				
				tp0 = new xchar[tc1+1];
				tp0[tc1]=0;
				for(uxlong j=0; j<tc1; j++)
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
				while(bf[cc] != system->eol())
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
					for(uxlong j=0; j<tc1; j++)
					{
						tp1[j] = bf[cc+j];
					}
				}
				else 
				{
					tp1 = new xchar[tc1+1];
					tp1[tc1]=0;
					for(uxlong j=0; j<tc1; j++)
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
		}
			
		//reset fopr next line
		while(bf[cc] != system->eol()) cc++;
		cc++;
		aps=0;
		//*

	}
	
	return r;
}

#endif

