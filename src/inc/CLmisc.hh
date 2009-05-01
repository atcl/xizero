//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMISC
#define HH_CLMISC
#pragma message "Compiling " __FILE__ " ! TODO: all"

void one()
{
	return 1;
}

void crosslevel()
{

}

//id:
xlong generateid() { }
xlong generatecrc() { }
bool checkcrc() {}

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
void getexename() { }
void getexesize() { }
void getenv() { }
void writetofile() { }
void appendfile() { }

//class mouse
void hide() { }
void show() { }
void setto() { }

//class string
void replace() { }
void left() { }
void right() { }
void mid() { }
void concat() { }
void length() { }
void compare() { }


#endif

//old functions, jusst for reference:

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
