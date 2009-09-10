//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAPI
#define HH_CLAPI
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLversion.hh"
#include "CLresource.hh"
#include "CLutils.hh"


//forward declaration
namespace CLsound {	void exit(); }
//*

namespace CLsystem
{
	xchar eol = '\n'; //ignore WIN32 screwed line ends
	xchar eof = 0x1A;
	
	CLexe*	exe(xchar** a);
	void    exit(xlong r,void(*e)(),const xchar* f="",const xchar* m="");
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,const xchar* d);
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,xlong d);
	CLfile* getfile(const xchar* fn,bool s=true); //CLfile as parameter and return bool for if ok?
	bool    appendfile(const xchar* fn,xlong* b,xlong s);
	bool    appendfile(const xchar* fn,xchar* b,xlong s);
	bool    writefile(const xchar* fn,xlong* b,xlong s,bool ow=0);
	bool    writefile(const xchar* fn,xchar* b,xlong s,bool ow=0);
	void    print(const xchar* c,bool i=1);
	void    print(const xlong l,bool i=1);
	void    print(const float l,bool i=1);
	void    waitforkey();
	void    wait(xlong milliseconds);
	xlong   getmilliseconds(); //since midnight
	xlong   doevery(every* e);
	xlong   system(const xchar* c);
	xlong   ato(const xchar* c); //todo: template version for return type: xlong,xfixed,float,xchar
	xlong   cmpcstr(const xchar* a,const xchar* b,xlong l=0);
	void    installsystemkey(xchar scancode,void *action);
	xlong   msgbox(const xchar* message);
};


CLexe* CLsystem::exe(xchar** a)
{
	//fill CLexe struct
	CLexe* r = new CLexe;
	r->name = a[0];
	r->title = CLtitle;
	r->size = 0;
	r->version = (CLmajor<<24)+(CLminor<<16)+(CLbuild<<8)+CLextra;
	//r->icon = &CLicon[0];
	return r;
	//*
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m)
{
	if(e!=0) e();

	std::cout << f << ": "<< m <<  std::endl;
	CLsound::exit();
	::exit(r);
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m,const xchar* d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	CLsound::exit();
	::exit(r);
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m,xlong d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	CLsound::exit();
	::exit(r);
}

CLfile* CLsystem::getfile(const xchar* fn,bool s)
{
	CLfile* re = new CLfile;

	FILE *of;

	//check if file exists
	if( !( of = fopen(fn,"rb") ) ) 
	{
		if(s==0)
		{
			delete re;
			return 0;
		}
		else
		{
			delete re;
			exit(1,0,__func__,"cannot open file",fn);
		}
	}
	//*
	
	of = fopen(fn,"rb"); //necessary?
	
	//get file size
	fseek (of,0,SEEK_END);
	re->size = (ftell(of));
	re->lsize = re->size>>2;
	//*
	
	//read file contents
	fseek (of,0,SEEK_SET );
	re->text = new xchar[(re->size)+1];
	re->data = static_cast<xlong*>(static_cast<void*>(&re->text[0]));
	fread(re->text,1,re->size,of);
	//*
	
	//set end of file marker
	re->text[(re->size)] = eof;
	//
	
	//close file
	fclose(of);
	//*

	return re;
}

bool CLsystem::appendfile(const xchar* fn,xlong* b,xlong s)
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

bool CLsystem::appendfile(const xchar* fn,xchar* b,xlong s)
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

bool CLsystem::writefile(const xchar* fn,xlong* b,xlong s,bool ow)
{
	FILE* of;
	
	//if file exist and overwrite not set return
	if( of = fopen(fn,"rb") ) 
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

bool CLsystem::writefile(const xchar* fn,xchar* b,xlong s,bool ow)
{
	FILE* of;
	
	//if file exist and overwrite not set return
	if( of = fopen(fn,"rb") ) 
	{
		fclose(of);
		if(!ow)	return 0;
	}
	//*
	
	//open for (over-)writing
	if( (of = fopen(fn,"w")) == 0 ) return 0;
	//*
	
	//write sequential data to file
	fwrite(b,1,s,of);
	//*
	
	//close file
	fclose(of);
	//*
	
	return 1;
}

void CLsystem::print(const xchar* c,bool i)
{
	//print xchar array, i decides if line end is output
	std::cout << c;

	if(i)
	{
		std::cout << std::endl;
	}
	//*
}

void CLsystem::print(const xlong l,bool i)
{
	//print xlong, i decides if line end is output
	std::cout << l;

	if(i)
	{
		std::cout << std::endl;
	}
	//*
}

void CLsystem::print(const float l,bool i)
{
	//print float, i decides if line end is output
	std::cout << l;

	if(i)
	{
		std::cout << std::endl;
	}
	//*
}

void CLsystem::waitforkey()
{
	std::cin.get(); //change to any key, now only enter
}

void CLsystem::wait(xlong milliseconds)
{
	//get current time and calc stop time
	xlong starttime = xlong(1000 * clock() / CLOCKS_PER_SEC);
	xlong stoptime = (starttime + milliseconds);
	//*

	//do nothing while waiting for stoptime
	xlong nowtime = 0;
	while(nowtime < stoptime) nowtime = xlong(1000 * clock() / CLOCKS_PER_SEC);
	//*
}

xlong CLsystem::getmilliseconds() //since midnight
{
	return xlong(1000 * clock() / CLOCKS_PER_SEC);
}

xlong CLsystem::doevery(every* e)
{
	float now = getmilliseconds();

	e->last = now;

	if(now - e->last < e->interval) return 1;

	if(e->count == e->times) return 1;

	e->function();

	e->count++;

	return 0;
}

xlong CLsystem::system(const xchar* c)
{
	return ::system(c);
}

xlong CLsystem::ato(const xchar* c)
{
	return atoi(c);
}

xlong CLsystem::cmpcstr(const xchar* a,const xchar* b,xlong l)
{
	if(l==0) return strcmp(a,b);
	else return strncmp(a,b,l);
}

void CLsystem::installsystemkey(xchar scancode,void *action)
{

}

xlong CLsystem::msgbox(const xchar* message)
{
	//commandline enframing
	const xchar* env1 = "MAIN_DIALOG=<vbox><text><label>";
	const xchar* env3 = "</label></text><button><label>[  OK  ]</label></button></vbox>";
	//*
	
	//determine length of commandline parts
	xlong env1l = CLutils::chararraylength(env1);
	xlong env2l = CLutils::chararraylength(message);
	xlong env3l = CLutils::chararraylength(env3);
	//*
	
	//allocate space for complete commandline
	xchar* env = new xchar[env1l+env2l+env3l+1];
	//*
	
	//assemble commandline
	CLutils::copychararray(&env[0],env1,env1l);
	CLutils::copychararray(&env[env1l],message,env2l);
	CLutils::copychararray(&env[env1l+env2l],env3,env3l);
	env[env1l+env2l+env3l] = 0;
	//*

	//call msgbox
	putenv(env);
	return ::system("gtkdialog --program=MAIN_DIALOG");
	//*
}

#endif
