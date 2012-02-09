// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZformat.hh
// Fileformat Library 

///guard
#ifndef HH_XZFORMAT
#define HH_XZFORMAT
//#pragma message "Compiling " __FILE__ "..." " TODO: AR archive reader"
///*

///includes
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZgfx.hh"
#include "XZstring.hh"
///*

///declarations
struct info
{
	char** name;
	char** value;
	sint   count;
	char*  operator[](const char* n) const {for(sint i=0;i<count;++i){if(string::find(n,name[i])==0){return value[i];}} return 0;} 
};

struct file
{
	char* name;
	char* data;
	sint  size;
	sint* files;
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
///*

///definitions 
namespace format
{
	/*OK*/ char** csv(const char* x,char y=',');	//load comma seperated values
	/*OK*/ tile*  xpm(const char** x);		//load xpm image (from resource)
	/*OK*/ tile*  xpm(const char* x);		//load xpm image (from file)
	/*OK*/ info*  ini(const char* x);		//load ini configuartion
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
		i++;
	}
	return r;
}

tile* format::xpm(const char** x)
{
	sint   index = 0;
	char** line  = string::split(x[index++],' ');

	guard(string::str2int(line[3])!=1,0);

	const sint width  = string::str2int(line[0]);
	const sint height = string::str2int(line[1]);
	const sint colors = string::str2int(line[2]);
	tile* r = new tile({width, height, new sint[width*height]});

	uint* color = new uint[256];
	for(sint i=0;i<colors;++i)
	{
		line = string::split(x[index++],' ');
		color[sint(line[0][0])] = math::set(TRANS,string::find(line[2],"None")!=0);
		color[sint(line[0][0])] = math::set(math::ndn(string::str2hex(line[2])<<4),line[2][0]=='#');
	}

	for(sint i=0,o=0;i<height;++i,++index)
	{
		for(sint j=0;j<width;++j,++o)
		{
			r->data[o] = color[sint(x[index][j])];
		}
	}

	delete[] color;
	return r;
}

tile* format::xpm(const char* x)
{
	return xpm((const char**)string::split(x,'\n'));
}

info* format::ini(const char* x) 
{
	const sint m = string::count(x,'=');
	const sint n = string::count(x,'\n');
	char** s = string::split(x,'\n');
	char** t;

	info* r = new info;
	r->count = m;
	r->name = new char*[m];
	r->value = new char*[m];

	for(sint i=0,j=0;i<n;++i)
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

///*

#endif

