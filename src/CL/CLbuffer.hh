///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLstruct.hh"
#include "CLdetect.hh"
///*

///header
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
///*

///definitions
template <typename T>
class CLbuffer : public CLbase<CLbuffer<T>,0>
{
	private:
		static CLdetect* cldetect;
	protected:
		T* buffer;
		uxlong size;
		uxlong bsize;
		xlong  ttype;
	public:
		CLbuffer(uxlong s,T ival=0);
		CLbuffer(const CLbuffer<T>& c);
		~CLbuffer() { delete buffer; };
		void clear(T v=0);
		void copy(CLbuffer<T>* dst,xlong o=0) const;
		uxlong getsize() const { return size; };
		uxlong getbytesize() const { return bsize; };
		T* getbuffer() const { return buffer; };
		T& operator[](uxlong i);
};

template <typename T>
CLdetect* CLbuffer<T>::cldetect = CLdetect::instance();
///*

///implementation
template <typename T>
CLbuffer<T>::CLbuffer(uxlong s,T ival) //! noncritical
{
	//adjust size and allocate buffer
	size = s + (s%4); // make sure is a multiple of 16byte
	bsize = size<<2;
	buffer = new T[size];
	ttype = cldetect->mmx() + cldetect->sse();
	ttype = 1;
	clear(ival);
	//*
}

template <typename T>
CLbuffer<T>::CLbuffer(const CLbuffer<T>& c) //! noncritical
{
	size = c.size;
	bsize = c.bsize;
	buffer = new T[size];
	ttype = cldetect->mmx() + cldetect->sse();
	ttype = 1;
	c.copy(buffer);
}

template <typename T>
void CLbuffer<T>::clear(T v) //! critical
{
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong purev = (reinterpret_cast<xlong*>(&v))[0];
	xlong* pack = new xlong[4];
	pack[0] = pack[1] = pack[2] = pack[3] = purev;
	register xlong i = size;

	//x86 SSE1 assembly clear 
	if(ttype==3)
	{
		for(i>>=5;i>0;i--)
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
	else for(;i>=0;i--) { buffer[i] = v; }
	//*
}

template <typename T>
void CLbuffer<T>::copy(CLbuffer<T>* dst,xlong o) const //! critical
{
	T* dstbuf = dst->getbuffer();
	xlong* puresrc = static_cast<xlong*>(static_cast<void*>(&buffer[0]));
	xlong* puredst = static_cast<xlong*>(static_cast<void*>(&dstbuf[0]));
	register xlong i = size;
	doubleword tx = { 0 };
	doubleword ty = { 0 };
	doubleword tz = { 0 };
	doubleword tw = { 0 };
		
	//special copy methods utilizing logical and arthmic operators to combine source with buffers
	switch(o)
	{
		case 1: //FAST
		
			//~ __asm__ __volatile__ (
			//~ "1: prefetchnta (%0)\n"
			//~ "   prefetchnta 64(%0)\n"
			//~ "   prefetchnta 128(%0)\n"
			//~ "   prefetchnta 192(%0)\n"
			//~ : : "r" (puresrc) );
		
			for(i>>=5;i>0;i--)
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
			break;
			
		case 2: //2xAA = 2xRGMS : (2*(x,y) + (x+1,y-1) + (x-1,y+1))/4
			for(i-=(XRES+2);i>=XRES+1;i--)
			{
				tx.dd = buffer[i-XRES-1];
				ty.dd = buffer[i+XRES+1];
				tz.dd = buffer[i];

				tw.db[0] = uxchar( ( (uxlong(tx.db[0])<<1) + uxlong(ty.db[0]) + uxlong(tz.db[0]) ) >>2 );
				tw.db[1] = uxchar( ( (uxlong(tx.db[1])<<1) + uxlong(ty.db[1]) + uxlong(tz.db[1]) ) >>2 );  
				tw.db[2] = uxchar( ( (uxlong(tx.db[2])<<1) + uxlong(ty.db[2]) + uxlong(tz.db[2]) ) >>2 );  
				tw.db[3] = uxchar( ( (uxlong(tx.db[3])<<1) + uxlong(ty.db[3]) + uxlong(tz.db[3]) ) >>2 ); 
				
				dstbuf[i] = tw.dd;  
			}
		break;
			
		case 3: for(i--;i>=0;i--) { dstbuf[i] = !buffer[i]; } break; //NOT
		
		//~ case 4: for(i--;i>=0;i--) { dstbuf[i] = dstbuf[i] & buffer[i]; } break; //AND
		//~ 
		//~ case 5: for(i--;i>=0;i--) { dstbuf[i] = dstbuf[i] | buffer[i]; } break; //OR
		//~ 
		//~ case 6:	for(i--;i>=0;i--) { dstbuf[i] = !(dstbuf[i] & buffer[i]); } break; //NAND
		//~ 
		//~ case 7:	for(i--;i>=0;i--) { dstbuf[i] = !(dstbuf[i] | buffer[i]); } break; //NOR
		//~ 
		//~ case 8:	for(i--;i>=0;i--) { dstbuf[i] = dstbuf[i] ^ buffer[i]; } break; //XOR
		
		case 9:	 for(i--;i>=0;i--) { dstbuf[i] = dstbuf[i] + buffer[i]; } break; //ADD
		
		case 10: for(i--;i>=0;i--) { dstbuf[i] = dstbuf[i] - buffer[i]; } break; //SUB
		
		case 11: for(i--;i>=0;i--) { dstbuf[i] = byteadd(dstbuf[i],buffer[i]); } break; //BYTE ADD
		
		case 12: for(i--;i>=0;i--) { dstbuf[i] = bytesub(dstbuf[i],buffer[i]); } break; //BYTE SUB
		
		default: for(i--;i>=0;i--) { dstbuf[i] = buffer[i]; } break; //NONE
	}
	//*
}

template <typename T>
T& CLbuffer<T>::operator[](uxlong i) //! critical
{
	if(i>=size) { return buffer[size-1]; }
	return buffer[i];
}
///*

///declarations
typedef CLbuffer<float> CLfbuffer;
typedef CLbuffer<xlong> CLlbuffer;
typedef CLbuffer<uxlong> CLubuffer;
///*

#endif

