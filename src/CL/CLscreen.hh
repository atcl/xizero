///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLSCREEN
#define HH_CLSCREEN
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLbuffer.hh"
///*

///header
/* class name:	CLscreen
 * 
 * description:
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLscreen : public CLbase<CLscreen,1>
{
	friend class CLbase<CLscreen,1>;

	protected:
		uxlong bytewidth;
		uxlong byteheight;
		uxlong bytedepth;
		uxlong bytesize;
		uxlong pixelwidth;
		uxlong pixelheight;
		uxlong pixeldepth;
		uxlong pixelsize;
		CLscreen();
		~CLscreen();
	public:
		uxlong getbytewidth() const { return bytewidth; };
		uxlong getbyteheight() const { return byteheight; };
		uxlong getbytedepth() const { return bytedepth; };
		uxlong getbytesize() const { return bytesize; };
		uxlong getpixelwidth() const { return pixelwidth; };
		uxlong getpixelheight() const { return pixelheight; };
		uxlong getpixeldepth() const { return pixeldepth; };
		uxlong getpixelsize() const { return pixelsize; };
		CLubuffer cldoublebuffer;
		CLubuffer clframebuffer;
		CLubuffer clstencilbuffer;
		CLfbuffer clzbuffer;
		//CLubuffer clabuffer;
};
///*

///implementation
CLscreen::CLscreen() : cldoublebuffer((XRES*YRES),0), clframebuffer((XRES*YRES),0), clstencilbuffer((XRES*YRES),0), clzbuffer((XRES*YRES),ZRES)
{
	bytewidth = XRES<<2;
	byteheight = YRES<<2;
	bytedepth = 4;
	bytesize = (XRES*YRES)<<2;
	pixelwidth = XRES;
	pixelheight = YRES;
	pixeldepth = ZRES;
	pixelsize = XRES*YRES;
}

CLscreen::~CLscreen()
{
	
}
///*

#endif

