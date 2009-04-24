//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
#pragma message "Compiling " __FILE__ " ! TODO: rewrite as dynamic class (w templates?)"


#include <string.h>

template <typename T>class CLbuffer
{
	private:
		static xlong version;
		T* buffer;
		uxlong size;
		
	public:
		CLbuffer(uxlong s);
		~CLbuffer();
		
		void clear(T v);
		void fastclear(xlong v);
		void copy(T *dst);
		void copy(CLbuffer *dst);
		void fastcopy(xlong *dst);
		void blendcopy(T *dst,xlong o);
		uxlong getsize();
		T* getbuffer();
		T& operator[](uxlong i);
		xlong getversion();
};

template <typename T>xlong CLbuffer<T>::version = 0x00010000;

template <typename T>CLbuffer<T>::CLbuffer(uxlong s)
{
	size = s;
	buffer = new T[s];
}

template <typename T>CLbuffer<T>::~CLbuffer() { }

template <typename T>void CLbuffer<T>::clear(T v)
{
	for(int i=0;i<size;i++)
	{
		buffer[i] = v;
	}
}

template <typename T>void CLbuffer<T>::fastclear(xlong v)
{
	memset(buffer,v,size<<2);
}

template <typename T>void CLbuffer<T>::copy(T *dst)
{
	for(int i=0;i<size;i++)
	{
		dst[i] = buffer[i];
	}
}

template <typename T>void CLbuffer<T>::copy(CLbuffer *dst)
{
	for(int i=0;i<size;i++)
	{
		dst[i] = buffer[i];
	}
}

template <typename T>void CLbuffer<T>::fastcopy(xlong *dst)
{
	memcpy(dst,buffer,size<<2);
}

template <typename T>void CLbuffer<T>::blendcopy(T* dst,xlong o)
{
	switch(o)
	{
		case 0:		//NONE
			for(int i=0;i<size;i++)
			{
				dst[i] = buffer[i];
			}
		break;
		case 1:		//AND
			for(int i=0;i<size;i++)
			{
				dst[i] = dst[i] && buffer[i];
			}
		break;
		case 2:		//OR
			for(int i=0;i<size;i++)
			{
				dst[i] = dst[i] || buffer[i];
			}
		break;
		case 3:		//OR
			for(int i=0;i<size;i++)
			{
				dst[i] = dst[i] ^ buffer[i];
			}
		break;
		case 4:		//ADD
			for(int i=0;i<size;i++)
			{
				dst[i] = dst[i] + buffer[i];
			}
		break;
	}
	
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

template <typename T>xlong CLbuffer<T>::getversion()
{
	return version;
}

#endif
