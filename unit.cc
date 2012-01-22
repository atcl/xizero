// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZunit.cc
// Experimental Testing Ground

///includes
#include "xizero.hh"
///*

///definition
/*void generatefx()
{
	#include <math.h>
	#include <iostream>

	double t = 1;
	for(long i=0;i<FX;++i)
	{
		std::cout << "at[" << i << "]: " << atan(t) << "\t";
		t /= 2;
		std::cout << "ah[" << i << "]: " << 0.5*log((1+t)/(1-t)) << std::endl;
	}

	fixed x = FXONE;
	fixed y = 0;
	fixed z = 0;
	fx::cordic(x,y,z,FXMON,0);
	system::say(string::conf(x),1);
	system::say(string::conf(fx::div(FXONE,x)),1);
	
	x = FXONE;
	y = 0;
	z = 0;
	fx::cordic(x,y,z,FXMON,1);
	system::say(string::conf(x),1);
	system::say(string::conf(fx::div(FXONE,x)),1);
}*/

void viewer(object& u,long k)
{
		const lvector pos(400,300,100);
		static long rc = R_F;
		object::linear.clear();
		switch(k)
		{
			case UP:     object::linear.rotatex(FXONE); break;
			case DOWN:   object::linear.rotatex(FXMON); break;
			case RIGHT:  object::linear.rotatey(FXONE); break;
			case LEFT:   object::linear.rotatey(FXMON); break;
			case PGUP:   object::linear.rotatez(FXONE); break;
			case PGDOWN: object::linear.rotatez(FXMON); break;

			case 'a': object::linear.translate(FXMON,0,0); break;
			case 'd': object::linear.translate(FXONE,0,0); break;
			case 'w': object::linear.translate(0,FXONE,0); break;
			case 's': object::linear.translate(0,FXMON,0); break;
			case 'q': object::linear.translate(0,0,FXONE); break;
			case 'e': object::linear.translate(0,0,FXMON); break;

			case '1': object::linear.scale(FXONE+FXTNT,FXONE,FXONE); break;
			case '2': object::linear.scale(FXONE-FXTNT,FXONE,FXONE); break;
			case '3': object::linear.scale(FXONE,FXONE+FXTNT,FXONE); break;
			case '4': object::linear.scale(FXONE,FXONE-FXTNT,FXONE); break;
			case '5': object::linear.scale(FXONE,FXONE,FXONE+FXTNT); break;
			case '6': object::linear.scale(FXONE,FXONE,FXONE-FXTNT); break;

			case 'r': u.reset(); rc=R_F; break;
			case 't': object::linear.scale(FXONE+FXTNT,FXONE+FXTNT,FXONE+FXTNT); break;
			case 'z': object::linear.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); break;
			case 'u': break;
			case 'i': u.pull(FXTNT); break;
			case 'o': u.pull(-FXTNT); break;
			case 'p': rc=R_B; break;
			case 'l': rc=R_S; break;

			case '0': menu();
		}
		u.update();
		u.display(pos,rc);
}
///*

///implementation
int main(int argc,char** argv)
{
	font::init(system::ldf("dat/XZsys.fnt"),SYSFONT);

	object u(system::ldf(argv[1]));

	level v(system::ldf("dat/level0.lvl"));

	//unit-test screen + buffer
	screen::init(XRES,YRES,TITLE" "VERSION);

	//mainmenu();

	while(screen::run())
	{
		//viewer(u,screen::turbo());
		polygon::counter = 0;
		v.update(screen::turbo());
		v.display();
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
	//*
	
	//generate();
	
	return 0;
}
///*

