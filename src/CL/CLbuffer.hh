//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
#pragma message "Compiling " __FILE__ " ! TODO: rewrite as dynamic class (w templates?)"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLdetect.hh"

/* class name:	CLbuffer
 * 
 * description:	This class handles memory buffers.
 * 
 * author:	atcl
 * 
 * notes:	complete assembly routines, currently max size for buffers is 2GB.
 * 
 * version: 0.1
 */

template <typename T>class CLbuffer : public virtual CLcl
{
	private:
		T* buffer;
		uxlong size;
		uxlong bsize;
		uxlong ds;
		uxlong qs;
		xlong ttype;
	public:
		CLbuffer(uxlong s,T ival=0);
		~CLbuffer();
		void clear(T v);
		void copy(T* dst);
		void copy(CLbuffer* dst);
		void blendcopy(T* dst,xlong o); //too slow!!!
		void blendcopy(CLbuffer<T>* dst,xlong o);
		uxlong getsize() const;
		uxlong getbytesize() const;
		T* getbuffer() const;
		T& operator[](uxlong i);
};

template <typename T>CLbuffer<T>::CLbuffer(uxlong s,T ival)
{
	//adjust size and allocate buffer
	size = (s+1) + (s%4); // make sure is amultiple of 16byte
	bsize = size<<2;
	buffer = new T[size];
	ttype = 1; // 0 + cldetect->x86() + cldetect->mmx() + cldetect->sse(); //crashes when calling cldetect members here
	clear(ival);
	//*
}

template <typename T>CLbuffer<T>::~CLbuffer() { } //how to delete buffer?

template <typename T>void CLbuffer<T>::clear(T v)
{
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong purev = (reinterpret_cast<xlong*>(&v))[0];
	xlong* pack = new xlong[4];
	pack[0] = pack[1] = pack[2] = pack[3] = purev;
	register xlong i = size;

	//x86 SSE1 assembly clear (very fast)
	if(size>262144 && ttype==3 )
	{
		i>>=5;
		for(;i>0;i--)
		{
			//blast with all 8 xmm regs, and movaps instead of movups (?)
			__asm__ __volatile__ (
			"movups (%0),  %%xmm0;"
			"movups %%xmm0,(%1);"
			"movups %%xmm0,16(%1);"
			"movups %%xmm0,32(%1);"
			"movups %%xmm0,48(%1);"
			"movups %%xmm0,64(%1);"
			"movups %%xmm0,80(%1);"
			"movups %%xmm0,96(%1);"
			"movups %%xmm0,112(%1);"
			: : "r"(pack),"r"(puredst) );
			puredst+=32;
		}
	}
	//*
	
	//x86 MMX assembly clear (fast)
	else if(size>65536 && ttype==2 )
	{
		i>>=4;
		for(;i>0;i--)
		{
			//blast with all 8 mm regs
			__asm__ __volatile__ (
			"movq (%0), %%mm0;"
			"movq %%mm0,(%1);"
			"movq %%mm0,8(%1);"
			"movq %%mm0,16(%1);"
			"movq %%mm0,24(%1);"
			"movq %%mm0,32(%1);"
			"movq %%mm0,40(%1);"
			"movq %%mm0,48(%1);"
			"movq %%mm0,56(%1);"		
			: : "r"(pack),"r"(puredst) );
			puredst+=16;
		}
	}
	//*
	
	//fallback if no SIMD, neither MMX or SSE1 is available
	else if(ttype==1) __asm__ __volatile__ (	"cld; rep; stosl;" : : "a"(purev),"D"(puredst),"c"(size));
	//*
	
	//default for loop clear (slow)
	else for(;i>=0;i--) { buffer[i] = v; }
	//*
}

