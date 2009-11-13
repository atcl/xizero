///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///license
#ifndef HH_CLCONSTS
#define HH_CLCONSTS
///*

///header
/* class name:	CLconsts
 * 
 * description:	All constants used by multiple classes
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///declarations
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
//*

//gui default colors
#define CLguifront 0x00000000
#define CLguiback  0x00C0C0C0
#define CLguiframe 0x00000000
//*

//render flags
#define CENTER  0b0000000010000000 //center object on screen
#define FLAT    0b0000000001000000 //use flatshading
#define AMBIENT 0b0000000000100000 //use ambient lighting
#define SHADOW  0b0000000000010000 //render blinn shadows (cancels flat shading) 
#define SHADER  0b0000000000001000 //use pixel shader 
#define LPROJ   0b0000000000000100 //use linear projection instead of simple projection
#define SHAPE   0b0000000000000010 //render wireframe
#define DEBUG   0b0000000000000001 //use not shaded color
#define XPLUS   0b0000000100000000 //don't render polygons with a normal s*(1,0,0),s>0
#define XMINUS  0b0000001000000000 //don't render polygons with a normal s*(-1,0,0),s>0
#define YPLUS   0b0000010000000000 //don't render polygons with a normal s*(0,1,0),s>0
#define YMINUS  0b0000100000000000 //don't render polygons with a normal s*(0,-1,0),s>0
#define ZPLUS   0b0001000000000000 //don't render polygons with a normal s*(0,0,1),s>0
#define ZMINUS  0b0010000000000000 //don't render polygons with a normal s*(0,0,-1),s>0
#define ZLIGHT  0b0100000000000000 //light polygons with normal s*(0,0,1),s!=0, higher depending on their z position
#define ANTICY  0b1000000000000000 //purposely screw the order of polygon vertices up anti-cyclicly
//*

//bench flags
#define FPS		 0b00000001
#define RAM		 0b00000010
#define VRAM	 0b00000100
#define CPU		 0b00001000
#define COUT_FPS 0b00010001
#define COUT_RAM 0b00100010
#define COUT_VRAM 0b01000100
#define COUT_CPU 0b10001000
//*

//keycodes
#define LEFT    81
#define RIGHT   83
#define UP      82
#define DOWN    84
#define ESC     27
#define SPACE   32
#define LCTRL   -29
#define ENTER   13
#define PGUP    73
#define PGDOWN  80
//*

//enitiy types
#define PLAYER 0
#define ENEMY  1
#define BOSS   2
//*

//font types
#define TELEFONT 0;
#define MONOFONT 1;
#define LINEFONT 2;
#define TERMFONT 3;
#define SEGMFONT 4;
#define TALLFONT 5;
//*

//3d engine consts
#define prjx 80
#define prjy 95
//*
///*

#endif

