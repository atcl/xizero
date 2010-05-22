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
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
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
		xlong  readlong(xchar* f,xchar*& r) const;
		float   readfloat(xchar* f,xchar*& r) const;
		xchar*   readstring(xchar* f,xchar*& r) const;
	public:
		xlong*   loadcsv(CLfile* f,xchar s=',') const;
		sprite*  loadras(CLfile* f) const;
		sprite*  loadxpm(const xchar* x[]) const;
		sprite*  loadxpm(const xchar* x) const;
		sprite** loadtileset(CLfile* f,xlong w,xlong h) const;
		xchar**  loadlvl(CLfile* f) const;
		xmap*    loadini(CLfile* f) const;
		
		CLfile*  saveras(sprite* s,const xchar* n="test.ras") const;
};

CLstring* CLformat::clstring = CLstring::instance();
///*

///implementation
xlong CLformat::readlong(xchar* f,xchar*& r) const //! noncritical
{
	xlong p = clstring->scan(f);
	r = &f[p];
	return clstring->tolong(f);
}

float CLformat::readfloat(xchar* f,xchar*& r) const //! noncritical
{
	xlong p = clstring->scan(f);
	r = &f[p];
	return clstring->tofloat(f);
}

xchar* CLformat::readstring(xchar* f,xchar*& r) const //! noncritical
{
	xlong p = clstring->scan(f);
	r = &f[p];
	return clstring->copy(f,p);
}

xlong* CLformat::loadcsv(CLfile* f,xchar s) const //! noncritical
{
	xchar* bf = f->text;
	
	//get linecount
	xlong lc = clstring->linecount(bf);
	//*

	//get comma count
	xlong cc = 0;
	xlong co = 0;
	while(cc<f->size)
	{
		if(bf[cc]==s) { co++; }
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
	while(tvc<=vc && cc<f->size)
	{
		r[tvc] = clstring->tolong(&bf[cc]);
		tvc++;
		while(bf[cc]!=s && bf[cc]!='\n' && cc<f->size) { cc++; }
		cc++;
	}
	r[0] = tvc-1;
	//*

	//r is now array of xlongs, r[0] is count of values
	return r;
	//*
}

sprite* CLformat::loadras(CLfile* f) const //! noncritical
{
	xlong* lf = f->data;

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
	
	xchar* bf = &((f->text)[32]);
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

sprite* CLformat::loadxpm(const xchar* x[]) const //! noncritical
{
	uxlong xpm_ptr = 0;
	
	//read width,height,colors and chars per pixel
	uxlong width = clstring->tolong(&x[0][xpm_ptr]);  xpm_ptr++; while( (x[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong height = clstring->tolong(&x[0][xpm_ptr]); xpm_ptr++; while( (x[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong colors = clstring->tolong(&x[0][xpm_ptr]); xpm_ptr++; while( (x[0][xpm_ptr]) !=' ') { xpm_ptr++; }
	uxlong charpp = clstring->tolong(&x[0][xpm_ptr]); //this will work only for 1 char per pixel!!!
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
		cindex = x[i][0];
		xpm_ptr++; while( (x[i][xpm_ptr]) !=' ') { xpm_ptr++; }
		xpm_ptr++;
		if(x[i][xpm_ptr]!='c' && x[i][xpm_ptr]!='C') { return 0; }
		xpm_ptr++; while( (x[i][xpm_ptr]) !=' ') { xpm_ptr++; }
		xpm_ptr++;
		if(x[i][xpm_ptr]=='#') { xpm_ptr++; ctable[cindex] = clstring->hex(&(x[i][xpm_ptr])); }
		else if( (x[i][xpm_ptr]=='N' || x[i][xpm_ptr]=='n') && (x[i][xpm_ptr+1]=='o' && x[i][xpm_ptr+2]=='n' && x[i][xpm_ptr+3]=='e') ) { ctable[cindex] = 0xFF000000; }
		else return 0;
	}
	//*

	//fill data
	uxlong data_ptr = 0; 
	height += colors;
	for(xlong j=colors; j<height; j++) { for(xlong k=0; k<width; k++,data_ptr++) { r->data[data_ptr] = ctable[x[j][k]]; } }
	//*

	return r;
}

sprite** CLformat::loadtileset(CLfile* f,xlong w,xlong h) const //! noncritical
{
	sprite* sp = loadras(f);
	if( (sp->width%w!=0) || (sp->height%h!=0) ) return 0;
	uxlong tilecount = (sp->width / w) * (sp->height / h);
	uxlong tilesize = h*w;
	sprite** r = new sprite*[tilecount];

	xlong tilesperrow = sp->width / w;
	xlong sourceindex = 0;
	
	for(xlong i=0; i<tilecount; i++)
	{
		r[i] = new sprite;
		r[i]->width = w;
		r[i]->height = h;
		r[i]->size = tilesize;
		r[i]->index = i;
		r[i]->count = tilecount;
		r[i]->data = new uxlong[tilesize];
		sourceindex = ((i/tilesperrow)*sp->width*h) + ((i%tilesperrow)*w); 
		
		for(xlong j=0; j<h; j++) { for(xlong k=0; k<w; k++) { r[i]->data[j*w+k] = sp->data[sourceindex+(j*sp->width+k)]; } }
	}
	
	return r;
}

xchar** CLformat::loadlvl(CLfile* f) const //! noncritical
{
	xchar* bf = f->text;
	
	//get startpositions of filenames
	xchar* fs[5] = { 0,0,0,0,0 };
	xlong fl[5] = { 0,0,0,0,0 };
	bool marks = 0;
	xlong fsc = 0;
	for(xlong i=0; i<f->size; i++)
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
			if(fsc>=5) { break; } 
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

xmap* CLformat::loadini(CLfile* f) const //! noncritical
{
	//inis need newline at end of file!!
	xmap* r = new xmap;
	xchar* bf = f->text;

	//get linecount
	xlong lc = clstring->linecount(bf);
	//*
	
	xlong cc = 0;
	for(xlong i=0; i<lc; i++)
	{
		while(bf[cc]==' ') { cc++; }
		
		if(bf[cc]!=';' && bf[cc]!='#' && bf[cc]!='\n' && cc<f->size)
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
		
		while(bf[cc]!='\n' && cc<f->size) { cc++; }
		cc++;
	}
	
	return r;
}

CLfile* CLformat::saveras(sprite* s,const xchar* n) const //! noncritical
{
	CLfile* r = new CLfile;
	r->name = clstring->copy(n);
	r->lsize = (s->size) + 8;
	r->size = (r->lsize) * 4;
	r->text = new xchar[(r->size)];
	r->data = static_cast<xlong*>(static_cast<void*>(&r->text[0]));
	
	r->data[0] = 0x956aa659;
	r->data[1] = endian(s->width);
	r->data[2] = endian(s->height);
	r->data[3] = endian(32);
	r->data[4] = endian(s->size);
	r->data[5] = endian(1);
	r->data[6] = 0;
	r->data[7] = 0;
	
	for(xlong i=0; i<s->size; i++) { r->data[8+i] = endian(s->data[i]); }
	
	return r;
}
///*

#endif

