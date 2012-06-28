// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// xizero.hh
// XiZero main header

///includes
#include "xz.hh"
///*

///declarations
#define LEVELS 1 
///*

///definition
inline void init();
sint start(sint i);
sint start();
inline sint close();
inline sint control();
inline sint about();
inline sint leave();
void menu();
void intro();
void mainmenu();
void won(const lvector& p);
void lost();
void over();
void bench();
///*

///Implementation
void init()
{
	screen::init(format::xpm(resource::cursor));
	screen::set(1,1);
}

sint start(sint i)
{
	level l(system::ldf("level0.a"));

	while(screen::run())
	{
		polygon::counter = 0;
		if(screen::turbo()==ESCAPE) { menu(); l.resume(); }

		switch(l.update(screen::key(),0))
		{
			case -1: lost(); return 0;
			case 1:  won(l.ppos());  return 0;
		}
				
		l.display();
		//screen::back.fsaamb(screen::accum);
		l.gauges();
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
	over();
	return 0;
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
	return dialog::msgbox("XiZero\nby atCROSSLEVEL studios\nVersion: " VERSION );
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
	sint cbrk = 0;
	while(screen::run() && screen::key()!=ESCAPE && cbrk==0)
	{
		gfx::sprite(*scr,0,0,1);
		gfx::rect(0,0,XRES,17,RED,RED,1,0);
		gfx::rect(0,18,XRES,35,SYSCOL,SYSCOL,1,0);
		
		font::draw(2,1," atCROSSLEVEL XiZero",WHITE,RED);
		b_close.draw();
		b_about.draw();
		b_leave.draw();
		cbrk = button::check(screen::mousex(),screen::mousey(),screen::mouseb());
		gfx::sprite(*screen::cursor(),screen::mousex(),screen::mousey());
	}
	delete[] scr->data;
	delete scr;
	//*
}

void intro()
{
	//load, clone and place object
	char* f = system::ldf("intro.a");
	info* a = format::ar(f);
	object q((*a)["level.y3d"]);
	object c((*a)["cross.y3d"]);
	object b[4] = { q,object(q),object(q),object(q) };

	lvector p(400,300,100);
	object::linear.clear(); object::linear.translate(0,fx::l2f(60),0);  c.update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(30),0);  b[0].update();
	object::linear.clear(); object::linear.translate(0,0,0);            b[1].update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(-30),0); b[2].update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(-60),0); b[3].update();
	//*

	//animate in phases
	const sint last = screen::time();
	      sint curr = last;
	      sint prog = 0;

	while(screen::run() && curr<last+4500 && screen::key()!=ESCAPE)
	{
		curr = screen::time();
		object::linear.clear();
		const sint diff = curr-last;
		switch( diff/1000 )
		{
			case 1: object::linear.translate(0,fx::mul(fx::l2f(20),prog),0); break;
			case 2: object::linear.rotatex(fx::mul(fx::l2f(14),prog));       break;
			case 3: object::linear.translate(0,0,fx::mul(fx::l2f(23),prog)); break;
		}

		c.update();
		b[0].update();
		b[1].update();
		b[2].update();
		b[3].update();
		screen::back.clear(BLACK);
		screen::depth.clear(fx::l2f(400));
		c.display(p,R_F|R_Z);
		b[0].display(p,R_F|R_Z);
		b[1].display(p,R_F|R_Z);
		b[2].display(p,R_F|R_Z);
		b[3].display(p,R_F|R_Z);
		prog = fx::mul(fx::l2f(curr-screen::time()),FXHUN);
	}
	//*

	delete a;
	delete f;
}

void mainmenu()
{
	//load, clone and place objects
	char* f = system::ldf("intro.a");
	info* a = format::ar(f);
	const object v((*a)["segver.y3d"]);
	const object h((*a)["seghor.y3d"]);
	object x[3] = { object(h),object(h),object(h) };
	object z[6] = { object(h),object(v),object(v),object(v),object(v),object(h) };

	lvector p(400,300,100);
	object::linear.clear(); object::linear.translate(fx::l2f(-120),fx::l2f(-140),0); x[0].update();
	object::linear.clear(); object::linear.translate(fx::l2f(-120),0,0);             x[1].update();
	object::linear.clear(); object::linear.translate(fx::l2f(-120),fx::l2f(140),0);  x[2].update();
	object::linear.clear(); object::linear.translate(fx::l2f(120),fx::l2f(-140),0);  z[0].update();
	object::linear.clear(); object::linear.translate(fx::l2f(60),fx::l2f(-70),0);    z[1].update();
	object::linear.clear(); object::linear.translate(fx::l2f(200),fx::l2f(-70),0);   z[2].update();
	object::linear.clear(); object::linear.translate(fx::l2f(60),fx::l2f(70),0);     z[3].update();
	object::linear.clear(); object::linear.translate(fx::l2f(200),fx::l2f(70),0);    z[4].update();
	object::linear.clear(); object::linear.translate(fx::l2f(120),fx::l2f(140),0);   z[5].update();
	//*

	//enlist buttons
	const button b_start("Start",&start,0,(XRES-(XRES>>2))>>1,200,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_control("Controls",&control,0,(XRES-(XRES>>2))>>1,280,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_about("About",&about,0,(XRES-(XRES>>2))>>1,360,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	const button b_leave("Exit",&leave,0,(XRES-(XRES>>2))>>1,440,XRES>>2,YRES>>3,RED,SYSCOL,DWHITE,1);
	//*

	//draw menu
	while(screen::run())
	{
		screen::back.clear(BLACK);
		//draw menu object
		x[0].display(p,R_S);
		x[1].display(p,R_S);
		x[2].display(p,R_S);
		z[0].display(p,R_S);
		z[1].display(p,R_S);
		z[2].display(p,R_S);
		z[3].display(p,R_S);
		z[4].display(p,R_S);
		z[5].display(p,R_S);
		//*
		b_start.draw();
		b_control.draw();
		b_about.draw();
		b_leave.draw();
		font::draw(XRES-160,YRES-20,"Version: " VERSION,ORANGE,TRANS);
		button::check(screen::mousex(),screen::mousey(),screen::mouseb());
		gfx::sprite(*screen::cursor(),screen::mousex(),screen::mousey());
	}
	//*

	delete a;
	delete f;
}

void won(const lvector& p)
{
	trans::circleblend(p.x,p.y,60);
	font::draw(40,40,"You won.",WHITE,TRANS);
	screen::run();
	screen::sleep(3000);
	trans::dissolve();
}

void lost()
{
	trans::fadeout();
	font::draw(40,40,"You lost.",WHITE,TRANS);
	screen::run();
	screen::sleep(3000);
	trans::dissolve();
}

void over()
{
	font::draw(40,40,"Thanks for playing.",WHITE,BLACK);
	screen::run();
	screen::sleep(3000);
}

void bench()
{
	const sint fps = screen::fps();
	ifu(fps!=-1)
	{
		system::say(string::int2str(polygon::counter*fps)); 			
		system::say("T/s - ");
		system::say(string::int2str(polygon::counter)); 
		system::say("@");
		system::say(string::int2str(fps),1);
	}
}
///*

