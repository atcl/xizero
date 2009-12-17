///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFORMAT
#define HH_CLFORMAT
///*

///includes
#include <map>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLstring.hh"
#include "CLmacros.hh"
///*

///header
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
///*

///definitions
struct cmpstr { bool operator()(const xchar* a,const xchar* b) { return lt(a,b) < 0; } };

typedef std::map <const xchar*,const xchar*,cmpstr> xmap;

class CLformat : public virtual CLcl, public CLsingle<CLformat>
{
	friend class CLsingle<CLformat>;
	
	private:
		CLformat() { };
		~CLformat() { };
	public:
		inline xlong*   loadcsv(const xchar* sf,xchar sep=',') const;
		inline xchar**  loadmap(const xchar* sf,xlong subconst,xchar rc,xlong rv) const;
		inline sprite*  loadras(const xchar* sf) const;
		inline tileset* loadtileset(const xchar* sf,xlong tw,xlong th) const;
		inline xchar**  loadlvl(const xchar* sf) const;
		inline xmap*    loadini(const xchar* bf) const;
	
		xlong*   loadcsv(CLfile* sf,xchar sep=',') const;
		xchar**  loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const;
		sprite*  loadras(CLfile* sf) const;
		sprite*  loadxpm(const xchar* xpm[]) const;
		sprite*  loadxpm(const xchar* xpm) const;
		tileset* loadtileset(CLfile* sf,xlong tw,xlong th) const;
		xchar**  loadlvl(CLfile* sf) const;
		xmap*    loadini(CLfile* bf) const;
};
///*

///implementation
xlong* CLformat::loadcsv(const xchar* sf,xchar sep) const { return loadcsv(clsystem->getfile(sf),sep); } //! noncritical

xchar** CLformat::loadmap(const xchar* sf,xlong subconst,xchar rc,xlong rv) const { return loadmap(clsystem->getfile(sf),subconst,rc,rv); } //! noncritical

sprite* CLformat::loadras(const xchar* sf) const { return loadras(clsystem->getfile(sf)); } //! noncritical

tileset* CLformat::loadtileset(const xchar* sf,xlong tw,xlong th) const { return loadtileset(clsystem->getfile(sf),tw,th); } //! noncritical

xchar** CLformat::loadlvl(const xchar* sf) const { return loadlvl(clsystem->getfile(sf)); } //! noncritical

xmap* CLformat::loadini(const xchar* sf) const { return loadini(clsystem->getfile(sf)); } //! noncritical


xlong* CLformat::loadcsv(CLfile* sf,xchar sep) const //! noncritical
{
	//add template parameter to change between xlong*,xchar**,xfixed* and float*
	//Works only for integers!
	
	//! bug with new line at end of file !
	//! add comma at end of line !
	//! no comma after value means end of data !
	//! crashes in line 84 (with new line at end of file) !
	
	xchar* bf = sf->text;
	
	//get linecount
	xlong  lc = clstring->linecount(bf);
	//*

	//get comma count per line
	xlong* cc = new xlong[lc];
	xlong tfc = 0;
	xlong tcc = 0;
	xlong tlc = 0;
	while(tlc<lc)
	{
		if(bf[tfc]==sep) tcc++;
		else if(bf[tfc]=='\n')
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
		r[tvc] = clstring->tolong(&bf[tfc]);
		tvc++;
		while(bf[tfc]!=sep && bf[tfc]!='\n') { tfc++; }
		tfc++;
	}
	//*

	//r is now array of xlongs, r[0] is coutn of values
	return r;
	//*
}
xchar** CLformat::loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const //! noncritical
{
	//calc subconst yourself by rc and rv
	
	xchar* bf = sf->text;
	//xlong bs = sf->size;
	xlong lc = clstring->linecount(bf);

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
			else
			{
				err(__func__,u8"Map not conform with given width");
				return 0;
			}			
			li++;
		}
		li++;
	}
	//*

	return rev;
}

sprite* CLformat::loadras(CLfile* sf) const //! noncritical
{
	xlong* lf = sf->data;

	if(clutils->endian(lf[0])!=0x59A66A95) return 0; //test for magic number
	xlong width = clutils->endian(lf[1]);
	xlong height = clutils->endian(lf[2]);
	xlong depth = clutils->endian(lf[3]);
	if(depth!=32 && depth!=24) return 0; //only 24bpp and 32bpp!
	xlong length = clutils->endian(lf[4]);
	xlong type = clutils->endian(lf[5]);
	if(type!=0 && type!=1 && type!=2) return 0; //only types: old, standard and rle are supported
	if(clutils->endian(lf[6])!=0) return 0; //color maps are not supported
	if(clutils->endian(lf[7])!=0) return 0; //color maps are not supported
	
	xchar* bf = &((sf->text)[32]);
	xlong pixelindex = 0;
	xlong pixelcount = 0;
	xlong dataindex = 0;
	xlong size = width * height;
	xlong fullsize = size<<2;
	xlong bytedepth = depth>>3;
	uxchar temp = 0;
	
	sprite* r = new sprite;
	r->size = size;
	r->width = width;
	r->height = height;
	uxchar* data = new uxchar[fullsize];
	r->data = static_cast<uxlong*>(static_cast<void*>(&data[0]));
	
	if(type==2)
	{
		while(pixelindex<fullsize)
		{			
			if(uxchar(bf[dataindex])==0x80)
			{
				dataindex++;
				temp = uxchar(bf[dataindex]);
				
				if(temp!=0)
				{
					dataindex++;	
					for(xlong i=0; i<=temp; i++)
					{
						if(width%2==1 && pixelindex%(width*4)==1) temp--;
						
						if(depth==24 && (pixelindex+1)%4==0)
						{
							data[pixelindex] = 0;
							pixelindex++;
							pixelcount++;
						}
						
						data[pixelindex] = bf[dataindex];
						pixelindex++;
					}
					dataindex++;
				}
				else
				{
					if(width%2==1 && pixelindex%(width*4)==1) dataindex++;
					else
					{
						if(depth==24 && (pixelindex+1)%4==0)
						{
							data[pixelindex] = 0;
							pixelindex++;
						}
						
						data[pixelindex] = 0x80;
						pixelindex++;
						dataindex++;
					}
				}
			}
			else
			{
				if(depth==24 && (pixelindex+1)%4==0)
				{
					data[pixelindex] = 0;
					pixelindex++;
				}		
						
				data[pixelindex] = bf[dataindex];
				pixelindex++;
				dataindex++;
			}
		}
	}
	else
	{
		xlong i = 0;
		xlong j = 0;
		xlong k = 0;
		while(i<fullsize)
		{
			data[i] = bf[j]; i++; j++;
			data[i] = bf[j]; i++; j++;
			data[i] = bf[j]; i++; j++;
			data[i] = 0; i++;
			k++;
			if(k==width) { j++; k=0; }
		}
	}
	
	return r;
}

