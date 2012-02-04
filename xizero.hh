// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// xizero.hh
// XiZero main header

///includes
#include "xz.hh"
///*

///definition
void init();
sint start(sint l);
sint start();
inline sint close();
inline sint load();
inline sint about();
inline sint leave();
void menu();
void intro();
void mainmenu();
void won();
void lost();
void bench();
///*

///Implementation
void init() //PMAIN
{
	font::init(system::ldf("dat/XZsys.fnt"),SYSFONT);
	//font::init(system::ldf("dat/XZbig.fnt"),BIGFONT);
	//font::init(system::ldf("dat/XZseg.fnt"),SEGFONT);
	//font::init(system::ldf("dat/XZsym.fnt"),SYMFONT);

	screen::init(XRES,YRES,TITLE" "VERSION,&polygon::counter,format::xpm(resource::cursor));
}

sint start(sint l)
{
	//load level
	//gfx::sprite(,0,0,0);
	//font::draw(40,40,m,WHITE,TRANS,SYSFONT);
	level v(system::ldf("dat/level0.lvl"));
	//gfx::fsprog(50,RED);
	//dialog::msgbox(n);
	//*

	while(screen::run())
	{
		if(UNLIKELY(screen::key()==ESCAPE)) { menu(); v.resume(); }

		v.update(screen::turbo());
		v.display();

		v.gauges();
		bench();
	}

	return 0;
}

sint start()
{
	return start(0);
}

sint close()
{
	return -1;
}

sint load()
{
	//return start(string::conl(screen::instance()->inpbox("Enter the level number you want to play (0-9):")));
}

sint about()
{
	dialog::msgbox("XiZero\n by atCROSSLEVEL studios\n Version: "VERSION);
	return 0;
}

sint leave()
{
	//return dialog::askbox("Are you sure?");
	system::bye();
	return 0;
}

void menu()
{
	//enlist buttons
	const button b_close("X",&close,1,XRES-20,1,16,16,BLACK,SYSCOL,WHITE,1);
	const button b_about("About",&about,0,2,19,50,16,BLACK,SYSCOL,SYSCOL,1);
	const button b_leave("Exit",&leave,0,52,19,50,16,BLACK,SYSCOL,SYSCOL,1);
	//*

	//draw menu
	tile* scr = gfx::save();
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())!=-1)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(0,0,XRES,17,RED,RED,1,0);
		gfx::rect(0,18,XRES,35,SYSCOL,SYSCOL,1,0);
		
		font::draw(2,1," atCROSSLEVEL: XiZero",WHITE,RED,SYSFONT);
		b_close.draw();
		b_about.draw();
		b_leave.draw();
		gfx::sprite(*(tile*)screen::cursor(),screen::mousex(),screen::mousey());
	}
	//*

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
	tile* cur = format::xpm(resource::cursor);
	//tile* back = format::ras(system::ldf("dat/back.ras"));
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
		gfx::sprite(*(tile*)screen::cursor(),screen::mousex(),screen::mousey());
	}
	//*
}

void won()
{
	//screen::back.clear(BLACK);
	//font::draw(40,40,m,WHITE,TRANS,SYSFONT);
}

void lost()
{
	//screen::back.clear(BLACK);
	//font::draw(40,40,m,WHITE,TRANS,SYSFONT);
}

void bench()
{
	const sint fps = system::fps();
	if(fps!=-1) //(isatty(0)&&fps!=-1)
	{
		system::say(string::int2str(polygon::counter*fps),0); 			
		system::say("p/s - ",0);
		system::say(string::int2str(polygon::counter),0); 
		system::say("@",0);
		system::say(string::int2str(fps),1);
	}
}
///*

