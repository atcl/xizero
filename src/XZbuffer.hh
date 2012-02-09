// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbuffer.hh
// Buffer Library 

///guard
#ifndef HH_XZBUFFER
#define HH_XZBUFFER
//#pragma message "Compiling " __FILE__ "..." " TODO: FSAA/FXAA + FSMB"
///*

///includes
#include <malloc.h> //memalign,free

#include "XZbasic.hh"
///*

///definitions
class buffer
{
	private:
		const uint tsize;	//size in typesize
		const uint bytes;	//size in bytes
		sint* data;		//pointer to data
	public:
		/*OK*/ buffer(uint s) : tsize(s),bytes((tsize<<2)+(tsize&31)),data(0) { data = (sint*)memalign(16,bytes); }
		/*OK*/ ~buffer() { free(data); }
		/*OK*/ inline sint& operator[](uint i) { return data[i]; }
		/*OK*/ inline sint  operator[](uint i) const { return data[i]; }
		/*OK*/ inline sint* pointer() const { return data; }
		/*OK*/        void  clear(sint x=0);
		/*OK*/        void  copy(const buffer& s,uint x) { for(uint i=x;i!=0;--i) { data[i]=s.data[i]; } }
		/*OK*/        void  swap(buffer & b) { sint* t=b.data; b.data=data; data=t; }
		              void  fsaamb(const buffer& b);
};
///*

///implementation
void buffer::clear(sint x)
{
#ifdef SSE
	static sint val[4];
	val[0] = val[1] = val[2] = val[3] = x;

	__asm__ __volatile__ (
	"shrl $7,%%ecx;\n"
	"movups (%%eax),%%xmm0;\n"
	"movaps %%xmm0,%%xmm1;\n"
	"movaps %%xmm0,%%xmm2;\n"
	"movaps %%xmm0,%%xmm3;\n"
	"movaps %%xmm0,%%xmm4;\n"
	"movaps %%xmm0,%%xmm5;\n"
	"movaps %%xmm0,%%xmm6;\n"
	"movaps %%xmm0,%%xmm7;\n"
	"set:\n"
	"movaps %%xmm0,(%%edi);\n"
	"movaps %%xmm1,16(%%edi);\n"
	"movaps %%xmm2,32(%%edi);\n"
	"movaps %%xmm3,48(%%edi);\n"
	"movaps %%xmm4,64(%%edi);\n"
	"movaps %%xmm5,80(%%edi);\n"
	"movaps %%xmm6,96(%%edi);\n"
	"movaps %%xmm7,112(%%edi);\n"
	"addl $128,%%edi;\n"
	"loop set;"
	: :"a"(&val),"D"(data),"c"(bytes):);
#else
	for(uint i=tsize;i!=0;--i) { data[i]=x; }
#endif
}

void buffer::fsaamb(const buffer& b)
{
#ifdef SSE
	__asm__ __volatile__ (
	"subl $"STR(XRES)",%%ecx;\n"
	"shrl $6,%%ecx;\n"
	"fsaa:\n"
	"movaps   (%%edi),%%xmm0;\n"
	"movaps 16(%%edi),%%xmm1;\n"
	"movaps 32(%%edi),%%xmm2;\n"
	"movaps 48(%%edi),%%xmm3;\n"
	"movups ("STR(XRES*4)"+4)(%%edi), %%xmm4;\n"
	"movups ("STR(XRES*4)"+20)(%%edi),%%xmm5;\n"
	"movups ("STR(XRES*4)"+36)(%%edi),%%xmm6;\n"
	"movups ("STR(XRES*4)"+52)(%%edi),%%xmm7;\n"
	"pavgb  %%xmm4,%%xmm0;\n"
	"pavgb  %%xmm5,%%xmm1;\n"
	"pavgb  %%xmm6,%%xmm2;\n"
	"pavgb  %%xmm7,%%xmm3;\n"
	//"movaps   (%%esi),%%xmm4;\n"
	//"movaps 16(%%esi),%%xmm5;\n"
	//"movaps 32(%%esi),%%xmm6;\n"
	//"movaps 48(%%esi),%%xmm7;\n"
	//"pavgb  %%xmm4,%%xmm0;\n"
	//"pavgb  %%xmm5,%%xmm1;\n"
	//"pavgb  %%xmm6,%%xmm2;\n"
	//"pavgb  %%xmm7,%%xmm3;\n"
	"movaps %%xmm0,  (%%edi);\n"
	"movaps %%xmm1,16(%%edi);\n"
	"movaps %%xmm2,32(%%edi);\n"
	"movaps %%xmm3,48(%%edi);\n"
	"addl $64,%%edi;\n"
	"loop fsaa;"
	: :"S"(b.data),"D"(data),"c"(bytes):);
#endif
}
///*

#endif 