sprite* CLformat::loadxpm(const xchar* xpm[]) const //! noncritical
{
	uxlong xpm_ptr = 0;
	
	//read width,height,colors and chars per pixel
	uxlong width = clstring->tolong(&xpm[0][xpm_ptr]);  xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong height = clstring->tolong(&xpm[0][xpm_ptr]); xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong colors = clstring->tolong(&xpm[0][xpm_ptr]); xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong charpp = clstring->tolong(&xpm[0][xpm_ptr]); //this will work only for 1 char per pixel!!!
	if(charpp!=1) { return 0; }
	//uxlong bytesize = (width*height)<<2;
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
		if(xpm[i][xpm_ptr]=='#')
		{
			xpm_ptr++;
			currcol  = clutils->hatoi(xpm[i][xpm_ptr])<<20; xpm_ptr++; 
			currcol += clutils->hatoi(xpm[i][xpm_ptr])<<16; xpm_ptr++;
			currcol += clutils->hatoi(xpm[i][xpm_ptr])<<12; xpm_ptr++;
			currcol += clutils->hatoi(xpm[i][xpm_ptr])<<8;  xpm_ptr++;
			currcol += clutils->hatoi(xpm[i][xpm_ptr])<<4;  xpm_ptr++;
			currcol += clutils->hatoi(xpm[i][xpm_ptr]);
			ctable[cindex] = currcol;
		}
		else if( (xpm[i][xpm_ptr]=='N' || xpm[i][xpm_ptr]=='n') && (xpm[i][xpm_ptr+1]=='o' && xpm[i][xpm_ptr+2]=='n' && xpm[i][xpm_ptr+3]=='e') )
		{
			ctable[cindex] = 0xFF000000;
		}
		else return 0;
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

tileset* CLformat::loadtileset(CLfile* fp,xlong tw,xlong th) const //! noncritical
{
	sprite* sp = loadras(fp);
	tileset* r = new tileset;
	r->tilewidth = tw;
	r->tileheight = th;
	r->tilesize = tw * th;
	if( (sp->width%tw!=0) || (sp->height%th!=0) ) return 0;
	r->tilecount = (sp->width / tw) * (sp->height / th);
	r->tiledata = new uxlong*[r->tilecount];

	xlong tilesperrow = sp->width / tw;
	xlong sourceindex = 0;
	
	for(xlong i=0; i<r->tilecount; i++)
	{
		r->tiledata[i] = new uxlong[r->tilesize];
		sourceindex = ((i/tilesperrow)*sp->width*th) + ((i%tilesperrow)*tw); 
		
		for(xlong j=0; j<th; j++) { for(xlong k=0; k<tw; k++) { r->tiledata[i][j*tw+k] = sp->data[sourceindex+(j*sp->width+k)]; } }
	}
	
	return r;
}

xchar** CLformat::loadlvl(CLfile* sf) const //! noncritical
{
	xchar* bf = sf->text;
	
	//get startpositions of filenames
	xchar* fs[6] = { 0,0,0,0,0,0 };
	xlong fl[6] = { 0,0,0,0,0,0 };
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
			if(fsc>=6) break; 
		}
		else if(marks==1)
		{
			fl[fsc]++;
		}
	}
	//*
	
	//copy filenames to buffer
	xchar** fn = new xchar*[6];
	for(uxlong j=0; j<6; j++) { fn[j] = clstring->copy(fs[j],fl[j]); }
	//*
	
	return fn;
}

xmap* CLformat::loadini(CLfile* sf) const //! noncritical
{
	//inis need newline at end of file!!
	xmap* r = new xmap;
	xchar* bf = sf->text;

	//get linecount
	xlong lc = clstring->linecount(bf) - 1;
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
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!='\n')
		{
			//pre equal sign
			tc0 = cc;
			tc1=0;
			bool noinfo = 0;
			bool noequal = 0;
			while(bf[cc]!='=')
			{
				if(bf[cc]!=' ') noinfo = 1;
				if(bf[cc]=='\n' && noinfo==1) noequal = 1;
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
				while(bf[cc] != '\n')
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
		
		if(bf[cc]=='\n') { cc++; }
		else
		{
			//reset for next line
			while(bf[cc] != '\n') cc++;
			cc++;
			//*
		}
		aps=0;
	}
	
	return r;
}
///*

#endif

