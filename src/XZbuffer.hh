///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Buffer Class ( XZbuffer.hh )
#pragma once
///</header>

///<include>
#include <stdalign.h> // aligned_alloc
#include <cstdlib>    // free
#include <string.h>   // memcpy

#include "XZbasic.hh"
///</include>

///<define>
class buffer
{
	private:
		      xint* data;			// pointer to data
		const yint  size;			// size in xints
		const yint  bytes;			// size in bytes
		      bool  self;			// allocated memory exists already
		buffer(const buffer& b);		// copy constructor (not implemented to deny copy)
		buffer& operator=(const buffer& b);	// assignment (not implemented to deny copy)
	public:
		buffer(yint s,bool a=1) : data(0),size(s),bytes((s<<2)+(s&31)),self(a) { if(a) { data = (xint*)aligned_alloc(4096,bytes); clear(); } } 
		~buffer() { if(self) { free(data); } }

		inline xint& operator[](uint i) { return data[i]; }
		inline xint  operator[](uint i) const { return data[i]; }
		inline xint* pointer() const { return data; }
		inline void  pointer(void* a) { data = static_cast<xint*>(a); }
		inline void  copy(const buffer& s) { memcpy(data,s.data,bytes); }
		inline void  clear(xint x=0) { for(yint i=0;i<size;++i) { data[i]=x; } }

		       void  fsaa(const buffer& s);
};
///</define>

///<code>
void buffer::fsaa(const buffer& s)
{
/*#ifdef __SSE__
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
#else*/
	memcpy(data,s.data,bytes);
//#endif
}
///</code>

