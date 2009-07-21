//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLCONSTS
#define HH_CLCONSTS
#pragma message "warning " __FILE__ " ! TODO: more"

//basic colors
#define CLsyscol 0x00C0C0C0
#define CLtrans  0xFF000000
#define CLblack  0x00000000
#define CLwhite  0x00FFFFFF
#define CLred    0x00FF0000
#define CLblue   0x0000FF00
#define CLgreen  0x000000FF
#define CLyellow 0x00FFFF00
#define CLpink   0x00FF00FF
#define CLcyan   0x0000FFFF

//render flags
#define CENTER  0b10000000
#define FLAT    0b01000000
#define AMBIENT 0b00100000
#define SHADOW  0b00010000
#define SHADER  0b00001000
#define LPROJ   0b00000100
#define SHAPE   0b00000010
#define DEBUG   0b00000001

//bench flags
#define FPS		 0b00000001
#define RAM		 0b00000010
#define VRAM	 0b00000100
#define CPU		 0b00001000
#define COUT_FPS 0b00010001
#define COUT_RAM 0b00100010
#define COUT_VRAM 0b01000100
#define COUT_CPU 0b10001000

//keycodes
#define LEFT    81
#define RIGHT   83
#define UP      82
#define DOWN    84
#define ESC     27
#define SPACE   32
#define CTRL    -29
#define ENTER   13

//CPUFLAGS
#define MMXFLAG 0b00000000100000000000000000000000
#define SSEFLAG 0b00000010000000000000000000000000

//3d engine consts
#define prjx 80
#define prjy 95

#endif

