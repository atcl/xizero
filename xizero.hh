// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// xizero.hh
// XiZero main header

///includes
#include "xz.hh"
///*

///declarations
#define LEVELS 2 
///*

///definition
void init();
sint start(sint l);
sint start();
inline sint close();
inline sint control();
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
void init()
{
	screen::init(XRES,YRES,TITLE" "VERSION,format::xpm(resource::cursor));
}

sint start(sint l)
{
	level v(system::ldf("dat/level0.lvl"));

	while(screen::run())
	{
		polygon::counter = 0;
		if(screen::key()==ESCAPE) { menu(); v.resume(); }

		v.update(screen::turbo(),screen::joy());
		v.display();
		//screen::back.fsaamb(screen::accum);
		v.gauges();
		bench();
	}

	return 0;
}

sint start()
{
	for(long i=0;i<LEVELS;++i)
	{
		start(i);
	}
}

sint close()
{
	return -1;
}

sint control()
{
	return dialog::msgbox(
"UP        Move Forward\n"\
"DOWN      Move Backward\n"\
"LEFT      Steer Left\n"\
"RIGHT     Steer Right\n"\
"A         Turn Tower Left\n"\
"D         Turn Tower Right\n"\
"W         Align Tower Center\n"\
"SPACE     Fire\n"\
"ESC       Menu\n");
}

sint about()
{
	return dialog::msgbox("XiZero\nby atCROSSLEVEL studios\nVersion: "VERSION);
}

sint leave()
{
	//if(dialog::askbox("Are you sure?")==-1) { return -1; };
	screen::close();
	system::bye();
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
	while(screen::run() && button::check(screen::mousex(),screen::mousey(),screen::mousel())==0 && screen::key()!=ESCAPE)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(0,0,XRES,17,RED,RED,1,0);
		gfx::rect(0,18,XRES,35,SYSCOL,SYSCOL,1,0);
		
		font::draw(2,1," atCROSSLEVEL XiZero",WHITE,RED);
		b_close.draw();
		b_about.draw();
		b_leave.draw();
		gfx::sprite(*(tile*)screen::cursor(),screen::mousex(),screen::mousey());
	}
	delete scr;
	//*
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
	const button b_start("Start",&start,0,(XRES-(XRES>>2))>>1,200,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_control("Controls",&control,0,(XRES-(XRES>>2))>>1,280,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_about("About",&about,0,(XRES-(XRES>>2))>>1,360,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_leave("Exit",&leave,0,(XRES-(XRES>>2))>>1,440,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	//*

	//draw menu
	//object xizero();
	while(screen::run())
	{
		button::check(screen::mousex(),screen::mousey(),screen::mousel());
		screen::back.clear(BLACK);
		//draw menu object
		b_start.draw();
		b_control.draw();
		b_about.draw();
		b_leave.draw();
		font::draw(XRES-160,YRES-20,"Version: "VERSION,ORANGE,TRANS);
		gfx::sprite(*(tile*)screen::cursor(),screen::mousex(),screen::mousey());
	}
	//*
}

void won()
{
	screen::back.clear(BLACK);
	font::draw(40,40,"You won.",WHITE,TRANS);
}

void lost()
{
	screen::back.clear(BLACK);
	font::draw(40,40,"You lost.",WHITE,TRANS);
}

void bench()
{
	const sint fps = screen::fps();
	if(fps!=-1)
	{
		system::say(string::int2str(polygon::counter*fps)); 			
		system::say("p/s - ");
		system::say(string::int2str(polygon::counter)); 
		system::say("@");
		system::say(string::int2str(fps),1);
	}
}
///*

