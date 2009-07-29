//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMISC
#define HH_CLMISC
#pragma message "Compiling " __FILE__ " ! TODO: all"

void crosslevel()
{

}

//guifx
void drawmousegfx() { }
void drawmouseorbitter() { }

//extrak 10k buffer
void clearbuffer() { }
xlong* getbufferptr() { }

//fakecd
void loadcd() { }
void playcd() { }
void stopcd() { }
void settrack() { }
void skiptrack() { }
void prevtrack() { }
void pausecd() { }
void ejectcd() { }

//detect
void detectram() { }
void detectgpu() { }
void detectvram() { }
void detectsb() { }
void detectps() { }

//basic
bool eof() { }
xchar inkey() { }
void int() { }

//cfg
void loadconfig() { }

//fltk
void systemfontout() { }

//kern
xchar* getexesize() { }
void getenv() { }
void writetofile() { }
void appendfile() { }

//class mouse
void hide() { }
void show() { }
void setto() { }

//console: clk,end,info,ver,mem,set,run,see,

class CLmap
{
	xchar** key;
	xchar** dat;
	
	xchar* operator[](xchar* i);
};

//ASCIImissile
//CLxml

//static polymorphism for CLvector, CLfixed
//template version of CLformat.loadcsv 
//CLmath.acos,asin mit lookup tables
//template version of CLsystem.ato
//change z orientation (polygon.visible, polygon.rasterize,object.ctor)
//flatshade in asm
//enemy copy ctor
//terrain dep transformations and zlevel

#endif

