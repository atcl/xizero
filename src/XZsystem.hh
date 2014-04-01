///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZsystem.hh
// System Library
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
	/*OK*/ inline void  bye(xint x=0) { exit(x); }
	/*OK*/ inline void  ifx(void (*x)(void)) { atexit(x); }
	/*OK*/        void  say(const char* x,bool y=0) { for(yint i=0;x[i]!=0;++i) { putchar(x[i]); } if(y) { putchar('\n'); } }
	/*OK*/        void  err(bool c,const char* m) { ifu(c) { say(m,1); exit(1); } }
	/*OK*/        char* ldf(const char* x) { FILE* f=fopen(x,"rb"); guard(f==0,0); fseek(f,0,SEEK_END); const xint n=ftell(f); char* r=new char[n+1]; fseek(f,0,SEEK_SET); fread(r,n,1,f); r[n]=0; fclose(f); return r; }        
	//              void  svf(const char* x,void* d,yint l) FILE* f=fopen(x,"wb"); if(f==0) { return 0; } fwrite(d,l,1,f); fclose(f); return r; }
}
}
///</define>

