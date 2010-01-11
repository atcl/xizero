///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLAPI
#define HH_CLAPI
///*

///sys includes
#include <stdlib.h>
#include <stdio.h>
///*

///idp includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLresource.hh"
#include "CLutils.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLsystem
 * 
 * description:	Takes care of all kinds of os dependent functionality
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLsystem : public CLbase<CLsystem,1>
{
	friend class CLbase<CLsystem,1>;
	
	protected:
		CLsystem() { };
		~CLsystem() { };
	public:
		CLfile* getfile(const xchar* fn);
		bool    appendfile(const xchar* fn,xlong* b,xlong s);
		bool    appendfile(const xchar* fn,xchar* b,xlong s);
		bool    writefile(const xchar* fn,xlong* b,xlong s,bool ow=0);
		bool    writefile(const xchar* fn,xchar* b,xlong s,bool ow=0);
		bool    writefile(CLfile* f,bool ow=0);
		xlong   system(const xchar* c);
};
///*

///implementation
CLfile* CLsystem::getfile(const xchar* fn) //! noncritical
{
	CLfile* re = new CLfile;

	FILE *of;

	//check if file exists
	if( !( of = fopen(fn,"rb") ) ) 
	{
		delete re;
		tty(__func__);
		say(fn);
		return 0;
	}
	//*

	//get file size
	fseek (of,0,SEEK_END);
	re->size = (ftell(of));
	if(re->size==0) //recheck if filesize seems to be zero (ie proc/meminfo or alike)
	{
		uxlong n=0;
		while(!feof(of)) { fgetc(of); n++; }
		re->size = n;
	}
	xlong mr = 4 - (re->size%4);
	if(mr!=0) { re->size += mr; }
	re->lsize = re->size>>2;
	//*

	//read file contents
	fseek (of,0,SEEK_SET);
	re->text = new xchar[((re->size)+4)];
	re->data = static_cast<xlong*>(static_cast<void*>(&re->text[0]));
	fread(re->text,1,re->size,of);
	//*

	//set end of file marker
	re->text[(re->size)] = 0;
	re->text[(re->size)+1] = 0;
	re->text[(re->size)+2] = 0;
	re->text[(re->size)+3] = 0x1A;
	//

	//close file
	fclose(of);
	//*

	return re;
}

bool CLsystem::appendfile(const xchar* fn,xlong* b,xlong s) //! noncritical
{
	FILE* of;
	
	//open for appending
	if( (of = fopen(fn,"ab")) == 0 ) return 0;
	//*
	
	//write binary data to file
	fwrite(b,4,s,of);
	//*
	
	//close file
	fclose(of);
	//*
	
	return 1;
}

bool CLsystem::appendfile(const xchar* fn,xchar* b,xlong s) //! noncritical
{
	FILE* of;
	
	//open for appending
	if( (of = fopen(fn,"a")) == 0 ) return 0;
	//*
	
	//write sequential data to file
	fwrite(b,1,s,of);
	//*
	
	//close file
	fclose(of);
	//*
	
	return 1;
}

bool CLsystem::writefile(const xchar* fn,xlong* b,xlong s,bool ow) //! noncritical
{
	FILE* of;
	
	//if file exist and overwrite not set return
	if(of = fopen(fn,"rb") ) 
	{
		fclose(of);
		if(!ow)	return 0;
	}
	//*
	
	//open for (over-)writing
	if( (of = fopen(fn,"wb")) == 0 ) return 0;
	//*
	
	//write binary data to file
	fwrite(b,4,s,of);
	//*
	
	//close file
	fclose(of);
	//*
	
	return 1;
}

bool CLsystem::writefile(const xchar* fn,xchar* b,xlong s,bool ow) //! noncritical
{
	FILE* of;
	
	//if file exist and overwrite not set return
	if(of = fopen(fn,"rb") ) 
	{
		fclose(of);
		if(!ow)	return 0;
	}
	//*
	
	//open for (over-)writing
	if( (of = fopen(fn,"wb")) == 0 ) return 0;
	//*
	
	//write sequential data to file
	fwrite(b,1,s,of);
	//*
	
	//close file
	fclose(of);
	//*
	
	return 1;
}

bool CLsystem::writefile(CLfile* f,bool ow) //! noncritical
{
	FILE* of;
	
	//if file exist and overwrite not set return
	of = fopen(f->name,"rb");
	if(of && !ow) { fclose(of); return 0; } 
	fclose(of);
	//*
	
	//open for (over-)writing
	of = fopen(f->name,"wb");
	fwrite(f->text,1,f->size,of);
	fclose(of);
	//*
	
	return 1;
}

xlong CLsystem::system(const xchar* c) { return ::system(c); } //! noncritical
///*

#endif
