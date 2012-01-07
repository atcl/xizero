// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbuffer.hh
// Buffer Library 

///guard
#ifndef HH_XZBUFFER
#define HH_XZBUFFER
///*

///includes
#include <malloc.h> //memalign,free
#include "XZbasic.hh"
///*

///definitions
class buffer
{
	private:
		const ulong tsize;
		const ulong bytes;
		long* __restrict data;
	public:
		/*OK*/ buffer(ulong s) : tsize(s),bytes((tsize<<2)+(tsize&31)),data(0) { data = (long*)memalign(16,bytes); }
		/*OK*/ ~buffer() { free(data); }
		/*OK*/ INLINE long& operator[](ulong i) { return data[i]; }
		/*OK*/ INLINE long  operator[](ulong i) const { return data[i]; }
		/*OK*/ INLINE long* pointer() const { return data; }
		/*OK*/ void clear(long x=0) { for(ulong i=tsize;i!=0;--i) { data[i]=x; /*PREFETCH(&data[i+1],1,1);*/ } }
		/*OK*/ void copy(const buffer& s,ulong x) { for(ulong i=x;i!=0;--i) { data[i]=s.data[i]; /*PREFETCH(&data[i+1],1,1); PREFETCH(&s.data[i+1],0,1);*/ } }
		       void fsaamb(const buffer& b) { for(ulong i=tsize;i!=0;--i) { ; } }
		       void set(long x=0); //overload clear if x86
};
///*

///implementation
void buffer::set(long x)
{
	//long val[4] = {x,x,x,x}; //doesn't work for whatever reason
	static long* val = new long[4];
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
	"movaps %%xmm0,(%%ebx);\n"
	"movaps %%xmm1,16(%%ebx);\n"
	"movaps %%xmm2,32(%%ebx);\n"
	"movaps %%xmm3,48(%%ebx);\n"
	"movaps %%xmm4,64(%%ebx);\n"
	"movaps %%xmm5,80(%%ebx);\n"
	"movaps %%xmm6,96(%%ebx);\n"
	"movaps %%xmm7,112(%%ebx);\n"
	"addl $128,%%ebx;\n"
	"loop set;"
	: :"a"(val),"b"(data),"c"(bytes):);
}
///*

#endif 