template <typename T>void CLbuffer<T>::copy(T *dst)
{
	xlong* puresrc = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&dst[0]));
	register xlong i = size;
	
	//x86 SSE1 assembly copy (very fast)
	if(size>262144 && ttype==3 )
	{
		i>>=5;
		for(;i>0;i--)
		{
			//blast wth all 8 xmm regs, and movaps instead of movups (?)
			__asm__ __volatile__ (
			"prefetch 128(%0);"
			"movups (%0),   %%xmm0;"
			"movups 16(%0), %%xmm1;"
			"movups 32(%0), %%xmm2;"
			"movups 48(%0), %%xmm3;"
			"movups 64(%0), %%xmm4;"
			"movups 80(%0), %%xmm5;"
			"movups 96(%0), %%xmm6;"
			"movups 112(%0),%%xmm7;"
			"movups %%xmm0,(%1);"
			"movups %%xmm1,16(%1);"
			"movups %%xmm2,32(%1);"
			"movups %%xmm3,48(%1);"
			"movups %%xmm4,64(%1);"
			"movups %%xmm5,80(%1);"
			"movups %%xmm6,96(%1);"
			"movups %%xmm7,112(%1);"	
			: : "r"(puresrc),"r"(puredst) );
			puresrc+=32;
			puredst+=32;
		}
	}
	//*
	
	//x86 MMX assembly copy (fast)
	else if(size>65536 && ttype==2 )
	{
		i>>=4;
		for(;i>0;i--)
		{
			//blast with all 8 mm regs
			__asm__ __volatile__ (
			"movq   (%0),%%mm0;"
			"movq  8(%0),%%mm1;"
			"movq 16(%0),%%mm2;"
			"movq 24(%0),%%mm3;"
			"movq 32(%0),%%mm4;"
			"movq 40(%0),%%mm5;"
			"movq 48(%0),%%mm6;"
			"movq 56(%0),%%mm7;"
			"movq %%mm0,  (%1);"
			"movq %%mm1, 8(%1);"
			"movq %%mm2,16(%1);"
			"movq %%mm3,24(%1);"
			"movq %%mm4,32(%1);"
			"movq %%mm5,40(%1);"
			"movq %%mm6,48(%1);"
			"movq %%mm7,56(%1);"		
			: : "r"(puresrc),"r"(puredst) );
			puresrc+=16;
			puredst+=16;
		}
	}
	//*
	
	//fallback if no SIMD neither MMX or SSE1 is available
	else if(ttype==1) __asm__ __volatile__ (	"cld; rep; movsl;" : :"S"(puresrc),"D"(puredst),"c"(i));
	//*
	
	//default for loop copy (slow)
	else { i--; for(;i>=0;i--) { dst[i] = buffer[i]; } }
	//*
}

template <typename T>void CLbuffer<T>::copy(CLbuffer* dst) { copy(dst->getbuffer()); }

