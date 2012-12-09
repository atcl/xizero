///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZsystem.hh
// System Library 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///</header>

///<include>
#include <cstdio> //exit,putchar,fopen,fseek,ftell,fread,fwrite,fclose

#include "XZbasic.hh"
///</include>

///<define>
namespace
{
namespace system
{
	/*OK*/ inline void  bye(sint x=0) { exit(x); }
	/*OK*/ inline void  ifx(void (*x)(void)) { atexit(x); }
	/*OK*/        void  say(const char* x,bool y=0) { for(sint i=0;x[i]!=0;++i) { putchar(x[i]); } if(y) { putchar('\n'); } }
	/*OK*/        void  err(bool c,const char* m) { ifu(c) { say(m,1); exit(1); } }
	/*OK*/        char* ldf(const char* x) { FILE* f=fopen(x,"rb"); guard(f==0,0); fseek(f,0,SEEK_END); const sint n=ftell(f); char* r=new char[n+1]; fseek(f,0,SEEK_SET); fread(r,n,1,f); r[n]=0; fclose(f); return r; }        
	//              void  svf(const char* x,void* d,uint l) FILE* f=fopen(x,"wb"); if(f==0) { return 0; } fwrite(d,l,1,f); fclose(f); return r; }
}
}
///</define>

