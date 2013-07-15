///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZbutton.hh
// GUI Button Library 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
#include "XZlist.hh"
#include "XZbutton.hh"
///</include>

///<define>
class buttons
{
	private:
		list<button> bl;
	public:
		buttons() : bl() { };
		void add(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v);
		xint check(xint k);
		void draw();
};
///</define>

///<code>
void buttons::add(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v)
{
	button* z = new button(x,a,d,l,t,w,h,c,m,b,f,v);
	bl.append(z);
	bl.first();
}

xint buttons::check(xint k)
{
	switch(k)
	{
		case SPACE: return bl.current()->click(); break;
		case LEFT:  case UP:   if(!bl.isfirst()) { bl.prev(); } break;
		case RIGHT: case DOWN: if(!bl.islast())  { bl.next(); } break;
	}
	return 0;
}

void buttons::draw()
{
	bl.save();
	for(bl.first();bl.notlast();bl.next())
	{
		((button*)bl.current())->draw(bl.same());
	}
	bl.load();
}
///</code>

