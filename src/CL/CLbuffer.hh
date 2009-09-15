//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
#pragma message "Compiling " __FILE__ " ! TODO: rewrite as dynamic class (w templates?)"

#include <stdio.h>
#include <string.h>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLdetect.hh"

template <typename T>class CLbuffer : public virtual CLcl
{
	private:
		T* buffer;
		uxlong size;
		uxlong bsize;
		uxlong ds;
		uxlong qs;
		bool   havemmx;
		bool   havesse;	
	public:
		CLbuffer(uxlong s);
		~CLbuffer();
		void clear(T v);
		void fastclear(T v);
		void ultraclear(T v);
		void copy(T* dst);
		void copy(CLbuffer* dst);
		void fastcopy(xlong* dst);
		void ultracopy(xlong* dst);
		void blendcopy(T* dst,xlong o);
		void blendcopy(CLbuffer<T>* dst,xlong o);
		uxlong getsize();
		uxlong getbytesize();
		T* getbuffer();
		T& operator[](uxlong i);
};

template <typename T>CLbuffer<T>::CLbuffer(uxlong s)
{
	//adjust size and allocate buffer
	size = (s+1) + (s%4); // make sure is amultiple of 16byte
	bsize = size<<2;
	buffer = new T[size];
	//*
	
	//determine cpu type to decide which copy or clear method can be used
	havemmx = 1;
	havesse = 0; 
	
	xlong cpucaps = CLdetect::instance()->cpu();
	//if( (cpucaps & MMX) != 0 ) havemmx = 1; else havemmx = 0;
	//if( (cpucaps & SSE) != 0 ) havesse = 1; else havesse = 0;
	//*
}

template <typename T>CLbuffer<T>::~CLbuffer()
{
	delete[] buffer;
}

template <typename T>void CLbuffer<T>::clear(T v)
{
	//default for loop clear (slow)
	for(xlong i=size-1;i>=0;i--)
	{
		buffer[i] = v;
	}
	//*
}

template <typename T>void CLbuffer<T>::fastclear(T v)
{
	//default x86 assembly clear (average)
	xlong* btemp = reinterpret_cast<xlong*>(&buffer[0]);
	xlong* vtemp = reinterpret_cast<xlong*>(&v);

	__asm__ __volatile__ ( "cld; rep stosl;" : : "a"(*vtemp),"D"(btemp),"c"(size) );
	//*
}

template <typename T>void CLbuffer<T>::ultraclear(T v)
{
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong purev = *(reinterpret_cast<xlong*>(&v));
	xlong  packed[4];
	packed[0] = packed[1] = packed[2] = packed[3] = purev;
	uxlong i = size;

	//x86 SSE1 assembly clear (very fast)
	if(size>262144 && havesse)
	{
		i>>=5;
		for(;i>0;i--)
		{
			//blast with all 8 xmm regs, and movaps instead of movups (?)
			__asm__ __volatile__ (
			"movups (%0),%%xmm0;"
			"movups %%xmm0,%%xmm1;"
			"movups %%xmm0,%%xmm2;"
			"movups %%xmm0,%%xmm3;"
			"movups %%xmm0,%%xmm4;"
			"movups %%xmm0,%%xmm5;"
			"movups %%xmm0,%%xmm6;"
			"movups %%xmm0,%%xmm7;"
			"movups %%xmm0,(%1);"
			"movups %%xmm1,16(%1);"
			"movups %%xmm2,32(%1);"
			"movups %%xmm3,48(%1);"
			"movups %%xmm4,64(%1);"
			"movups %%xmm5,80(%1);"
			"movups %%xmm6,96(%1);"
			"movups %%xmm7,112(%1);"
			: : "r"(&packed[0]),"r"(puredst) );
			puredst+=32;
		}
	}
	//*
	
	//86 MMX assembly clear (fast)
	else if(size>65536 && havemmx)
	{
		i>>=4;
		for(;i>0;i--)
		{
			//blast with all 8 mm regs
			__asm__ __volatile__ (
			"movq (%0),%%mm0;"
			"movq %%mm0,%%mm1;"
			"movq %%mm0,%%mm2;"
			"movq %%mm0,%%mm3;"
			"movq %%mm0,%%mm4;"
			"movq %%mm0,%%mm5;"
			"movq %%mm0,%%mm6;"
			"movq %%mm0,%%mm7;"
			"movq %%mm0,(%1);"
			"movq %%mm1,8(%1);"
			"movq %%mm2,16(%1);"
			"movq %%mm3,24(%1);"
			"movq %%mm4,32(%1);"
			"movq %%mm5,40(%1);"
			"movq %%mm6,48(%1);"
			"movq %%mm7,56(%1);"		
			: : "r"(&packed[0]),"r"(puredst) );
			puredst+=16;
		}
	}
	//*
	
	//fallback if no SIMD neither MMX or SSE1 is available
	else
	{
		__asm__ __volatile__ (	"cld; rep; stosl;" : : "a"(v),"D"(puredst),"c"(size));
	}
	//*
}

