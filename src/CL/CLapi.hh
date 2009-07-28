//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAPI
#define HH_CLAPI
#pragma message "Compiling " __FILE__ " ! TODO: underscores everywhere"

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstring>

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLversion.hh"
#include "CLresource.hh"


namespace CLsystem
{
	xchar eol = '\n'; //change for WIN32
	
	CLexe*	exe(xchar** a);
	void    exit(xlong r,void(*e)(),const xchar* f="",const xchar* m="");
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,const xchar* d);
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,xlong d);
	CLfile* getfile(const xchar* fn,bool s=true);
	void    print(const xchar* c,bool i=1);
	void    print(const xlong l,bool i=1);
	void    print(const float l,bool i=1);
	void    waitforkey();
	void    wait(xlong milliseconds);
	xlong   getmilliseconds(); //since midnight
	xlong   doevery(every* e);
	xlong   system(const xchar* c);
	void    installsystemkey(xchar scancode,void *action);
};


CLexe* CLsystem::exe(xchar** a)
{
	CLexe* r = new CLexe;
	r->name = a[0];
	r->title = CLtitle;
	r->size = 0;
	r->version = (CLmajor<<24)+(CLminor<<16)+(CLbuild<<8)+CLextra;
	r->icon = &CLicon[0];
	r->splash = &CLsplash[0];
	return r;
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m)
{
	if(e!=0) e();

	std::cout << f << ": "<< m <<  std::endl;
	::exit(r);
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m,const xchar* d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	::exit(r);
}

void CLsystem::exit(xlong r,void(*e)(),const xchar *f,const xchar *m,xlong d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	::exit(r);
}

CLfile* CLsystem::getfile(const xchar* fn,bool s)
{
	CLfile* re = new CLfile;

	FILE *of;

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
	of = fopen(fn,"rb");
	fseek (of,0,SEEK_END);
	re->size = (ftell(of));
	re->lsize = re->size>>2;
	fseek (of,0,SEEK_SET );
	re->text = new xchar[re->size];
	re->data = reinterpret_cast<xlong*>(&re->text[0]); //!
	fread(re->text,1,re->size,of);
	fclose(of);

	return re;
}

void CLsystem::print(const xchar* c,bool i)
{
	std::cout << c;

	if(i)
	{
		std::cout << std::endl;
	}
}

void CLsystem::print(const xlong l,bool i)
{
	std::cout << l;

	if(i)
	{
		std::cout << std::endl;
	}
}

void CLsystem::print(const float l,bool i)
{
	std::cout << l;

	if(i)
	{
		std::cout << std::endl;
	}
}

void CLsystem::waitforkey()
{
	std::cin.get(); //change to any key, now only enter
}

void CLsystem::wait(xlong milliseconds)
{
	xlong starttime = clock() / CLOCKS_PER_SEC * 1000;
	xlong stoptime = (starttime + milliseconds) / 1000 * CLOCKS_PER_SEC;
	xlong nowtime = 0;

	do
	{
		nowtime = clock();
	} while(nowtime < stoptime);
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

void CLsystem::installsystemkey(xchar scancode,void *action)
{

}

#endif
