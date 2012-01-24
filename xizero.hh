// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// xizero.hh
// XiZero main header

///includes
#include "xz.hh"
///*

///definition
void init();
long start(long l);
long start();
INLINE long close();
INLINE long load();
INLINE long about();
INLINE long leave();
void menu();
void intro();
void mainmenu();
void won();
void lost();
void bench();
///*

///Implementation
void init()
{
	font::init(system::ldf("dat/XZsys.fnt"),SYSFONT);
	//font::init(system::ldf("dat/XZbig.fnt"),BIGFONT);
	//font::init(system::ldf("dat/XZseg.fnt"),SEGFONT);
	//font::init(system::ldf("dat/XZsym.fnt"),SYMFONT);

	screen::init(XRES,YRES,TITLE" "VERSION,&polygon::counter);
}

long start(long l)
{
	level v(system::ldf("dat/level0.lvl"));

	while(screen::run())
	{
		if(UNLIKELY(screen::key()==ESCAPE)) { menu(); v.resume(); }

		v.update(screen::turbo());
		v.display();

		bench();
	}

	return 0;
}

long start()
{
	return start(0);
}

long close()
{
	return -1;
}

long load()
{
	//return start(string::conl(screen::instance()->inpbox("Enter the level number you want to play (0-9):")));
}

long about()
{
	//screen::instance()->msgbox("XiZero\n by atCROSSLEVEL studios\n Version: "VERSION);
	return 0;
}

long leave()
{
	//return screen::instance()->askbox("Are you sure?");
	system::bye();
}

void menu()
{
	//enlist buttons
	button b_close("X",&close,1,XRES-20,1,16,16,BLACK,SYSCOL,WHITE,1);
	button b_about("About",&about,0,2,19,50,16,BLACK,SYSCOL,SYSCOL,1);
	button b_leave("Exit",&leave,0,52,19,50,16,BLACK,SYSCOL,SYSCOL,1);
	//*

	//draw menu
	tile* scr = gfx::save();
	tile* cur = format::xpm(cursor);
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())!=-1)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(0,0,XRES,17,RED,RED,1,0);
		gfx::rect(0,18,XRES,35,SYSCOL,SYSCOL,1,0);
		
		font::draw(2,1," atCROSSLEVEL: XiZero",WHITE,RED,SYSFONT);
		b_close.draw();
		b_about.draw();
		b_leave.draw();
		gfx::sprite(*cur,screen::mousex(),screen::mousey());
	}
	//*

	delete cur;
	delete scr;
}

void intro()
{
	//load object

	//*

	//animate in phases

	//*
}

void mainmenu()
{
	//enlist buttons
	button b_start("Start",&start,0,(XRES-(XRES>>2))>>1,200,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	button b_load("Load",&load,0,(XRES-(XRES>>2))>>1,280,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	button b_about("About",&about,0,(XRES-(XRES>>2))>>1,360,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	button b_leave("Exit",&leave,0,(XRES-(XRES>>2))>>1,440,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	//*

	//draw menu
	//tile* back = format::ras(system::ldf("dat/back.ras"));
	tile*   cur = format::xpm(cursor);
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())==0)
	{
		screen::back.clear(BLACK); //temp
		//gfx::sprite(*back,0,0);
		//font::draw(100,20,"Xi Zero",WHITE,TRANS,BIGFONT);
		b_start.draw();
		b_load.draw();
		b_about.draw();
		b_leave.draw();
		font::draw(XRES-160,YRES-20,"Version: "VERSION,ORANGE,TRANS,SYSFONT);
		gfx::sprite(*cur,math::lim(0,screen::mousex(),XRES-cur->width),math::lim(0,screen::mousey(),YRES-cur->height));
	}
	//*
}

void won()
{

}

void lost()
{

}

void bench()
{
	const long fps = system::fps();
	if(fps!=-1)
	{
		system::say(string::cons(polygon::counter*fps),0); 			
		system::say("p/s - ",0);
		system::say(string::cons(polygon::counter),0); 
		system::say("@",0);
		system::say(string::cons(fps),1);
	}
}
///*

