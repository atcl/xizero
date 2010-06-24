///license
//atCROSSLEVEL studios 2009,2010
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
	friend class CLglobal;
	
	protected:
		CLsystem() { };
		~CLsystem() { };
	public:
		CLfile* getfile(const xchar* fn) const;
		bool    existsfile(const xchar* fn) const;
		bool    writefile(CLfile* f,bool ow=0) const;
		template<typename T>bool writefile(const xchar* fn,T* b,xlong s,bool ow=0) const;
		template<typename T>bool appendfile(const xchar* fn,T* b,xlong s) const;
		xlong   system(const xchar* c) const;
};
///*

///implementation
CLfile* CLsystem::getfile(const xchar* fn) const //! noncritical
{
	CLfile* re = new CLfile;

	FILE *of;

	//check if file exists
	if( !( of = fopen(fn,"rb") ) ) 
	{
		//delete re;
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

bool CLsystem::existsfile(const xchar* fn) const //! noncritical
{
	FILE* cf;
	if( (cf = fopen(fn,"r"))!=0 ) { fclose(cf); return 1; }
	return 0;
}

bool CLsystem::writefile(CLfile* f,bool ow) const //! noncritical
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

template<typename T>
bool CLsystem::appendfile(const xchar* fn,T* b,xlong s) const //! noncritical
{
	FILE* of;
	if( (of = fopen(fn,"ab")) == 0 ) { return 0; }
	fwrite(b,sizeof(T),s,of);
	fclose(of);
	return 1;
}

template<typename T>
bool CLsystem::writefile(const xchar* fn,T* b,xlong s,bool ow) const //! noncritical
{
	FILE* of;
	
	//if file exist and overwrite not set return
	of = fopen(fn,"rb");
	if(of && !ow) { fclose(of); return 0; } 
	fclose(of);
	//*
	
	//open for (over-)writing
	if( (of = fopen(fn,"wb")) == 0 ) { return 0; }
	fwrite(b,sizeof(T),s,of);
	fclose(of);
	//*
	
	return 1;
}

xlong CLsystem::system(const xchar* c) const { return ::system(c); } //! noncritical
///*

#endif
