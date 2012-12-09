// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZunit.cc
// Experimental Testing Ground

///includes
#include "xizero.hh"
///*

///definition

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

object* torus(sint r1,sint r2,sint s1,sint s2,uint e)
{
	const fixed q1 = fx::l2f(r1);
	const fixed q2 = fx::l2f(r2);
	const fixed c1 = fx::div(FXTAU,fx::l2f(s1));
	const fixed c2 = fx::div(FXTAU,fx::l2f(s2));
	fvector*    t1 = new fvector[s1];
	fvector*    t2 = new fvector[s2];

	for(sint i=0,j=0;i<s1;++i,j+=c1)
	{
		t1[i].set(fx::cos(j),fx::sin(j),0);
		t1[i] *= q1;
	}
	for(sint i=0,j=0;i<s2;++i,j+=c2)
	{
		t2[i].set(fx::cos(j),0,fx::sin(j));
		t2[i] *= q2;
	}

	const fixed a1 = fx::div(fx::l2f(360),fx::l2f(s1));
	const fixed a2 = fx::div(fx::l2f(360),fx::l2f(s2));
	fmatrix m1;
	fmatrix m2;
	m2.rotatez(a1);

	const sint n = s1*s2;
	lvector*   a = new lvector[n];
	lvector*   b = new lvector[n];
	lvector*   c = new lvector[n];
	lvector*   d = new lvector[n];

	for(sint i=0,h=0;i<s1;++i)
	{
		const sint k = math::set(0,i+1,i==(s1-1));
		for(sint j=0;j<s2;++j,++h)
		{
			const sint l = math::set(0,j+1,j==(s2-1));
			a[h] = t1[i]+m1.transform(t2[j]);
			b[h] = t1[k]+m2.transform(t2[j]);
			c[h] = t1[k]+m2.transform(t2[l]);
			d[h] = t1[i]+m1.transform(t2[l]);
		}
		m1.rotatez(a1);
		m2.rotatez(a2);
	}

	object* r = new object(a,b,c,d,n,e);
	delete[] t1;
	delete[] t2;
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
	return r;
}

void viewer(object* u,long k)
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
			//case '5': object::linear.scale(FXONE,FXONE,FXONE+FXTNT); break;
			//case '6': object::linear.scale(FXONE,FXONE,FXONE-FXTNT); break;

			case 't': object::linear.scale(FXONE+FXTNT,FXONE+FXTNT,FXONE+FXTNT); break;
			case 'z': object::linear.scale(FXONE-FXTNT,FXONE-FXTNT,FXONE-FXTNT); break;
			case 'u': break;
			case 'i': u->pull(FXTNT); break;
			case 'o': u->pull(-FXTNT); break;
			case 'p': rc=R_B; break;
			case 'l': rc=R_S; break;
			case 'm': rc=R_F; break;
		}
		u->update();
		u->display(pos,rc);
}
///*

///implementation
int main(int argc,char** argv)
{
	init();

	object* u = 0;

	if(argc>1) { u = new object(system::ldf(argv[1])); } else { u = torus(70,30,5,5,ORANGE); }

	while(screen::run())
	{
		if(screen::key()==ESCAPE) { menu(); }

		screen::back.clear(SKY);
		screen::depth.clear(fx::l2f(400));
		if(screen::turbo()=='R') { delete u; if(argc>1) { u = new object(system::ldf(argv[1])); } else { u = torus(70,30,5,5,ORANGE); } }
		viewer(u,screen::turbo());

		bench();
	}
	
	//generate();

	screen::close();

	return 0;
}
///*

