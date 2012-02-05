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
		sint* __restrict data;	//pointer to data
	public:
		/*OK*/ buffer(uint s) : tsize(s),bytes((tsize<<2)+(tsize&31)),data(0) { data = (sint*)memalign(16,bytes); }
		/*OK*/ ~buffer() { free(data); }
		/*OK*/ inline sint& operator[](uint i) { return data[i]; }
		/*OK*/ inline sint  operator[](uint i) const { return data[i]; }
		/*OK*/ inline sint* pointer() const { return data; }
		/*OK*/        void  clear(sint x=0);
		/*OK*/        void  copy(const buffer& s,uint x) { for(uint i=x;i!=0;--i) { data[i]=s.data[i]; } }
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
	: :"a"(&val),"b"(data),"c"(bytes):);
#else
	for(uint i=tsize;i!=0;--i) { data[i]=x; }
#endif
}

void buffer::fsaamb(const buffer& b)
{
#ifdef SSE
	//TODO
#else
	for(uint i=tsize;i!=0;--i) { /*TODO*/; }
#endif
}
///*

#endif 
