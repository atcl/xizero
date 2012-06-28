// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZbuffer.hh
// Buffer Library 

///guard
#ifndef HH_XZBUFFER
#define HH_XZBUFFER
//#pragma message "Compiling " __FILE__ "..." " TODO: FSMB, emissive glow"
///*

///includes
#include <malloc.h> //memalign
#include <string.h> //memcpy
#include "XZbasic.hh"
///*

///definitions
class buffer //TODO =,cpy-ctor
{
	private:
		const uint tsize;	//size in typesize
		const uint bytes;	//size in bytes
		const bool later;	//self allocated or not
		      sint* data;	//pointer to data
	public:
		/*OK*/ buffer(uint s,bool a=0) : tsize(s),bytes((tsize<<2)+(tsize&31)),later(a),data(0) { if(a==0) { data = (sint*)memalign(16,bytes); } } 
		/*OK*/ ~buffer() { if(later==0) { free(data); } }
		/*OK*/ inline sint& operator[](uint i) { return data[i]; }
		/*OK*/ inline sint  operator[](uint i) const { return data[i]; }
		/*OK*/ inline sint* pointer() const { return data; }
		/*OK*/ inline void  pointer(void* a) { data = static_cast<sint*>(a); }
		/*OK*/ inline void  copy(const buffer& s) { memcpy(data,s.data,bytes); }
		/*OK*/ inline void  swap(buffer & b) { sint* t=b.data; b.data=data; data=t; }
		/*OK*/        void  clear(sint x=0);
		              void  fsaamb(const buffer& b);
		              void  glow();
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
	"movups (%0),%%xmm0;\n"
	"movaps %%xmm0,%%xmm1;\n"
	"movaps %%xmm0,%%xmm2;\n"
	"movaps %%xmm0,%%xmm3;\n"
	"movaps %%xmm0,%%xmm4;\n"
	"movaps %%xmm0,%%xmm5;\n"
	"movaps %%xmm0,%%xmm6;\n"
	"movaps %%xmm0,%%xmm7;\n"
	"set:\n"
	"movntps %%xmm0,(%1);\n"
	"movntps %%xmm1,16(%1);\n"
	"movntps %%xmm2,32(%1);\n"
	"movntps %%xmm3,48(%1);\n"
	"movntps %%xmm4,64(%1);\n"
	"movntps %%xmm5,80(%1);\n"
	"movntps %%xmm6,96(%1);\n"
	"movntps %%xmm7,112(%1);\n"
	"addl $128,%1;\n"
	"loop set;"
	"sfence;"
	: :"r"(&val),"r"(data),"c"(bytes):"memory");
#else
	for(uint i=tsize;i!=0;--i) { data[i]=x; }
#endif
}

void buffer::fsaamb(const buffer& b)
{
#ifdef SSE
	__asm__ __volatile__ (
	"subl $" STR(XRES) ",%%ecx;\n"
	"shrl $6,%%ecx;\n"
	"fsaa:\n"
	//"prefetch   0(%%edi);\n"
	//"prefetch  16(%%edi);\n"
	//"prefetch  32(%%edi);\n"
	//"prefetch  48(%%edi);\n"
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
	"loop fsaa;"
	: :"S"(b.data),"D"(data),"c"(bytes):);
#endif
}

void buffer::glow()
{

}
///*

#endif 
