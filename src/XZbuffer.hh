///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZbuffer.hh
// Buffer Library 
///</header>

///<include>
#pragma once
#include <stdalign.h> //aligned_alloc
#include <cstdlib>    //free
#include <string.h>   //memcpy

#include "XZbasic.hh"
///</include>

///<define>
class buffer
{
	private:
		const yint  bytes;			//size in bytes
		      xint* data;			//pointer to data
		      bool  self;			//buffer exists already
		buffer(const buffer& b);		//Copy Constructor (not implemented to deny copy)
		buffer& operator=(const buffer& b);	//Assignment (not implemented to deny copy)
	public:
		/*OK*/ buffer(yint s,bool a=1) : bytes((s<<2)+(s&31)),data(0) { if(a) { data = (xint*)aligned_alloc(4096,bytes); clear(); } } 
		/*OK*/ ~buffer() { if(self) { free(data); } }
		/*OK*/ inline xint& operator[](uint i) { return data[i]; }
		/*OK*/ inline xint  operator[](uint i) const { return data[i]; }
		/*OK*/ inline xint* pointer() const { return data; }
		/*OK*/ inline void  pointer(void* a) { data = static_cast<xint*>(a); }
		/*OK*/        void  copy(const buffer& s) { memcpy(data,s.data,bytes); };
		              void  fsaa(const buffer& s);
		/*OK*/        void  clear(xint x=0);
};
///</define>

///<code>
void buffer::clear(xint x)
{
#ifdef __SSE__
	const xint val[4] = {x,x,x,x};

	__asm__ __volatile__ (
	"shrl $7,%2;\n"
	"movups (%0),%%xmm0;\n"
	"0: ;\n"
	"prefetch (%1);\n"
	"movntps %%xmm0,(%1);\n"
	"movntps %%xmm0,16(%1);\n"
	"movntps %%xmm0,32(%1);\n"
	"movntps %%xmm0,48(%1);\n"
	"movntps %%xmm0,64(%1);\n"
	"movntps %%xmm0,80(%1);\n"
	"movntps %%xmm0,96(%1);\n"
	"movntps %%xmm0,112(%1);\n"
	"addq $128,%1;\n"
	"loop 0b;"
	"sfence;"
	: :"r"(&val),"r"(data),"c"(bytes):"memory");
#else
	for(yint i=1+bytes>>2;i!=0;--i) { data[i]=x; }
#endif
}

void buffer::fsaa(const buffer& s)
{
#ifdef __SSE__
	__asm__ __volatile__ (
	"subl $" STR(XRES) ",%2;\n"
	"shrl $6,%2;\n"
	"2:\n"
	"prefetch  (%1);\n"
	"prefetch  (" STR(XRES*4) ")(%0);\n"
	"movaps   (%0),%%xmm0;\n"
	"movaps 16(%0),%%xmm1;\n"
	"movaps 32(%0),%%xmm2;\n"
	"movaps 48(%0),%%xmm3;\n"
	"movups (" STR(XRES*4) "+ 4)(%0),%%xmm4;\n"
	"movups (" STR(XRES*4) "+20)(%0),%%xmm5;\n"
	"movups (" STR(XRES*4) "+36)(%0),%%xmm6;\n"
	"movups (" STR(XRES*4) "+52)(%0),%%xmm7;\n"
	"pavgb   %%xmm4,%%xmm0;\n"
	"pavgb   %%xmm5,%%xmm1;\n"
	"pavgb   %%xmm6,%%xmm2;\n"
	"pavgb   %%xmm7,%%xmm3;\n"
	"movntps %%xmm0,  (%1);\n"
	"movntps %%xmm1,16(%1);\n"
	"movntps %%xmm2,32(%1);\n"
	"movntps %%xmm3,48(%1);\n"
	"addq $64,%0;\n"
	"addq $64,%1;\n"
	"loop 2b;"
	"sfence;"
	: :"r"(s.data),"r"(data),"c"(bytes):"memory");
#else
	memcpy(data,s.data,bytes);
#endif
}
///</code>

