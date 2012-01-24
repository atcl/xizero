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

			case 'A': object::linear.translate(FXMON,0,0); break;
			case 'D': object::linear.translate(FXONE,0,0); break;
			case 'W': object::linear.translate(0,FXONE,0); break;
			case 'S': object::linear.translate(0,FXMON,0); break;
			case 'Q': object::linear.translate(0,0,FXONE); break;
			case 'E': object::linear.translate(0,0,FXMON); break;

			case '1': object::linear.scale(FXONE+FXTNT,FXONE,FXONE); break;
			case '2': object::linear.scale(FXONE-FXTNT,FXONE,FXONE); break;
			case '3': object::linear.scale(FXONE,FXONE+FXTNT,FXONE); break;
			case '4': object::linear.scale(FXONE,FXONE-FXTNT,FXONE); break;
			case '5': object::linear.scale(FXONE,FXONE,FXONE+FXTNT); break;
			case '6': object::linear.scale(FXONE,FXONE,FXONE-FXTNT); break;

			case 'R': u.reset(); rc=R_F; break;
			case 'T': object::linear.scale(FXONE+FXTNT,FXONE+FXTNT,FXONE+FXTNT); break;
			case 'Z': object::linear.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); break;
			case 'U': break;
			case 'I': u.pull(FXTNT); break;
			case 'O': u.pull(-FXTNT); break;
			case 'P': rc=R_B; break;
			case 'L': rc=R_S; break;
		}
		u.update();
		u.display(pos,rc);
}
///*

///implementation
int main(int argc,char** argv)
{
	init();

	object u(system::ldf(argv[1]));

	while(screen::run())
	{
		if(UNLIKELY(screen::key()==ESCAPE)) { menu(); }

		screen::back.clear(BLACK);
		screen::depth.clear(fx::l2f(200));

		viewer(u,screen::turbo());

		bench();
	}
	//*
	
	//generate();
	
	return 0;
}
///*

