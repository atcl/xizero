///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Fileformat Library ( XZformat.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZtile.hh"
#include "XZstring.hh"
///</include>

///<define>
struct info
{
	char** name;
	char** data;
	yint*  size;
	const  yint count;
	char*  operator[](const char* n) const
	{
		for(yint i=0;i<count;++i)
		{
			if(string::find(n,name[i])!=-1) { return data[i]; }
		}
		return 0;
	} 
};

namespace format
{
	char** csv(const char* x,char y=',');	// load comma seperated values
	tile   xpm(const char* x);		// load xpm image
	info   ini(const char* x);		// load ini configuartion
	info   ar(char* x);			// load ar archive
}
///</define>

///<code>
char** format::csv(const char* x,char y)
{
	char* s = string::repl(x,'\n',y);
	return string::split(s,y);
}

tile format::xpm(const char* x)
{
	//split by line ends
	yint   index = 0;
	char** y     = string::split(x,'\n');
	char** line  = string::split(y[index++],' ');

	//get image dimensions
	const yint width  = string::str2int(line[0]);
	const yint height = string::str2int(line[1]);
	const yint colors = string::str2int(line[2]);
	tile r(width,height);

	//read color table
	yint* color = new uint[256];
	for(yint i=0;i<colors;++i)
	{
		line = string::split(y[index++],' ');
		color[xint(line[0][0])] = math::set(string::hex2int(line[2])>>4,TRANS,line[2][0]=='#'); 
	}

	//read image data
	for(yint i=0,o=0;i<height;++i,++index)
	{
		for(yint j=0;j<width;++j,++o)
		{
			r.data[o] = color[xint(y[index][j])];
		}
	}

	//clean up
	delete y;
	delete line; 
	delete[] color;

	return r;
}

info format::ini(const char* x)
{
	//split by equal signs and line ends
	const yint m = string::count(x,'=');
	char** s = string::split(x,'\n');

	//create name value container
	info r{ new char*[m],new char*[m],0,m };

	//extract names and values
	for(yint i=0,j=0;i<m;++i)
	{
		if(string::count(s[i],'=')!=0)
		{
			char** t = string::split(s[i],'=');
			r.name[j] = string::trim(t[0]);
			r.data[j] = string::trim(t[1]);
			++j;
		}
	}

	return r;
}

info format::ar(char* x)
{
	//Read magic number
	//guard(string::find(x,"!<arch>\n")<0,0);

	//Count files
	yint c = 0;
	yint t = 8;
	do
	{
		t += 48;
		const xint s = string::str2int(&x[t]);
		t += 12+s+(s&1);
		++c;	
	}
	while(x[t+58]=='`');
	info r{ new char*[c],new char*[c],new yint[c],c };

	//Unpack
	t = 8;
	for(yint i=0;i<c;++i)
	{
		r.name[i] = &x[t];
		t += 16;
		for(xint j=0;j<16;++j) { if(x[t-j]=='/') { x[t-j]=0; break; } }
		t += 32;
		const yint s = r.size[i] = string::str2int(&x[t]);
		t += 12;
		r.data[i] = &x[t];
		t += s+(s&1);
		x[t-1] = 0;
	}

	return r;
}
///</code>

