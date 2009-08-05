//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
//#pragma message "Compiling " __FILE__ " ! TODO: rewrite as dynamic class (w templates?)"

#include <stdio.h>
#include <string.h>

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLmacros.hh"


template <typename T>class CLbuffer : public virtual CLcl
{
	private:
		T* buffer;
		uxlong size;
		uxlong ds;
		uxlong qs;
		bool   mmx;
		bool   sse;
		
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
		T* getbuffer();
		T& operator[](uxlong i);
};

template <typename T>CLbuffer<T>::CLbuffer(uxlong s)
{
	size = s;
	buffer = new T[s];
	//ds = s >> 2;
	//qs = s >> 4;
	
	mmx = 0;
	sse = 0;
	//detectCPU(mmx,sse); //crashes..
}

template <typename T>CLbuffer<T>::~CLbuffer() { }

template <typename T>void CLbuffer<T>::clear(T v)
{
	for(xlong i=size-1;i>=0;i--)
	{
		buffer[i] = v;
	}
}

template <typename T>void CLbuffer<T>::fastclear(T v)
{
	xlong* btemp = reinterpret_cast<xlong*>(&buffer[0]);
	xlong* vtemp = reinterpret_cast<xlong*>(&v);

	//memset(btemp,*vtemp,size<<2);

	__asm__ __volatile__ ( "cld; rep stosl;" : : "a"(*vtemp),"D"(btemp),"c"(size) );
}

template <typename T>void CLbuffer<T>::ultraclear(T v)
{
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong i=0;

	if(size>262144 && sse)
	{
		i=0;
		for(;i>0;i--)
		{
			//blast wth all 8 xmm regs
			//__asm__ __volatile__ ();
		}
	}
	else if(size>65536 && mmx)
	{
		i=0;
		for(;i>0;i--)
		{
			//blast with all 8 mm regs
			//__asm__ __volatile__ ();
		}
	}
	else
	{
		__asm__ __volatile__ (	"cld; rep; stosl;" : : "a"(v),"D"(puredst),"c"(size));
	}
}

template <typename T>void CLbuffer<T>::copy(T *dst)
{
	for(xlong i=size-1;i>=0;i--)
	{
		dst[i] = buffer[i];
	}
}

template <typename T>void CLbuffer<T>::copy(CLbuffer *dst)
{
	for(xlong i=size-1;i>=0;i--)
	{
		dst[i] = buffer[i];
	}
}

template <typename T>void CLbuffer<T>::fastcopy(xlong *dst)
{
	memcpy(dst,buffer,size<<2);
}

template <typename T>void CLbuffer<T>::ultracopy(xlong *dst)
{
	xlong* puresrc = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&dst[0]));
	xlong i=0;
		
	if(size>262144 && sse)
	{
		i=0;
		for(;i>0;i--)
		{
			//prefetch
			//blast with all 8 xmm regs
			//__asm__ __volatile__ ();
		}
	}
	else if(size>65536 && mmx)
	{
		i=0;
		for(;i>0;i--)
		{
			//blast with all 8 mm regs
			//__asm__ __volatile__ ();
		}
	}
	else
	{
		__asm__ __volatile__ (	"cld; rep; movsl;" : :"S"(puresrc),"D"(puredst),"c"(size));
	}
}

template <typename T>void CLbuffer<T>::blendcopy(T* dst,xlong o)
{
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
}

template <typename T>void CLbuffer<T>::blendcopy(CLbuffer<T>* dst,xlong o)
{

}

template <typename T>uxlong CLbuffer<T>::getsize()
{
	return size;
}

template <typename T>T* CLbuffer<T>::getbuffer()
{
	return buffer;
}

template <typename T>T& CLbuffer<T>::operator[](uxlong i)
{
	return buffer[i];
}

//typedefs:
typedef CLbuffer<float> CLfbuffer;
typedef CLbuffer<xlong> CLlbuffer;
//*

#endif

