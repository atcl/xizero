//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBUFFER
#define HH_CLBUFFER
#pragma message "Compiling " __FILE__ " ! TODO: rewrite as dynamic class (w templates?)"


class CLbuffer
{
	private:
		static xlong version;
		//xlong* lbuffer;
		//float* fbuffer;
		uxlong size;
		
	public:
		CLbuffer();
		~CLbuffer();
		
		xlong* makelong(uxlong s);
		float* makefloat(uxlong s);
		void clear(xlong v,xlong* buffer,uxlong s);
		void clear(float v,float* buffer,uxlong s);
		void copy(xlong *dst,xlong* src,uxlong s);
		void copy(float *dst,float* src,uxlong s);
		void fastcopy(xlong *dst,xlong *src,uxlong s);
		void fastcopy(float *dst,float *src,uxlong s);
		void blendcopy(xlong *dst,xlong* src,uxlong s,xlong o);
		//uxlong getsize();
		//xlong operator[](xlong i);
		//float operator[](xlong i);
		xlong getversion();
};

xlong CLbuffer::version = 0x00010000;

CLbuffer::CLbuffer()
{

}

CLbuffer::~CLbuffer() { }

float* CLbuffer::makefloat(uxlong s)
{
	float* buffer = new float[s];

	return buffer;
}

xlong* CLbuffer::makelong(uxlong s)
{
	xlong* buffer = new xlong[s];

	return buffer;
}

void CLbuffer::clear(xlong v,xlong* buffer,uxlong s)
{
	for(int i=0;i<s;i++)
	{
		buffer[i] = v;
	}
}

void CLbuffer::clear(float v,float* buffer,uxlong s)
{
	for(int i=0;i<s;i++)
	{
		buffer[i] = v;
	}
}

void CLbuffer::copy(xlong *dst,xlong* src,uxlong s)
{
	for(int i=0;i<s;i++)
	{
		dst[i] = src[i];
	}
}

void CLbuffer::copy(float *dst,float *src,uxlong s)
{
	for(int i=0;i<s;i++)
	{
		dst[i] = src[i];
	}
}

void CLbuffer::fastcopy(xlong *dst,xlong* src,uxlong s)
{

}

void CLbuffer::fastcopy(float *dst,float *src,uxlong s)
{

}

void CLbuffer::blendcopy(xlong *dst,xlong* src,uxlong s,xlong o)
{
	switch(o)
	{
		case 0:		//NONE
			for(int i=0;i<s;i++)
			{
				dst[i] = src[i];
			}
		break;
		case 1:		//AND
			for(int i=0;i<s;i++)
			{
				dst[i] = dst[i] && src[i];
			}
		break;
		case 2:		//OR
			for(int i=0;i<s;i++)
			{
				dst[i] = dst[i] || src[i];
			}
		break;
		case 3:		//OR
			for(int i=0;i<s;i++)
			{
				dst[i] = dst[i] ^ src[i];
			}
		break;
		case 4:		//ADD
			for(int i=0;i<s;i++)
			{
				dst[i] = dst[i] + src[i];
			}
		break;
	}
	
}

// uxlong CLbuffer::getsize()
// {
// 	return size;
// }

// xlong CLbuffer::operator[](xlong i)
// {
// 	return lbuffer[i];
// }

// float CLbuffer::operator[](xlong i)
// {
// 	return fbuffer[i];
// }

xlong CLbuffer::getversion()
{
	return version;
}

#endif
