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


namespace CLsystem
{
	void    exit(xlong r,void(*e)(),const xchar* f="",const xchar* m="");
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,const xchar* d);
	void    exit(xlong r,void(*e)(),const xchar* f,const xchar* m,xlong d);
	CLfile* getfile(const xchar* fn,bool s=true);
	xchar** getcsvfile(const char* fn);
	void    print(const xchar* c,bool i=1);
	void    print(const xlong l,bool i=1);
	void    waitforkey();
	void    wait(xlong milliseconds);
	xlong   getmilliseconds(); //since midnight
	xlong   doevery(every* e);
	xlong   system(const xchar* c);
	void    installsystemkey(xchar scancode,void *action);
};


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
	re->data = reinterpret_cast<xlong*>(&re->text[0]);
	fread(re->text,1,re->size,of);
	fclose(of);

	return re;
}

xchar** CLsystem::getcsvfile(const char* fn)
{
	std::ifstream of;
	char **bf;
	uxlong vc = 0;

	std::string line;
	std::string value;
	of.open(fn);
	if( of.fail() ) exit(1,0,__func__,"cannot open file",fn);
	
	while( getline(of,line) )
	{
		std::istringstream csvstream(line);

		while( getline(csvstream, value, ',') )
		{
			vc++;
		}

	}
	bf = new char*[vc];
	vc = 0;

	of.close();
	of.open(fn);
	if( of.fail() ) exit(1,0,__func__,"cannot open file",fn);

	while( getline(of,line) )
	{
		std::istringstream csvstream(line);

		while( getline(csvstream, value, ',') )
		{
			bf[vc] = new char[4];
			bf[vc][0] = value[0];
			bf[vc][1] = value[1];
			bf[vc][2] = value[2];
			bf[vc][3] = value[3];
			vc++;
		}

	}

	of.close();

	return bf;
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
