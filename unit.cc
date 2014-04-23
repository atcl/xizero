///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Experimental Testing Ground ( unit.cc )
///</header>

///<include>
#include "xizero.hh"
///</include>

///<define>

//generate torus

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

object* torus(xint r1,xint r2,xint s1,xint s2,yint e)
{
	const fixed q1 = fx::l2f(r1);
	const fixed q2 = fx::l2f(r2);
	const fixed c1 = fx::div(FXTAU,fx::l2f(s1));
	const fixed c2 = fx::div(FXTAU,fx::l2f(s2));
	vector*    t1 = new vector[s1];
	vector*    t2 = new vector[s2];

	for(xint i=0,j=0;i<s1;++i,j+=c1)
	{
		t1[i] = vector{fx::cos(j),fx::sin(j),0,0};
		t1[i] = fx::mul(t1[i],q1);
	}
	for(xint i=0,j=0;i<s2;++i,j+=c2)
	{
		t2[i] = vector{fx::cos(j),0,fx::sin(j)};
		t2[i] = fx::mul(t2[i],q2);
	}

	const fixed a1 = fx::div(fx::l2f(360),fx::l2f(s1));
	const fixed a2 = fx::div(fx::l2f(360),fx::l2f(s2));
	matrix m1;
	matrix m2;
	m2.rotatez(a1);

	const xint n = s1*s2;
	vector* a = new vector[n];
	vector* b = new vector[n];
	vector* c = new vector[n];
	vector* d = new vector[n];

	for(xint i=0,h=0;i<s1;++i)
	{
		const xint k = math::set(0,i+1,i==(s1-1));
		for(xint j=0;j<s2;++j,++h)
		{
			const xint l = math::set(0,j+1,j==(s2-1));
			a[h] = t1[i]+m1*t2[j];
			b[h] = t1[k]+m2*t2[j];
			c[h] = t1[k]+m2*t2[l];
			d[h] = t1[i]+m1*t2[l];
		}
		m1.rotatez(a1);
		m2.rotatez(a2);
	}

	object* r = new object(a,d,c,b,n,e);
	delete[] t1;
	delete[] t2;
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
	return r;
}

void viewer(object* u,yint k)
{
	const vector pos{ XRES/2, YRES/2, 100, 0};
	static xint rc = R_F;
	object::linear.clear();
	switch(k)
	{
		case UP:     object::linear.rotatex(FXONE); break;
		case DOWN:   object::linear.rotatex(FXMON); break;
		case RIGHT:  object::linear.rotatey(FXONE); break;
		case LEFT:   object::linear.rotatey(FXMON); break;
		case PGUP:   object::linear.rotatez(FXONE); break;
		case PGDOWN: object::linear.rotatez(FXMON); break;

		case 'd': object::linear.translate(FXONE,0,0); break;
		case 'a': object::linear.translate(FXMON,0,0); break;
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

		case 't': object::linear.scale(FXONE+FXTNT,FXONE+FXTNT,FXONE+FXTNT); break;
		case 'z': object::linear.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); break;
		case 'u': break;
		case 'i': u->explode(FXTNT); break;
		case 'o': u->explode(-FXTNT); break;
		case 'j': u->implode(FXCEN); break;
		case 'k': u->implode(-FXCEN); break;
		case 'p': rc=R_B; break;
		case 'l': rc=R_S; break;
		case 'm': rc=R_F; break;
	}

	u->update();
	u->display(pos,rc);
}
///</define>

///<code>
int main(int argc,char** argv)
{
	//generate();

	init();

	object* u = 0;

	if(argc>1) { u = new object(system::ldf(argv[1])); } else { u = torus(60,30,16,16,ORANGE); }

	while(screen::run())
	{
		if(screen::key()==ESCAPE) { menu::show(); }

		screen::frame.clear(WHITE);
		screen::depth.clear(0);
		ifu(screen::turbo()=='r') { delete u; if(argc>1) { u = new object(system::ldf(argv[1])); } else { u = torus(60,30,16,16,ORANGE); } }
		viewer(u,screen::turbo());

		game::benchmark();
	}

	

	return 0;
}
///</code>

