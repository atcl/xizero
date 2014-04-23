///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// XiZero header ( xizero.hh )
///</header>

///<include>
#include "xz.hh"
///</include>

///<declare>
#define LEVELS 1 
///</declare>

///<define>
static const char* ascii = "\n\
 XXXXX  000\n\
       0   0\n\
  XXX  0   0\n\
       0   0\n\
 XXXXX  000\n"; //X to Ξ

static const char* keys = "\
UP        Move Forward\n\
DOWN      Move Backward\n\
LEFT      Steer Left\n\
RIGHT     Steer Right\n\
A         Turn Tower Left\n\
D         Turn Tower Right\n\
W         Align Tower Center\n\
SPACE     Fire\n\
ENTER     Menu\n";

info config;

void init();
void intro();
void mainmenu();

xint gameloop(xint i);

void won(const vector& p);
void lost();
void over();
///</define>

///<code>
void init()
{
	config = format::ini(system::ldf("xz.sys"));
	screen::init(format::xpm(resource::cursor));
	system::say(ascii,1);
}

void intro()
{
	//load, clone and place object
	char* f = system::ldf("intro.a");
	info a = format::ar(f);
	object q(a["level.y3d"]);
	object c(a["cross.y3d"]);
	object b[4] = { q,object(q),object(q),object(q) };

	vector p{XRES/2,YRES/2,400,0};
	object::linear.clear(); object::linear.translate(0,fx::l2f(60),0);     c.update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(30),0);  b[0].update();
	object::linear.clear(); object::linear.translate(0,0,0);            b[1].update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(-30),0); b[2].update();
	object::linear.clear(); object::linear.translate(0,fx::l2f(-60),0); b[3].update();
	//*

	//animate in phases
	const xint start = screen::time();
	xint diff = start;

	while(screen::run() && (diff=screen::time()-start)<3200 && screen::key()!=SPACE)
	{
		const fixed prog = fx::div(fx::l2f(diff),FX(200));
		object::linear.clear();

		if(prog<FX(4) ) { const fixed s0=-FXHLF; object::linear.translate(0,s0,0); }
		if(prog<FX(8) ) { const fixed s1=0; object::linear.rotatex(s1); }
		if(prog<FX(12)) { const fixed s2=0; object::linear.translate(0,0,s2); }

		   c.update();
		b[0].update();
		b[1].update();
		b[2].update();
		b[3].update();

		screen::frame.clear(BLACK);
		screen::depth.clear(0);

		   c.display(p,R_F|R_Z);
		b[0].display(p,R_F|R_Z);
		b[1].display(p,R_F|R_Z);
		b[2].display(p,R_F|R_Z);
		b[3].display(p,R_F|R_Z);
	}
	//*

	delete f;
}

void mainmenu()
{
	//load, clone and place objects
	char* f = system::ldf("intro.a");
	info  a = format::ar(f);
	const object v(a["segver.y3d"]);
	const object h(a["seghor.y3d"]);
	object x[3] = { object(h),object(h),object(h) };
	object z[6] = { object(h),object(v),object(v),object(v),object(v),object(h) };

	vector p{XRES/2,YRES/2,450,0};
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
	buttons bl;
	#define VIS BLACK,RED,GREY,DWHITE,1
	bl.add("Start",[]() { for(yint i=0;i<LEVELS;++i) { gameloop(i); } over(); return 0; },0,(XRES-(XRES/4))/2,120,XRES/4,YRES/8,VIS);
	bl.add("Controls",[](){ return dialog::msgbox(keys); },0,(XRES-(XRES/4))/2,200,XRES/4,YRES/8,VIS);
	bl.add("About",[](){ dialog::msgbox("XiZero\nby atCROSSLEVEL studios\nVersion: " VERSION ); return 0; },0,(XRES-(XRES/4))/2,280,XRES/4,YRES/8,VIS);
	bl.add("Exit",[](){ if(dialog::msgbox("Are you sure?",1)==1) { system::bye(); }; return 0; },0,(XRES-(XRES/4))/2,360,XRES/4,YRES/8,VIS);
	//*

	//draw menu
	while(screen::run())
	{
		screen::frame.clear(BLACK);
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
		font::draw(XRES-(XRES/4),YRES-font::height(),"Version: " VERSION,ORANGE);

		bl.draw();
		bl.check(screen::key());
	}
	//*

	delete f;
}

xint gameloop(xint i)
{

	level l(system::ldf("level0.a"));
system::say("hi",1);
	while(screen::run())
	{
		polygon::counter = 0;
		ifu(screen::key()==ESCAPE) { menu::show(); l.resume(); }

		switch(l.update(0))
		{
			case -1: lost(); return 0;
			case  1: won(l.ppos()); return 0;
		}
				
		l.display();
		game::benchmark();
	}

	return 0;
}

void won(const vector& p)
{
	trans::circleblend(p.x,p.y,60);
	font::draw(40,40,"You won.",WHITE);
	screen::run();
	screen::sleep(3000);
	trans::dissolve();
}

void lost()
{
	trans::fadeout();
	font::draw(40,40,"You lost.",WHITE);
	screen::run();
	screen::sleep(3000);
	trans::dissolve();
}

void over()
{
	screen::frame.clear(0);
	font::draw(40,40,"Thanks for playing.",WHITE);
	font::draw((XRES>>1)-60,(YRES>>1),"atcrosslevel.de",RED);
	screen::run();
	screen::sleep(3000);
}
///</code>

