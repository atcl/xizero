//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCONSTS
#define HH_CLCONSTS
#pragma message "Compiling " __FILE__ " ! TODO: more"

#define xmin 1
#define xmax xres-1
#define ymin 1
#define ymax yres-1
#define zmin 1
#define zmax zres-1

namespace CL
{ 
	#define CLsystem 0x00C0C0C0
	#define CLtrans  0xFF000000
	#define CLblack  0x00000000
	#define CLwhite  0x00FFFFFF
	#define CLred    0x00FF0000
	#define CLblue   0x0000FF00
	#define CLgreen  0x000000FF
	#define CLyellow 0x00FFFF00
	#define CLpink   0x00FF00FF
	#define CLcyan   0x0000FFFF
}

#endif

