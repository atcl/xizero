///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZbuffer.hh
// Buffer Library 
///</header>

///<include>
#pragma once
#include <string.h> //memcpy
#include "XZbasic.hh"
///</include>

///<define>
class buffer
{
	private:
		const uint  tsize;	//size in typesize
		const uint  bytes;	//size in bytes
		      sint* data;	//pointer to data
		buffer(const buffer& b);
		buffer& operator=(const buffer& b);
	public:
		/*OK*/ buffer(uint s) : tsize(s),bytes((tsize<<2)+(tsize&31)),data(0) { data = (sint*)aligned_new(4096,bytes); clear(); } 
		/*OK*/ ~buffer() { free(data); }
		/*OK*/ inline sint& operator[](uint i) { return data[i]; }
		/*OK*/ inline sint  operator[](uint i) const { return data[i]; }
		/*OK*/ inline sint* pointer() const { return data; }
		/*OK*/ inline void  pointer(void* a) { data = static_cast<sint*>(a); }
		/*OK*/        void  copy(const buffer& s);
		/*OK*/        void  clear(sint x=0);
		              void  fsaa();
};
///</define>

///<code>
void buffer::clear(sint x)
{
#ifdef __SSE__
	sint val[4] = {x,x,x,x};

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
	"addl $128,%1;\n"
	"loop 0b;"
	"sfence;"
	: :"r"(&val),"r"(data),"c"(bytes):"memory");
#else
	for(uint i=tsize;i!=0;--i) { data[i]=x; } //wmemset here?
#endif
}

void buffer::copy(const buffer& s)
{
#ifdef __SSE__
	__asm__ __volatile__ (
	"shrl $7,%2;\n"
	"1: ;\n"
	"prefetch (%0);\n"
	"prefetch (%1);\n"
	"movaps   (%0),%%xmm0;\n"
	"movaps 16(%0),%%xmm1;\n"
	"movaps 32(%0),%%xmm2;\n"
	"movaps 48(%0),%%xmm3;\n"
	"movaps 64(%0),%%xmm4;\n"
	"movaps 80(%0),%%xmm5;\n"
	"movaps 96(%0),%%xmm6;\n"
	"movaps 112(%0),%%xmm7;\n"
	"movntps %%xmm0,  (%1);\n"
	"movntps %%xmm1,16(%1);\n"
	"movntps %%xmm2,32(%1);\n"
	"movntps %%xmm3,48(%1);\n"
	"movntps %%xmm4,64(%1);\n"
	"movntps %%xmm5,80(%1);\n"
	"movntps %%xmm6,96(%1);\n"
	"movntps %%xmm7,112(%1);\n"
	"addl $128,%0;\n"
	"addl $128,%1;\n"
	"loop 1b;"
	"sfence;"
	: :"r"(s.data),"r"(data),"c"(bytes):"memory");
#else
	memcpy(data,s.data,bytes);
#endif
}

void buffer::fsaa()
{
#ifdef __SSE__
	__asm__ __volatile__ (
	"subl $" STR(XRES) ",%1;\n"
	"shrl $6,%1;\n"
	"2:\n"
	"prefetch  (%0);\n"
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
	"movntps %%xmm0,  (%0);\n"
	"movntps %%xmm1,16(%0);\n"
	"movntps %%xmm2,32(%0);\n"
	"movntps %%xmm3,48(%0);\n"
	"addl $64,%0;\n"
	"loop 2b;"
	"sfence;"
	: :"r"(data),"c"(bytes):);
#endif
}
///</code>

