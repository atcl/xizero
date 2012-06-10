// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZformat.hh
// Fileformat Library 

///guard
#ifndef HH_XZFORMAT
#define HH_XZFORMAT
//#pragma message "Compiling " __FILE__ "..." " TODO:"
///*

///includes
#include "XZbasic.hh"
#include "XZgfx.hh"
#include "XZstring.hh"
///*

///declarations
struct info
{
	char** name;
	char** value;
	sint   count;
	char*  operator[](const char* n) const
	{
		for(sint i=0;i<count;++i)
		{
			if(string::find(n,name[i])==0) { return value[i]; }
		}
		return 0;
	} 
};

#ifndef TILE
#define TILE
struct tile
{
	sint  width;
	sint  height;
	sint* data;
};
#endif

struct file
{
	const char* name;
	const char* data;
	const uint  size;
	const uint  count;
};
///*

///definitions 
namespace format
{
	/*OK*/ char** csv(const char* x,char y=',');	//load comma seperated values
	/*OK*/ tile*  xpm(const char* x);		//load xpm image
	/*OK*/ info*  ini(const char* x);		//load ini configuartion
	       file** ar(char* x);			//load ar archive
}
///*

///implementation
char** format::csv(const char* x,char y)
{
	char* s = string::repl(x,'\n',y);
	const sint l = string::count(s,y);
	char** r = new char*[l];
	r[0] = s;
	sint i = 0;
	sint j = 1;
	while(s[i]!=0 && j<l)
	{
		if(s[i]==y) { s[i] = 0; r[j++] = &s[++i]; }
		++i;
	}
	return r;
}

tile* format::xpm(const char* x)
{
	sint   index = 0;
	char** y     = string::split(x,'\n');
	char** line  = string::split(y[index++],' ');

	guard(string::str2int(line[3])!=1,0);

	const sint width  = string::str2int(line[0]);
	const sint height = string::str2int(line[1]);
	const sint colors = string::str2int(line[2]);
	tile* r = new tile({width, height, new sint[width*height]});

	uint* color = new uint[256];
	for(sint i=0;i<colors;++i)
	{
		line = string::split(y[index++],' ');
		color[sint(line[0][0])] = math::set(string::str2hex(line[2])>>4,TRANS,line[2][0]=='#'); 
	}

	for(sint i=0,o=0;i<height;++i,++index)
	{
		for(sint j=0;j<width;++j,++o)
		{
			r->data[o] = color[sint(y[index][j])];
		}
	}

	delete y;
	delete line; 
	delete[] color;
	return r;
}

info* format::ini(const char* x) 
{
	const sint m = string::count(x,'=');
	const sint n = string::count(x,'\n');
	char** s = string::split(x,'\n');

	info* r = new info;
	r->count = m;
	r->name = new char*[m];
	r->value = new char*[m];

	for(sint i=0,j=0;i<n;++i)
	{
		if(string::count(s[i],'=')!=0)
		{
			char** t = string::split(s[i],'=');
			r->name[j] = string::trim(t[0]);
			r->value[j] = string::trim(t[1]);
			++j;
		}
	}

	return r;
}

file** format::ar(char* x)
{
	//Read magic number
	guard(string::find(x,"!<arch>\n")<0,0);
	//*

	//Count files
	uint c = 0;
	uint t = 8;
	do
	{
		t += 48;
		const uint s = string::str2int(&x[t]);
		t += 12+s+(s&1);
		++c;	
	}
	while(x[t+58]=='`');
	file** r = new file*[c];
	//*

	//Unpack
	t = 8;
	for(uint i=0;i<c;++i)
	{
		const char* n = &x[t];
		t += 16;
		for(sint j=0;j<16;++j) { if(x[t-j]=='/') { x[t-j]=0; break; } }
		t += 32;
		const uint s = string::str2int(&x[t]);
		t += 12;
		const char* d = &x[t];
		t += s+(s&1);
		r[i] = new file{ n,d,s,c };
	}
	//*

	return r;
}
///*

#endif

