///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFORMAT
#define HH_CLFORMAT
///*

///sys includes
#include <map>
///*

///idp includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLutils.hh"
///*

///api includes
#include "CLbase.hh"
#include "CLstring.hh"
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

class CLformat : public CLbase<CLformat,1>
{
	friend class CLbase<CLformat,1>;
	
	private:
		static CLstring* clstring;
	protected:
		CLformat() { };
		~CLformat() { };
	public:
		xlong*   loadcsv(CLfile* sf,xchar sep=',') const;
		xchar**  loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const;
		sprite*  loadras(CLfile* sf) const;
		sprite*  loadxpm(const xchar* xpm[]) const;
		sprite*  loadxpm(const xchar* xpm) const;
		sprite** loadtileset(CLfile* sf,xlong tw,xlong th) const;
		xchar**  loadlvl(CLfile* sf) const;
		xmap*    loadini(CLfile* bf) const;
		
		CLfile*  saveras(sprite* sp,const xchar* fn="test.ras") const;
};

CLstring* CLformat::clstring = CLstring::instance();
///*

///implementation
xlong* CLformat::loadcsv(CLfile* sf,xchar sep) const //! noncritical
{
	xchar* bf = sf->text;
	
	//get linecount
	xlong lc = clstring->linecount(bf);
	//*

	//get comma count
	xlong cc = 0;
	xlong co = 0;
	while(cc<sf->size)
	{
		if(bf[cc]==sep) { co++; }
		cc++;
	}
	//*

	//get value count
	xlong vc = co + lc;
	//*
	
	//copy values
	xlong* r = new xlong[vc+1];	
	xlong tvc = 1; 
	cc = 0;
	while(tvc<=vc && cc<sf->size)
	{
		r[tvc] = clstring->tolong(&bf[cc]);
		tvc++;
		while(bf[cc]!=sep && bf[cc]!='\n' && cc<sf->size) { cc++; }
		cc++;
	}
	r[0] = tvc-1;
	//*

	//r is now array of xlongs, r[0] is count of values
	return r;
	//*
}
xchar** CLformat::loadmap(CLfile* sf,xlong subconst,xchar rc,xlong rv) const //! noncritical
{
	xchar* bf = sf->text;
	xlong  lc = clstring->linecount(bf);
	
	//determine line length
	xlong lw = 0;
	xlong cc = 0;
	while(bf[cc]!='\n') { lw++; cc++; }
	//*

	//for each row create xchar array of line length
	xchar** rev = new xchar*[lc];
	for(xlong i=0; i<lc; i++) { rev[i] = new xchar[lw]; }
	//*

	//copy and manipulate contents depending on subconst,rc,rv
	cc = 0;
	for(xlong j=0; j<lc; j++)
	{
		for(xlong k=0; k<lw; k++)
		{
			if(bf[cc]!='\n')
			{
				if(bf[cc]==rc) { rev[j][k] = rv; }
				else { rev[j][k] = bf[cc] - subconst; }
			}
			else
			{
				err(__FILE__,__func__,u8"Map not conform with given width");
				return 0;
			}			
			cc++;
		}
		cc++;
	}
	//*

	return rev;
}

sprite* CLformat::loadras(CLfile* sf) const //! noncritical
{
	xlong* lf = sf->data;

	if(endian(lf[0])!=0x59A66A95) return 0; //test for magic number
	xlong width = endian(lf[1]);
	xlong height = endian(lf[2]);
	xlong depth = endian(lf[3]);
	if(depth!=32 && depth!=24) return 0; //only 24bpp and 32bpp!
	xlong length = endian(lf[4]);
	xlong type = endian(lf[5]);
	if(type!=0 && type!=1 && type!=2) return 0; //only types: old, standard and rle are supported
	if(endian(lf[6])!=0) return 0; //color maps are not supported
	if(endian(lf[7])!=0) return 0; //color maps are not supported
	
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
	r->index = 0;
	r->count = 1;
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
	//*
	
	//allocate
	sprite* r = new sprite;
	r->width = width;
	r->height = height;
	r->size = width*height;
	r->index = 0;
	r->count = 1;
	uxlong* data = new uxlong[r->size];
	r->data = data;
	//*
	
	//read color table and set image buffers colors
	colors++;
	uxlong ctable[256];
	uxchar cindex = 0;
	uxlong currcol = 0;
	for(xlong i=1; i<colors; i++)
	{
		xpm_ptr = 0;
		cindex = xpm[i][0];
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') xpm_ptr++;
		xpm_ptr++;
		if(xpm[i][xpm_ptr]!='c' && xpm[i][xpm_ptr]!='C') return 0;
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') { xpm_ptr++; }
		xpm_ptr++;
		if(xpm[i][xpm_ptr]=='#') { xpm_ptr++; ctable[cindex] = clstring->hex(&(xpm[i][xpm_ptr])); }
		else if( (xpm[i][xpm_ptr]=='N' || xpm[i][xpm_ptr]=='n') && (xpm[i][xpm_ptr+1]=='o' && xpm[i][xpm_ptr+2]=='n' && xpm[i][xpm_ptr+3]=='e') ) { ctable[cindex] = 0xFF000000; }
		else return 0;
	}
	//*

	//fill data
	uxlong data_ptr = 0; 
	height += colors;
	for(xlong j=colors; j<height; j++) { for(xlong k=0; k<width; k++,data_ptr++) { r->data[data_ptr] = ctable[xpm[j][k]]; } }
	//*

	return r;
}

