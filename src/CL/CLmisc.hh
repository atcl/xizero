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
void detectcpu() { }
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
xchar* getexename() { }
xchar* getexesize() { }
void getenv() { }
void writetofile() { }
void appendfile() { }

//class mouse
void hide() { }
void show() { }
void setto() { }
xlong getmousex();
xlong getmousey();
xlong getmouseb1();
xlong getmouseb2();


//console: clk,end,info,ver,mem,set,run,see,


#endif

//old functions, just for reference:

//from level-constructor, display a level:
// 	for(int s=0; s<levelheight ;s++)
// 	{
// 		for(int t=0; t<levelwidth ;t++)
// 		{
// 			cout << xchar(levellayers[0][s][t]+20) << " ";
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
//*