template <typename T>void CLbuffer<T>::blendcopy(T* dst,xlong o)
{
	xlong* puresrc = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&dst[0]));
	register xlong i = size;
	doubleword tx = { 0 };
	doubleword ty = { 0 };
	doubleword tz = { 0 };
		
	//special copy methods utilizing logical and arthmic operators to combine source with buffers
	if(ttype==4) //set back to 1 if byte add and sub solved!
	{		
		switch(o)
		{
			case 0:		//NONE //?default
				__asm__ __volatile__ (	"cld; rep; movsl;" : :"S"(puresrc),"D"(puredst),"c"(i));
			break;
			
			case 1:		//AND
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"andl %%ebx,%%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 2:		//OR
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"orl %%ebx,%%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 3:		//NAND
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"andl %%ebx,%%eax;"
											"notl %%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 4:		//NOR
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"orl %%ebx,%%eax;"
											"notl %%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 5:		//XOR
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"xorl %%ebx,%%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 6:		//ADD
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"addl %%ebx,%%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 7:		//SUB
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"xchg %%esi,%%edi;"
											"subl %%ebx,%%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 8:		//BYTE ADD
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"movl %%ebx,%%ecx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"movl %%eax,%%edx;"
											"xchg %%esi,%%edi;"
											"addb %%bl,%%al;"
											//~ "cmovob $255,%%al;"
											"addb %%bh,%%ah;"
											//~ "cmovob $255,%%ah;"
											"bswap %%eax;"
											"bswap %%ebx;"
											"addb %%bl,%%al;"
											//~ "cmovob $255,%%al;"
											"addb %%bh,%%ah;"
											//~ "cmovob $255,%%ah;"
											"bswap %%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 9:		//BYTE SUB
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"movl %%eax,%%ebx;"
											"movl %%ebx,%%ecx;"
											"xchg %%esi,%%edi;"
											"lodsl;"
											"movl %%eax,%%edx;"
											"xchg %%esi,%%edi;"
											"subb %%bl,%%al;"
											//~ "cmovob $0,%%al;"
											"subb %%bh,%%ah;"
											//~ "cmovob $0,%%ah;"
											"bswap %%eax;"
											"bswap %%ebx;"
											"subb %%bl,%%al;"
											//~ "cmovob $0,%%al;"
											"subb %%bh,%%ah;"
											//~ "cmovob $0,%%ah;"
											"bswap %%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 10:	//2xAA
				i--;
				for(;i>=0;i--)
				{
					//2xRGMS ( (x,y) + ( (x+1,y-1) + (x-1,y+1) / 2 )
				}
			break;
			
			case 11:	//ALPHA
			
			break;
			
			case 12:	//NOT
				i--;
				for(;i>=0;i--)
				{
					__asm__ __volatile__ (	"cld;" 
											"lodsl;"
											"notl %%eax;"
											"stosl;" 
											: :"S"(puresrc),"D"(puredst));
				}
			break;
			
			case 13:	//BYTE MUL
			
			break;
		}
	}
	else
	{
		switch(o)
		{
			case 0:		//NONE //?default
				i--; for(;i>=0;i--) { dst[i] = buffer[i]; }
			break;
			
			case 1:		//AND
				i--; for(;i>=0;i--) { dst[i] = dst[i] & buffer[i]; }
			break;
			
			case 2:		//OR
				i--; for(;i>=0;i--) { dst[i] = dst[i] | buffer[i]; }
			break;
			
			case 3:		//NAND
				i--; for(;i>=0;i--) { dst[i] = !(dst[i] & buffer[i]); }
			break;
			
			case 4:		//NOR
				i--; for(;i>=0;i--) { dst[i] = !(dst[i] | buffer[i]); }
			break;
			
			case 5:		//XOR
				i--; for(;i>=0;i--) { dst[i] = dst[i] ^ buffer[i]; }
			break;
			
			case 6:		//ADD
				i--; for(;i>=0;i--) { dst[i] = dst[i] + buffer[i]; }
			break;
			
			case 7:		//SUB
				i--; for(;i>=0;i--) { dst[i] = dst[i] - buffer[i]; }
			break;
			
			case 8:		//BYTE ADD
				for(;i>=0;i--)
				{
					tx.dd = dst[i];
					ty.dd = buffer[i];

					tz.db[0] = tx.db[0] + ty.db[0]; if(tz.db[0]<tx.db[0]) tz.db[0] = 0xFF;
					tz.db[1] = tx.db[1] + ty.db[1]; if(tz.db[1]<tx.db[1]) tz.db[1] = 0xFF;
					tz.db[2] = tx.db[2] + ty.db[2]; if(tz.db[2]<tx.db[2]) tz.db[2] = 0xFF;
					tz.db[3] = tx.db[3] + ty.db[3]; if(tz.db[3]<tx.db[3]) tz.db[3] = 0xFF;
					//~ tz.db[0] = tx.db[0] + ty.db[0]; tz.db[0] = ~(uxchar(tz.db[0]>=tx.db[0])) | tz.db[0];
					//~ tz.db[1] = tx.db[1] + ty.db[1]; tz.db[1] = ~(uxchar(tz.db[1]>=tx.db[1])) | tz.db[1];
					//~ tz.db[2] = tx.db[2] + ty.db[2]; tz.db[2] = ~(uxchar(tz.db[2]>=tx.db[2])) | tz.db[2];
					//~ tz.db[3] = tx.db[3] + ty.db[3]; tz.db[3] = ~(uxchar(tz.db[3]>=tx.db[3])) | tz.db[3];
					//
					dst[i] = tz.dd;
				}
			break;
			
			case 9:		//BYTE SUB
				for(;i>=0;i--)
				{
					tx.dd = dst[i];
					ty.dd = buffer[i];
					
					//~ tz.db[0] = tx.db[0] - ty.db[0]; if(tz.db[0]>tx.db[0]) tz.db[0] = 0;
					//~ tz.db[1] = tx.db[1] - ty.db[1]; if(tz.db[1]>tx.db[1]) tz.db[1] = 0;
					//~ tz.db[2] = tx.db[2] - ty.db[2]; if(tz.db[2]>tx.db[2]) tz.db[2] = 0;
					//~ tz.db[3] = tx.db[3] - ty.db[3]; if(tz.db[3]>tx.db[3]) tz.db[3] = 0;
					tz.db[0] = tx.db[0] - ty.db[0]; tz.db[0] = ~(uxchar(tz.db[0]>tx.db[0])) & tz.db[0];
					tz.db[1] = tx.db[1] - ty.db[1]; tz.db[1] = ~(uxchar(tz.db[1]>tx.db[1])) & tz.db[1];
					tz.db[2] = tx.db[2] - ty.db[2]; tz.db[2] = ~(uxchar(tz.db[2]>tx.db[2])) & tz.db[2];
					tz.db[3] = tx.db[3] - ty.db[3]; tz.db[3] = ~(uxchar(tz.db[3]>tx.db[3])) & tz.db[3];
					//
					dst[i] = tz.dd;
				}
			break;
			
			case 10:	//2xAA
				i--;
				for(;i>=0;i--)
				{
					//2xRGMS ( (x,y) + ( (x+1,y-1) + (x-1,y+1) / 2 )
				}
			break;
			
			case 11:	//ALPHA
			
			break;
			
			case 12:	//NOT
				i--; for(;i>=0;i--) { dst[i] = !buffer[i]; }
			break;
			
			case 13:	//BYTE MUL
			
			break;
		}
	}
	//*
}

template <typename T>void CLbuffer<T>::blendcopy(CLbuffer<T>* dst,xlong o) { blendcopy(dst->getbuffer(),o); }

template <typename T>uxlong CLbuffer<T>::getsize() const { return size; }

template <typename T>uxlong CLbuffer<T>::getbytesize() const { return bsize; }

template <typename T>T* CLbuffer<T>::getbuffer() const { return buffer; }

template <typename T>T& CLbuffer<T>::operator[](uxlong i)
{
	if(i>=size) return buffer[size];
	return buffer[i];
}

//typedefs:
typedef CLbuffer<float> CLfbuffer;
typedef CLbuffer<xlong> CLlbuffer;
typedef CLbuffer<uxlong> CLubuffer;
//*

#endif

