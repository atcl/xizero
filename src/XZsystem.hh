///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// System Library ( XZsystem.hh )
#pragma once
///</header>

///<include>
#include <cstdio>  // putchar,fopen,fseek,ftell,fread,fwrite,fclose,FILE
#include <cstdlib> // exit,atexit

#include "XZbasic.hh"
///</include>

///<define>
namespace
{
namespace system
{
	inline void  bye(xint x=0) { exit(x); }
	inline void  onx(void (*x)(void)) { atexit(x); }
	       void  say(const char* x,bool y=0);
	       void  err(bool c,const char* m);
	       char* ldf(const char* x);
	//       void  svf(const char* x,void* d,yint l);
}
}
///</define>

///<code>
void system::say(const char* x,bool y)
{
	for(yint i=0;x[i]!=0;++i) { putchar(x[i]); } 
	if(y) { putchar('\n'); }
}

void system::err(bool c,const char* m)
{
	ifu(c) { say(m,1); exit(1); }
}

char* system::ldf(const char* x)
{
	FILE* f(fopen(x,"rb"));
	guard(f==0,0);
	
	fseek(f,0,SEEK_END);
	const xint n = ftell(f);
	char* r = new char[n+1]; //shared_ptr?
	r[n]=0;
	
	fseek(f,0,SEEK_SET);
	fread(r,n,1,f);
	fclose(f);
	
	return r;
}

/*
void system::svf(const char* x,void* d,yint l)
{
	FILE* f = fopen(x,"wb");
	guard(f==0,0);
	
	fwrite(d,l,1,f);
	fclose(f);
}
*/
///</code>

