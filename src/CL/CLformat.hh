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

/* class name:	CLformat
 * 
 * description:	This class processes all kinds of file formats.
 * 
 * author:	atcl
 * 
 * notes:	fix loadcsv.
 * 
 * version: 0.1
 */

struct cmpstr { bool operator()(const xchar* a,const xchar* b) { return CLsystem::instance()->cmpcstr(a,b) < 0; } };

typedef std::map <const xchar*,const xchar*,cmpstr> xmap;

class CLformat : public virtual CLcl, public CLsingle<CLformat>
{
	friend class CLsingle<CLformat>;
	
	private:
		CLformat() { };
		~CLformat() { };
	public:
		xlong*   loadcsv(const xchar* sf,xchar sep=',') const;
		xlong*   loadcsv(CLfile* sf,xchar sep=',') const;
		xchar**  loadmap(const xchar* sf,xlong subconst,xchar rc,xlong rv) const;
		xchar**  loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const;
		sprite*  loadtga(const xchar* sf) const;
		sprite*  loadtga(CLfile* sf) const;
		sprite*  loadxpm(const xchar** xpm) const;
		sprites* loadtileset(const xchar* sf,xlong tw,xlong th) const;
		sprites* loadtileset(CLfile* sf,xlong tw,xlong th) const;
		sprites* loadfont(const xchar* sf) const;
		sprites* loadfont(CLfile* sf) const;
		xchar**  loadlvl(const xchar* sf) const;
		xchar**  loadlvl(CLfile* sf) const;
		xmap*    loadini(const xchar* bf) const;
		xmap*    loadini(CLfile* bf) const;
};

xlong* CLformat::loadcsv(const xchar* sf,xchar sep) const { return loadcsv(clsystem->getfile(sf),sep); }

xlong* CLformat::loadcsv(CLfile* sf,xchar sep) const
{
	//add template parameter to change between xlong*,xchar**,xfixed* and float*
	//Works only for integers!
	
	//! bug with new line at end of file !
	//! add comma at end of line !
	//! no comma after value means end of data !
	//! crashes in line 84 (with new line at end of file) !
	
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = clutils->getlinecount(sf);
	//*

	//get comma count per line
	xlong* cc = new xlong[lc];
	xlong tfc = 0;
	xlong tcc = 0;
	xlong tlc = 0;
	while(tlc<lc)
	{
		if(bf[tfc]==sep) tcc++;
		else if(bf[tfc]==clsystem->eol())
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
		r[tvc] = clsystem->ato(&bf[tfc]);
		tvc++;
		while(bf[tfc]!=sep && bf[tfc]!=clsystem->eol())
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

xchar** CLformat::loadmap(const xchar* sf,xlong subconst,xchar rc,xlong rv) const
{ return loadmap(clsystem->getfile(sf),subconst,rc,rv); }

xchar** CLformat::loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const
{
	//calc subconst yourself by rc and rv
	
	xchar* bf = sf->text;
	//xlong bs = sf->size;
	xlong lc = clutils->getlinecount(sf);

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
			else clsystem->exit(1,0,__func__,"Map not conform with given width");
			
			li++;
		}
		li++;
	}
	//*

	return rev;
}

sprite* CLformat::loadtga(const xchar* sf) const { return loadtga(clsystem->getfile(sf)); }

sprite* CLformat::loadtga(CLfile* sf) const
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
	r->data = static_cast<uxlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	//*

	return r;
}

