//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAPI
#define HH_CLAPI
#pragma message "Compiling " __FILE__ " ! TODO: underscores everywhere"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

void CLexit_(const char *f,const char *m, xlong r)
{
	cout << f << ": "<< m << endl;
	exit(r);
}

xlong CLgetfilesize_(const char* fn)
{
	FILE *of;
	uxlong fl;

	if( !( of = fopen(fn,"rb") ) ) CLexit_(__func__,"cannot open file",1);
	fseek (of,0,SEEK_END);
	fl = (ftell(of));
	fclose(of);

	return fl;
}

xlong* CLgetfile_(const char* fn)
{
	FILE *of;
	xlong *bf;
	uxlong fl;

	if( !( of = fopen(fn,"rb") ) ) CLexit_(__func__,"cannot open file",1);
	of = fopen(fn,"rb");
	fseek (of,0,SEEK_END);
	fl = (ftell(of))>>2;
	rewind(of);
	bf = new xlong[fl];
	fread(bf,4,fl,of);
	fclose(of);

	return bf;
}

char* CLgetcharfile_(const char* fn)
{
	FILE* of;
	char* bf;
	uxlong fl;

	if( !( of = fopen(fn,"rb") ) ) CLexit_(__func__,"cannot open file",1);
	fseek (of,0,SEEK_END);
	fl = (ftell(of));
	rewind(of);
	bf = new char[fl];
	fread(bf,1,fl,of);
	fclose(of);

	return bf;
}

char** CLgetcsvfile_(const char* fn)
{
	ifstream of;
	char **bf;
	uxlong vc = 0;

	string line;
	string value;
	of.open(fn);
	if( of.fail() ) CLexit_(__func__,"cannot open file",1);
	
	while( getline(of,line) )
	{
		istringstream csvstream(line);

		while( getline(csvstream, value, ',') )
		{
			vc++;
		}

	}
	bf = new char*[vc];
	vc = 0;

	of.close();
	of.open(fn);
	if( of.fail() ) CLexit_(__func__,"cannot open file",1);

	while( getline(of,line) )
	{
		istringstream csvstream(line);

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

xlong CLgetfilelinecount_(const char* fn)
{
	FILE *of;
	xlong *bf;
	xlong lc = 0;
	int c = 0;

	if( !( of = fopen(fn,"r") ) ) CLexit_(__func__,"cannot open file",1);
	
	do
	{
		c = fgetc(of);
		if (c == '\n') lc++;
	} while (c != EOF);

	fclose(of);

	return lc;
}

void CLdebug_(const char* c,xlong v)
{
	cout << c << v << endl;
}

void CLprint_(const char* c)
{
	cout << c << endl;
}

void CLwaitforkey_()
{
	cin.get(); //change to any key, now only enter
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

float CLgettime_()
{
	float current_time = clock()  / CLOCKS_PER_SEC * 1000;

	return current_time;
}

xlong CLsystem_(const char* c)
{
	return system(c);
}

xlong getchararraysize_(xchar* c)
{
	return xlong(strlen(c));
}

xlong getchararraysize_(const xchar* c)
{
	return xlong(strlen(c));
}

#endif
