//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAPI
#define HH_CLAPI
#warning "Compiling " __FILE__ " ! TODO: underscores everywhere"

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstring>

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLmatrix.hh"


namespace CLsystem
{

	
}


//prototypes:
void    CLexit_(xlong r,void(*e)(),const xchar *f="",const xchar *m="");
void    CLexit_(xlong r,void(*e)(),const xchar *f,const xchar *m,const xchar* d);
void    CLexit_(xlong r,void(*e)(),const xchar *f,const xchar *m,xlong d);
CLfile* CLgetfile_(const xchar* fn,bool s=true);
// xlong   CLgetfilesize_(const xchar* fn);
xchar** CLgetcsvfile_(const char* fn);
void    CLdebug_(const xchar* c,xlong v);
void    CLprint_(const xchar* c);
void    CLprint_(const xlong l);
void    CLttyout_(const xchar* c);
void    CLttyout_(const xlong l);
void    CLwaitforkey_();
void    CLwait_(xlong milliseconds);
float   CLgetmilliseconds_(); //since midnight
xlong   CLsystem_(const xchar* c);
xlong   getchararraysize_(const xchar* c);
void    installsystemkey(xchar scancode,void *action);
//

void CLexit_(xlong r,void(*e)(),const xchar *f,const xchar *m)
{
	if(e!=0) e();

	std::cout << f << ": "<< m <<  std::endl;
	exit(r);
}

void CLexit_(xlong r,void(*e)(),const xchar *f,const xchar *m,const xchar* d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	exit(r);
}

void CLexit_(xlong r,void(*e)(),const xchar *f,const xchar *m,xlong d)
{
	if(e!=0) e();

	std::cout << f << ": "<< m << " " << d << std::endl;
	exit(r);
}

CLfile* CLgetfile_(const xchar* fn,bool s)
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
			CLexit_(1,0,__func__,"cannot open file",fn);
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

// xlong CLgetfilesize_(const xchar* fn)
// {
// 	FILE *of;
// 	xlong fl;
// 
// 	if( !( of = fopen(fn,"rb") ) ) CLexit_(1,0,__func__,"cannot open file");
// 	fseek (of,0,SEEK_END);
// 	fl = (ftell(of));
// 	fclose(of);
// 
// 	return fl;
// }

xchar** CLgetcsvfile_(const char* fn)
{
	std::ifstream of;
	char **bf;
	uxlong vc = 0;

	std::string line;
	std::string value;
	of.open(fn);
	if( of.fail() ) CLexit_(1,0,__func__,"cannot open file",fn);
	
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
	if( of.fail() ) CLexit_(1,0,__func__,"cannot open file",fn);

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

void CLdebug_(const xchar* c,xlong v)
{
	std::cout << c << v << std::endl;
}

void CLprint_(const xchar* c)
{
	std::cout << c << std::endl;
}

void CLprint_(const xlong l)
{
	std::cout << l << std::endl;
}

void CLttyout_(const xchar* c)
{
	std::cout << c;
}

void CLttyout_(const xlong l)
{
	std::cout << l;
}

void CLwaitforkey_()
{
	std::cin.get(); //change to any key, now only enter
}

void CLwait_(xlong milliseconds)
{
	xlong starttime = clock() / CLOCKS_PER_SEC * 1000;
	xlong stoptime = (starttime + milliseconds) / 1000 * CLOCKS_PER_SEC;
	xlong nowtime = 0;

	do
	{
		nowtime = clock();
	} while(nowtime < stoptime);
}

//! change all seconds stuff to xlong

float CLgetmilliseconds_() //since midnight
{
	return float(1000 * clock() / CLOCKS_PER_SEC);
}

xlong CLdoevery_(every* e)
{
	float now = CLgetmilliseconds_();

	e->last = now;

	if(now - e->last < e->interval) return 1;

	if(e->count == e->times) return 1;

	e->function();

	e->count++;

	return 0;
}

xlong CLsystem_(const xchar* c)
{
	return system(c);
}

xlong getchararraysize_(const xchar* c)
{
	return xlong(strlen(c));
}

void installsystemkey(xchar scancode,void *action)
{

}

#endif
