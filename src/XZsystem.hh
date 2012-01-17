// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZsystem.hh
// System Library 

///guard
#ifndef HH_XZSYSTEM
#define HH_XZSYSTEM
///*

///includes
#include <cstdlib> //exit,system
#include <cstdio> //putchar,fopen,fseek,ftell,fread,fclose
#include <ctime> //clock,CLOCKS_PER_SEC
///*

///definitions
namespace
{
namespace system
{
	/*OK*/ INLINE void  bye(long x=0) { exit(x); }
	/*OK*/ INLINE long  sys(const char* x) { return ::system(x); }
	/*OK*/ INLINE long  clk() { return (clock()*(1024000/CLOCKS_PER_SEC))>>10; }
	/*OK*/        void  slp(long x) { const long e=clock()+x*CLOCKS_PER_SEC/1000; while(clock()<e) { ; } }
	/*OK*/        void  say(const char* x,bool y) { long i=0; while(x[i]!=0) { putchar(x[i]); i++; } if(y) { putchar('\n'); } }
	/*OK*/        char* ldf(const char* x) { FILE* f=fopen(x,"rb"); fseek(f,0,SEEK_END); const long n=ftell(f); char* r=new char[n]; fseek(f,0,SEEK_SET); fread(r,n,1,f); fclose(f); return r; } 
	/*OK*/        long  fps(bool o=1) { static long f=0; static long l=0; long t=clk(); f+=o; if(t>=l&&o==1) { l=t+4000; t=f>>2; f=0; return t; } return -1; }
}
}
///*

#endif

