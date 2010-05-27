///license
//atCROSSLEVEL studios 2009,2010
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
#include "CLvector.hh"
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
	friend class CLglobal;

	protected:
		uxlong bytewidth;
		uxlong byteheight;
		uxlong bytedepth;
		uxlong bytesize;
		uxlong pixelwidth;
		uxlong pixelheight;
		uxlong pixeldepth;
		uxlong pixelsize;
		CLubuffer doublebuffer;
		CLubuffer framebuffer;
		CLubuffer stencilbuffer;
		CLfbuffer zbuffer;
		//CLubuffer clabuffer;
		CLfvector light;
		CLfvector plane;
		CLfvector slight;
		static uxlong nolight;
		CLscreen();
		~CLscreen() { };
	public:
		uxlong getbytewidth() const { return bytewidth; };
		uxlong getbyteheight() const { return byteheight; };
		uxlong getbytedepth() const { return bytedepth; };
		uxlong getbytesize() const { return bytesize; };
		uxlong getpixelwidth() const { return pixelwidth; };
		uxlong getpixelheight() const { return pixelheight; };
		uxlong getpixeldepth() const { return pixeldepth; };
		uxlong getpixelsize() const { return pixelsize; };
		CLubuffer& cldoublebuffer;
		CLubuffer& clframebuffer;
		CLubuffer& clstencilbuffer;
		CLfbuffer& clzbuffer;
		CLfvector& cllight;
		CLfvector& clplane;
		CLfvector& clslight;
		uxlong& clnolight;
};

uxlong CLscreen::nolight = 0x00303030;
///*

///implementation
CLscreen::CLscreen() : doublebuffer((XRES*YRES),0), framebuffer((XRES*YRES),0), stencilbuffer((XRES*YRES),0), zbuffer((XRES*YRES),ZRES), cldoublebuffer(doublebuffer), clframebuffer(framebuffer), clstencilbuffer(stencilbuffer), clzbuffer(zbuffer), light(1,1,1), plane(0,0.1,1), slight(0,0.4,1.1), cllight(light), clplane(plane), clslight(slight), clnolight(nolight)
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
///*

#endif