sprite* CLformat::loadxpm(const xchar** xpm) const
{
	uxlong xpm_ptr = 0;
	
	//read width,height,colors and chars per pixel
	uxlong width = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong height = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong colors = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong charpp = CLsystem::instance()->ato(&xpm[0][xpm_ptr]); //this will work only for 1 char per pixel!!!
	if(charpp!=1) return 0;
	uxlong bytesize = (width*height)<<2;
	//*
	
	//allocate
	sprite* r = new sprite;
	r->width = width;
	r->height = height;
	r->size = width*height;
	uxlong* data = new uxlong[r->size];
	r->data = data;
	//*
	
	//read color table and set image buffers colors
	colors++;
	uxlong ctable[256];
	uxchar cindex = 0;
	uxlong currcol = 0; 
	for(uxlong i=1; i<colors; i++)
	{
		xpm_ptr = 0;
		cindex = xpm[i][0];
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') xpm_ptr++;
		xpm_ptr++;
		if(xpm[i][xpm_ptr]!='c' && xpm[i][xpm_ptr]!='C') return 0;
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') xpm_ptr++;
		xpm_ptr++;
		if(xpm[i][xpm_ptr]!='#') return 0;
		xpm_ptr++;
		currcol  = clutils->hatoi(xpm[i][xpm_ptr])<<20; xpm_ptr++; 
		currcol += clutils->hatoi(xpm[i][xpm_ptr])<<16; xpm_ptr++;
		currcol += clutils->hatoi(xpm[i][xpm_ptr])<<12; xpm_ptr++;
		currcol += clutils->hatoi(xpm[i][xpm_ptr])<<8;  xpm_ptr++;
		currcol += clutils->hatoi(xpm[i][xpm_ptr])<<4;  xpm_ptr++;
		currcol += clutils->hatoi(xpm[i][xpm_ptr]);
		ctable[cindex] = currcol;
	}
	//*

	//fill data
	uxlong data_ptr = 0; 
	height += colors;
	for(uxlong j=colors; j<height; j++)
	{
		for(uxlong k=0; k<width; k++)
		{
			r->data[data_ptr] = ctable[xpm[j][k]];
			data_ptr++;
		}
	}
	//*
	
	return r;
}

sprites* CLformat::loadtileset(const xchar* sf,xlong tw,xlong th) const { return loadtileset(clsystem->getfile(sf),tw,th); }

sprites* CLformat::loadtileset(CLfile* sf,xlong tw,xlong th) const
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
	r->data = static_cast<uxlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	//*
	
	if( (r->width%r->tilewidth!=0) || (r->height%r->tileheight!=0) ) clsystem->exit(1,0,__func__,"tile dimensions do not match image dimensions");
	r->tilecount = (r->width/r->tilewidth) * (r->height*r->tileheight);

	return r;
}

sprites* CLformat::loadfont(const xchar* sf) const { return loadfont(clsystem->getfile(sf)); }

sprites* CLformat::loadfont(CLfile* sf) const
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

	if( (imagewidth%256)!=0 ) clsystem->exit(1,0,__func__,"Not 256 font tiles in one row!");

	//fill CLfont struct
	CLfont* r = new CLfont;
	r->size = imagewidth * imageheight;
	r->width = imagewidth;
	r->height = imageheight;
	r->tilewidth = imagewidth / 256;
	r->tileheight = imageheight;
	r->tilesize = (imagewidth / 256) * imageheight;
	r->data = static_cast<uxlong*>(static_cast<void*>(&bf[18])); // + imageoffset); //!
	r->tilecount = 256;
	//*

	return r;
}

xchar** CLformat::loadlvl(const xchar* sf) const { return loadlvl(clsystem->getfile(sf)); }

xchar** CLformat::loadlvl(CLfile* sf) const
{
	xchar* bf = sf->text;
	
	//get startpositions of filenames
	xchar* fs[5] = { 0,0,0,0,0 };
	xlong fl[5] = { 0,0,0,0,0 };
	bool marks = 0;
	xlong fsc = 0;
	for(uxlong i=0; i<sf->size; i++)
	{
		if(marks==0 && bf[i]=='"')
		{
			marks = 1;
			fs[fsc] = &bf[i+1];
		}
		else if(marks==1 && bf[i]=='"')
		{
			marks = 0;
			fsc++;
			if(fsc>=5) break; 
		}
		else if(marks==1)
		{
			fl[fsc]++;
		}
	}
	//*
	
	//copy filenames to buffer
	xchar** fn = new xchar*[5];
	for(uxlong j=0; j<5; j++)
	{
		fn[j] = new xchar[fl[j]+1];
		fn[j][fl[j]] = 0;
		clutils->copychararray(fn[j],fs[j],fl[j]);
	}
	//*
	
	return fn;
}

xmap* CLformat::loadini(const xchar* sf) const { return loadini(clsystem->getfile(sf)); }

xmap* CLformat::loadini(CLfile* sf) const
{
	//inis need newline at end of file!!
	xmap* r = new xmap;
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = clutils->getlinecount(sf);
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
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!=clsystem->eol())
		{
			//pre equal sign
			tc0 = cc;
			tc1=0;
			bool noinfo = 0;
			bool noequal = 0;
			while(bf[cc]!='=')
			{
				if(bf[cc]!=' ') noinfo = 1;
				if(bf[cc]==clsystem->eol() && noinfo==1) noequal = 1;
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
				while(bf[cc] != clsystem->eol())
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
		while(bf[cc] != clsystem->eol()) cc++;
		cc++;
		aps=0;
		//*

	}
	
	return r;
}

#endif

