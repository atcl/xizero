///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// xizero.hh
// XiZero main header
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

inline void init();
xint start(xint i);
xint start();
inline xint control();
inline xint about();
inline xint leave();
void menu();
void intro();
void mainmenu();
void won(const tuple& p);
void lost();
void over();
void bench();
///</define>

///<code>
void init()
{
	screen::init(format::xpm(resource::cursor));
	system::say(ascii,1);
}

xint start(xint i)
{
	level l(system::ldf("level0.a"));

	while(screen::run())
	{
		polygon::counter = 0;
		ifu(screen::key()==ESCAPE) { menu(); l.resume(); }

		switch(l.update(0))
		{
			case -1: lost(); return 0;
			case  1: won(l.ppos()); return 0;
		}
				
		l.display();
		bench();
	}

	return 0;
}

xint start()
{
	for(yint i=0;i<LEVELS;++i)
	{
		start(i);
	}
	over();
	return 0;
}

xint contr()
{
	return dialog::msgbox(keys);
}

xint about()
{
	dialog::msgbox("XiZero\nby atCROSSLEVEL studios\nVersion: " VERSION );
	return 0;
}

xint leave()
{
	if(dialog::msgbox("Are you sure?",1)==1) { system::bye(); };
	return 0;
}

void menu()
{
	//enlist buttons
	buttons bl;
	bl.add("About",&about,0,2,19,50,16,BLACK,RED,GREY,GREY,1);
	bl.add("Exit",&leave,0,52,19,50,16,BLACK,RED,GREY,GREY,1);
	bl.add("X",[](){ return 1; },1,XRES-20,1,16,16,BLACK,RED,GREY,WHITE,1);
	//*

	//draw menu
	tile ico = format::xpm(resource::icon);
	tile scr = gfx::save();
	xint xit = 0;
	while(screen::run() && xit==0)
	{
		gfx::draw(scr);
		gfx::draw(ico);
		gfx::rect(16,0,XRES,17,RED,RED,1,0);
		gfx::rect(0,18,XRES,35,GREY,GREY,1,0);
		font::draw(20,1,"atCROSSLEVEL XiZero",WHITE,RED);

		bl.draw();
		xit = bl.check(screen::key());
	}
	//*

}

void intro()
{
	//load, clone and place object
	char* f = system::ldf("intro.a");
	info a = format::ar(f);
	object q(a["level.y3d"]);
	object c(a["cross.y3d"]);
	object b[4] = { q,object(q),object(q),object(q) };

	tuple p{XRES/2,YRES/2,500};
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

		if(prog<FX(4) ) { const fixed a=-FXHLF; object::linear.translate(0,a,0); }
		if(prog<FX(8) ) { const fixed b=0; object::linear.rotatex(b); }
		if(prog<FX(12)) { const fixed c=0; object::linear.translate(0,0,c); }

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

	tuple p{XRES/2,YRES/2,450};
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
	bl.add("Start",&start,0,(XRES-(XRES/4))/2,120,XRES/4,YRES/8,VIS);
	bl.add("Controls",&contr,0,(XRES-(XRES/4))/2,200,XRES/4,YRES/8,VIS);
	bl.add("About",&about,0,(XRES-(XRES/4))/2,280,XRES/4,YRES/8,VIS);
	bl.add("Exit",&leave,0,(XRES-(XRES/4))/2,360,XRES/4,YRES/8,VIS);
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
		font::draw(XRES-(XRES/4),YRES-font::height(),"Version: " VERSION,ORANGE,TRANS);

		bl.draw();
		bl.check(screen::key());
	}
	//*

	delete f;
}

void won(const tuple& p)
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
	screen::frame.clear(0);
	font::draw(40,40,"Thanks for playing.",WHITE,BLACK);
	font::draw((XRES>>1)-60,(YRES>>1),"atcrosslevel.de",RED,BLACK);
	screen::run();
	screen::sleep(3000);
}

void bench()
{
	const xint fps = screen::fps();
	ifu(fps>0)
	{
		system::say(string::int2str(polygon::counter*fps)); 			
		system::say("T/s - ");
		system::say(string::int2str(polygon::counter)); 
		system::say("@");
		system::say(string::int2str(fps),1);
	}
}
///</code>