template <typename T>void CLbuffer<T>::copy(T *dst)
{
	//default for loop copy (slow)
	for(xlong i=size-1;i>=0;i--)
	{
		dst[i] = buffer[i];
	}
	//*
}

template <typename T>void CLbuffer<T>::copy(CLbuffer *dst)
{
	//default for loop copy (slow)
	for(xlong i=size-1;i>=0;i--)
	{
		dst[i] = buffer[i];
	}
	//*
}

template <typename T>void CLbuffer<T>::fastcopy(xlong *dst)
{
	//default x86 assembly copy (average)
	memcpy(dst,buffer,size<<2); //temp
	//*
}

template <typename T>void CLbuffer<T>::ultracopy(xlong *dst)
{
	xlong* puresrc = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&dst[0]));
	uxlong i = size;
	
	//x86 SSE1 assembly copy (very fast)
	if(size>262144 && havesse)
	{
		i>>=5;
		for(;i>0;i--)
		{
			//blast wth all 8 xmm regs, and movaps instead of movups (?)
			__asm__ __volatile__ (
			"prefetch 320(%0);"
			"movups (%0),   %%xmm0;"
			"movups 16(%0), %%xmm1;"
			"movups 32(%0), %%xmm2;"
			"movups 48(%0), %%xmm3;"
			"movups 64(%0), %%xmm4;"
			"movups 80(%0), %%xmm5;"
			"movups 96(%0), %%xmm6;"
			"movups 112(%0),%%xmm7;"
			"movntps %%xmm0,(%1);"
			"movntps %%xmm1,16(%1);"
			"movntps %%xmm2,32(%1);"
			"movntps %%xmm3,48(%1);"
			"movntps %%xmm4,64(%1);"
			"movntps %%xmm5,80(%1);"
			"movntps %%xmm6,96(%1);"
			"movntps %%xmm7,112(%1);"	
			: : "r"(puresrc),"r"(puredst) );
			puresrc+=32;
			puredst+=32;
		}
	}
	//*
	
	//x86 MMX assembly copy (very fast)
	else if(size>65536 && havemmx)
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
	else
	{
		__asm__ __volatile__ (	"cld; rep; movsl;" : :"S"(puresrc),"D"(puredst),"c"(i));
	}
	//*
}

template <typename T>void CLbuffer<T>::blendcopy(T* dst,xlong o)
{
	//special copy methods utilizing logical and arthmic operators to combine source with buffers
	switch(o)
	{
		case 0:		//NONE //?default
			for(xlong i=size-1;i>=0;i--)
			{
				dst[i] = buffer[i];
			}
		break;
		
		case 1:		//AND
			for(xlong i=size-1;i>=0;i--)
			{
				dst[i] = dst[i] && buffer[i];
			}
		break;
		
		case 2:		//OR
			for(xlong i=size-1;i>=0;i--)
			{
				dst[i] = dst[i] || buffer[i];
			}
		break;
		
		case 3:		//OR
			for(xlong i=size-1;i>=0;i--)
			{
				dst[i] = dst[i] ^ buffer[i];
			}
		break;
		
		case 4:		//ADD
			for(xlong i=size-1;i>=0;i--)
			{
				dst[i] = dst[i] + buffer[i];
			}
		break;
		
		case 5:		//AA
			for(xlong i=size-1;i>=0;i--)
			{
				//2xRGMS ( (x,y) + ( (x+1,y-1) + (x-1,y+1) / 2 )
			}
		break;
	}
	//*
}

template <typename T>void CLbuffer<T>::blendcopy(CLbuffer<T>* dst,xlong o)
{

}

template <typename T>uxlong CLbuffer<T>::getsize()
{
	return size;
}

template <typename T>uxlong CLbuffer<T>::getbytesize()
{
	return bsize;
}

template <typename T>T* CLbuffer<T>::getbuffer()
{
	return buffer;
}

template <typename T>T& CLbuffer<T>::operator[](uxlong i)
{
	if(i>=size) return buffer[size];
	return buffer[i];
}

//typedefs:
typedef CLbuffer<float> CLfbuffer;
typedef CLbuffer<xlong> CLlbuffer;
//*

#endif

