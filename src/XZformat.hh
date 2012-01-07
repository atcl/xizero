// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZformat.hh
// Fileformat Library 

///guard
#ifndef HH_XZFORMAT
#define HH_XZFORMAT
///*

///includes
#include "XZutil.hh"
#include "XZgfx.hh"
#include "XZstring.hh"
#include "XZvector.hh"
///*

///declarations
struct info
{
	char** name;
	char** value;
	long   count;
	char*  operator[](const char* n) const {for(long i=0;i<count;++i){if(string::find(n,name[i])==0){return value[i];}} return 0;} 
};

struct file
{
	char* name;
	char* data;
	long  size;
	long* files;
};
///*

///definitions 
namespace format
{
	/*OK*/ char** csv(const char* x,char y=',');
	/*OK*/ tile*  ras(const char* x);
	/*OK*/ tile*  xpm(const char** x);
	/*OK*/ info*  ini(const char* x);
	file* ar(const char* x,long s);
}
///*

///implementation
char** format::csv(const char* x,char y)
{
	char* s = string::repl(x,'\n',y);
	const long  l = string::count(s,y); 
	char** r = new char*[l];
	r[0] = s;
	long i = 0;
	long j = 1;
	while(s[i]!=0 && j<l)
	{
		if(s[i]==y) { r[j++] = &s[++i]; }
		i++;
	}
	string::repl(s,y,0);
	return r;
}

tile* format::ras(const char* x)
{
	const long* head  = reinterpret_cast<const long*>(x);
	const long magic  = math::ndn(head[0]);
	const long width  = math::ndn(head[1]);
	const long height = math::ndn(head[2]);
	const long depth  = math::ndn(head[3]);
	//const long length = math::ndn(head[4]);
	const long type   = math::ndn(head[5]);
	if(magic!=0x59A66A95 || (depth!=32 && depth!=24) || (type!=0 && type!=1 && type!=2) || head[6]!=0 || head[7]!=0) { return 0; }

	const unsigned char* source = reinterpret_cast<const unsigned char*>(&(x[32]));
	const long size = (width*height)<<2;

	tile* r = new tile;
	r->width = width;
	r->height = height;
	r->data = new long[width*height];
	unsigned char* data = reinterpret_cast<unsigned char*>(r->data);

	if(type==2) //run-length encoded
	{
		long src = 0;
		long dst = 0;
	
		while(dst<size)
		{
			if(source[src]==0x80) //is control symbol
			{
				src++;
				const unsigned char run = source[src]; src++; //run-length

				if(run!=0) //default
				{
					const unsigned char cur = source[src]; src++; //run-value

					for(long i=0;i<run;++i)
					{
						//i += (width%2==1 && dst%(width<<2)==1);
						data[dst] = 0;
						dst += (depth==24 && (dst%4)==3);
						data[dst] = cur; dst++;
					}
					dst++;
				}
				else //color byte equals control symbol
				{
					data[dst] = 0;
					dst += (depth==24 && (dst%4)==3);
					data[dst] = 0x80; dst++; 
				}
			}
			else //uncompressed byte
			{
				data[dst] = 0;
				dst += (depth==24 && (dst%4)==3);
				data[dst] = source[src]; src++; dst++;
			}
		}
	}
	else //uncompressed
	{
		long i = 0;
		long j = 0;
		long k = 0;
		while(i<size)
		{
			data[i] = source[j]; i++; j++;
			data[i] = source[j]; i++; j++;
			data[i] = source[j]; i++; j++;
			data[i] = 0; i++; 
			j += ((width%2)==1 && k==width-1);
			k = math::set(0,k+1,k==width-1);	
		}
	}

	return r;
}

tile* format::xpm(const char** x)
{
	long   index = 0;
	char** line  = string::split(x[index++],' ');

	guard(string::conl(line[3])!=1,0);

	const long width  = string::conl(line[0]);
	const long height = string::conl(line[1]);
	const long colors = string::conl(line[2]);
	long* data = new long[width*height];
	tile* r = new tile;
	r->width = width;
	r->height = height;
	r->data = data;

	unsigned long* color = new unsigned long[256];
	for(long i=0;i<colors;++i)
	{
		line = string::split(x[index++],' ');
		if(string::find(line[2],"None")!=0)
		{
			color[line[0][0]] = TRANS; 
		}
		if(line[2][0]=='#')
		{
			for(long j=1;j<7;++j)
			{
				const char curr = line[2][j];
				unsigned long temp = curr;
				temp -= math::set(48,(curr>=48 && curr<=57));
				temp -= math::set(55,(curr>=65 && curr<=70));
				temp -= math::set(87,(curr>=97 && curr<=102));
				temp += color[line[0][0]];
				temp <<= 4;
				color[line[0][0]] = temp;
			}
			color[line[0][0]] <<= 4;
			color[line[0][0]] = math::ndn(color[line[0][0]]);
		}
	}

	for(long i=0,o=0;i<height;++i,++index)
	{
		for(long j=0;j<width;++j,++o)
		{
			data[o] = color[x[index][j]];
		}
	}

	delete[] color;
	return r;
}

info* format::ini(const char* x) 
{
	const long m = string::count(x,'=');
	const long n = string::count(x,'\n');
	char** s = string::split(x,'\n');
	char** t;

	info* r = new info;
	r->count = m;
	r->name = new char*[m];
	r->value = new char*[m];

	for(long i=0,j=0;i<n;++i)
	{
		if(string::count(s[i],'=')!=0)
		{
			t = string::split(s[i],'=');
			r->name[j] = string::trim(t[0]);
			r->value[j] = string::trim(t[1]);
			j++;
		}
	}

	return r;
}

file* format::ar(const char* x,long s)
{
	//ar can contain max 127 files!!!
	file* r = new file[128];

	//check for "magic-string"
	if(x[0]=='!'&&x[1]=='<'&&x[2]=='a'&&x[3]=='r'&&x[4]=='c'&&x[5]=='h'&&x[6]=='>'&&x[7]=='<')
	{
		//init variables
		long  bc = 8;
		long* fc = new long[1];
		//*

		//check for illegal mem access
		while(bc<s)
		{
			//read member header
			char* fn = string::copy(&x[bc],16);	//member filename
			bc += 48;	//no necessary information here, so skip
			//*
			
			//decode filesize of current ar member
			const long fs = string::conl(&x[bc]) + 1;
			bc += 12; //goto end of header
			//*

			//create char array for current ar member
			char* dat = new char[fs];
			for(long i=0;i<fs;i++,bc++) { dat[i] = x[bc]; } //? invalid read of size 4 here
			//*

			//make new armember
			r[*fc].name = fn;
			r[*fc].data = dat;
			r[*fc].size = fs;
			r[*fc].files = fc;
			//*

			//adjust global ar variables
			bc += (fs&1)!=0; //increment source ptr if filesize is odd
			(*fc)++; //increment filecount
			//*
		}
		//*
	}
	else return 0;
}
///*

#endif

