///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZbuffer.hh
// Buffer Library 
#pragma once
///</header>

///<include>
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
		/*OK*/ inline void  copy(const buffer& s) { memcpy(data,s.data,bytes); }
		/*OK*/ inline void  swap(buffer& b) { sint* t=b.data; b.data=data; data=t; }
		/*OK*/        void  clear(sint x=0);
		              void  fsaamb(const buffer& b);
		              void  glow();
};
///</define>

///<code>
void buffer::clear(sint x)
{
#ifdef SSE
	sint val[4] = {x,x,x,x};

	__asm__ __volatile__ (
	"shrl $7,%%ecx;\n"
	"movups (%0),%%xmm0;\n"
	".LCLEAR: ;\n"
	"movntps %%xmm0,(%1);\n"
	"movntps %%xmm0,16(%1);\n"
	"movntps %%xmm0,32(%1);\n"
	"movntps %%xmm0,48(%1);\n"
	"movntps %%xmm0,64(%1);\n"
	"movntps %%xmm0,80(%1);\n"
	"movntps %%xmm0,96(%1);\n"
	"movntps %%xmm0,112(%1);\n"
	"addl $128,%1;\n"
	"loop .LCLEAR;"
	"sfence;"
	: :"r"(&val),"r"(data),"c"(bytes):"memory");
#else
	for(uint i=tsize;i!=0;--i) { data[i]=x; }
#endif
}

void buffer::fsaamb(const buffer& b) //TODO: motion-blur
{
#ifdef SSE
	__asm__ __volatile__ (
	"subl $" STR(XRES) ",%%ecx;\n"
	"shrl $6,%%ecx;\n"
	"FSAA:\n"
	//"prefetch   0(%%edi);\n"
	//"prefetch  16(%%edi);\n"
	//"prefetch  32(%%edi);\n"
	//"prefetch  48(%%edi);\n"
	//"prefetch  (" STR(XRES*4) "+4)(%%edi);\n"
	//"prefetch  (" STR(XRES*4) "+20)(%%edi);\n"
	//"prefetch  (" STR(XRES*4) "+36)(%%edi);\n"
	//"prefetch  (" STR(XRES*4) "+52)(%%edi);\n"
	"movaps   (%%edi),%%xmm0;\n"
	"movaps 16(%%edi),%%xmm1;\n"
	"movaps 32(%%edi),%%xmm2;\n"
	"movaps 48(%%edi),%%xmm3;\n"
	"movups (" STR(XRES*4) "+4)(%%edi), %%xmm4;\n"
	"movups (" STR(XRES*4) "+20)(%%edi),%%xmm5;\n"
	"movups (" STR(XRES*4) "+36)(%%edi),%%xmm6;\n"
	"movups (" STR(XRES*4) "+52)(%%edi),%%xmm7;\n"
	"pavgb  %%xmm4,%%xmm0;\n"
	"pavgb  %%xmm5,%%xmm1;\n"
	"pavgb  %%xmm6,%%xmm2;\n"
	"pavgb  %%xmm7,%%xmm3;\n"
	//"prefetch   0(%%esi);\n"
	//"prefetch  16(%%esi);\n"
	//"prefetch  32(%%esi);\n"
	//"prefetch  48(%%esi);\n"
	//"movaps   (%%esi),%%xmm4;\n"
	//"movaps 16(%%esi),%%xmm5;\n"
	//"movaps 32(%%esi),%%xmm6;\n"
	//"movaps 48(%%esi),%%xmm7;\n"
	//"pavgb  %%xmm4,%%xmm0;\n"
	//"pavgb  %%xmm5,%%xmm1;\n"
	//"pavgb  %%xmm6,%%xmm2;\n"
	//"pavgb  %%xmm7,%%xmm3;\n"
	"movntps %%xmm0,  (%%edi);\n"
	"movntps %%xmm1,16(%%edi);\n"
	"movntps %%xmm2,32(%%edi);\n"
	"movntps %%xmm3,48(%%edi);\n"
	"addl $64,%%edi;\n"
	"loop FSAA;"
	"sfence;"
	: :"S"(b.data),"D"(data),"c"(bytes):);
#endif
}

void buffer::glow()
{

}
///</code>