sprite** CLformat::loadtileset(CLfile* fp,xlong tw,xlong th) const //! noncritical
{
	sprite* sp = loadras(fp);
	if( (sp->width%tw!=0) || (sp->height%th!=0) ) return 0;
	uxlong tilecount = (sp->width / tw) * (sp->height / th);
	uxlong tilesize = th*tw;
	sprite** r = new sprite*[tilecount];

	xlong tilesperrow = sp->width / tw;
	xlong sourceindex = 0;
	
	for(xlong i=0; i<tilecount; i++)
	{
		r[i] = new sprite;
		r[i]->width = tw;
		r[i]->height = th;
		r[i]->size = tilesize;
		r[i]->index = i;
		r[i]->count = tilecount;
		r[i]->data = new uxlong[tilesize];
		sourceindex = ((i/tilesperrow)*sp->width*th) + ((i%tilesperrow)*tw); 
		
		for(xlong j=0; j<th; j++) { for(xlong k=0; k<tw; k++) { r[i]->data[j*tw+k] = sp->data[sourceindex+(j*sp->width+k)]; } }
	}
	
	return r;
}

xchar** CLformat::loadlvl(CLfile* sf) const //! noncritical
{
	xchar* bf = sf->text;
	
	//get startpositions of filenames
	xchar* fs[5] = { 0,0,0,0,0 };
	xlong fl[5] = { 0,0,0,0,0 };
	bool marks = 0;
	xlong fsc = 0;
	for(xlong i=0; i<sf->size; i++)
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
	for(xlong j=0; j<5; j++) { fn[j] = clstring->copy(fs[j],fl[j]); }
	//*
	
	return fn;
}

xmap* CLformat::loadini(CLfile* sf) const //! noncritical
{
	//inis need newline at end of file!!
	xmap* r = new xmap;
	xchar* bf = sf->text;

	//get linecount
	xlong lc = clstring->linecount(bf);
	//*
	
	xlong cc = 0;
	for(xlong i=0; i<lc; i++)
	{
		while(bf[cc]==' ') { cc++; }
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!='\n' && cc<sf->size)
		{
			//pre equal sign
			xlong prestart = cc;
			xlong prestop = cc;
			xchar* prevalue = 0;
			while(bf[cc]!='=' && bf[cc]!=' ')
			{
				prestop++;
				cc++;
			}
			prevalue = clstring->copy(&(bf[prestart]),prestop-prestart);
			//*

			//equal sign
			while(bf[cc]!='=') { cc++; }
			cc++;
			while(bf[cc]==' ') { cc++; }
			//*
			
			//post equal sign
			xlong poststart = cc;
			xlong poststop = cc;
			xchar* postvalue = 0;
			while(bf[cc]!=' ' && bf[cc]!=';' && bf[cc]!='#' && bf[cc]!='\n')
			{
				if(bf[cc]!=' ') { poststop++; }
				cc++;
			}
			if( (bf[poststart]=='"' && bf[poststop]=='"') || (bf[poststart]=='\'' && bf[poststop]=='\'') )
			{
				poststart++;
				poststop--;
			}
			postvalue = clstring->copy(&(bf[poststart]),poststop-poststart);
			//*
				
			//map values in xmap
			(*r)[prevalue] = postvalue;
			//*
		}
		
		while(bf[cc]!='\n' && cc<sf->size) { cc++; }
		cc++;
	}
	
	return r;
}

CLfile* CLformat::saveras(sprite* sp,const xchar* fn) const //! noncritical
{
	CLfile* r = new CLfile;
	r->name = clstring->copy(fn);
	r->lsize = (sp->size) + 8;
	r->size = (r->lsize) * 4;
	r->text = new xchar[(r->size)];
	r->data = static_cast<xlong*>(static_cast<void*>(&r->text[0]));
	
	r->data[0] = 0x956aa659;
	r->data[1] = endian(sp->width);
	r->data[2] = endian(sp->height);
	r->data[3] = endian(32);
	r->data[4] = endian(sp->size);
	r->data[5] = endian(1);
	r->data[6] = 0;
	r->data[7] = 0;
	
	for(xlong i=0; i<sp->size; i++) { r->data[8+i] = endian(sp->data[i]); }
	
	return r;
}
///*

#endif

